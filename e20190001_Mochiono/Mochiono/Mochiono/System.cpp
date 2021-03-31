#include "StdInc.h"

typedef struct FrameTask_st
{
	int (*Func)(void *); // ret == 0: 終了, 1: 継続
	void *Info;
	int Layer;
}
FrameTask_t;

#define FRAMETASK_MAX 8192

static FrameTask_t FrameTaskList[FRAMETASK_MAX];
static int FTIndex;

void AddFrameTaskLayer(int (*func)(void *), void *info, int layer)
{
	FrameTask_t *i;

	errorCase(!func);
	errorCase(FRAMETASK_MAX <= FTIndex);
	errorCase(layer < FT_LAYER_MIN || FT_LAYER_MAX < layer);

	i = FrameTaskList + FTIndex;
	i->Func = func;
	i->Info = info;
	i->Layer = layer;

	FTIndex++;
}
void AddFrameTask(int (*func)(void *), void *info)
{
	AddFrameTaskLayer(func, info, 0);
}
int Sys_GetFrameTaskCount(void)
{
	return FTIndex;
}
void ExecFrameTasks(void)
{
	for(int layer = FT_LAYER_MIN; layer <= FT_LAYER_MAX; layer++)
	{
		for(int index = 0; index < FTIndex; index++)
		{
			FrameTask_t *i = FrameTaskList + index;

			if(i->Layer == layer)
			{
				if(i->Func(i->Info) == 0)
				{
					FTIndex--;
					FrameTaskList[index] = FrameTaskList[FTIndex];
					index--;
				}
			}
		}
	}
}
int EFCDone;
void ExecFrameCurtain(void)
{
	if(EFCDone) return;

	nearize_adj(Dc->BCPct, Dc->BlackCurtain, 0.9, 0.05);
	nearize_adj(Dc->WCPct, Dc->WhiteCurtain, 0.9, 0.05);

	if(0.0 < Dc->BCPct) {
		SetAlpha(Dc->BCPct);
		SimpleDraw(Dc->PicBlackWall, 0, 0, 0);
		ResetAlpha();
	}
	if(0.0 < Dc->WCPct) {
		SetAlpha(Dc->WCPct);
		SimpleDraw(Dc->PicWhiteWall, 0, 0, 0);
		ResetAlpha();
	}
	EFCDone = 1;
}
void WaitToCurtain(int picWall)
{
	while(
		Dc->BCPct != Dc->BlackCurtain ||
		Dc->WCPct != Dc->WhiteCurtain
		)
	{
		SimpleDraw(picWall, 0, 0, 0);
		SwapFrame();
	}
}

int Sys_EscapeDisable;
int Sys_EscapePressed;

#define SOKUTEI_MAX 14

typedef struct SokuteiInfo_st
{
	int Inited;
	int LastTime;
	int CurrTime;
	int FrameCount;
	int SokuteiCount;
	int Hz60Count;
	int Hz60Env;
}
SokuteiInfo_t;
SokuteiInfo_t SokuteiInfo;

void SFS_Reset(void)
{
	SokuteiInfo_t *t = &SokuteiInfo;

	if(t->Hz60Env)
	{
		t->Inited = 0; // 測定やり直し！
	}
}
#if 0 // 廃止
void SwapFrameScreen(void)
{
	SokuteiInfo_t *t = &SokuteiInfo;

	if(!t->Inited)
	{
		memset(t, 0x00, sizeof(SokuteiInfo_t));

		t->Inited = 1;
		t->Hz60Env = 1; // 測定のため、暫定的に 60 hz と見なす。

		SetWaitVSyncFlag(true); // V-Sync 待ち有効！
	}

	// 測定
	{
		t->CurrTime = GetNowCount() / 500; // 0.5 秒単位

		if(t->SokuteiCount < SOKUTEI_MAX)
		{
			t->FrameCount++;

			if(t->LastTime != t->CurrTime)
			{
				t->LastTime = t->CurrTime;
				t->SokuteiCount++;

				if(27 <= t->FrameCount && t->FrameCount <= 33) // ? だいたい 60 hz
				{
					t->Hz60Count++;
				}
				t->FrameCount = 0;

				if(t->SokuteiCount == SOKUTEI_MAX)
				{
					t->Hz60Env = SOKUTEI_MAX / 2 <= t->Hz60Count; // 半分以上 60 hz と判定されたなら、60 hz と見なす。

					if(!t->Hz60Env) // ? not 60 hz
					{
						SetWaitVSyncFlag(false); // V-Sync 待ち off!
					}
				}
			}
		}
	}

	if(!t->Hz60Env) // ? not 60 hz
	{
		static int frmcnt;
		static int nextms;

		if(!nextms)
			nextms = GetNowCount();

		do
		{
			ScreenFlip();
		}
		while(GetNowCount() < nextms);

		frmcnt++;
		nextms += frmcnt % 3 ? 17 : 16;
	}
	else
		ScreenFlip();
}
static void ScreenZoomControl(void)
{
	static int pressCnt;
	int input = 0;

	if(IsPress(KEY_INPUT_HOME)) input = 'H';
	if(IsPress(KEY_INPUT_PGUP)) input = 'U';
	if(IsPress(KEY_INPUT_PGDN)) input = 'D';

	if(input)
	{
		if(pressCnt && pressCnt < 20 || pressCnt & 1)
		{
			input = 0;
		}
		pressCnt++;
	}
	else
		pressCnt = 0;

#define WINSZEX_DENOM 10.0

	if(input)
		SettInfo.ScreenZoom = (double)(int)(GetWindowSizeExtendRate() * WINSZEX_DENOM + 0.5) / WINSZEX_DENOM;

	switch(input)
	{
	case 'H':
		SettInfo.ScreenZoom = 1.0;
		break;
	case 'U':
		SettInfo.ScreenZoom = (double)(int)(SettInfo.ScreenZoom * WINSZEX_DENOM + 1.0 + 0.5) / WINSZEX_DENOM;
		break;
	case 'D':
		SettInfo.ScreenZoom = (double)(int)(SettInfo.ScreenZoom * WINSZEX_DENOM - 1.0 + 0.5) / WINSZEX_DENOM;
		break;
	case 0:
		break;

	default:
		error();
	}

	{
		static int frmcnt;
		int atFirst = !frmcnt;

		if(input || (atFirst && SettInfo.ScreenZoom != 1.0))
		{
			m_range(SettInfo.ScreenZoom, 0.1, 10.0);

			if(atFirst)
				SetWindowSizeChangeEnableFlag(false);

			SetWindowSizeExtendRate(SettInfo.ScreenZoom);

			if(atFirst)
				SetWindowSizeChangeEnableFlag(true);
		}
		if(frmcnt == 1)
		{
			SetWindowSizeChangeEnableFlag(true);
		}
		frmcnt++;
	}
}
#endif
int ProcFrmCnt;
int Pub_MainScreen = -1;
void SwapFrame(void)
{
	ProcFrmCnt++;

	SoundFrame();

	ExecFrameTasks();
	ExecFrameCurtain();
	EFCDone = 0;

#if 1
	{
		int &mainScreen = Pub_MainScreen;

		if(mainScreen == -1)
		{
			mainScreen = MakeScreen(800, 600);
			errorCase(mainScreen == -1);
		}
		else
		{
			errorCase(SetDrawScreen(DX_SCREEN_BACK)); // ? 失敗

			DrawExtendGraph(
				0,
				0,
				800,
				600,
				mainScreen,
				0
				);
		}
		ScreenFlip();

		errorCase(SetDrawScreen(mainScreen)); // ? 失敗
	}
#else // old
	SwapFrameScreen();
	ScreenZoomControl();
#endif

	if(IsPress(KEY_INPUT_ESCAPE))
	{
		if(!Sys_EscapeDisable)
			termination(0);

		Sys_EscapePressed = 1;
	}
	if(ProcessMessage() == -1)
	{
		termination(0);
	}
//	ClearDrawScreen();

	RefreshInput();

	IH_LastPressed = IH_Pressed;
	IH_Pressed = 0;

	IH_PadLastPressed = IH_PadPressed;
	IH_PadPressed = 0;


	// fps --->

#if LOG_ENABLED
	static int fps;
	{
		static int lastTime;
		static int frmcnt;
		int currTime = GetNowCount() / 1000;

		frmcnt++;

		if(lastTime != currTime)
		{
			lastTime = currTime;
			fps = frmcnt;
			frmcnt = 0;
		}
	}

	clsDx();
	printfDx("FPS:%d (60hz:%d,%d) ", fps, SokuteiInfo.Hz60Env, SokuteiInfo.Hz60Count);
#endif
}

// ----

int IsPress(int key)
{
	// key:
	KEY_INPUT_BACK;

	return CheckHitKey(key);
}
int IH_LastPressed;
int IH_Pressed;
int IsHit(int key)
{
	if(IsPress(key))
	{
		IH_Pressed = 1;
		return IH_LastPressed == 0;
	}
	return 0;
}
int IsRenda(int key)
{
	return (IsPress(KEY_INPUT_LSHIFT) ? IsPress : IsHit)(key);
}

int GetPressPad(int padNo) // padNo : 0 - 1
{
	return GetJoypadInputState(padNo == 0 ? DX_INPUT_PAD1 : DX_INPUT_PAD2);
}
int IH_PadLastPressed;
int IH_PadPressed;
int GetHitPad(int padNo) // padNo : 0 - 1
{
	int status = GetPressPad(padNo);

	if(status)
	{
		IH_PadPressed = 1;
		return IH_PadLastPressed ? 0 : status;
	}
	return 0;
}

void SetMusPos(int xp, int yp)
{
	if(SetMousePoint(xp, yp) != 0)
	{
		error();
	}
}
void GetMusPos(int *xp, int *yp)
{
	if(GetMousePoint(xp, yp) != 0)
	{
		error();
	}
}
int GetMusBtn(void) // ret : (左: 1) | (右: 2)
{
	int status = GetMouseInput();

	return
		((status & MOUSE_INPUT_LEFT)  ? 1 : 0) |
		((status & MOUSE_INPUT_RIGHT) ? 2 : 0);
}

static int GCLastStatus;

int GetClick(void)
{
	int status = GetMusBtn();
	int click;

	click = status & ~GCLastStatus;
	GCLastStatus = status;
	return click;
}
int IsMusIn(int x1, int x2, int y1, int y2)
{
	int mx;
	int my;

	GetMusPos(&mx, &my);

	return
		x1 <= mx && mx <= x2 &&
		y1 <= my && my <= y2;
}
int IsMusNear(int x, int y, int r)
{
	int mx;
	int my;

	GetMusPos(&mx, &my);

	x -= mx;
	y -= my;
	return x * x + y * y <= r * r;
}

void DispMusCursor(int doDisp)
{
	if(doDisp)
	{
		SetMouseDispFlag(true);
	}
	else
	{
#if 0
		SetMouseDispFlag(false);
#else
		SetMouseDispFlag(true);
		SetMusPos(799, 599);
#endif
	}
}

#define STRFILE_RESRC "Resource.dxa"
#define STRDIR_RESRC_PRO "Resource"
#define STRDIR_RESRC_DEV "..\\..\\dat"

char *ResourcePath(char *innerPath)
{
	static char *rootDir;
	static char *path;

	if(!rootDir)
	{
		if(!_access(STRFILE_RESRC, 0)) // ? リソース有り
			rootDir = STRDIR_RESRC_PRO;
		else
			rootDir = STRDIR_RESRC_DEV;
	}
	if(path)
		memFree(path);

	path = xcout("%s\\%s", rootDir, innerPath);
	return path;
}
char *x_ResourcePath(char *innerPath)
{
	return strx(ResourcePath(innerPath));
}
char *ResourcePath_x(char *innerPath)
{
	char *out = ResourcePath(innerPath);
	memFree(innerPath);
	return out;
}

int LDrawRGB;
void LDraw(char *ljchr, int x, int y)
{
	int color = GetColor(
		(LDrawRGB >> 16) & 0xff,
		(LDrawRGB >>  8) & 0xff,
		(LDrawRGB >>  0) & 0xff
		);

	if(DrawString(x, y, ljchr, color) != 0)
	{
		error();
	}
}

//#define KUSSETSU_MOTO 0.2 // 0.0 < < 0.1
//#define KUSSETSU_SAKI 0.8 // 0.0 < < 0.1
#define KUSSETSU_MOTO 0.2
#define KUSSETSU_SAKI 0.8

void SetDougaVolume(int mov, double vol)
{
	// ボリュームと実音量の対応がおかしい？、高いボリュームに敏感に反応する。わけがわからないよ、、、
	if(vol < KUSSETSU_MOTO)
	{
		vol *= KUSSETSU_SAKI;
		vol /= KUSSETSU_MOTO;
	}
	else // KUSSETSU_MOTO <= vol
	{
		vol = 1.0 - vol;
		vol *= (1.0 - KUSSETSU_SAKI);
		vol /= (1.0 - KUSSETSU_MOTO);
		vol = 1.0 - vol;
	}
	m_range(vol, 0.0, 1.0); // 2bs

	SetMovieVolumeToGraph((int)(vol * 10000.0), mov);
}
void PlayDouga(char *file)
{
	int mov = LoadGraph(file);
//	int rate = GetOneFrameTimeMovieToGraph(mov);

	SetDougaVolume(mov, Dc->BGMVolume * 0.75); // mpgの音が少しでかい。
//	SetDougaVolume(mov, Dc->BGMVolume);
//	SetMovieVolumeToGraph(10000, mov);
//	SetMovieVolumeToGraph((int)(Dc->BGMVolume * 10000.0), mov);
	PlayMovieToGraph(mov);

	double vol = 0.2;

	Sys_EscapeDisable = 1;

	int maskcnt = 10;

	while(GetMovieStateToGraph(mov) == 1)
	{
		int chuudan = 0;

#if LOG_ENABLED
		if(IsHit(KEY_INPUT_1)) SetDougaVolume(mov, Dc->BGMVolume -= 0.01);
		if(IsHit(KEY_INPUT_2)) SetDougaVolume(mov, Dc->BGMVolume += 0.01);
		m_range(Dc->BGMVolume, 0.0, 1.0);

		printfDx("vol:%f\n", Dc->BGMVolume);
#endif

		if(GetHit(INP_ROT_DECIDE) || GetHit(INP_ROT_CANCEL) ||
			GetHit(INP_DECIDE) || GetHit(INP_CANCEL) ||
			GetClick() ||
			Sys_EscapePressed)
		{
			chuudan = 1;
		}

		if(chuudan)
		{
			PauseMovieToGraph(mov);
			break;
		}
		DrawExtendGraph(0, 0, 800, 600, mov, 0);

		/*
			最初の数フレームに変な映像が入り込む。
			DxLib 3.07a にしてから、初めて見た気がする。デグレった？
		*/
		if(maskcnt)
		{
			SimpleDraw(Dc->PicBlackWall, 0, 0, 0);
			maskcnt--;
		}
		SwapFrame();
	}
	DeleteGraph(mov);

	if(Sys_EscapePressed)
	{
		termination(0);
	}
	Sys_EscapeDisable = 0;
}
void GameTermination(void)
{
	termination(0);
}

char *GetBuiltStamp(void)
{
	static char *stamp;

	if(!stamp)
	{
		char *line = __DATE__ " " __TIME__;

		errorCase(strlen(line) != BUILT_STAMP_LEN);
		stamp = strx(line);
		line2domain(stamp);
	}
	return stamp;
}

static void KillCommonEffect(CommonEffect_t *i)
{
	memFree(i);
}
static int CommonEffectAct(CommonEffect_t *i)
{
	i->FrmCnt++;
	errorCase(15 * 60 < i->FrmCnt); // 15 sec -> 長すぎ。

	// 加速
	i->Xa += i->Xaa;
	i->Ya += i->Yaa;
	i->Ra += i->Raa;
	i->Za += i->Zaa;
	i->Aa += i->Aaa;

	// 移動
	i->X += i->Xa;
	i->Y += i->Ya;
	i->R += i->Ra;
	i->Z += i->Za;
	i->A += i->Aa;

	m_range(i->X, -100.0, 900.0);
	m_range(i->Y, -100.0, 700.0);
	m_range(i->R, (double)-IMAX, (double)IMAX); // たぶん要らない
	m_range(i->Z, 0.1, 10.0);

	if(i->A < 0.0)
	{
		KillCommonEffect(i);
		return 0;
	}
	m_range(i->A, 0.0, 1.0);

	SetAntiAlias(1);
	SetAlpha(i->A, i->KasanMode);

	DppInit(i->Pic, i->X, i->Y, 1);
	DppRotate(i->R);
	DppZoom(i->Z);
	DppDraw();

	ResetAlpha();
	SetAntiAlias(0);

	if(i->LifeCount)
	{
		m_countDown(i->LifeCount);

		if(i->LifeCount == 0)
		{
			double aa = i->DeadAa;

			if(aa <= 0.0)
				aa = 1.0 / -30;

			i->Aa = aa;
			i->Aaa = 0.0;
		}
	}
	if(i->KasokuRate != 0.0)
	{
		i->Xa *= i->KasokuRate;
		i->Ya *= i->KasokuRate;
	}
	return 1;
}
CommonEffect_t *AddCommonEffect(
	int pic,
	int kasanMode,
	double x,
	double y,
	double r,
	double z,
	double a,
	double xa,
	double ya,
	double ra,
	double za,
	double aa,
	double xaa,
	double yaa,
	double raa,
	double zaa,
	double aaa
	)
{
	CommonEffect_t *i = (CommonEffect_t *)memAlloc(sizeof(CommonEffect_t));

	memset(i, 0x00, sizeof(CommonEffect_t));

	i->Pic = pic;
	i->KasanMode = kasanMode;
	i->X = x;
	i->Y = y;
	i->R = r;
	i->Z = z;
	i->A = a;
	i->Xa = xa;
	i->Ya = ya;
	i->Ra = ra;
	i->Za = za;
	i->Aa = aa;
	i->Xaa = xaa;
	i->Yaa = yaa;
	i->Raa = raa;
	i->Zaa = zaa;
	i->Aaa = aaa;

	AddFrameTask((int (*)(void *))CommonEffectAct, (void *)i);
	return i;
}
