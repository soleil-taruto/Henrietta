#include "StdInc.h"

char *GC_PicFiles[PIC_MAX]; // NULL == 画像の割り当てなし
int GC_PicList[PIC_MAX]; // 未ロード時は、Dc->PicDummy とする。

void GC_ProcInitReset(void) // ウィンドウモード変更時にも走る。
{
	for(int i = 0; i < PIC_MAX; i++)
	{
		GC_PicList[i] = Dc->PicDummy;
	}
}
void GC_ProcInit(void)
{
	int i = 0;

	GC_PicFiles[i++] = NULL;

	GC_PicFiles[i++] = NULL;
	GC_PicFiles[i++] = "シナリオ\\System\\メッセージウィンドウ.png";
	GC_PicFiles[i++] = "シナリオ\\System\\Save.png";
	GC_PicFiles[i++] = "シナリオ\\System\\Save後.png";
	GC_PicFiles[i++] = "シナリオ\\System\\Load.png";
	GC_PicFiles[i++] = "シナリオ\\System\\Load後.png";
	GC_PicFiles[i++] = "シナリオ\\System\\Auto.png";
	GC_PicFiles[i++] = "シナリオ\\System\\Auto後.png";
	GC_PicFiles[i++] = "シナリオ\\System\\Skip.png";
	GC_PicFiles[i++] = "シナリオ\\System\\Skip後.png";
	GC_PicFiles[i++] = "シナリオ\\System\\Menu.png";
	GC_PicFiles[i++] = "シナリオ\\System\\Menu後.png";
	GC_PicFiles[i++] = NULL;

	GC_PicFiles[i++] = NULL;
	GC_PicFiles[i++] = "シナリオ\\選択肢\\どのルートで.png";
	GC_PicFiles[i++] = "シナリオ\\選択肢\\未開通.png";
	GC_PicFiles[i++] = "シナリオ\\選択肢\\未開通後.png";
	GC_PicFiles[i++] = "シナリオ\\選択肢\\ドイツ日本.png";
	GC_PicFiles[i++] = "シナリオ\\選択肢\\ドイツ日本後.png";
	GC_PicFiles[i++] = "シナリオ\\選択肢\\イタリアプロイセン.png";
	GC_PicFiles[i++] = "シナリオ\\選択肢\\イタリアプロイセン後.png";
	GC_PicFiles[i++] = "シナリオ\\選択肢\\ドイツプロイセン.png";
	GC_PicFiles[i++] = "シナリオ\\選択肢\\ドイツプロイセン後.png";
	GC_PicFiles[i++] = "シナリオ\\選択肢\\イタリア日本.png";
	GC_PicFiles[i++] = "シナリオ\\選択肢\\イタリア日本後.png";
	GC_PicFiles[i++] = "シナリオ\\選択肢\\ドイツイタリア.png";
	GC_PicFiles[i++] = "シナリオ\\選択肢\\ドイツイタリア後.png";
	GC_PicFiles[i++] = "シナリオ\\選択肢\\プロイセン日本.png";
	GC_PicFiles[i++] = "シナリオ\\選択肢\\プロイセン日本後.png";
	GC_PicFiles[i++] = NULL;

	GC_PicFiles[i++] = NULL;
	GC_PicFiles[i++] = "シナリオ\\System\\セーブ画面.png";
	GC_PicFiles[i++] = "シナリオ\\System\\ロード画面.png";
	GC_PicFiles[i++] = "シナリオ\\System\\戻る.png";
	GC_PicFiles[i++] = "シナリオ\\System\\戻る後.png";
	GC_PicFiles[i++] = "シナリオ\\System\\SaveLoad\\セーブしますか？.png";
	GC_PicFiles[i++] = "シナリオ\\System\\SaveLoad\\ロードしますか？.png";
	GC_PicFiles[i++] = "シナリオ\\System\\SaveLoad\\する.png";
	GC_PicFiles[i++] = "シナリオ\\System\\SaveLoad\\する後.png";
	GC_PicFiles[i++] = "シナリオ\\System\\SaveLoad\\しない.png";
	GC_PicFiles[i++] = "シナリオ\\System\\SaveLoad\\しない後.png";
	GC_PicFiles[i++] = NULL;

	GC_PicFiles[i++] = NULL;
	GC_PicFiles[i++] = "Puzzle\\必殺\\必殺左.png";
	GC_PicFiles[i++] = "Puzzle\\必殺\\必殺右.png";
	GC_PicFiles[i++] = "Puzzle\\必殺\\低速左.png";
	GC_PicFiles[i++] = "Puzzle\\必殺\\低速右.png";
	GC_PicFiles[i++] = "Puzzle\\必殺\\高速左.png";
	GC_PicFiles[i++] = "Puzzle\\必殺\\高速右.png";
	GC_PicFiles[i++] = "Puzzle\\必殺\\日隠し.png";
	GC_PicFiles[i++] = "Puzzle\\必殺\\露隠し.png";
	GC_PicFiles[i++] = "Puzzle\\必殺\\西隠し.png";
	GC_PicFiles[i++] = "Puzzle\\必殺\\独日隠し.png";
	GC_PicFiles[i++] = "Puzzle\\必殺\\英日隠し.png";
	GC_PicFiles[i++] = "Puzzle\\必殺\\米英隠し.png";
	GC_PicFiles[i++] = "Puzzle\\必殺\\露普隠し.png";
	GC_PicFiles[i++] = NULL;

	GC_PicFiles[i++] = NULL;

	errorCase(i != PIC_MAX);

	GC_ProcInitReset();
	GC_NmBgmLoadAll();
}
void GC_ProcFnlz(void)
{
	// noop
}

void GC_Load(int index)
{
	errorCase(index < 0 || PIC_MAX <= index);

	if(GC_PicList[index] == Dc->PicDummy && GC_PicFiles[index] != NULL)
	{
		GC_PicList[index] = LoadPicture(ResourcePath(GC_PicFiles[index]));
	}
}
void GC_Unload(int index)
{
	errorCase(index < 0 || PIC_MAX <= index);

	if(GC_PicList[index] != Dc->PicDummy)
	{
		UnloadPicture(GC_PicList[index]);
		GC_PicList[index] = Dc->PicDummy;
	}
}
void GC_LoadRange(int minidx, int maxidx)
{
	for(int index = minidx; index <= maxidx; index++)
	{
		GC_Load(index);
	}
}
void GC_UnloadRange(int minidx, int maxidx)
{
	for(int index = minidx; index <= maxidx; index++)
	{
		GC_Unload(index);
	}
}
void GC_UnloadAll(void)
{
	GC_UnloadRange(PIC_Start, PIC_End);
}

int GC_Pic(int index)
{
	errorCase(index < 0 || PIC_MAX <= index);
	return GC_PicList[index];
}
int GC_LPic(int index)
{
	GC_Load(index);
	return GC_Pic(index);
}

static char *NmPicResrc(char *name)
{
	char *relFile = xcout("シナリオ\\画像\\%s.png", name);
	char *file;

	file = ResourcePath(relFile);
	memFree(relFile);
	return file;
}
static char *NmBgmResrc(char *name)
{
	char *relFile = xcout("シナリオ\\音\\%s.mp3", name);
	char *file;

	file = ResourcePath(relFile);
	memFree(relFile);
	return file;
}

char *GC_NmPicNames[GC_NMPIC_MAX];
char *GC_NmPicFiles[GC_NMPIC_MAX];
int GC_NmPicList[GC_NMPIC_MAX];
int GC_NmPicIndex;

void GC_NmPicLoad(char *name)
{
	name = strx(name);
	char *file = strx(NmPicResrc(name));
	int pic = LoadPicture(file);

	errorCase(GC_NMPIC_MAX <= GC_NmPicIndex);

	GC_NmPicNames[GC_NmPicIndex] = name;
	GC_NmPicFiles[GC_NmPicIndex] = file;
	GC_NmPicList[GC_NmPicIndex] = pic;
	GC_NmPicIndex++;
}
int GC_IsNmPicLoaded(char *name)
{
	for(int index = 0; index < GC_NmPicIndex; index++)
	{
		if(_stricmp(name, GC_NmPicNames[index]) == 0) // ? found
		{
			return 1;
		}
	}
	return 0;
}
void GC_NmPicUnloadAll(void)
{
	while(GC_NmPicIndex)
	{
		GC_NmPicIndex--;

		memFree(GC_NmPicNames[GC_NmPicIndex]);
		memFree(GC_NmPicFiles[GC_NmPicIndex]);
		UnloadPicture(GC_NmPicList[GC_NmPicIndex]);
	}
}
int GC_NmPic(char *name)
{
	for(int index = 0; index < GC_NmPicIndex; index++)
	{
		if(_stricmp(name, GC_NmPicNames[index]) == 0) // ? found
		{
			return GC_NmPicList[index];
		}
	}
	error();
	return 0; // dummy
}

char *GC_NmBgmNames[GC_NMBGM_MAX];
int GC_NmBgmList[GC_NMBGM_MAX];
int GC_NmBgmCount;

void GC_NmBgmLoadAll(void)
{
	int fp = DFOpen(ResourcePath("シナリオ_音.txt"));

	for(; ; )
	{
		char *line = DFReadLine(fp);

		if(line[0] == '\0')
		{
			break;
		}
		char *name = line;

		// -- 拡張子除去 --
		char *p = strchr(name, '.');

		errorCase(!p);
		p[0] = '\0';
		// ----

		name = strx(name);

		int bgm;

		// -- Add --
		errorCase(GC_NMBGM_MAX <= GC_NmBgmCount);

		GC_NmBgmNames[GC_NmBgmCount] = name;
		GC_NmBgmList[GC_NmBgmCount] = bgm = LoadSoundMem(NmBgmResrc(line)); // 拡張子いらん
		GC_NmBgmCount++;
		// ----

		errorCase(bgm == -1);
	}
	DFClose(fp);
}
int GC_NmBgm(char *name)
{
	for(int index = 0; index < GC_NmBgmCount; index++)
	{
		if(strcmp(name, GC_NmBgmNames[index]) == 0) // ? found
		{
			return GC_NmBgmList[index];
		}
	}

printfDx("音楽データが見つかりません: %s\n", name);

	error();
	return 0; // dummy
}
