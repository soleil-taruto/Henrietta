/*
	ScInit() -> ScMain() -> ScFnlz()
*/

#include "StdInc.h"

static void SaveFunc(int index)
{
	SaveData_t *i = SettInfo.SaveDataList + index;
	char *lui = xcout("%s %s", GetJStamp(time(NULL)), Scdc->ScnrTitle);

	strcpy(i->UIText, lui);
	strcpy(i->Scenario, Scdc->ScnrTitle);
	i->ActionIndex = Scdc->ActionIndex;

	memFree(lui);
}

#define CN_DRAW_X 50
#define CN_DRAW_Y 413

#define MESS_DRAW_X 50
#define MESS_DRAW_Y 455
#define MESS_STEP_X 22
#define MESS_STEP_Y 25
#define MESS_X_BOUND 31

#define WALL_SWAP_SPEED 0.01
#define PZPLYR_SWAP_SPEED 0.05

static int RouteSentakuMode;
static void ExecScene(void)
{
	Scdc->WallA += WALL_SWAP_SPEED;
	m_range(Scdc->WallA, 0.0, 1.0);

	if(Scdc->MessForDisp) Scdc->MessA += WALL_SWAP_SPEED * 3.0;
	else                  Scdc->MessA -= WALL_SWAP_SPEED;
	m_range(Scdc->MessA, 0.0, 1.0);

	for(int chara = 0; chara < 4; chara++)
	{
		ScCharaInfo_t *ci = Scdc->CharaList + chara;
		double yureR = rnd() * PI * 2.0;

		// -- 揺れ --
		ci->YureX = sin(yureR) * ci->YureFar;
		ci->YureY = cos(yureR) * ci->YureFar;
		ci->YureFar -= 0.3;

		if(ci->YureFar < 0.0)
		{
			ci->YureFar = 0.0;
		}
		// ----

		if(0 < Scdc->CharaList[chara].SleepCnt) {
			Scdc->CharaList[chara].SleepCnt--;
			continue;
		}

		Scdc->CharaList[chara].A += PZPLYR_SWAP_SPEED;
		m_range(Scdc->CharaList[chara].A, 0.0, 1.0);
	}

	int mx;
	int my;
	GetMusPos(&mx, &my);

	Scdc->SelectButton = -1;

	for(int index = 0; index < SC_MESSBTN_MAX; index++)
	{
		int ii = isinside(
			mx,
			my,
			Scdc->ButtonPosList[index][0],
			Scdc->ButtonPosList[index][1],
			Scdc->ButtonPosList[index][0] + Scdc->ButtonXSize,
			Scdc->ButtonPosList[index][1] + Scdc->ButtonYSize
			);

		if(ii)
		{
			Scdc->SelectButton = index;
		}
	}
}
static void DrawScene(void)
{
	if(Scdc->PicSuperWall != -1)
	{
		SimpleDraw(Scdc->PicSuperWall, 0, 0, 0);
	}
	if(Scdc->PicWall != -1)
	{
		SetAlpha(Scdc->WallA);
		SimpleDraw(Scdc->PicWall, 0, 0, 0);
		ResetAlpha();
	}
	for(int chara = 0; chara < 4; chara++)
	{
		if(Scdc->CharaList[chara].PrevPic != -1 || Scdc->CharaList[chara].Pic != -1)
		{
			int x = Scdc->CharaList[chara].DrawX + (int)Scdc->CharaList[chara].YureX;
			int y = Scdc->CharaList[chara].DrawY + (int)Scdc->CharaList[chara].YureY;

			if(Scdc->CharaList[chara].PrevPic != -1)
			{
				SetAlpha(1.0 - Scdc->CharaList[chara].A);
				SimpleDraw(
					Scdc->CharaList[chara].PrevPic,
					x,
					y,
					1
					);
				ResetAlpha();
			}
			if(Scdc->CharaList[chara].Pic != -1)
			{
				SetAlpha(Scdc->CharaList[chara].A);
				SimpleDraw(
					Scdc->CharaList[chara].Pic,
					x,
					y,
					1
					);
				ResetAlpha();
			}
		}
	}

	SetAlpha(Scdc->MessA);
	SimpleDraw(GC_Pic(PIC_SCSys_MessageWindow), 0, 600 - 193, 1);

	for(int index = 0; index < SC_MESSBTN_MAX; index++)
	{
		int ipic = index == Scdc->SelectButton ? Scdc->IPicFocusButtonList[index] : Scdc->IPicButtonList[index];

		SimpleDraw(
			GC_Pic(ipic),
			Scdc->ButtonPosList[index][0],
			Scdc->ButtonPosList[index][1],
			1
			);
	}
	ResetAlpha();

	ScRotSenDrawFrame();
}

#define PG_CURTAIN 0.7
#define PG_ITEM_RX 120
#define PG_ITEM_RY 20

static int PG_ExitGame;

static void PauseGamen(void)
{
	// [][x, y]
	int itemPosList[][2] =
	{
		{ 400, 210 },
		{ 400, 270 },
		{ 400, 330 },
		{ 400, 390 },
	};

	// init ret
	PG_ExitGame = 0;

	Dc->BlackCurtain = PG_CURTAIN;

	int currIndex = -1; // -1: no-selected

	for(; ; )
	{
		DrawScene();
		ExecFrameCurtain();

		int mx;
		int my;
		GetMusPos(&mx, &my);

		for(int index = 0; index < lengthof(itemPosList); index++)
		{
			DppInit(Dc->PicPauseItem[index][index == currIndex ? 1 : 0], itemPosList[index][0], itemPosList[index][1], 1);
			DppDraw();

			if(isinside(mx, my,
				itemPosList[index][0] - PG_ITEM_RX,
				itemPosList[index][1] - PG_ITEM_RY,
				itemPosList[index][0] + PG_ITEM_RX,
				itemPosList[index][1] + PG_ITEM_RY
				))
			{
				currIndex = index;
			}
		}

		int moved = 0;
		if(GetRendaHit(INP_DIR_4) || GetRendaHit(INP_DIR_8)) // 上へ
		{
			currIndex--;
			moved = 1;
		}
		if(GetRendaHit(INP_DIR_2) || GetRendaHit(INP_DIR_6)) // 下へ
		{
			currIndex++;
			moved = 1;
		}
		if(moved)
		{
			m_range(currIndex, 0, lengthof(itemPosList) - 1);

			mx = itemPosList[currIndex][0] + PG_ITEM_RX;
			my = itemPosList[currIndex][1] + PG_ITEM_RY;

			SetMusPos(mx, my);
		}

		SwapFrame();

		int cStatus = GetClick();
		if(cStatus == 1 || GetHit(INP_ROT_DECIDE))
		{
			switch(currIndex)
			{
			case 0: // Return to Game
				goto end_loop;

			case 1: // Exit App
				GameTermination();
				error();

			case 2: // Exit Game
				Scdc->ReturnBackToTitle = 1;
				PG_ExitGame = 1;
				goto end_loop;

			case 3: // Settings
				Dc->BlackCurtain = 0.0;
				DoConfigScenario();
				Dc->BlackCurtain = PG_CURTAIN;
				Dc->BCPct = 1.0;
				break;
			}
		}
		else if(cStatus == 2 || GetHit(INP_ROT_CANCEL))
		{
			break;
		}
	}
end_loop:

	Dc->BlackCurtain = 0.0;
}

static void InitFont(void)
{
	LDrawRGB = 0x00000000;
	SetFontSize(MESS_STEP_X);
	SetFontThickness(3);
	ChangeFontType(DX_FONTTYPE_ANTIALIASING);
}
void ScMain(void)
{
	int frmidx;
	double frmpct;

	// init
	ScProcDc.RouteSelected = SC_ROUTE_NONE;
	ScInitByScenarioStart();

	strcpy(Scdc->ScnrFile, ResourcePath_x(xcout("シナリオ\\Text\\%s.txt", Scdc->ScnrTitle)));

	GC_LoadRange(PIC_SCSys_Start, PIC_SCSys_End);
	ScLoadScenario(Scdc->ScnrFile);

	for(int aidx = 0; aidx < Scdc->Scenario.ActCnt; aidx++)
	{
		ScAction_t *a = Scdc->Scenario.ActLst + aidx;

		for(int cidx = 0; cidx < a->CmdCnt; cidx++)
		{
			ScCommand_t *c = a->CmdLst + cidx;

			switch(c->Command)
			{
			case SC_CMD_DISP_WALL:
			case SC_CMD_DISP_CHARA:
				if(_stricmp(c->Extra, SC_EXTRA_OFF) && !GC_IsNmPicLoaded(c->Extra))
				{
					GC_NmPicLoad(c->Extra);
				}
				break;
			}
		}
	}

	GetMouseWheelRotVol(); // クリア
	InitFont();

#define AUTO_CYCLE_MAX 600

	int automode = 0;
	int autocnt;
	int autocycle = 100;

	int skipmode = 0;
	int kdk_hiaidx = GetKidokuHiestActionIndex(Scdc->ScnrTitle);

	int koroSleep = 0;
#define SHITA_KORO_SLEEP 5
	int messSleep = 20;

	char *loadBGM = SC_EXTRA_OFF;

	for(; ; )
	{
		errorCase(Scdc->ActionIndex < 0 || Scdc->Scenario.ActCnt <= Scdc->ActionIndex);

		ScAction_t *a = Scdc->Scenario.ActLst + Scdc->ActionIndex;
		int doInterval = 0;
		int whiteout = 0;
		int loadchuu = 0;

		ScRouteSentakuMode = 0;

		// -- ロード --
		if(Scdc->ActionIndex < Scdc->JumpActionIndex) // ロード場所までスキップ
		{
			loadchuu = 1;
		}
		else if(Scdc->ActionIndex == Scdc->JumpActionIndex && 0 < Scdc->JumpActionIndex) // ロードした && ロード場所到着
		{
			Scdc->PicSuperWall = Dc->PicBlackWall;

			for(int chara = 0; chara < 4; chara++)
			{
				Scdc->CharaList[chara].PrevPic = -1;
				Scdc->CharaList[chara].YureFar = 0.0;
			}

			if(!_stricmp(loadBGM, SC_EXTRA_OFF)) FadeOut(Dc->BGMVolume, Dc->BGMVolume / 60.0);
			else                                 PlayBGM(GC_NmBgm(loadBGM));
		}
		// ----

		for(int cidx = 0; cidx < a->CmdCnt; cidx++)
		{
			ScCommand_t *c = a->CmdLst + cidx;

			if(c->Command == SC_CMD_INTERVAL || c->Command == SC_CMD_WHITEOUT)
			{
				doInterval = 1;
				whiteout = c->Command == SC_CMD_WHITEOUT;
			}
		}
		if(doInterval)
		{
			int wallPic = whiteout ? Dc->PicWhiteWall : Dc->PicBlackWall;

			if(!loadchuu)
			{
				FadeOut(Dc->BGMVolume, Dc->BGMVolume / 60.0);

				int frmmax = 90;
				if(skipmode) frmmax = 30;

				frameloop(frmidx, frmpct, frmmax)
				{
					DrawScene();

					SetAlpha(frmpct);
					SimpleDraw(wallPic, 0, 0, 0);
					ResetAlpha();

					SwapFrame();
				}
			}
			loadBGM = SC_EXTRA_OFF;

			Scdc->PicSuperWall = wallPic;
			Scdc->PicWall = wallPic;

			Scdc->MessA = 0.0;

			for(int chara = 0; chara < 4; chara++)
			{
				Scdc->CharaList[chara].PrevPic = -1;
				Scdc->CharaList[chara].Pic = -1;

				Scdc->CharaList[chara].SleepCnt = 30;
			}
		}

		for(int cidx = 0; cidx < a->CmdCnt; cidx++)
		{
			ScCommand_t *c = a->CmdLst + cidx;

			switch(c->Command)
			{
			case SC_CMD_DISP_WALL:
				{
					Scdc->PicSuperWall = Scdc->PicWall;

					if(_stricmp(c->Extra, SC_EXTRA_OFF)) Scdc->PicWall = GC_NmPic(c->Extra);
					else                                 Scdc->PicWall = Dc->PicBlackWall;

					Scdc->WallA = 0.0;
				}
				break;

			case SC_CMD_DISP_CHARA:
				{
					int cIndex = c->PosNo;
					errorCase(cIndex < 0 || 3 < cIndex);

					int pic;
					if(!_stricmp(c->Extra, SC_EXTRA_OFF)) pic = -1;
					else                                  pic = GC_NmPic(c->Extra);

					Scdc->CharaList[cIndex].PrevPic = Scdc->CharaList[cIndex].Pic;
					Scdc->CharaList[cIndex].Pic = pic;
					Scdc->CharaList[cIndex].A = 0.0;
				}
				break;

			case SC_CMD_EFFECT_CHARA:
				{
					int cIndex = c->PosNo;
					errorCase(cIndex < 0 || 3 < cIndex);

					if(!strcmp(c->Extra, "揺れる"))
					{
						Scdc->CharaList[cIndex].YureFar = 10.0;
					}
					else if(!strcmp(c->Extra, "ここに追加"))
					{
						error();
					}
					else
					{
						error();
					}
				}
				break;

			case SC_CMD_PLAY_BGM:
				{
					if(!loadchuu)
					{
						if(!_stricmp(c->Extra, SC_EXTRA_OFF)) FadeOut(Dc->BGMVolume, Dc->BGMVolume / 60.0);
						else                                  PlayBGM(GC_NmBgm(c->Extra));
					}
					loadBGM = c->Extra;
				}
				break;

			case SC_CMD_PLAY_SE:
				{
					if(!loadchuu)
					{
						PlaySe(GC_NmBgm(c->Extra));
					}
				}
				break;

			case SC_CMD_INTERVAL:
			case SC_CMD_WHITEOUT:
				// 既に処理しているので、noop
				break;

			case SC_CMD_ROUTE_SENTAKU:
				ScRouteSentakuMode = 1;
				break;

			default:
				error();
			}
		}
		if(loadchuu)
		{
			goto end_action;
		}

#define MESSCYC_MAX 30

		int messcnt = 0;
		int messcycle = (int)(SettInfo.MessSpeed * MESSCYC_MAX);

		if(messcycle == 0)
		{
			if(0.0 < SettInfo.MessSpeed) messcycle = 1;
		}

		for(frmidx = 0; ; frmidx++) // メッセージ
		{
			int koro = GetMouseWheelRotVol();

			if( koroSleep) {
				koroSleep--;
				koro = 0;
			}
			int cStatus = GetClick();

			ExecScene();
			DrawScene();

			LDraw(a->CharaName, CN_DRAW_X, CN_DRAW_Y); // キャラ名

			char *mp = a->Message;
			int x = 0;
			int y = 0;

			if(messcycle == 0 || skipmode)
			{
				messcnt = 0x7fffffff;
			}
			else if(frmidx % messcycle == 0)
			{
				if(messcnt < 0x7fffffff) messcnt++;
			}
			int messended = 0;

			// -- Message Sleep --
			if(messSleep) {
				messcnt = 0;
			}
			messSleep--;
			m_range(messSleep, 0, 0x7fffffff);
			// ----

			for(int messidx = 0; messidx < messcnt; messidx++)
			{
				if(*mp == '\0')
				{
					messended = 1;
					break;
				}

				if(*mp == '\n')
				{
					mp++;
					x = 0;
					y++;
				}
				else
				{
					char *cp;

					if(_ismbblead(*mp))
					{
						errorCase(!mp[1]);
						errorCase(!_ismbbtrail(mp[1]));

						cp = mp;
						mp += 2;
					}
					else if('0' <= *mp && *mp <= '9')
					{
						static char cb[3];

						strcpy(cb, "０");
						cb[1] += *mp - '0';

						cp = cb;
						mp++;
					}
					else
					{
						error();
					}
					char ljchr[3];

					ljchr[0] = cp[0];
					ljchr[1] = cp[1];
					ljchr[2] = '\0';

					if(MESS_X_BOUND <= x)
					{
						x = 0;
						y++;
					}
					LDraw(ljchr,
						MESS_DRAW_X + x * MESS_STEP_X,
						MESS_DRAW_Y + y * MESS_STEP_Y
						);

					x++;
				}
			}
			SwapFrame();

#ifdef CHEAT_MODE
			printfDx(
				"R:(%d,%d,%d) E:(%d,%d,%d,%d,%d,%d) P:(%d,%d) "
				,SettInfo.RouteOpenList[ROL_A]
				,SettInfo.RouteOpenList[ROL_B]
				,SettInfo.RouteOpenList[ROL_C]
				,SettInfo.EndingWatchedList[0] // ED_GI
				,SettInfo.EndingWatchedList[1] // ED_GJ
				,SettInfo.EndingWatchedList[2] // ED_GP
				,SettInfo.EndingWatchedList[3] // ED_IJ
				,SettInfo.EndingWatchedList[4] // ED_IP
				,SettInfo.EndingWatchedList[5] // ED_PJ
				,Scdc->ActionIndex
				,kdk_hiaidx
				);
#endif

			if(ScRouteSentakuMode)
			{
				skipmode = 0;
				automode = 0;

				int rsx_inp_decide = GetHit(INP_ROT_DECIDE) || cStatus == 1;
				int rsx_inp_dir4   = GetHit(INP_DIR_4);
				int rsx_inp_dir6   = GetHit(INP_DIR_6);

				if(ScRotSenExecFrame(rsx_inp_decide, rsx_inp_dir4, rsx_inp_dir6))
				{
					break;
				}
				// Sace, Load ... があるので、decide は下方でブロックする。
				if(rsx_inp_dir4 || rsx_inp_dir6)
//				if(rsx_inp_decide || rsx_inp_dir4 || rsx_inp_dir6)
				{
					goto end_input;
				}
			}
			if(skipmode)
			{
				// ? このページは未読 && 未読は飛ばさない
				if(kdk_hiaidx <= Scdc->ActionIndex && !SettInfo.Midokumotobasu)
					skipmode = 0;
			}
			if(skipmode)
			{
				static int lclfrmcnt;

				if( GetHit(INP_ROT_DECIDE) || cStatus == 1 ||
					GetHit(INP_ROT_CANCEL) || cStatus == 2)
				{
					skipmode = 0;
				}

				lclfrmcnt++;
				if(lclfrmcnt % 4)
					goto end_input;

				goto end_action;
			}
			if(automode)
			{
				autocycle = (int)(SettInfo.AutoSpeed * AUTO_CYCLE_MAX);

				if(messended)
				{
					if(autocycle <= autocnt)
					{
						autocnt = 0;
						goto end_action;
					}
					autocnt++;
				}
				if( GetHit(INP_ROT_DECIDE) || cStatus == 1 ||
					GetHit(INP_ROT_CANCEL) || cStatus == 2)
				{
					automode = 0;
				}
				goto end_input;
			}

			int doOnSkip = 0;

			if(cStatus == 1 && Scdc->SelectButton != -1)
			{
				switch(Scdc->SelectButton)
				{
				case SC_MESSBTN_SAVE:
					{
						SLSaveMode = 1;
						SLSaveFunc = SaveFunc;

						SaveLoad();
						Dc->BCPct = 1.0;

						InitFont();
					}
					break;

				case SC_MESSBTN_LOAD:
					{
						SLSaveMode = 0;
						SLSaveFunc = NULL;

						SaveLoad();
						Dc->BCPct = 1.0;

						if(SLDoLoad)
						{
							Scdc->DoReload = 1;
							goto end_scenario;
						}
						InitFont();
					}
					break;

				case SC_MESSBTN_AUTO:
					{
						automode = 1;
						autocnt = 0;
					}
					break;

				case SC_MESSBTN_SKIP:
					{
						doOnSkip = 1;
					}
					break;

				case SC_MESSBTN_MENU:
					{
						PauseGamen();
						InitFont(); // PADでフォントを変更されるため。2013.1.24

						if(PG_ExitGame)
							goto end_scenario;
					}
					break;

				default:
					error();
				}
			}
			else if(GetHit(INP_ROT_DECIDE) || cStatus == 1 || koro < 0)
			{
				if(koro < 0)
					koroSleep = SHITA_KORO_SLEEP;

				if(messended && !ScRouteSentakuMode)
					break;

				messcnt = 0x7fffffff;
			}
			else if(GetHit(INP_ROT_CANCEL) || cStatus == 2)
			{
				int rsm_bk = ScRouteSentakuMode;

				Scdc->MessForDisp = 0;
				ScRouteSentakuMode = 0;

				for(; ; )
				{
					ExecScene();
					DrawScene();

					SwapFrame();

					cStatus = GetClick();
					if( GetHit(INP_ROT_DECIDE) || cStatus == 1 ||
						GetHit(INP_ROT_CANCEL) || cStatus == 2)
					{
						break;
					}
				}
				Scdc->MessForDisp = 1;
				ScRouteSentakuMode = rsm_bk;
			}
			else if(GetHit(INP_DIR_8) || GetHit(INP_DIR_4) || 0 < koro) // ログ読み
			{
				ScLogView(DrawScene);
				InitFont();
				koroSleep = SHITA_KORO_SLEEP;
			}
			else if(GetHit(INP_BOMB))
			{
				doOnSkip = 1;
			}

			if(doOnSkip)
			{
				skipmode = 1;
			}
end_input:;
		}
end_action:
		Scdc->ActionIndex++;

		if(kdk_hiaidx < Scdc->ActionIndex) // 前ページを既読化
		{
			kdk_hiaidx = Scdc->ActionIndex;
			SetKidokuHiestActionIndex(Scdc->ScnrTitle, kdk_hiaidx);
		}
		if(Scdc->Scenario.ActCnt <= Scdc->ActionIndex)
		{
			break;
		}
	}
end_scenario:

	FadeOut(Dc->BGMVolume, Dc->BGMVolume / 90);

	frameloop(frmidx, frmpct, 90)
	{
		DrawScene();

		SetAlpha(frmpct);
		SimpleDraw(Dc->PicBlackWall, 0, 0, 0);
		ResetAlpha();

		SwapFrame();
	}

	GC_NmPicUnloadAll();
	ScUnloadScenario();
	GC_UnloadAll();
}
