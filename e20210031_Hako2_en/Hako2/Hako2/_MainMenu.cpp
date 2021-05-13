#include "all.h"

#define DEF_BORDER_COLOR GetColor(100, 50, 100)

static Random *Rnd;

// --- Wall ----

typedef struct WallBox_st
{
	double X;
	double Y;
	double XAdd;
	double YAdd;
	double Br_R;
	double Br_G;
	double Br_B;
	double BrD_R;
	double BrD_G;
	double BrD_B;
	double Zoom;
	double DZoom;
}
WallBox_t;

static autoList<WallBox_t *> *WallBoxes;
static double Wall_RotRate;
static double Wall_WallX;
static double Wall_WallY;
static double Wall_WallDX;
static double Wall_WallDY;
static taskList *Wall_DLLogo;

static void InitWall(void)
{
	WallBoxes = new autoList<WallBox_t *>();

	for(int c = 0; c < 100; c++)
	{
		WallBox_t *wb = nb_(WallBox_t);

		wb->X = Rnd->DRnd() * SCREEN_W;
		wb->Y = Rnd->DRnd() * SCREEN_H;
		wb->XAdd = Rnd->ERnd() * 2.0;
		wb->YAdd = Rnd->ERnd() * 2.0;
		wb->Br_R = Rnd->DRnd();
		wb->Br_G = Rnd->DRnd();
		wb->Br_B = Rnd->DRnd();
		wb->BrD_R = Rnd->DRnd();
		wb->BrD_G = Rnd->DRnd();
		wb->BrD_B = Rnd->DRnd();
		wb->Zoom = 10.0;
		wb->DZoom = 1.0 + Rnd->DRnd() * 3.0;

		WallBoxes->AddElement(wb);
	}
	Wall_RotRate = 0.01;
	Wall_WallX = 0.0;
	Wall_WallY = 0.0;
	Wall_WallDX = SCREEN_W - Pic_W(P_TITLE_WALL);
	Wall_WallDY = SCREEN_H - Pic_H(P_TITLE_WALL);
	Wall_DLLogo = new taskList();
}
static void FnlzWall(void)
{
	for(int index = 0; index < WallBoxes->GetCount(); index++)
	{
		memFree(WallBoxes->GetElement(index));
	}
	delete WallBoxes;
	WallBoxes = NULL;
	delete Wall_DLLogo;
}
static void DrawWall(void)
{
//	DrawCurtain();
	DrawSimple(P_TITLE_WALL, Wall_WallX, Wall_WallY);

	m_approach(Wall_WallX, Wall_WallDX, 0.9999);
	m_approach(Wall_WallY, Wall_WallDY, 0.9999);

	for(int index = 0; index < WallBoxes->GetCount(); index++)
	{
		WallBox_t *wb = WallBoxes->GetElement(index);

		wb->X += wb->XAdd;
		wb->Y += wb->YAdd;

		{
			const double MARGIN = 200.0;

			if(wb->X < 0.0 - MARGIN) wb->X += SCREEN_W + MARGIN * 2.0;
			if(wb->Y < 0.0 - MARGIN) wb->Y += SCREEN_H + MARGIN * 2.0;
			if(SCREEN_W + MARGIN < wb->X) wb->X -= SCREEN_W + MARGIN * 2.0;
			if(SCREEN_H + MARGIN < wb->Y) wb->Y -= SCREEN_H + MARGIN * 2.0;
		}

		wb->XAdd += Rnd->ERnd() * 0.01;
		wb->YAdd += Rnd->ERnd() * 0.01;

		m_range(wb->XAdd, -2.0, 2.0);
		m_range(wb->YAdd, -2.0, 2.0);

		m_approach(wb->Br_R, wb->BrD_R, 0.97);
		m_approach(wb->Br_G, wb->BrD_G, 0.97);
		m_approach(wb->Br_B, wb->BrD_B, 0.97);
		m_approach(wb->Zoom, wb->DZoom, 0.97);

		if(Rnd->DRnd() < 0.01) wb->BrD_R = Rnd->DRnd();
		if(Rnd->DRnd() < 0.01) wb->BrD_G = Rnd->DRnd();
		if(Rnd->DRnd() < 0.01) wb->BrD_B = Rnd->DRnd();
		if(Rnd->DRnd() < 0.01) wb->DZoom = 1.0 + Rnd->DRnd() * 4.0;

		DPE_SetBright(wb->Br_R, wb->Br_G, wb->Br_B);
		DrawBegin(P_WBOX32, wb->X, wb->Y);
		DrawZoom(wb->Zoom);
		DrawRotate((wb->X + wb->Y) * Wall_RotRate);
		DrawEnd();
		DPE_Reset();
	}

	m_approach(Wall_RotRate, 0.001, 0.9999);
}

// ----

static void DrawTitleLogo(void)
{
	const char *logoMap =
		"# # ### # # ### # $$$$$"
		"# # # # # # # # #  $ $ "
		"# # # # # # # # #  $ $ "
		"### ### ##  # # #  $ $ "
		"# # # # # # # # #  $ $ "
		"# # # # # # # #    $ $ "
		"# # # # # # ### # $$$$$";
	const int LM_W = 23;
	const int LM_H = 7;

	errorCase(strlen(logoMap) != LM_W * LM_H); // 2bs

	const int DR_L = 32;
	const int DR_T = 32;
	const int cellSpan = 32;

	for(int xz = -1; xz <= 1; xz++)
	for(int yz = -1; yz <= 1; yz++)
	for(int x = 0; x < LM_W; x++)
	for(int y = 0; y < LM_H; y++)
	{
		int lmIndex = x + LM_W * y;

		if(logoMap[lmIndex] != ' ')
		{
			double bright = 1.0;

			if((x + y + ProcFrame / 30) % 2)
				bright = 0.9;

			if(logoMap[lmIndex] == '$')
				bright = 1.0 - ((x + y + ProcFrame / 10) % 10) * 0.02;

			int drX = DR_L + x * cellSpan;
			int drY = DR_T + y * cellSpan;

			if(xz != 0 || xz != 0)
			{
				bright *= 0.3;
				drX += xz * 5;
				drY += yz * 5;
				DPE_SetAlpha(0.3);
			}
			else
				DPE.TL = Wall_DLLogo;

			DPE_SetBright(bright, bright, bright);
			DrawRect(P_WHITEBOX, drX, drY, cellSpan, cellSpan);
			DPE_Reset();
		}
	}

	Wall_DLLogo->ExecuteAllTask();
}

// ゲームスタート系 -->

static void LeaveFromMainMenu(int keepBGM = 0)
{
	if(!keepBGM)
		MusicFade();

	SetCurtain(30, -1.0);

	forscene(40)
	{
		DrawWall();
		EachFrame();
	}
	sceneLeave();
}
static void ReturnToMainMenu(void)
{
	MusicPlay(MUS_TITLE);
	SetCurtain();
	FreezeInput();
}
static void ShowHint(void)
{
	SetCurtain();

	forscene(5 * 60)
	{
		if(60 < sc_numer)
		if(
			1 <= GetInput(INP_A) ||
			1 <= GetInput(INP_B)
			)
			break; // スキップ

		DrawSimple(P_HINT, 0, 0);
		EachFrame();
	}
	sceneLeave();

	SetCurtain(30, -1.0);

	forscene(40)
	{
		DrawSimple(P_HINT, 0, 0);
		EachFrame();
	}
	sceneLeave();

	FreezeInput();
}
static void PlayGame(void)
{
	LeaveFromMainMenu();
	ShowHint();
	FloorMasterMain(0, 0, 0);
	ReturnToMainMenu();
}

#define SUBMENU_WHITE_LV -0.3
#define SUBMENU_WINDOW_A 0.5

static void FloorSelect(int playEditMapMode = 0, int editMode = 0)
{
	SetCurtain(60, SUBMENU_WHITE_LV);
	FreezeInput();

	const int selectMax = FLOOR_NUM + 1; // 最後はexit
	int selectIndex = 0;

	for(; ; )
	{
		int selectIndexAdd = 0;

		if(GetPound(INP_DIR_8))
			selectIndexAdd = -1;

		if(GetPound(INP_DIR_4))
			selectIndexAdd = -4;

		if(GetPound(INP_DIR_6))
			selectIndexAdd = 4;

		if(GetPound(INP_DIR_2))
			selectIndexAdd = 1;

		do
		{
			selectIndex += selectIndexAdd;
		}
		while(m_isRange(selectIndex, Gnd.UnclearedFloorIndex + 1, FLOOR_NUM - 1));

		adjustSelectIndex(selectIndex, selectMax);

		if(GetPound(INP_B))
		{
			if(selectIndex == selectMax - 1)
				break;

			selectIndex = selectMax - 1;
		}
		if(GetPound(INP_A))
		{
			if(selectIndex < FLOOR_NUM)
			{
				LeaveFromMainMenu();
				FloorMasterMain(selectIndex, playEditMapMode, editMode);
				ReturnToMainMenu();

				goto endMenu;
			}
			break; // ? exit
		}

		DrawWall();
		CurtainEachFrame();

		{
			const int WIN_L = 280;
			const int WIN_T = 188;
			const int WIN_W = 240;
			const int WIN_H = 224;

			DPE_SetAlpha(SUBMENU_WINDOW_A);
			DPE_SetBright(0.0, 0.0, 0.0);
			DrawRect(P_WHITEBOX, WIN_L, WIN_T, WIN_W, WIN_H);
			DPE_Reset();

			DrawWindowFrame(D_WINDOW_Y_00 | DTP, WIN_L, WIN_T, WIN_W, WIN_H);

			const int COLOR = GetColor(255, 255, 255);
			const int BORDER_COLOR = DEF_BORDER_COLOR;

			SetPrint(345, 230, 40);
			PE.Color = COLOR;
			PE_Border(BORDER_COLOR);
			int c = 0;

			for(int floorIndex = 0; floorIndex < FLOOR_NUM; floorIndex++)
			{
				if(floorIndex == Gnd.UnclearedFloorIndex + 1)
				{
					PE.Color = GetColor(100, 100, 100);
					PE_Border(GetColor(50, 50, 50));
				}
				Print_x(xcout("[%c] FLOOR %d", selectIndex == c++ ? '>' : ' ', floorIndex + 1));
				PrintRet();
			}
			PE.Color = COLOR;
			PE_Border(BORDER_COLOR);

			Print_x(xcout("[%c] 戻る", selectIndex == c++ ? '>' : ' '));
			PE_Reset();
		}

		EachFrame();
	}
endMenu:
	SetCurtain();
	FreezeInput();
}
static void EditModeMenu(void)
{
	SetCurtain(60, SUBMENU_WHITE_LV);
	FreezeInput();

	const int selectMax = 4;
	int selectIndex = 0;

	for(; ; )
	{
		int selectIndexAdd = 0;

		if(GetPound(INP_DIR_8))
			selectIndexAdd = -1;

		if(GetPound(INP_DIR_2))
			selectIndexAdd = 1;

		do
		{
			selectIndex += selectIndexAdd;
		}
		while(m_isRange(selectIndex, Gnd.UnclearedFloorIndex + 1, FLOOR_NUM - 1));

		adjustSelectIndex(selectIndex, selectMax);

		if(GetPound(INP_B))
		{
			if(selectIndex == selectMax - 1)
				break;

			selectIndex = selectMax - 1;
		}
		if(GetPound(INP_A))
		{
			switch(selectIndex)
			{
			case 0:
				RestoreOriginalMapsIfNeed();
				FloorSelect(1, 1);
				goto endMenu;

			case 1:
				{
					LeaveFromMainMenu();
					FloorMasterMain(0, 1, 0);
					ReturnToMainMenu();
				}
				goto endMenu;

			case 2:
				FloorSelect(1);
				goto endMenu;

			case 3:
				goto endMenu;

			default:
				error();
			}
		}

		DrawWall();
		CurtainEachFrame();

		{
			const int WIN_L = 200;
			const int WIN_T = 172;
			const int WIN_W = SCREEN_W - WIN_L * 2;
			const int WIN_H = SCREEN_H - WIN_T * 2;

			DPE_SetAlpha(SUBMENU_WINDOW_A);
			DPE_SetBright(0.0, 0.0, 0.0);
			DrawRect(P_WHITEBOX, WIN_L, WIN_T, WIN_W, WIN_H);
			DPE_Reset();

			DrawWindowFrame(D_WINDOW_Y_00 | DTP, WIN_L, WIN_T, WIN_W, WIN_H);

			SetPrint(240, 230, 40);
			PE_Border(DEF_BORDER_COLOR);
			int c = 0;
			Print_x(xcout("[%c] マップ編集モード", selectIndex == c++ ? '>' : ' '));
			PrintRet();
			Print_x(xcout("[%c] 自作マップで遊ぶ", selectIndex == c++ ? '>' : ' '));
			PrintRet();
			Print_x(xcout("[%c] 自作マップで遊ぶ (FLOOR SELECT)", selectIndex == c++ ? '>' : ' '));
			PrintRet();
			Print_x(xcout("[%c] 戻る", selectIndex == c++ ? '>' : ' '));
			PE_Reset();
		}

		EachFrame();
	}
endMenu:
	SetCurtain();
	FreezeInput();
}

// 設定 -->

static void DrawSettingWall(int w = SCREEN_W / 2)
{
	DrawWall();

	DPE_SetAlpha(SUBMENU_WINDOW_A);
	DPE_SetBright(0.0, 0.0, 0.0);
	DrawRect(P_WHITEBOX, 0, 0, w, SCREEN_H);
	DPE_Reset();

	CurtainEachFrame();
}
static int BC_GetPressPadBtnId(void)
{
	int padId = Gnd.PrimaryPadId;

	/*
		todo: パッドを探すようにする。
	*/
	if(padId == -1) // ? 未定義
		padId = 0; // 適当なデフォ

	for(int btnId = 0; btnId < PAD_BUTTON_MAX; btnId++)
	{
		if(GetPadInput(padId, btnId) == 1)
		{
			return btnId;
		}
	}
	return -1; // not found
}
static void ButtonConfig(void)
{
	int *pPadBtns[] =
	{
		&Gnd.PadBtnId.Dir_8,
		&Gnd.PadBtnId.Dir_2,
		&Gnd.PadBtnId.Dir_4,
		&Gnd.PadBtnId.Dir_6,
		&Gnd.PadBtnId.A,
		&Gnd.PadBtnId.B,
		&Gnd.PadBtnId.L,
		&Gnd.PadBtnId.R,
		&Gnd.PadBtnId.C,
		&Gnd.PadBtnId.Pause,
	};
	char *btnNames[] =
	{
		"上",
		"下",
		"左",
		"右",
		"ジャンプ / 決定",
		"低速移動 / キャンセル",
		"始点移動",
		"リスポーン",
		"リスポーン地点設置",
		"ポーズ",
	};
	errorCase(lengthof(pPadBtns) != lengthof(btnNames));
	int btnMax = lengthof(pPadBtns);
	int btnIndex = 0;
	void *backup = memClone(&Gnd.PadBtnId, sizeof(Gnd.PadBtnId));

	/*
		不使用ボタンのクリア
	*/
	Gnd.PadBtnId.D = -1;
	Gnd.PadBtnId.E = -1;
	Gnd.PadBtnId.F = -1;
	Gnd.PadBtnId.Start = -1;

	FreezeInput();

	while(btnIndex < btnMax)
	{
		if(GetKeyInput(KEY_INPUT_SPACE) == 1) // キャンセル
		{
			memcpy(&Gnd.PadBtnId, backup, sizeof(Gnd.PadBtnId)); // 復元
			break;
		}
		if(GetKeyInput(KEY_INPUT_Z) == 1) // 割り当てナシ
		{
			*pPadBtns[btnIndex] = -1;
			btnIndex++;
		}

		{
			int btnId = BC_GetPressPadBtnId();

			if(btnId != -1)
			{
				for(int i = 0; i < btnIndex; i++)
					if(*pPadBtns[i] == btnId)
						goto endDecideBtnId;

				*pPadBtns[btnIndex] = btnId;
				btnIndex++;
			}
endDecideBtnId:;
		}

		DrawSettingWall(600);

		PE_Border(DEF_BORDER_COLOR);
		SetPrint(40, 40, 36);
		Print("**** ゲームパッドのボタン設定 ****");
		PrintRet();

		for(int index = 0; index < btnMax; index++)
		{
			Print_x(xcout("[%c] %s ボタン", index == btnIndex ? '>' : ' ', btnNames[index]));

			if(index < btnIndex)
			{
				int btnId = *pPadBtns[index];

				if(btnId == -1)
					Print(" -> 割り当てナシ");
				else
					Print_x(xcout(" -> %02d", *pPadBtns[index]));
			}
			PrintRet();
		}
		PrintRet();
		Print("# カーソルの指す機能に割り当てるボタンを押して下さい。");
		PrintRet();
		Print("# Ｚキーを押すと割り当てをスキップします。");
		PrintRet();
		Print("# スペースキーを押すとキャンセルします。");
		PE_Reset();

		EachFrame();
	}
	FreezeInput();

	memFree(backup);
}
static void WindowSizeConfig(void)
{
	FreezeInput();

	const int selectMax = 13;
	int selectIndex = 0;

	for(; ; )
	{
		if(GetPound(INP_DIR_8))
			selectIndex--;

		if(GetPound(INP_DIR_2))
			selectIndex++;

		adjustSelectIndex(selectIndex, selectMax);

		if(GetPound(INP_B))
		{
			if(selectIndex == selectMax - 1)
				break;

			selectIndex = selectMax - 1;
		}
		if(GetPound(INP_A))
		{
			switch(selectIndex)
			{
			case 0: SetScreenSize(800, 600); break;
			case 1: SetScreenSize(900, 675); break;
			case 2: SetScreenSize(1000, 750); break;
			case 3: SetScreenSize(1100, 825); break;
			case 4: SetScreenSize(1200, 900); break;
			case 5: SetScreenSize(1300, 975); break;
			case 6: SetScreenSize(1400, 1050); break;
			case 7: SetScreenSize(1500, 1125); break;
			case 8: SetScreenSize(1600, 1200); break;
			case 9: SetScreenSize(1700, 1275); break;
			case 10: SetScreenSize(1800, 1350); break;

			case 11:
				{
					int real_w = GetSystemMetrics(SM_CXSCREEN);
					int real_h = GetSystemMetrics(SM_CYSCREEN);

					int w = real_w;
					int h = real_h;

					if(w * SCREEN_H < h * SCREEN_W) // 縦長モニタ -> 横幅に合わせる
					{
						h = m_d2i(((double)w * SCREEN_H) / SCREEN_W);
					}
					else // 横長モニタ -> 縦幅に合わせる
					{
						w = m_d2i(((double)h * SCREEN_W) / SCREEN_H);
					}
					SetScreenSize(real_w, real_h);

					Gnd.ScreenDraw_L = (real_w - w) / 2;
					Gnd.ScreenDraw_T = (real_h - h) / 2;
					Gnd.ScreenDraw_W = w;
					Gnd.ScreenDraw_H = h;
				}
				break;

			case 12:
				goto endMenu;

			default:
				error();
			}
		}

		DrawSettingWall();

		SetPrint(40, 40, 38);
		PE_Border(DEF_BORDER_COLOR);
		Print("**** ウィンドウサイズ設定 ****");
		PrintRet();
		int c = 0;
		Print_x(xcout("[%c] 800 x 600 (デフォルト)", selectIndex == c++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c] 900 x 675", selectIndex == c++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c] 1000 x 750", selectIndex == c++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c] 1100 x 825", selectIndex == c++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c] 1200 x 900", selectIndex == c++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c] 1300 x 975", selectIndex == c++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c] 1400 x 1050", selectIndex == c++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c] 1500 x 1125", selectIndex == c++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c] 1600 x 1200", selectIndex == c++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c] 1700 x 1275", selectIndex == c++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c] 1800 x 1350", selectIndex == c++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c] フルスクリーン", selectIndex == c++ ? '>' : ' '));
		PrintRet();
		Print_x(xcout("[%c] 戻る", selectIndex == c++ ? '>' : ' '));
		PE_Reset();

		EachFrame();
	}
endMenu:
	FreezeInput();
}
static void TuneVolume(int seFlag)
{
	double dvol = seFlag ? Gnd.SEVolume : Gnd.MusicVolume;
	int volume = m_d2i(dvol * 100);
	int volorig = volume;
	int lastvol = volume;

	FreezeInput();

	for(; ; )
	{
		if(GetPound(INP_DIR_4))
			volume--;

		if(GetPound(INP_DIR_6))
			volume++;

		if(GetPound(INP_DIR_2))
			volume -= 10;

		if(GetPound(INP_DIR_8))
			volume += 10;

		if(GetPound(INP_B))
		{
			if(volume == volorig)
				break;

			volume = volorig;
		}
		m_range(volume, 0, 100);

		if(GetPound(INP_A))
			break;

		if(lastvol != volume)
		{
			if(seFlag)
			{
				Gnd.SEVolume = volume / 100.0;
				UpdateSEVolume();
			}
			else
			{
				Gnd.MusicVolume = volume / 100.0;
				UpdateMusicVolume();
			}
			lastvol = volume;
		}
		if(seFlag && ProcFrame % 60 == 0)
		{
			SEPlay(Rnd->DRnd() < 0.5 ? SE_PAUSE_IN : SE_PAUSE_OUT);
		}
		DrawSettingWall(500);

		PE_Border(DEF_BORDER_COLOR);
		SetPrint(40, 40, 38);
		Print_x(xcout("**** %s音量設定 ****", seFlag ? "ＳＥ" : "ＢＭＧ"));
		PrintRet();
		Print_x(xcout("[ %3d ]", volume));
		PrintRet(); PrintRet(); PrintRet();
		PrintRet(); PrintRet(); PrintRet();
		PrintRet(); PrintRet(); PrintRet();
		Print("# 右 または 上 ＝ 上げる");
		PrintRet();
		Print("# 左 または 下 ＝ 下げる");
		PrintRet();
		Print("# 調整が終わったら決定ボタンを押して下さい。");
		PrintRet();
		Print("# キャンセルボタンを押すと変更をキャンセルします。");

		EachFrame();
	}
	FreezeInput();
}
static int SettingConfirm(char *prompt, char *option1, char *option2, int wall_w = -1) // ret: ? option1 を選択した。
{
	int retval = 0;

	FreezeInput();

	const int selectMax = 2;
	int selectIndex = 1;

	for(; ; )
	{
		if(GetPound(INP_DIR_8))
			selectIndex--;

		if(GetPound(INP_DIR_2))
			selectIndex++;

		adjustSelectIndex(selectIndex, selectMax);

		if(GetPound(INP_B))
		{
			if(selectIndex == selectMax - 1)
				break;

			selectIndex = selectMax - 1;
		}
		if(GetPound(INP_A))
		{
			switch(selectIndex)
			{
			case 0:
				retval = 1;
				goto endMenu;

			case 1:
				goto endMenu;

			default:
				error();
			}
		}

		if(wall_w == -1)
			DrawSettingWall();
		else
			DrawSettingWall(wall_w);

		SetPrint(40, 40, 40);
		PE_Border(DEF_BORDER_COLOR);
		Print_x(xcout("**** %s ****", prompt));
		PrintRet();
		int c = 0;
		Print_x(xcout("[%c] %s", selectIndex == c++ ? '>' : ' ', option1));
		PrintRet();
		Print_x(xcout("[%c] %s", selectIndex == c++ ? '>' : ' ', option2));
		PE_Reset();

		EachFrame();
	}
endMenu:
	FreezeInput();

	return retval;
}
static void UnlockAllFloor(void)
{
	Gnd.UnclearedFloorIndex = FLOOR_NUM;
	RestoreOriginalMapsIfNeed();
}
static void RespawnCoinStockMenu(void)
{
	FreezeInput();

	const int selectMax = RESPAWN_MAX_SELECTABLE_MAX + 3;
	int selectIndex = Gnd.RespawnMax;

	for(; ; )
	{
		if(GetPound(INP_DIR_8))
			selectIndex--;

		if(GetPound(INP_DIR_2))
			selectIndex++;

		adjustSelectIndex(selectIndex, selectMax);

		if(GetPound(INP_B))
		{
			if(selectIndex == selectMax - 1)
				break;

			selectIndex = selectMax - 1;
		}
		if(GetPound(INP_A))
		{
			if(selectIndex == selectMax - 1)
				break;

			if(selectIndex == selectMax - 2)
				Gnd.RespawnMax = RESPAWN_MAX_GORGEOUS_MAX;
			else
				Gnd.RespawnMax = selectIndex;
		}

		DrawSettingWall(660);

		SetPrint(40, 40, 33);
		PE_Border(DEF_BORDER_COLOR);
		Print("**** リスポーン・コイン数の変更 (リスポーン地点設置回数の変更) ****");
		PrintRet();
		int c = 0;

		while(c <= RESPAWN_MAX_SELECTABLE_MAX)
		{
			Print_x(xcout(
				"[%c] %c %d COIN%s%s"
				,selectIndex == c ? '>' : ' '
				,c == Gnd.RespawnMax ? '*' : ' '
				,c
				,c == 1 ? "" : "S"
				,c == RESPAWN_DEF ? "　(デフォルト)" : ""
				));
			PrintRet();
			c++;
		}

		{
			Print_x(xcout(
				"[%c] %c GORGEOUS"
				,selectIndex == c ? '>' : ' '
				,Gnd.RespawnMax == RESPAWN_MAX_GORGEOUS_MAX ? '*' : ' '
				));
			PrintRet();
			c++;
		}

		Print_x(xcout("[%c] 戻る", selectIndex == c++ ? '>' : ' '));
		PrintRet();

		PE_Reset();

		EachFrame();
	}
//endMenu:
	FreezeInput();
}
static void Setting(void)
{
	SetCurtain(60, SUBMENU_WHITE_LV);
	FreezeInput();

	const int selectMax = 8;
	int selectIndex = 0;

	for(; ; )
	{
		if(GetPound(INP_DIR_8))
			selectIndex--;

		if(GetPound(INP_DIR_2))
			selectIndex++;

		adjustSelectIndex(selectIndex, selectMax);

		if(GetPound(INP_B))
		{
			if(selectIndex == selectMax - 1)
				break;

			selectIndex = selectMax - 1;
		}
		if(GetPound(INP_A))
		{
			switch(selectIndex)
			{
			case 0:
				ButtonConfig();
				break;

			case 1:
				WindowSizeConfig();
				break;

			case 2:
				TuneVolume(0);
				break;

			case 3:
				TuneVolume(1);
				break;

				// 廃止
				/*
			case 4:
				Gnd.GraphicalMode = (Gnd.GraphicalMode + 1) % GM_MAX;
				break;
				*/

			case 4:
				Gnd.RecordingMode = Gnd.RecordingMode ? 0 : 1;
				break;
				
			case 5:
//				if(SettingConfirm(NULL, NULL)) {
				if(SettingConfirm("ロックされているフロアを開放します。", "実行", "キャンセル", 500)) {
					UnlockAllFloor();
					SEPlay(SE_FLOORCLEAR);
				}
				break;

			case 6:
				RespawnCoinStockMenu();
				break;

			case 7:
				goto endMenu;

			default:
				error();
			}
		}

		DrawSettingWall();

		{
			SetPrint(40, 40, 48);
			PE_Border(DEF_BORDER_COLOR);
			Print("**** 設定 ****");
			PrintRet();
			int c = 0;
			Print_x(xcout("[%c] ゲームパッドのボタン設定", selectIndex == c++ ? '>' : ' '));
			PrintRet();
			Print_x(xcout("[%c] ウィンドウサイズ設定", selectIndex == c++ ? '>' : ' '));
			PrintRet();
			Print_x(xcout("[%c] ＢＧＭ音量設定", selectIndex == c++ ? '>' : ' '));
			PrintRet();
			Print_x(xcout("[%c] ＳＥ音量設定", selectIndex == c++ ? '>' : ' '));
			PrintRet();
			// 廃止
			/*
			Print_x(xcout("[%c] グラフィックモード : %s", selectIndex == c++ ? '>' : ' ', GetGraphicalModeCaption(Gnd.GraphicalMode)));
			PrintRet();
			*/
			Print_x(xcout("[%c] リプレイの保存 : %s", selectIndex == c++ ? '>' : ' ', Gnd.RecordingMode ? "有効" : "無効"));
			PrintRet();
			Print_x(xcout("[%c] ロックされているフロアの開放", selectIndex == c++ ? '>' : ' '));
			PrintRet();
			Print_x(xcout("[%c] リスポーン・コインの数", selectIndex == c++ ? '>' : ' '));
			PrintRet();
			Print_x(xcout("[%c] 戻る", selectIndex == c++ ? '>' : ' '));
			PE_Reset();
		}

		EachFrame();
	}
endMenu:
	ExportSaveData();

	SetCurtain();
	FreezeInput();
}

// ----

void MainMenu(void)
{
	Rnd = new Random((int)time(NULL));
	InitWall();
	MakeDummyMapsIfNeed();

	// <-- init

	if(UnlockAllFloorEnabled)
		UnlockAllFloor();

	MusicPlay(MUS_TITLE);
	SetCurtain();
	FreezeInput();

	const int selectMax = 5;
//	const int editModeIndex = 4; // 廃止 @ 2016.10.24
	int selectIndex = 0;

	for(; ; )
	{
		int selectIndexAdd = 0;

		if(GetPound(INP_DIR_8))
			selectIndexAdd = -1;

		if(GetPound(INP_DIR_2))
			selectIndexAdd = 1;

		selectIndex += selectIndexAdd;

		// 廃止 @ 2016.10.24
		/*
		if(selectIndex == editModeIndex && Gnd.UnclearedFloorIndex < FLOOR_NUM) // ? edit mode && not editable
			selectIndex += selectIndexAdd;
			*/

		adjustSelectIndex(selectIndex, selectMax);

		if(GetPound(INP_B))
		{
			if(selectIndex == selectMax - 1)
				break;

			selectIndex = selectMax - 1;
		}
		if(GetPound(INP_A))
		{
			switch(selectIndex)
			{
			case 0:
				PlayGame();
				break;

			case 1:
				FloorSelect();
				break;

			case 2:
				LeaveFromMainMenu(1);
				ReplayMainMenu();
				ReturnToMainMenu();
				break;

			case 3:
				Setting();
				break;

			// 廃止 @ 2016.10.24
			/*
			case 4:
				EditModeMenu();
				break;
				*/

			case 4:
				goto endMenu;

			default:
				error();
			}
		}

		// 隠しモード >

#if LOG_ENABLED
		if(GetKeyPound(KEY_INPUT_U))
		{
			UnlockAllFloor();
			SEPlay(SE_FLOORCLEAR);
		}
#endif

#if LOG_ENABLED
		if(GetKeyPound(KEY_INPUT_E))
#else
		if(GetKeyPound(KEY_INPUT_E) && EditModeEnabled)
#endif
		{
			UnlockAllFloor();
			SEPlay(SE_FLOORCLEAR);

			EditModeMenu();
		}

		// < 隠しモード

		DrawWall();
		DrawTitleLogo();

		PE_Border(GetColor(70, 20, 70));
		SetPrint(278, 584);
		Print(GetDatString(DATSTR_COPYRIGHT));
		PE_Reset();

		{
			const int WIN_L = 280;
			const int WIN_T = 320;
			const int WIN_W = 240;
			const int WIN_H = 240;

			DPE_SetAlpha(0.65);
			DPE_SetBright(0.0, 0.0, 0.0);
			DrawRect(P_WHITEBOX, WIN_L, WIN_T, WIN_W, WIN_H);
			DPE_Reset();

			DrawWindowFrame(D_WINDOW_Y_00 | DTP, WIN_L, WIN_T, WIN_W, WIN_H);

			const int COLOR = GetColor(255, 255, 255);
			const int BORDER_COLOR = DEF_BORDER_COLOR;

			SetPrint(330, 352, 40);
			PE.Color = COLOR;
			PE_Border(BORDER_COLOR);
			int c = 0;
			Print_x(xcout("[%c] GAME START", selectIndex == c++ ? '>' : ' '));
			PrintRet();
			Print_x(xcout("[%c] FLOOR SELECT", selectIndex == c++ ? '>' : ' '));
			PrintRet();
			Print_x(xcout("[%c] REPLAY", selectIndex == c++ ? '>' : ' '));
			PrintRet();
			Print_x(xcout("[%c] SETTING", selectIndex == c++ ? '>' : ' '));
			PrintRet();

			// 廃止 @ 2016.10.24
			/*
			if(Gnd.UnclearedFloorIndex < FLOOR_NUM) // ? not editable
			{
				PE.Color = GetColor(100, 100, 100);
				PE_Border(GetColor(50, 50, 50));
			}
			Print_x(xcout("[%c] EDIT MODE", selectIndex == c++ ? '>' : ' '));
			PrintRet();
			PE.Color = COLOR;
			PE_Border(BORDER_COLOR);
			*/

			Print_x(xcout("[%c] EXIT", selectIndex == c++ ? '>' : ' '));
			PE_Reset();
		}

		EachFrame();
	}
endMenu:
//	FreezeInput();
	MusicFade();
	SetCurtain(30, -1.0);

	forscene(40)
	{
		DrawWall();
		EachFrame();
	}
	sceneLeave();

	// fnlz -->

	FnlzWall();
	delete Rnd;
}
