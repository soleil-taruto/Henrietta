#include "all.h"

int IgnoreEscapeKey;

// 他ファイルからは read only {
__int64 FrameStartTime;
int ProcFrame;
int ActFrame; // = 0; ok
int FreezeInputFrame;
int WindowIsActive;
int FrameRateDropCount;
int NoFrameRateDropCount;
// }

static void CheckHz(void)
{
	__int64 currTime = GetCurrTime();
	__int64 diffTime = currTime - FrameStartTime;

	if(diffTime < 15 || 18 < diffTime) // ? frame rate drop
		FrameRateDropCount++;
	else
		NoFrameRateDropCount++;

	FrameStartTime = currTime;
}
void EachFrame(void)
{
	if(!SEEachFrame())
	{
		MusicEachFrame();
	}
	Gnd.EL->ExecuteAllTask();
	CurtainEachFrame();

	if(Gnd.MainScreen && CurrDrawScreenHandle == GetHandle(Gnd.MainScreen))
	{
		ChangeDrawScreen(DX_SCREEN_BACK);

		if(Gnd.ScreenDraw_L || Gnd.ScreenDraw_T) // ? スクリーンサイズより描画領域が小さい。
		{
			int bgp_w = Pic_W(P_BG_PATTERN);
			int bgp_h = Pic_H(P_BG_PATTERN);

			SetDrawBright(30, 30, 30);

			for(int l = 0; l < Gnd.RealScreen_W; l += bgp_w)
			for(int t = 0; t < Gnd.RealScreen_H; t += bgp_h)
			{
				DrawExtendGraph(
					l,
					t,
					l + bgp_w,
					t + bgp_h,
					Pic(P_BG_PATTERN),
					0
					);
			}
			SetDrawBright(255, 255, 255); // restore
		}
		DrawExtendGraph(
			Gnd.ScreenDraw_L,
			Gnd.ScreenDraw_T,
			Gnd.ScreenDraw_L + Gnd.ScreenDraw_W,
			Gnd.ScreenDraw_T + Gnd.ScreenDraw_H,
			GetHandle(Gnd.MainScreen),
			0
			);
	}

	// DxLib >

	ScreenFlip();

	if(!IgnoreEscapeKey && CheckHitKey(KEY_INPUT_ESCAPE) == 1 || ProcessMessage() == -1)
	{
		EndProc();
	}

	// < DxLib

	CheckHz();

	ProcFrame++;
	errorCase(IMAX < ProcFrame); // 192.9日程度でカンスト
	ActFrame++;
	m_countDown(FreezeInputFrame);
	WindowIsActive = IsWindowActive();

	PadEachFrame();
	KeyEachFrame();
	InputEachFrame();
	MouseEachFrame();

	if(Gnd.RealScreen_W != SCREEN_W || Gnd.RealScreen_H != SCREEN_H)
	{
		if(!Gnd.MainScreen)
			Gnd.MainScreen = CreateSubScreen(SCREEN_W, SCREEN_H);

		ChangeDrawScreen(Gnd.MainScreen);
	}
#if LOG_ENABLED
		clsDx();
#endif
}
void FreezeInput(int frame) // frame: 1 == このフレームのみ, 2 == このフレームと次のフレーム ...
{
	errorCase(frame < 1 || IMAX < frame);
	FreezeInputFrame = frame;
}
