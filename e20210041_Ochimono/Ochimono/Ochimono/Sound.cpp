#include "StdInc.h"

#define SOUND_NUMMAX 128

static int SoundList[SOUND_NUMMAX];
static int SoundCnt;

static int FadeOutTaskAlive;
static int KillFadeOutTask;

int LoadSound(char *file)
{
	int snd = LoadSoundMem(file);

	errorCase(snd == -1);

	// Add snd
	{
		errorCase(SOUND_NUMMAX <= SoundCnt);
		SoundList[SoundCnt] = snd;
		SoundCnt++;
	}
	return snd;
}
void UnloadSound(int snd)
{
	error(); // 必要ないので、、、

	// Unadd snd
	{
		int index;

		for(index = 0; index < SoundCnt; index++)
			if(SoundList[index] == snd)
				break;

		errorCase(SoundCnt <= index);

		SoundCnt--;
		SoundList[index] = SoundList[SoundCnt];
	}
	DeleteSoundMem(snd);
}
void SetSeVolume(double volume) // volume : 0.0 - 1.0
{
	int pal = (int)(volume * 255);
	int index;

	errorCase(pal < 0 || 255 < pal);

	for(index = 0; index < SoundCnt; index++)
	{
		if(ChangeVolumeSoundMem(pal, SoundList[index]) != 0)
		{
			error();
		}
	}
}
void Direct_PlaySe(int snd)
{
	if(PlaySoundMem(snd, DX_PLAYTYPE_BACK, 1) != 0)
	{
		error();
	}
}

#define BGM_STOCK_MAX 128

static char *StockBGMFiles[BGM_STOCK_MAX];
static int StockBGMList[BGM_STOCK_MAX];
static int StockBGMCount;

int LoadBGM(char *file)
{
	errorCase(!file);
	errorCase(!*file);

	// ストックから返す。
	{
		for(int index = 0; index < StockBGMCount; index++)
		{
			if(!_stricmp(file, StockBGMFiles[index]))
			{
				return StockBGMList[index];
			}
		}
	}
	int bgm = LoadSoundMem(file);

	errorCase(bgm == -1);

	// ストックする。
	{
		errorCase(BGM_STOCK_MAX <= StockBGMCount);

		StockBGMFiles[StockBGMCount] = strx(file);
		StockBGMList[StockBGMCount] = bgm;
		StockBGMCount++;
	}
	return bgm;
}
void UnloadBGM(int bgm)
{
	// DeleteSoundMem(bgm); // 開放しない。
}

static int BGMPlayed;
static int BGMVolume;
static int CurrBGM;

static void ApplyBGMVolume(void)
{
	if(ChangeVolumeSoundMem(BGMVolume, CurrBGM) != 0)
	{
		error();
	}
}

void Direct_PlayBGM(int bgm, int playOnce)
{
	if(BGMPlayed)
	{
		if(CurrBGM == bgm)
			return;

		error(); // ここは来なくなるはず！
		/*
		StopBGM(); // StopBGM() した直後に再生するとノイズが走る！？
		*/
	}
	if(PlaySoundMem(bgm, playOnce ? DX_PLAYTYPE_BACK : DX_PLAYTYPE_LOOP, 1) != 0)
	{
		error();
	}
	BGMPlayed = 1;
	CurrBGM = bgm;

	SetBGMVolume(SettInfo.BGMVolume);
	ApplyBGMVolume();

	KillFadeOutTask = 1;
}
void Direct_StopBGM(void)
{
	/*
	if(FadeOutTaskAlive)
		return;
	*/

	if(!BGMPlayed)
		return;
	
	if(StopSoundMem(CurrBGM) != 0)
	{
		error();
	}
	BGMPlayed = 0;

	KillFadeOutTask = 1;
}
static int SBV_NoApply;
void Direct_SetBGMVolume(double volume) // volume : 0.0 - 1.0
{
	int pal = (int)(volume * 255);

	errorCase(pal < 0 || 255 < pal);
	BGMVolume = pal;

	if(BGMPlayed && !SBV_NoApply)
	{
		ApplyBGMVolume();
	}
}

static int FadeQuick;
static int FadeOutTask(void *vi)
{
	double *vols = (double *)vi;

	if(KillFadeOutTask)
		goto endTask;

	if(FadeQuick)
	{
		FadeQuick = 0;
		vols[0] = 0.0;
	}
	vols[0] -= vols[1];

	if(vols[0] < 0.0)
	{
//		SetBGMVolume(0.0);
		Direct_StopBGM();

endTask:
//		SBV_NoApply = 1; // ストーリーパートで音量が変になる？
//		SetBGMVolume(vols[2]); // ここでやる必要ない！？
//		SBV_NoApply = 0;

		memFree(vols);

		FadeOutTaskAlive = 0;
		return 0;
	}
	SetBGMVolume(vols[0]);
	return 1;
}
/*
	startvol : 0.0 - 1.0
	subvol : 0.0 <
*/
void Direct_FadeOut(double startvol, double subvol)
{
	if(FadeOutTaskAlive)
		return;

	double *vols = (double *)memAlloc(3 * sizeof(double));

	vols[0] = startvol;
	vols[1] = subvol;
	vols[2] = startvol;

	AddFrameTask(FadeOutTask, vols);

	FadeOutTaskAlive = 1;
	KillFadeOutTask = 0;

	FadeQuick = 0;
}

///////////////////
// DELAY version //
///////////////////

#define PLAY_DELAY 13

typedef struct PlayCommand_st
{
	int Command; // P = PLAY, S = STOP, V = set-VOLUME, F = FADE-OUT
	int BGM;
	double Volume1;
	double Volume2;
}
PlayCommand_t;

#define SE_QUEUE_SIZE 100
#define PLAY_QUEUE_SIZE 100

static int SeQueue[SE_QUEUE_SIZE];
static int SeQueueIndex;
static int SeWaitCount;

static PlayCommand_t PlayQueue[PLAY_QUEUE_SIZE];
static int PlayQueueIndex;
static int PlayWaitCount;

static void FrameSe(void)
{
	if(SeQueueIndex < 1)
	{
		return;
	}
	Direct_PlaySe(SeQueue[0]);

	SeQueueIndex--;
	for(int i = 0; i < SeQueueIndex; i++)
	{
		SeQueue[i] = SeQueue[i + 1];
	}
}
static void FrameBGM(void)
{
	if(0 < PlayWaitCount)
	{
		PlayWaitCount--;
		return;
	}
	if(PlayQueueIndex < 1)
	{
		return;
	}
	PlayCommand_t *pc = PlayQueue + 0;

	switch(pc->Command)
	{
	case '1':
		Direct_PlayBGM(pc->BGM, 1);
		break;

	case 'P':
		Direct_PlayBGM(pc->BGM);
		break;

	case 'S':
		Direct_StopBGM();
		break;

		/*
	case 'V':
		Direct_SetBGMVolume(pc->Volume1);
		break;
		*/

		/*
	case 'F':
		Direct_FadeOut(pc->Volume1, pc->Volume2);
		break;
		*/

	default:
		error();
	}

	PlayQueueIndex--;
	for(int i = 0; i < PlayQueueIndex; i++)
	{
		PlayQueue[i] = PlayQueue[i + 1];
	}
	PlayWaitCount = PLAY_DELAY;
}
void SoundFrame(void)
{
	FrameSe();
	FrameBGM();
}
static void AddPlayCommand(int command, int bgm, double volume1, double volume2)
{
	errorCase(PLAY_QUEUE_SIZE <= PlayQueueIndex);

	PlayCommand_t *i = PlayQueue + PlayQueueIndex;
	PlayQueueIndex++;

	i->Command = command;
	i->BGM = bgm;
	i->Volume1 = volume1;
	i->Volume2 = volume2;
}

void PlaySe(int snd)
{
	errorCase(SE_QUEUE_SIZE <= SeQueueIndex);

	SeQueue[SeQueueIndex] = snd;
	SeQueueIndex++;
}
void PlayBGM(int bgm, int playOnce)
{
	int playCommand = playOnce ? '1' : 'P';

	if(0 < PlayQueueIndex && PlayQueue[PlayQueueIndex - 1].Command == 'P')
	{
		PlayQueueIndex--;
		AddPlayCommand(playCommand, bgm, 0.0, 0.0);
		return;
	}
	if(BGMPlayed && CurrBGM == bgm && !FadeOutTaskAlive)
	{
		return;
	}
	AddPlayCommand('S', 0, 0.0, 0.0);
	AddPlayCommand(playCommand, bgm, 0.0, 0.0);
}
void StopBGM(void)
{
	if(0 < PlayQueueIndex && PlayQueue[PlayQueueIndex - 1].Command == 'S')
	{
		return;
	}
	FadeQuick = 1;
	AddPlayCommand('S', 0, 0.0, 0.0);
}
void SetBGMVolume(double volume)
{
	Direct_SetBGMVolume(volume);
	/*
	AddPlayCommand('V', 0, volume, 0.0);
	*/
}
void FadeOut(double startvol, double subvol)
{
	Direct_FadeOut(startvol, subvol);
	/*
	AddPlayCommand('F', 0, startvol, subvol);
	*/
}
