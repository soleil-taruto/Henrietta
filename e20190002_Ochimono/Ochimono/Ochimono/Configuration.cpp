#include "StdInc.h"

static int UseKeyList[] =
{
//	KEY_INPUT_RETURN, // デフォ決定

	KEY_INPUT_A,
	KEY_INPUT_B,
	KEY_INPUT_C,
	KEY_INPUT_D,
	KEY_INPUT_E,
	KEY_INPUT_F,
	KEY_INPUT_G,
	KEY_INPUT_H,
	KEY_INPUT_I,
	KEY_INPUT_J,
	KEY_INPUT_K,
	KEY_INPUT_L,
	KEY_INPUT_M,
	KEY_INPUT_N,
	KEY_INPUT_O,
	KEY_INPUT_P,
	KEY_INPUT_Q,
	KEY_INPUT_R,
	KEY_INPUT_S,
	KEY_INPUT_T,
	KEY_INPUT_U,
	KEY_INPUT_V,
	KEY_INPUT_W,
	KEY_INPUT_X,
	KEY_INPUT_Y,
	KEY_INPUT_Z,

	KEY_INPUT_1,
	KEY_INPUT_2,
	KEY_INPUT_3,
	KEY_INPUT_4,
	KEY_INPUT_5,
	KEY_INPUT_6,
	KEY_INPUT_7,
	KEY_INPUT_8,
	KEY_INPUT_9,
	KEY_INPUT_0,

	KEY_INPUT_AT,
	KEY_INPUT_LBRACKET,
	KEY_INPUT_RBRACKET,
	KEY_INPUT_COLON,
	KEY_INPUT_SEMICOLON,
	KEY_INPUT_COMMA,
	KEY_INPUT_PERIOD,
	KEY_INPUT_SLASH,
	KEY_INPUT_BACKSLASH,

	KEY_INPUT_NUMPAD0,
	KEY_INPUT_NUMPAD1,
	KEY_INPUT_NUMPAD2,
	KEY_INPUT_NUMPAD3,
	KEY_INPUT_NUMPAD4,
	KEY_INPUT_NUMPAD5,
	KEY_INPUT_NUMPAD6,
	KEY_INPUT_NUMPAD7,
	KEY_INPUT_NUMPAD8,
	KEY_INPUT_NUMPAD9,

	KEY_INPUT_DIVIDE,
	KEY_INPUT_MULTIPLY,
	KEY_INPUT_SUBTRACT,
	KEY_INPUT_ADD,
	KEY_INPUT_DECIMAL,

	KEY_INPUT_DOWN,
	KEY_INPUT_LEFT,
	KEY_INPUT_RIGHT,
	KEY_INPUT_UP,
};

#define KEY_MAX (lengthof(UseKeyList))

static int PicKeyList[KEY_MAX];
static int ScreenModeLock; // 0 以外のとき、スクリーンモード変更禁止

static void InitPicKeyList(void)
{
	int i;

	i = 0;
	PicKeyList[i++] = Dc->PicAscii['A'];
	PicKeyList[i++] = Dc->PicAscii['B'];
	PicKeyList[i++] = Dc->PicAscii['C'];
	PicKeyList[i++] = Dc->PicAscii['D'];
	PicKeyList[i++] = Dc->PicAscii['E'];
	PicKeyList[i++] = Dc->PicAscii['F'];
	PicKeyList[i++] = Dc->PicAscii['G'];
	PicKeyList[i++] = Dc->PicAscii['H'];
	PicKeyList[i++] = Dc->PicAscii['I'];
	PicKeyList[i++] = Dc->PicAscii['J'];
	PicKeyList[i++] = Dc->PicAscii['K'];
	PicKeyList[i++] = Dc->PicAscii['L'];
	PicKeyList[i++] = Dc->PicAscii['M'];
	PicKeyList[i++] = Dc->PicAscii['N'];
	PicKeyList[i++] = Dc->PicAscii['O'];
	PicKeyList[i++] = Dc->PicAscii['P'];
	PicKeyList[i++] = Dc->PicAscii['Q'];
	PicKeyList[i++] = Dc->PicAscii['R'];
	PicKeyList[i++] = Dc->PicAscii['S'];
	PicKeyList[i++] = Dc->PicAscii['T'];
	PicKeyList[i++] = Dc->PicAscii['U'];
	PicKeyList[i++] = Dc->PicAscii['V'];
	PicKeyList[i++] = Dc->PicAscii['W'];
	PicKeyList[i++] = Dc->PicAscii['X'];
	PicKeyList[i++] = Dc->PicAscii['Y'];
	PicKeyList[i++] = Dc->PicAscii['Z'];

	PicKeyList[i++] = Dc->PicAscii['!'];
	PicKeyList[i++] = Dc->PicAscii['"'];
	PicKeyList[i++] = Dc->PicAscii['#'];
	PicKeyList[i++] = Dc->PicAscii['$'];
	PicKeyList[i++] = Dc->PicAscii['%'];
	PicKeyList[i++] = Dc->PicAscii['&'];
	PicKeyList[i++] = Dc->PicAscii['\''];
	PicKeyList[i++] = Dc->PicAscii['('];
	PicKeyList[i++] = Dc->PicAscii[')'];
	PicKeyList[i++] = Dc->PicAscii['ｦ'];

	PicKeyList[i++] = Dc->PicAscii['@'];
	PicKeyList[i++] = Dc->PicAscii['['];
	PicKeyList[i++] = Dc->PicAscii[']'];
	PicKeyList[i++] = Dc->PicAscii[':'];
	PicKeyList[i++] = Dc->PicAscii[';'];
	PicKeyList[i++] = Dc->PicAscii[','];
	PicKeyList[i++] = Dc->PicAscii['.'];
	PicKeyList[i++] = Dc->PicAscii['/'];
	PicKeyList[i++] = Dc->PicAscii['\\'];

	PicKeyList[i++] = Dc->PicAscii['0'];
	PicKeyList[i++] = Dc->PicAscii['1'];
	PicKeyList[i++] = Dc->PicAscii['2'];
	PicKeyList[i++] = Dc->PicAscii['3'];
	PicKeyList[i++] = Dc->PicAscii['4'];
	PicKeyList[i++] = Dc->PicAscii['5'];
	PicKeyList[i++] = Dc->PicAscii['6'];
	PicKeyList[i++] = Dc->PicAscii['7'];
	PicKeyList[i++] = Dc->PicAscii['8'];
	PicKeyList[i++] = Dc->PicAscii['9'];

	PicKeyList[i++] = Dc->PicAscii['/'];
	PicKeyList[i++] = Dc->PicAscii['*'];
	PicKeyList[i++] = Dc->PicAscii['-'];
	PicKeyList[i++] = Dc->PicAscii['+'];
	PicKeyList[i++] = Dc->PicAscii['.'];

	PicKeyList[i++] = Dc->PicDir[2];
	PicKeyList[i++] = Dc->PicDir[4];
	PicKeyList[i++] = Dc->PicDir[6];
	PicKeyList[i++] = Dc->PicDir[8];

	errorCase(i != KEY_MAX);
}
void Pub_InitPicKeyList(void)
{
	InitPicKeyList();
}

static int GetPressKeyIndex(void) // ret : KEY_MAX == 押してない。
{
	int index;

	for(index = 0; index < KEY_MAX; index++)
	{
		if(CheckHitKey(UseKeyList[index]))
		{
			break;
		}
	}
	return index;
}
int Pub_GetPressKeyIndex(void)
{
	int keyidx = GetPressKeyIndex();

	if(keyidx == KEY_MAX)
		keyidx = -1;

	return keyidx;
}
int Pub_GetUseKey(int keyidx)
{
	return UseKeyList[keyidx];
}
static int KI2Index(int keyInput)
{
	int index;

	for(index = 0; index < KEY_MAX; index++)
	{
		if(UseKeyList[index] == keyInput)
		{
			return index;
		}
	}
	error();
	return 0; // dummy
}

#define VOLUME_X1 290
#define VOLUME_X2 690
#define VOLUME_WIDTH (VOLUME_X2 - VOLUME_X1)
#define BGM_VOL_Y 330
#define SE_VOL_Y 380
#define MESS_SP_Y 180
#define AUTO_SP_Y 230
#define VOLUME_R 30

static void PadButtonConfig(int padIndex);

void CfgMain(void)
{
	int bgmGripMode = 0;
	int seGripMode  = 0;
	int messSpGripMode = 0;
	int autoSpGripMode = 0;

	int frmidx;
	double frmpct;

	int padCount = GetJoypadNum();
	m_range(padCount, 0, 2);

	frameloop(frmidx, frmpct, 30)
	{
		SimpleDraw(Dc->PicBlackWall, 0, 0, 0);

		SetAlpha(frmpct);
		SimpleDraw(Dc->PicCfgWall, 0, 0, 0);
		ResetAlpha();

		SwapFrame();
	}
	InitPicKeyList();
	Input2SettInfo();

	int frameCounter;
	for(frameCounter = 0; ; frameCounter++)
	{
		if(GetHit(INP_ROT_DECIDE) || GetHit(INP_ROT_CANCEL))
			break;

		SimpleDraw(Dc->PicCfgWall, 0, 0, 0);

		// 1P
		SimpleDraw(PicKeyList[KI2Index(SettInfo.KeySettInfos[KSIIDX_P1][KSIIDX_KB].RotL)], 165, 448, 1);
		SimpleDraw(PicKeyList[KI2Index(SettInfo.KeySettInfos[KSIIDX_P1][KSIIDX_KB].RotR)], 365, 448, 1);
		SimpleDraw(PicKeyList[KI2Index(SettInfo.KeySettInfos[KSIIDX_P1][KSIIDX_KB].Dir6)], 465, 448, 1);
		SimpleDraw(PicKeyList[KI2Index(SettInfo.KeySettInfos[KSIIDX_P1][KSIIDX_KB].Dir4)], 535, 448, 1);
		SimpleDraw(PicKeyList[KI2Index(SettInfo.KeySettInfos[KSIIDX_P1][KSIIDX_KB].Dir8)], 600, 448, 1);
		SimpleDraw(PicKeyList[KI2Index(SettInfo.KeySettInfos[KSIIDX_P1][KSIIDX_KB].Dir2)], 660, 448, 1);
		SimpleDraw(PicKeyList[KI2Index(SettInfo.KeySettInfos[KSIIDX_P1][KSIIDX_KB].Bomb)], 755, 448, 1);

		int d12p = 66;

		// 2P
		SimpleDraw(PicKeyList[KI2Index(SettInfo.KeySettInfos[KSIIDX_P2][KSIIDX_KB].RotL)], 165, 448 + d12p, 1);
		SimpleDraw(PicKeyList[KI2Index(SettInfo.KeySettInfos[KSIIDX_P2][KSIIDX_KB].RotR)], 365, 448 + d12p, 1);
		SimpleDraw(PicKeyList[KI2Index(SettInfo.KeySettInfos[KSIIDX_P2][KSIIDX_KB].Dir6)], 465, 448 + d12p, 1);
		SimpleDraw(PicKeyList[KI2Index(SettInfo.KeySettInfos[KSIIDX_P2][KSIIDX_KB].Dir4)], 535, 448 + d12p, 1);
		SimpleDraw(PicKeyList[KI2Index(SettInfo.KeySettInfos[KSIIDX_P2][KSIIDX_KB].Dir8)], 600, 448 + d12p, 1);
		SimpleDraw(PicKeyList[KI2Index(SettInfo.KeySettInfos[KSIIDX_P2][KSIIDX_KB].Dir2)], 660, 448 + d12p, 1);
		SimpleDraw(PicKeyList[KI2Index(SettInfo.KeySettInfos[KSIIDX_P2][KSIIDX_KB].Bomb)], 755, 448 + d12p, 1);

		int on_window = 0;
		int on_fullScreen = 0;
		int on_kidoku = 0;
		int on_midoku = 0;
		int on_bgmVolume = 0;
		int on_seVolume = 0;
		int on_messSpeed = 0;
		int on_autoSpeed = 0;
		int on_p1RotL = 0;
		int on_p1RotR = 0;
		int on_p1Dir2 = 0;
		int on_p1Dir4 = 0;
		int on_p1Dir6 = 0;
		int on_p1Dir8 = 0;
		int on_p1Bomb = 0;
		int on_modoru = 0;

		// 2P
		int on_p2RotL = 0;
		int on_p2RotR = 0;
		int on_p2Dir2 = 0;
		int on_p2Dir4 = 0;
		int on_p2Dir6 = 0;
		int on_p2Dir8 = 0;
		int on_p2Bomb = 0;

		if(IsMusIn(300, 420, 110, 140)) on_window = 1;
		if(IsMusIn(485, 640, 110, 140)) on_fullScreen = 1;
		if(IsMusIn(300, 420, 250, 290)) on_kidoku = 1;
		if(IsMusIn(485, 640, 250, 290)) on_midoku = 1;
		if(IsMusIn( 20, 160, 440, 480)) on_p1RotL = 1;
		if(IsMusIn(190, 370, 440, 480)) on_p1RotR = 1;
		if(IsMusIn(380, 460, 440, 480)) on_p1Dir6 = 1;
		if(IsMusIn(500, 530, 440, 480)) on_p1Dir4 = 1;
		if(IsMusIn(560, 590, 440, 480)) on_p1Dir8 = 1;
		if(IsMusIn(620, 650, 440, 480)) on_p1Dir2 = 1;
		if(IsMusIn(680, 760, 440, 480)) on_p1Bomb = 1;
		if(IsMusIn(680, 760, 550, 590)) on_modoru = 1;

		// 2P
		if(IsMusIn( 20, 160, 440 + d12p, 480 + d12p)) on_p2RotL = 1;
		if(IsMusIn(190, 370, 440 + d12p, 480 + d12p)) on_p2RotR = 1;
		if(IsMusIn(380, 460, 440 + d12p, 480 + d12p)) on_p2Dir6 = 1;
		if(IsMusIn(500, 530, 440 + d12p, 480 + d12p)) on_p2Dir4 = 1;
		if(IsMusIn(560, 590, 440 + d12p, 480 + d12p)) on_p2Dir8 = 1;
		if(IsMusIn(620, 650, 440 + d12p, 480 + d12p)) on_p2Dir2 = 1;
		if(IsMusIn(680, 760, 440 + d12p, 480 + d12p)) on_p2Bomb = 1;

		if(!SettInfo.FullScreenMode) SimpleDraw(Dc->PicCfgWindow,     316, 110, 1);
		if( SettInfo.FullScreenMode) SimpleDraw(Dc->PicCfgFullScreen, 491, 112, 1);

		if(!SettInfo.Midokumotobasu) SimpleDraw(Dc->PicCfgKidoku, 304, 260, 1);
		if( SettInfo.Midokumotobasu) SimpleDraw(Dc->PicCfgMidoku, 499, 260, 1);

#define K_FZ_X -1
#define K_FZ_Y -1

		// 1P + modoru
		if(on_p1RotL) SimpleDraw(Dc->PicCfgRotL,    25 + K_FZ_X, 442 + K_FZ_Y, 1);
		if(on_p1RotR) SimpleDraw(Dc->PicCfgRotR,   198 + K_FZ_X, 446 + K_FZ_Y, 1);
		if(on_p1Dir6) SimpleDraw(Dc->PicCfgDir6,   386 + K_FZ_X, 442 + K_FZ_Y, 1);
		if(on_p1Dir4) SimpleDraw(Dc->PicCfgDir4,   501 + K_FZ_X, 442 + K_FZ_Y, 1);
		if(on_p1Dir8) SimpleDraw(Dc->PicCfgDir8,   564 + K_FZ_X, 442 + K_FZ_Y, 1);
		if(on_p1Dir2) SimpleDraw(Dc->PicCfgDir2,   626 + K_FZ_X, 442 + K_FZ_Y, 1);
		if(on_p1Bomb) SimpleDraw(Dc->PicCfgBomb,   684 + K_FZ_X, 442 + K_FZ_Y, 1);
		if(on_modoru) SimpleDraw(Dc->PicCfgModoru, 682 + K_FZ_X, 549 + K_FZ_Y, 1);

		// 2P
		if(on_p2RotL) SimpleDraw(Dc->PicCfgRotL,    25 + K_FZ_X, 442 + d12p + K_FZ_Y, 1);
		if(on_p2RotR) SimpleDraw(Dc->PicCfgRotR,   198 + K_FZ_X, 446 + d12p + K_FZ_Y, 1);
		if(on_p2Dir6) SimpleDraw(Dc->PicCfgDir6,   386 + K_FZ_X, 442 + d12p + K_FZ_Y, 1);
		if(on_p2Dir4) SimpleDraw(Dc->PicCfgDir4,   501 + K_FZ_X, 442 + d12p + K_FZ_Y, 1);
		if(on_p2Dir8) SimpleDraw(Dc->PicCfgDir8,   564 + K_FZ_X, 442 + d12p + K_FZ_Y, 1);
		if(on_p2Dir2) SimpleDraw(Dc->PicCfgDir2,   626 + K_FZ_X, 442 + d12p + K_FZ_Y, 1);
		if(on_p2Bomb) SimpleDraw(Dc->PicCfgBomb,   684 + K_FZ_X, 442 + d12p + K_FZ_Y, 1);

		int mx;
		int my;
		GetMusPos(&mx, &my);

		double vx = (double)(mx - VOLUME_X1) / VOLUME_WIDTH;
		int btx;
		int stx;
		int mtx;
		int atx;

		m_range(vx, 0.0, 1.0);

		SimpleDrawCenter(Dc->PicCfgTsumami,
			btx = (int)(VOLUME_X1 + VOLUME_WIDTH * Dc->BGMVolume),
			BGM_VOL_Y,
			1
			);
		SimpleDrawCenter(Dc->PicCfgTsumami,
			stx = (int)(VOLUME_X1 + VOLUME_WIDTH * Dc->SeVolume),
			SE_VOL_Y,
			1
			);
		SimpleDrawCenter(Dc->PicCfgTsumami,
			mtx = (int)(VOLUME_X1 + VOLUME_WIDTH * SettInfo.MessSpeed),
			MESS_SP_Y,
			1
			);
		SimpleDrawCenter(Dc->PicCfgTsumami,
			atx = (int)(VOLUME_X1 + VOLUME_WIDTH * SettInfo.AutoSpeed),
			AUTO_SP_Y,
			1
			);

		if(IsMusNear(btx, BGM_VOL_Y, VOLUME_R)) on_bgmVolume = 1;
		if(IsMusNear(stx, SE_VOL_Y,  VOLUME_R)) on_seVolume = 1;
		if(IsMusNear(mtx, MESS_SP_Y, VOLUME_R)) on_messSpeed = 1;
		if(IsMusNear(atx, AUTO_SP_Y, VOLUME_R)) on_autoSpeed = 1;

		int on_pad1 = 0;
		int on_pad2 = 0;

#define PAD_X 10
#define PAD_X_SIZE 16
#define PAD_X_STEP (PAD_X_SIZE + 3)
#define PAD_Y 570
#define PAD_Y_SIZE 21

		if(IsMusIn(PAD_X + PAD_X_STEP * 0, PAD_X + PAD_X_STEP * 4, PAD_Y, PAD_Y + PAD_Y_SIZE)) on_pad1 = 1;
		if(IsMusIn(PAD_X + PAD_X_STEP * 5, PAD_X + PAD_X_STEP * 9, PAD_Y, PAD_Y + PAD_Y_SIZE)) on_pad2 = 1;

		if(padCount)
		{
			int padZure = on_pad1 ? -1 : 0;

			SimpleDraw(Dc->PicAscii['P'], PAD_X + PAD_X_STEP * 0 + padZure, PAD_Y + padZure, 1);
			SimpleDraw(Dc->PicAscii['A'], PAD_X + PAD_X_STEP * 1 + padZure, PAD_Y + padZure, 1);
			SimpleDraw(Dc->PicAscii['D'], PAD_X + PAD_X_STEP * 2 + padZure, PAD_Y + padZure, 1);

			if(padCount == 2)
			{
				SimpleDraw(Dc->PicAscii['1'], PAD_X + PAD_X_STEP * 3 + padZure, PAD_Y + padZure, 1);

				padZure = on_pad2 ? -1 : 0;

				SimpleDraw(Dc->PicAscii['P'], PAD_X + PAD_X_STEP * 5 + padZure, PAD_Y + padZure, 1);
				SimpleDraw(Dc->PicAscii['A'], PAD_X + PAD_X_STEP * 6 + padZure, PAD_Y + padZure, 1);
				SimpleDraw(Dc->PicAscii['D'], PAD_X + PAD_X_STEP * 7 + padZure, PAD_Y + padZure, 1);
				SimpleDraw(Dc->PicAscii['2'], PAD_X + PAD_X_STEP * 8 + padZure, PAD_Y + padZure, 1);
			}
		}

		int mLClk = GetMusBtn() & 1;
		if(!mLClk)
		{
			bgmGripMode = 0;
			seGripMode = 0;
			messSpGripMode = 0;
			autoSpGripMode = 0;
		}
		else
		{
			if(ScreenModeLock == 0)
			{
				int changed = 0;

				if(on_window && SettInfo.FullScreenMode)
				{
//					ChangeWindowMode(true); // true: Window, false: Full Screen
					changed = 1;
				}
				if(on_fullScreen && !SettInfo.FullScreenMode)
				{
//					ChangeWindowMode(false); // true: Window, false: Full Screen
					changed = 1;
				}

				if(changed)
				{
					SettInfo.FullScreenMode = !SettInfo.FullScreenMode;

					int w;
					int h;

					if(SettInfo.FullScreenMode)
					{
						w = GetSystemMetrics(SM_CXSCREEN);
						h = GetSystemMetrics(SM_CYSCREEN);
					}
					else
					{
						w = 800;
						h = 600;
					}
					SetGraphMode(w, h, 32);

					// スクリーンモード変更に伴う、各種再初期化
					{
						DispMusCursor(true);

						DcInit_LoadAllPictureSafe();
						InitPicKeyList();
						GC_ProcInitReset();
						EDEx_ScreenModeChanged();
						SFS_Reset();

						SetDrawScreen(DX_SCREEN_BACK);

						Pub_MainScreen = -1;
					}

					AdjustScreenPosition(w, h);
					SetDrawScreen_LTWH(SettInfo.FullScreenMode);
				}
			}
			if(on_kidoku) SettInfo.Midokumotobasu = 0;
			if(on_midoku) SettInfo.Midokumotobasu = 1;

			if( bgmGripMode || on_bgmVolume) {
				bgmGripMode = 1;

				Dc->BGMVolume = vx;
				SetBGMVolume(Dc->BGMVolume);
			}
			if( seGripMode || on_seVolume) {
				seGripMode = 1;

				Dc->SeVolume = vx;
				SetSeVolume(Dc->SeVolume);

				if(frameCounter % 40 == 0)
				{
					int testselst[] = {
						Dc->SeIkuyo,
						Dc->SeUwa,
						Dc->SeEi,
						Dc->SeUwa,
						Dc->SeEi,
						Dc->SeSore,
						Dc->SeHa,
						Dc->SeGo,
						Dc->SeSpCmd,
//						Dc->SeRucca,
						Dc->SeJReady,
						Dc->SeRen3,
						Dc->SeRen5,
						Dc->SeRen7,
						Dc->SeWin,
						Dc->SeLose,
					};
					int testseidx = rndbnd(0, lengthof(testselst) - 1);

					PlaySe(testselst[testseidx]);
				}
			}
			if( messSpGripMode || on_messSpeed) {
				messSpGripMode = 1;
				SettInfo.MessSpeed = vx;
			}
			if( autoSpGripMode || on_autoSpeed) {
				autoSpGripMode = 1;
				SettInfo.AutoSpeed = vx;
			}

			if( on_p1RotL || on_p2RotL ||
				on_p1RotR || on_p2RotR ||
				on_p1Dir6 || on_p2Dir6 ||
				on_p1Dir4 || on_p2Dir4 ||
				on_p1Dir8 || on_p2Dir8 ||
				on_p1Dir2 || on_p2Dir2 ||
				on_p1Bomb || on_p2Bomb )
			{
				int keyidx;

				for(; ; )
				{
					keyidx = GetPressKeyIndex();
					if(keyidx < KEY_MAX)
					{
						break;
					}

					SimpleDraw(Dc->PicCfgWall, 0, 0, 0);

					SetAlpha(0.5);
					SimpleDraw(Dc->PicBlackWall, 0, 0, 0);
					ResetAlpha();

					SimpleDrawCenter(Dc->PicCfgPressKey, 400, 300, 1);

					SwapFrame();
				}

				// 1P
				if(on_p1RotL) SettInfo.KeySettInfos[KSIIDX_P1][KSIIDX_KB].RotL = UseKeyList[keyidx];
				if(on_p1RotR) SettInfo.KeySettInfos[KSIIDX_P1][KSIIDX_KB].RotR = UseKeyList[keyidx];
				if(on_p1Dir6) SettInfo.KeySettInfos[KSIIDX_P1][KSIIDX_KB].Dir6 = UseKeyList[keyidx];
				if(on_p1Dir4) SettInfo.KeySettInfos[KSIIDX_P1][KSIIDX_KB].Dir4 = UseKeyList[keyidx];
				if(on_p1Dir8) SettInfo.KeySettInfos[KSIIDX_P1][KSIIDX_KB].Dir8 = UseKeyList[keyidx];
				if(on_p1Dir2) SettInfo.KeySettInfos[KSIIDX_P1][KSIIDX_KB].Dir2 = UseKeyList[keyidx];
				if(on_p1Bomb) SettInfo.KeySettInfos[KSIIDX_P1][KSIIDX_KB].Bomb = UseKeyList[keyidx];

				// 2P
				if(on_p2RotL) SettInfo.KeySettInfos[KSIIDX_P2][KSIIDX_KB].RotL = UseKeyList[keyidx];
				if(on_p2RotR) SettInfo.KeySettInfos[KSIIDX_P2][KSIIDX_KB].RotR = UseKeyList[keyidx];
				if(on_p2Dir6) SettInfo.KeySettInfos[KSIIDX_P2][KSIIDX_KB].Dir6 = UseKeyList[keyidx];
				if(on_p2Dir4) SettInfo.KeySettInfos[KSIIDX_P2][KSIIDX_KB].Dir4 = UseKeyList[keyidx];
				if(on_p2Dir8) SettInfo.KeySettInfos[KSIIDX_P2][KSIIDX_KB].Dir8 = UseKeyList[keyidx];
				if(on_p2Dir2) SettInfo.KeySettInfos[KSIIDX_P2][KSIIDX_KB].Dir2 = UseKeyList[keyidx];
				if(on_p2Bomb) SettInfo.KeySettInfos[KSIIDX_P2][KSIIDX_KB].Bomb = UseKeyList[keyidx];

				SettInfo2Input();
				RefreshInput(); // 今押したキーが入力にならないように。
			}
			if(on_modoru)
			{
				break;
			}
			if(1 <= padCount && on_pad1)
			{
				PadButtonConfig(0);
			}
			if(2 <= padCount && on_pad2)
			{
				PadButtonConfig(1);
			}
		}
		SwapFrame();
	}
	SaveSettings();

	frameloop(frmidx, frmpct, 30)
	{
		SimpleDraw(Dc->PicBlackWall, 0, 0, 0);

		SetAlpha(1.0 - frmpct);
		SimpleDraw(Dc->PicCfgWall, 0, 0, 0);
		ResetAlpha();

		SwapFrame();
	}
}

void DoConfigTitle(void)
{
	CfgMain();
}
void DoConfigPz(void)
{
	ScreenModeLock = 1;
	CfgMain();
	ScreenModeLock = 0;
}
void DoConfigScenario(void)
{
	ScreenModeLock = 1;
	CfgMain();
	ScreenModeLock = 0;
}

static void PadButtonConfig(int padIndex) // padIndex : 0 or 1
{
	char *messages[] =
	{
		"方向 (下)",
		"方向 (左)",
		"方向 (右)",
		"方向 (上)",
		"ポーズ", // "スタート (…っていうか、ポーズ)",
		"", // "決定 (あまり使用しない)",
		"", // "キャンセル (あまり使用しない)",
		"左回転 (決定)",
		"右回転 (キャンセル)",
//		"", // "必殺技", // 廃止
	};
	errorCase(lengthof(messages) != lengthof(InputList[0])); // 2bs

	IH_PadLastPressed = 1; // 今押しっぱのボタンを検出しないように、、、

//	Dc->BlackCurtain = 0.9;

	for(int keyIndex = 0; keyIndex < lengthof(messages); )
	{
		if(messages[keyIndex][0] == '\0')
		{
			keyIndex++;
			continue;
		}
		SimpleDraw(Dc->PicTitle, 0, 0, 0);
//		SimpleDraw(Dc->PicCfgWall, 0, 0, 0);
		ExecFrameCurtain();

		MyCls();
		MyPrint("");
		MyPrint_x(xcout("　**** ＰＡＤ%sのボタン設定 ****", ZenInt(padIndex + 1)));
		MyPrint("");
		MyPrint("　ボタンを押しても反応が無い場合は、そのパッドに対応していないか、");
		MyPrint("　今、手に持っているパッドとは別のパッドの入力待ちをしている可能性があります。");
		MyPrint("");
		MyPrint("　この画面から抜け出すにはエンターキーを押して下さい。");
		MyPrint("");
		MyPrint("");
		MyPrint_x(xcout("　\"%s\" ボタンを押して下さい。", messages[keyIndex]));
		MyPrint("");

		int status = GetHitPad(padIndex);

		if(status)
		{
			InputList[padIndex][keyIndex].Map_Pad = status;
			keyIndex++;
		}
		ExecMyPrint();
		SwapFrame();

		static int lastEnter;
		static int currEnter;

		lastEnter = currEnter;
		currEnter = IsPress(KEY_INPUT_RETURN);

		int cStatus = GetClick();

		if((lastEnter && !currEnter) || cStatus == 2)
		{
			goto end_loop;
		}
	}
end_loop:

//	Dc->BlackCurtain = 0.0;
	clsDx();
}
void Pub_PadButtonConfig(int padIndex)
{
	PadButtonConfig(padIndex);
}
