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
		static SubScreen_t *bgScreen = NULL;

		{
			int real_w = Gnd.RealScreen_W;
			int real_h = Gnd.RealScreen_H;

			if(!bgScreen)
				bgScreen = CreateSubScreen(real_w, real_h);

			int w = real_w;
			int h = real_h;

			if(w * SCREEN_H < h * SCREEN_W) // 縦長モニタ -> 縦幅に合わせる
			{
				w = m_d2i(((double)h * SCREEN_W) / SCREEN_H);
			}
			else // 横長モニタ -> 横幅に合わせる
			{
				h = m_d2i(((double)w * SCREEN_H) / SCREEN_W);
			}
			int l = (real_w - w) / 2;
			int t = (real_h - h) / 2;

			ChangeDrawScreen(GetHandle(bgScreen));

			SetDrawBright(30, 30, 30);
//			SetDrawBright(100, 100, 100);
			DrawExtendGraph(
				l,
				t,
				l + w,
				t + h,
				GetHandle(Gnd.MainScreen),
				0
				);
			SetDrawBright(255, 255, 255); // restore

			GraphFilter(
				GetHandle(bgScreen),
				DX_GRAPH_FILTER_GAUSS,
				16,
				5000
				);

//			RestoreDrawScreen();
		}

		ChangeDrawScreen(DX_SCREEN_BACK);

		if(Gnd.ScreenDraw_L || Gnd.ScreenDraw_T) // ? スクリーンサイズより描画領域が小さい。
		{
#if 1
			DrawExtendGraph(
				0,
				0,
				Gnd.RealScreen_W,
				Gnd.RealScreen_H,
				GetHandle(bgScreen),
				0
				);
#else // Hako2 ver.
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
#endif
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
}
void FreezeInput(int frame) // frame: 1 == このフレームのみ, 2 == このフレームと次のフレーム ...
{
	errorCase(frame < 1 || IMAX < frame);
	FreezeInputFrame = frame;
}
