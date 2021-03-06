#include "all.h"

static char *GetVersionString(void)
{
	const char *CONCERT_PTN = "{a9a54906-791d-4e1a-8a71-a4c69359cf68}:0.00"; // shared_uuid@g
	return (char *)strchr(CONCERT_PTN, ':') + 1;
}

int ProcMtxHdl;
int DxLibInited;

static void ReleaseProcMtxHdl(void)
{
	mutexRelease(ProcMtxHdl);
	handleClose(ProcMtxHdl);
}
void EndProc(void)
{
	ExportSaveData();
	Gnd_FNLZ();

	if(DxLibInited)
	{
		DxLibInited = 0;
		errorCase(DxLib_End()); // ? 失敗
	}
	termination();
}

void SetScreenPosition_WH(int w, int h);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	memAlloc_INIT();

	{
		ProcMtxHdl = mutexOpen("{c808da96-120f-4de5-846a-8c1c120b4a69}"); // shared_uuid@g -- 全ゲーム同時プレイ禁止のため、global指定

		if(!waitForMillis(ProcMtxHdl, 0))
		{
			handleClose(ProcMtxHdl);
			return 0;
		}
		addFinalizer(ReleaseProcMtxHdl);
	}

	initRnd((int)time(NULL));

	if(argIs("/L"))
	{
		termination(LOG_ENABLED ? 1 : 0);
	}

	Gnd_INIT();
	ImportSaveData();

	// DxLib >

#if LOG_ENABLED
	SetApplicationLogSaveDirectory("C:\\tmp");
#endif
	SetOutApplicationLogValidFlag(LOG_ENABLED); // DxLib のログを出力 1: する 0: しない

	SetAlwaysRunFlag(1); // ? 非アクティブ時に 1: 動く 0: 止まる

	SetMainWindowText(xcout(
#if LOG_ENABLED
		"(LOG_ENABLED) "
#endif
		"%s / %s"
		,GetDatString(DATSTR_APPNAME)
		,GetVersionString()
		));

//	SetGraphMode(SCREEN_W, SCREEN_H, 32);
	SetGraphMode(Gnd.RealScreen_W, Gnd.RealScreen_H, 32);
	ChangeWindowMode(1); // 1: ウィンドウ 0: フルスクリーン

//	SetFullSceneAntiAliasingMode(4, 2); // 適当な値が分からん。フルスクリーン廃止したので不要

	SetWindowIconID(333);

	errorCase(DxLib_Init()); // ? 失敗

	DxLibInited = 1;

	SetMouseDispFlag(Gnd.ShowMouseCursorMode); // ? マウスを表示 1: する 0: しない
	SetWindowSizeChangeEnableFlag(0); // ウィンドウの右下をドラッグで伸縮 1: する 0: しない

	SetDrawScreen(DX_SCREEN_BACK);
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	SetScreenPosition_WH(Gnd.RealScreen_W, Gnd.RealScreen_H);

	// < DxLib

	// *_Reset
	{
		DPE_Reset();
		CEE_Reset();
		PE_Reset();
	}

#if LOG_ENABLED // test
	{
		char *b = na_(char, 32);
		int s = 32;
		camellia_decrypt_extend(b, s, 1);
		memFree(b);
	}
#endif

	LOGPOS();
	ProcMain();
	LOGPOS();

	EndProc();
	return 0; // dummy
}

// DxPrv_ >

static int DxPrv_GetMouseDispMode(void)
{
	return GetMouseDispFlag() ? 1 : 0;
}
static void DxPrv_SetMouseDispMode(int mode)
{
	SetMouseDispFlag(mode ? 1 : 0);
}
static void UnloadGraph(int &hdl)
{
	if(hdl != -1)
	{
		errorCase(DeleteGraph(hdl)); // ? 失敗
		hdl = -1;
	}
}
static void GetMonitorLT(int *out_l, int *out_t)
{
	int l;
	int t;
	int w;
	int h;
	int p1;
	int p2;

	GetDefaultState(&w, &h, &p1, &p2, &l, &t);

	errorCase(
		w < 1 || IMAX < w ||
		h < 1 || IMAX < h ||
		l < -IMAX || IMAX < l ||
		t < -IMAX || IMAX < t
		);

	*out_l = l;
	*out_t = t;
}
static void SetScreenPosition(int l, int t)
{
	SetWindowPosition(l, t);

	POINT p;

	p.x = 0;
	p.y = 0;

	ClientToScreen(GetMainWindowHandle(), &p);

	int pToTrgX = l - (int)p.x;
	int pToTrgY = t - (int)p.y;

	SetWindowPosition(l + pToTrgX, t + pToTrgY);
}
static void SetScreenPosition_WH(int w, int h)
{
	int real_w = GetSystemMetrics(SM_CXSCREEN);
	int real_h = GetSystemMetrics(SM_CYSCREEN);

	int l = (real_w - w) / 2;
	int t = (real_h - h) / 2;

	{
		int mon_l;
		int mon_t;

		GetMonitorLT(&mon_l, &mon_t);

		l += mon_l;
		t += mon_t;
	}

	SetScreenPosition(l, t);
}
static void DxPrv_SetScreenSize(int w, int h)
{
	int mdm = GetMouseDispMode();

	UnloadAllPicResHandle();
	UnloadAllSubScreenHandle();

	errorCase(SetGraphMode(w, h, 32) != DX_CHANGESCREEN_OK); // ? 失敗

	SetDrawScreen(DX_SCREEN_BACK);
	SetDrawMode(DX_DRAWMODE_BILINEAR);

	SetMouseDispMode(mdm);

	SetScreenPosition_WH(w, h);
}

// < DxPrv_

int GetMouseDispMode(void)
{
	return DxPrv_GetMouseDispMode();
}
void SetMouseDispMode(int mode)
{
	DxPrv_SetMouseDispMode(mode);
}
void ApplyScreenSize(void)
{
	DxPrv_SetScreenSize(Gnd.RealScreen_W, Gnd.RealScreen_H);
}
void SetScreenSize(int w, int h)
{
	m_range(w, SCREEN_W, SCREEN_W_MAX);
	m_range(h, SCREEN_H, SCREEN_H_MAX);

	if(Gnd.RealScreen_W != w || Gnd.RealScreen_H != h)
	{
		Gnd.RealScreen_W = w;
		Gnd.RealScreen_H = h;

		Gnd.ScreenDraw_L = 0;
		Gnd.ScreenDraw_T = 0;
		Gnd.ScreenDraw_W = w;
		Gnd.ScreenDraw_H = h;

		ApplyScreenSize();
	}
}
