#include "StdInc.h"

#define FLOWER_MAX 6

static int FlowersInited;
static int Flowers[FLOWER_MAX];

static double FlowerPct;

#define FLWPCT_MAX 0.17
#define FLWPCT_ADD (FLWPCT_MAX / (50 * 60))

static void AddFlowerRain(int flower_idx)
{
	if(flower_idx == -1)
		flower_idx = rndbnd(0, FLOWER_MAX - 1);

	errorCase(flower_idx < 0 || FLOWER_MAX <= flower_idx);

	if(!FlowersInited) {
		FlowersInited = 1;

		for(int i = 0; i < FLOWER_MAX; i++)
		{
			Flowers[i] = LoadPicture(ResourcePath_x(xcout("シナリオ\\エンディング\\Flower%d.png", i + 1)));
		}
	}
	double z = rnd();
	int pic = Flowers[flower_idx];

	z = z * z * z;

	CutIn_t *i = PzefAddEffect(rndbnd(0, 800), -50.0, 0.0, (z + 0.5) * 3.0, 0.0, 0.1, (z + 0.5) * 1.75, 777, pic);

	i->A = 0.3;
	i->FrameCount = (int)(700.0 / i->Ya);
}
static void ExecFrameFlowerRain(void)
{
	FlowerPct += FLWPCT_ADD;
	m_range(FlowerPct, 0.0, FLWPCT_MAX);

	if(krnd() < FlowerPct)
	{
		AddFlowerRain(-1);
	}
}
void EDEx_AddFlowerRain(void)
{
	AddFlowerRain(-1);
}
void EDEx_AddFlowerRainIdx(int flower_idx)
{
	AddFlowerRain(flower_idx);
}
void EDEx_ScreenModeChanged(void)
{
	FlowersInited = 0;
}

#define WALL_MAX 50

static int WallPicList[WALL_MAX];
static int WallCount;

void Ending(char *endCdLine)
{
	int frmidx;
	double frmpct;

	int endCode;

	     if(!strcmp(endCdLine, "E_GI")) endCode = ED_GI;
	else if(!strcmp(endCdLine, "E_GJ")) endCode = ED_GJ;
	else if(!strcmp(endCdLine, "E_GP")) endCode = ED_GP;
	else if(!strcmp(endCdLine, "E_IJ")) endCode = ED_IJ;
	else if(!strcmp(endCdLine, "E_IP")) endCode = ED_IP;
	else if(!strcmp(endCdLine, "E_PJ")) endCode = ED_PJ;
	else                                error();

	int bgm;
	int wallnum;
	int bgmendms;
	int *pWatched = SettInfo.EndingWatchedList + endCode;
	int watched = *pWatched;

	switch(endCode)
	{
	case ED_GJ:
	case ED_GP:
	case ED_IJ:
	case ED_IP:
		{
			bgm = LoadBGM(ResourcePath("シナリオ\\エンディング\\ED1.mp3"));
			bgmendms = 1000 * 95;
			wallnum = 2;
		}
		break;

	case ED_GI:
	case ED_PJ:
		{
			bgm = LoadBGM(ResourcePath("シナリオ\\エンディング\\ED2.mp3"));
			bgmendms = 1000 * 120;
			wallnum = 11;
		}
		break;

	default:
		error();
	}
	
	for(int index = 0; index < wallnum; index++)
	{
		char *file = xcout("シナリオ\\エンディング\\%s\\Wall_%02d.png", endCdLine, index + 1);

		WallPicList[index] = LoadPicture(ResourcePath(file));
		memFree(file);
	}
	WallCount = wallnum;
	errorCase(WallCount < 1);

	int lastPic = Dc->PicBlackWall;
	int currPic = WallPicList[0];

	DispMusCursor(false);

	PlayBGM(bgm, 1);
	int bgmstopms = GetNowCount() + bgmendms;

#define KIRIKAE_FRAMECNT 90
#define HYOUJI_MIN_FRAMECNT 240
#define OWARI_FRAMECNT 360

	int nextms = GetNowCount();
	int hyoujims = 9000;
	int hyoujimssaigo = 1000 * 3600;

	FlowerPct = 0.0;

	for(int index = 1; ; index++)
	{
		int saigo = WallCount <= index;

		nextms += saigo ? hyoujimssaigo : hyoujims;

		frameloop(frmidx, frmpct, KIRIKAE_FRAMECNT)
		{
			SimpleDraw(lastPic, 0, 0, 0);

			SetAlpha(frmpct);
			SimpleDraw(currPic, 0, 0, 0);
			ResetAlpha();

			ExecFrameFlowerRain();
			SwapFrame();
		}
		for(frmidx = 0; ; frmidx++)
		{
			SimpleDraw(currPic, 0, 0, 0);
			ExecFrameFlowerRain();
			SwapFrame();

			int nowms = GetNowCount();

#if LOG_ENABLED
			if(IsHit(KEY_INPUT_1))
			{
				nextms = 0;
				frmidx = HYOUJI_MIN_FRAMECNT;
			}
			printfDx("%d-%d=%d,%d "
				,nextms
				,nowms
				,nextms - nowms
				,HYOUJI_MIN_FRAMECNT - frmidx
				);
			printfDx("%d-%d=%d,%d\n"
				,bgmstopms
				,nowms
				,bgmstopms - nowms
				,1 // IsPlayBGM()
				);
			printfDx("%f ", FlowerPct);
#endif
			if(bgmstopms < nowms) // 二週目に入る前に、止める。
			{
				StopBGM();
			}
			if(HYOUJI_MIN_FRAMECNT < frmidx && nextms < nowms)
			{
				if(saigo)
				{
					CIEx_AddStar(300);
				}
				break;
			}

			if(watched || saigo)
			{
				if(GetHit(INP_ROT_DECIDE) || GetHit(INP_ROT_CANCEL) || GetClick())
				{
					goto endEnding;
				}
			}
		}

		if(saigo)
		{
			break;
		}

		lastPic = currPic;
		currPic = WallPicList[index];
	}

endEnding:
	FadeOut(Dc->BGMVolume, Dc->BGMVolume / OWARI_FRAMECNT);

	frameloop(frmidx, frmpct, OWARI_FRAMECNT)
	{
		SimpleDraw(currPic, 0, 0, 0);

		SetAlpha(frmpct);
		SimpleDraw(Dc->PicBlackWall, 0, 0, 0);
		ResetAlpha();

		SwapFrame();
	}
	for(int c = 0; c < 90; c++)
	{
		SimpleDraw(Dc->PicBlackWall, 0, 0, 0);
		SwapFrame();
	}
	StopBGM();
	
	// fnlz --->

	*pWatched = 1;

	SaveSettings();

	// release --->

	for(int index = 0; index < WallCount; index++)
	{
		UnloadPicture(WallPicList[index]);
	}
	UnloadBGM(bgm);
}
