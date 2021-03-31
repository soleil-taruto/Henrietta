/*
	PzInit() -> PzMain() -> PzFnlz()
	         ^           ^
	    set params  get results
*/

#include "StdInc.h"

static void NetworkModeAdjustSRand(void) // ネット対戦で、しばしば乱数のシークの同期を失う問題対策
{
	if(Pzdc.NetworkMode)
	{
		static int initcnt;

		initcnt = m_max(initcnt, (int)Pzdc.FrameCnt);
		SRand(initcnt);
		initcnt++;
	}
}
static void GenerateStar(double x, double y, int color, int rensanum)
{
	DataCenter_t *dc = &Pzdc;

	errorCase(DUST_NUMMAX <= dc->DustCount);

	Dust_t *i = dc->DustList + dc->DustCount;

	i->GIdx = Pzdc.Dust[color];
//	i->GIdx = color;
//	i->GIdx = Get_Rnd(STAR_PIC_NUM - 1);
	i->Zoom = Get_Rnd(80) * 0.01 + 0.3;
	i->X = x;
	i->Y = y;
	i->Xadd = rndpm() * 5.0;
	i->Yadd = rndpm() * 10.0 - 7.75;
	i->R = rnd() * PI * 2.0;
	i->Radd = rnd() * 0.975 + 0.025;
	i->Radd *= rndbnd(0, 1) ? -1 : 1;

	dc->DustCount++;

	if(rensanum && rensanum <= RENSA_MAX)
	{
		i->GIdx = Pzdc.Rensa[rensanum - 1];
		i->Zoom = 1.0 + rensanum * 0.15;
		i->Xadd /= 3.0;
		i->Yadd = rnd() * -2.0 - 8.0;
		i->R = 0.0;
		i->Radd /= 100.0;

		if(i->X < 400) i->Xadd += 1.5;
		else           i->Xadd -= 1.5;
	}

	// 45hz -> 60hz
	i->Xadd *= 0.75;
	i->Yadd *= 0.75;
	i->Radd *= 0.75;

	if(Pzdc.TokushuButtle.BigFlower)
	{
		i->Zoom *= 3.5;
		i->Radd *= 0.25;
	}
}
static void GenerateBomb(double x, double y, int color, int rensanum)
{
	if(rensanum)
	{
		GenerateStar(x, y, color, rensanum);
	}
	for(int c = 10; c; c--)
	{
		GenerateStar(x, y, color, 0);
	}
}

static int SR_fmap[MAP_X_BOUND][MAP_Y_BOUND];
static void Search_Relate(Map_t *m, sint x, sint y, int color)
{
	if(0 <= x && x < MAP_X_BOUND)
	if(0 <= y && y < MAP_Y_BOUND)
	{
		if(!SR_fmap[x][y])
		{
			Puyo_t *i = m->PanelTable[x][y].Puyo;

			if(i && i->Color == color)
			{
				SR_fmap[x][y] = 1;

				Search_Relate(m, x - 1, y - 0, color);
				Search_Relate(m, x - 0, y - 1, color);
				Search_Relate(m, x + 1, y + 0, color);
				Search_Relate(m, x + 0, y + 1, color);
			}
		}
	}
}
static void EJCheckJama(Map_t *m, sint x, sint y)
{
	if(0 <= x && x < MAP_X_BOUND)
	if(0 <= y && y < MAP_Y_BOUND)
	{
		if(m->PanelTable[x][y].Puyo &&
			m->PanelTable[x][y].Puyo->Color == 0)
		{
			m->PanelTable[x][y].Puyo->Erase = 1;
		}
	}
}
static void EraseJama(Map_t *m)
{
	for(sint x = 0; x < MAP_X_BOUND; x++)
	for(sint y = 0; y < MAP_Y_BOUND; y++)
	{
		if(m->PanelTable[x][y].Puyo &&
			m->PanelTable[x][y].Puyo->Color && // not Jama
			m->PanelTable[x][y].Puyo->Erase)
		{
			EJCheckJama(m, x - 1, y - 0);
			EJCheckJama(m, x - 0, y - 1);
			EJCheckJama(m, x + 1, y + 0);
			EJCheckJama(m, x + 0, y + 1);
		}
	}
}
static int GetRelateCount(void)
{
	int rCnt = 0;

	for(int x = 0; x < MAP_X_BOUND; x++)
	for(int y = 0; y < MAP_Y_BOUND; y++)
	{
		if(SR_fmap[x][y]) rCnt++;
	}
	return rCnt;
}
static void CheckDoErase(Map_t *m)
{
	for(int x = 0; x < MAP_X_BOUND; x++)
	for(int y = 0; y < MAP_Y_BOUND; y++)
	{
		if(SR_fmap[x][y])
		{
			m->PanelTable[x][y].Puyo->Erase = 1;
		}
	}
}
static int CheckErase(Map_t *m, int checkOnlyMode = 0)
{
	int eCount = 0;

	for(int x = 0; x < MAP_X_BOUND; x++)
	for(int y = 0; y < MAP_Y_BOUND; y++)
	{
		if(m->PanelTable[x][y].Puyo &&
			m->PanelTable[x][y].Puyo->Color && // not Jama
			m->PanelTable[x][y].Puyo->Erase == 0)
		{
			memset(SR_fmap, 0x00, sizeof(SR_fmap));

			Search_Relate(m, x, y, m->PanelTable[x][y].Puyo->Color);
			int rCnt = GetRelateCount();

			if(Pzdc.PuyoEraseMin <= rCnt) // ? 消してよし。
			{
				if(checkOnlyMode)
					return 1;

				CheckDoErase(m);
				eCount++;
			}
		}
	}
	if(eCount) EraseJama(m);

	return eCount;
}
static void UncheckErase(Map_t *m)
{
	for(int x = 0; x < MAP_X_BOUND; x++)
	for(int y = 0; y < MAP_Y_BOUND; y++)
	{
		if(m->PanelTable[x][y].Puyo) m->PanelTable[x][y].Puyo->Erase = 0;
	}
}
static void Gravitate(Map_t *m)
{
	// 再落下
	for(int x = 0; x < MAP_X_BOUND; x++)
	for(int y = 0; y < MAP_Y_BOUND - 1; y++)
	{
		if(m->PanelTable[x][y].Puyo && m->PanelTable[x][y + 1].Puyo == NULL)
		{
			m->PanelTable[x][y + 1].Puyo = m->PanelTable[x][y].Puyo;
			m->PanelTable[x][y].Puyo = NULL;

			y -= 2; // おまえかー！

			if(y < -1) y = -1;
		}
	}

	// 位置、再設定
	for(int x = 0; x < MAP_X_BOUND; x++)
	for(int y = 0; y < MAP_Y_BOUND; y++)
	{
		if(m->PanelTable[x][y].Puyo)
		{
			m->PanelTable[x][y].Puyo->TrueX = (double)x * PANEL_X_SIZE + m->DrawX;
			m->PanelTable[x][y].Puyo->TrueY = (double)y * PANEL_Y_SIZE + m->DrawY;
		}
	}
}

// -- PzDE* --
double PzDECenterX;
double PzDECenterY;
int PzDESeOff;
int PzDEEffectOff;
int PzDEEraseCount; // 邪魔除く
int PzDEJEraseCount;
int PzDEDeadXList[MAP_X_BOUND * MAP_Y_BOUND];
int PzDEDeadYList[MAP_X_BOUND * MAP_Y_BOUND];
int PzDEDeadColorList[MAP_X_BOUND * MAP_Y_BOUND];
int PzDEDeadCnt;
// ----

static void DoErase(Map_t *m)
{
	int bombcnt = 0;

	PzDECenterX = 0.0;
	PzDECenterY = 0.0;

	PzDEEraseCount = 0;
	PzDEJEraseCount = 0;

	PzDEDeadCnt = 0;

	// 消す。
	for(int x = 0; x < MAP_X_BOUND; x++)
	for(int y = 0; y < MAP_Y_BOUND; y++)
	{
		if(m->PanelTable[x][y].Puyo && m->PanelTable[x][y].Puyo->Erase)
		{
			Puyo_t *i = m->PanelTable[x][y].Puyo;
			m->PanelTable[x][y].Puyo = NULL;

			i->Death = 1;

			double dx;
			double dy;

			if(!PzDEEffectOff) // ポン＝☆
			{
				GenerateBomb(
					dx = (double)x * PANEL_X_SIZE + m->DrawX + (PANEL_X_SIZE / 2),
					dy = (double)y * PANEL_Y_SIZE + m->DrawY + (PANEL_Y_SIZE / 2),
					i->Color,
					bombcnt ? 0 : m->RensaCount
					);

				/* CommonEffect_t *ce = */ AddCommonEffect(
					Dc->PicFlowers[i->Color],
					0,
					i->RealX + PANEL_X_SIZE / 2,
					i->RealY + PANEL_Y_SIZE / 2,
					rnd() * 2.0 * PI,
					2.0,
					0.9,
					0.0,
					0.0,
					rndpm() * 0.2,
					-0.03,
					0.9 / -rndbnd(20, 30)
					);
			}
			bombcnt++;

			PzDECenterX += dx;
			PzDECenterY += dy;

			if(i->Color == 0) // ? 邪魔
				PzDEJEraseCount++;
			else
				PzDEEraseCount++;

			// -- PzDEDead* --
			errorCase(MAP_X_BOUND * MAP_Y_BOUND <= PzDEDeadCnt);

			PzDEDeadXList[PzDEDeadCnt] = x;
			PzDEDeadYList[PzDEDeadCnt] = y;
			PzDEDeadColorList[PzDEDeadCnt] = i->Color;
			PzDEDeadCnt++;
			// ----
		}
	}
	if(bombcnt)
	{
		if(!PzDESeOff)
			PlaySe(Dc->SeErase);

		PzDECenterX /= bombcnt;
		PzDECenterY /= bombcnt;
	}

	Gravitate(m);
}
static void DoEraseAll(Map_t *m) // ゲームオーバー用
{
	for(int x = 0; x < MAP_X_BOUND; x++)
	for(int y = 0; y < MAP_Y_BOUND; y++)
	{
		if(m->PanelTable[x][y].Puyo)
		{
			m->PanelTable[x][y].Puyo->Erase = 1;
		}
	}
	PzDESeOff = 1;
	DoErase(m);
	PzDESeOff = 0;
}
void PzEx_DoErase(Map_t *m)
{
	DoErase(m);
}
static void JamaSousai(void)
{
	errorCase(Pzdc.MapList[0].JamaCount < 0);
	errorCase(Pzdc.MapList[1].JamaCount < 0);

	while(Pzdc.MapList[0].JamaCount && Pzdc.MapList[1].JamaCount)
	{
		Pzdc.MapList[0].JamaCount--;
		Pzdc.MapList[1].JamaCount--;
	}
}
static void MyShuffle(int *list, int listSize, int exDir)
{
	if(exDir)
	{
		int i = 0;
		int j = listSize - 1;

		while(i < j)
		{
			int tmp = list[i];
			list[i] = list[j];
			list[j] = tmp;

			i++;
			j--;
		}
	}
}
static int BonusWarp(Map_t *m, int x, int y, int exDir) // ret : ? 移動した。
{
	if(m->PanelTable[x][y].Puyo == NULL) // ? ぷよ無し
	{
		return 0;
	}
	int xlst[MAP_X_BOUND];

	for(int xi = 0; xi < MAP_X_BOUND; xi++)
	{
		xlst[xi] = xi;
	}
//	Shuffle(xlst, MAP_X_BOUND); // BUG!
	MyShuffle(xlst, MAP_X_BOUND, exDir);

	int destX;

	for(int i = 0; i < MAP_X_BOUND; i++)
	{
		if(m->PanelTable[xlst[i]][2].Puyo == NULL) // ? 空いてる列発見
		{
			destX = xlst[i];
			goto foundDestX;
		}
	}
	return 0; // ワープ不可

foundDestX:
	int destY;

	for(destY = 3; destY < MAP_Y_BOUND; destY++)
	{
		if(m->PanelTable[destX][destY].Puyo != NULL)
		{
			break;
		}
	}
	destY--;

	Puyo_t *puyo = m->PanelTable[x][y].Puyo;

	m->PanelTable[destX][destY].Puyo = puyo;
	m->PanelTable[x][y].Puyo = NULL;

	puyo->TrueX = (double)destX * PANEL_X_SIZE + m->DrawX;
	puyo->TrueY = (double)destY * PANEL_Y_SIZE + m->DrawY;
//	puyo->RealX = puyo->TrueX;
//	puyo->RealY = (double)m->DrawY - PANEL_Y_SIZE;

	puyo->WarpRot = 30.0 + rnd();

	if(x < destX)
	{
		puyo->WarpRot *= -1.0;
	}

	PzefAddPuyoWarp(
		(x + 0.5) * PANEL_X_SIZE + m->DrawX,
		(y + 0.5) * PANEL_Y_SIZE + m->DrawY,
		puyo->Color
		);

	return 1;
}
static int IsRTFar(Map_t *m)
{
	for(int x = 0; x < MAP_X_BOUND; x++)
	for(int y = 0; y < MAP_Y_BOUND; y++)
	{
		Puyo_t *puyo = m->PanelTable[x][y].Puyo;

		if(puyo)
		if(
			PANEL_X_SIZE / 3.0 < abs(puyo->RealX - puyo->TrueX) ||
			PANEL_Y_SIZE / 3.0 < abs(puyo->RealY - puyo->TrueY)
			)
			return 1;
	}
	return 0;
}

#define RENSA_CHECK_START(m) \
	m->DoRensaProcess = 1; \
	m->RensaCount = 0; \
	m->RensaTimer = 20 /* CheckErase(m, 1) ? 30 : 20 */

static void ExecFrameMap(int mapIndex)
{
	Map_t *m = Pzdc.MapList + mapIndex;

	int kousokuAtFirst = m->Kousoku;

	if(!Pzdc.TokushuButtle.StealthDisable[mapIndex])
	{
		for(int x = 0; x < MAP_X_BOUND; x++)
		for(int y = 0; y < MAP_Y_BOUND; y++)
		{
			Puyo_t *p = m->PanelTable[x][y].Puyo;

			if(p && Pzdc.TokushuButtle.Stealth[p->Color])
			{
				p->A -= 0.003;
				m_range(p->A, 0.0, 1.0);
			}
		}
	}

	// -- 特殊効果 --
	if(m->TokushuRakka < 0) // 高速落下禁止
	{
		m->Kousoku = 0;
	}
	if(m->TokushuRakka > 0) // 常に高速落下
	{
		m->Kousoku = 1;
	}
	if(m->TokushuTimeout <= Pzdc.FrameCnt) // 特殊効果終了
	{
		m->TokushuRakka = 0;
		m->TokushuHidden = 0;
	}
	// ----

	if(m->ForceSleepTimer)
	{
		m->ForceSleepTimer--;
		goto endfunc;
	}
	if(m->DoHissatsu)
	{
		m->DoHissatsu = 0;
		m->HissUsedCount++;
		m->HissSleepTimer = 60;

		PzHissatsu(mapIndex, Pzdc.HissatsuType[mapIndex]);

		RENSA_CHECK_START(m); // ブロック消えによる再連鎖
		m->RensaTimer += 30;
		JamaSousai();

		// 不要だが、念のため
		{
			m->SecchiTimer = 0; // シビアなタイミングで接地すると何かあるかもしれないので、、、
		}
	}

#define RENSA_DELAY 55
#define RENSA_DELAY_ON_HISPEED 55
//#define RENSA_DELAY 40
//#define RENSA_DELAY_ON_HISPEED 40
//#define RENSA_DELAY 75
//#define RENSA_DELAY_ON_HISPEED 25

	JamaSousai();

	// -- 連鎖 --
	if(m->DoRensaProcess)
	{
		m->RensaTimer && m->RensaTimer--;

		if(!m->RensaTimer && !m->RensaCount && IsRTFar(m))
			m->RensaTimer++;

		if(!m->RensaTimer)
		{
			if(CheckErase(m))
			{
				int cScoreBrit = 1;
				int cScore = 0;
				int jCnt = 0;
				int rCnt = m->RensaCount + 1;

				switch(rCnt)
				{
				case 1: cScoreBrit =  1; jCnt =  0; break;
				case 2: cScoreBrit =  4; jCnt =  6; break;
				case 3: cScoreBrit =  7; jCnt = 10; break;
				case 4: cScoreBrit = 10; jCnt = 14; break;

					// 5～
				default:
					cScoreBrit =  rCnt + 6;
					jCnt       = (rCnt + 3) * 2;
					break;
				}

				int destjmidx = 1 - mapIndex;
//				Pzdc.MapList[destjmidx].JamaCount += jCnt; // エフェクトから加算してもらう。

				Map_t *destjm = Pzdc.MapList + destjmidx;

				m->RensaProcessChuu = 1;
				m->RensaCount++;
				m->RensaTimer = m->Kousoku ? RENSA_DELAY_ON_HISPEED : RENSA_DELAY;

				// m->RensaCount incr 済み！

				DoErase(m);

				cScore += PzDEEraseCount * 10;
				cScore += PzDEJEraseCount * 35;
				cScore *= cScoreBrit;

				     if(9 <= PzDEEraseCount + PzDEJEraseCount) jCnt += 1 + rndbnd(0, 1);
				else if(6 <= PzDEEraseCount + PzDEJEraseCount) jCnt += 1;
				else if(5 <= PzDEEraseCount + PzDEJEraseCount) jCnt += rndbnd(0, 1);

				// m->RensaCount incr 済み！

				{
					int tb_jExp = Pzdc.TokushuButtle.JamaExpand;
					int tb_sExp = Pzdc.TokushuButtle.ScoreExpand;

					if(tb_jExp)
					{
						errorCase(tb_jExp < 0 || BORDER_OF_NUMERIC < tb_jExp);
						jCnt *= tb_jExp;
					}
					if(tb_sExp)
					{
						errorCase(tb_sExp < 0 || BORDER_OF_NUMERIC < tb_sExp);
						cScore *= tb_sExp;
					}
				}

				m->Score += cScore;
//				jCnt     += cScore / 70;

				// 邪魔エフェクト
				{
					double decx = PzDECenterX;
					double decy = PzDECenterY;

					int endmi = -1; // -1: rnd, 0 - 1: L or R

					if(m->JamaCount) // ? 自分に邪魔が積まれている。
					{
						if(m->JamaCount < jCnt) // ? 相殺でお釣りが来る。
						{
							endmi = -1;
						}
						else
						{
							endmi = 0; // 自分のところ
						}
					}
					else
					{
						endmi = 1; // 相手のところ
					}
					if(mapIndex == 1 && endmi != -1)
					{
						endmi = 1 - endmi;
					}
//					endmi = -1; // test

#define JE_LENDX ( 43 + 16)
#define JE_RENDX (525 + 16)
#define JE_ENDY     6

					if(1 <= jCnt) // じゃまいくよー
					{
						for(int jei = 0; jei < 30; jei++)
						{
							double endx;
							double endy;

							// Set endx, endy
							{
								int tmpmi = endmi;

								if(tmpmi == -1)
									tmpmi = rndbnd(0, 1);

								errorCase(tmpmi < 0 || 1 < tmpmi);

								endx = tmpmi == 0 ? JE_LENDX : JE_RENDX;
								endy = JE_ENDY;
							}

							if(jei == 0)
							{
//								CI_AddJamaStar(jCnt, destjmidx, decx, decy, endx, endy); // タイムラグが問題

								destjm->JamaCount += jCnt;
								destjm->UIJamaSleep = 30;
							}
//							else
								CI_AddJamaStar(0, destjmidx, decx, decy, endx, endy);
						}
					}
				}

				// 連鎖エフェクト --->

				int effno = 0;
				int side = mapIndex;

				switch(m->RensaCount)
				{
				case 3: CI_AddCutIn(effno = 3, mapIndex, side); break;
				case 5: CI_AddCutIn(effno = 5, mapIndex, side); break;
				case 7: break;
				}
				if(7 <= m->RensaCount)
				{
					if(!CheckErase(m))
					{
						CI_AddCutIn(effno = 7, mapIndex, side);
					}
					UncheckErase(m);
				}
				switch(effno)
				{
				case 3: PlaySe(Dc->SeRen3); break;
				case 5: PlaySe(Dc->SeRen5); break;
				case 7: PlaySe(Dc->SeRen7); break;

				default:
					{
						static int lastSeh;
						int seh;

						do {
							seh = Get_Rnd(2);
						}
						while(seh == lastSeh);
						lastSeh = seh;

						switch(seh)
						{
						case 0: PlaySe(Dc->SeSore); break;
						case 1: PlaySe(Dc->SeEi); break;
						case 2: PlaySe(Dc->SeHa); break;

						default:
							error();
						}
					}
				}
			}
			else // 連鎖終了
			{
				if(m->Kousoku &&
					m->RensaCount)
					m->ForceSleepTimer = 0;
//					m->ForceSleepTimer = RENSA_DELAY - RENSA_DELAY_ON_HISPEED;

				m->DoRensaProcess = 0;
				m->RensaProcessChuu = 0;

				if(JAMA_OOSUGI_NUM <= Pzdc.MapList[1 - mapIndex].JamaCount &&
					2 <= m->RensaCount
					)
				{
					PlaySe(Dc->SeIkuyo);
				}

				if(mapIndex == 0 && Pzdc.P1Muteki)
				{
					DoEraseAll(m);
				}
				if(mapIndex == 1 && Pzdc.P2Muteki)
				{
					DoEraseAll(m);
				}
				m->NeedInitAI = 1;

				if(mapIndex == 1)
				{
					int rcnt = m->RensaCount;
					m_range(rcnt, 0, DBG_2P_RENSA_COUNTER_MAX - 1);

					Pzdc.P2RensaCounter[rcnt]++;
					Pzdc.P2SecchiCounter++;
				}
			}
		}

		/*
		if(m->Kousoku)
		{
			HiSpeedPuyoMap(m);
		}
		*/

		goto endfunc;
	}
	if(m->RensaCount)
	{
//		Pzdc.MapList[1 - mapIndex].JamaCount += m->RensaCount - 1; // 旧位置
		m->RensaCount = 0;
	}
	// ----

	JamaSousai();

	// -- 邪魔 --
	if(m->DoJamaProcess)
	{
		int j2ColorMode = m->Jama2ColorMode;
		m->Jama2ColorMode = 0;

		int cJCnt = m_min(m->JamaCount, JAMA_MAX_ONCE);
		int jCnt = cJCnt;
		int x;
		int y;
		int tsumiJun[MAP_X_BOUND];

		for(x = 0; x < MAP_X_BOUND; x++)
		{
			tsumiJun[x] = x;
		}
		for(x = 0; x < MAP_X_BOUND - 1; x++)
		{
			int xf = rndbnd(x, MAP_X_BOUND - 1);
			int tjSwap;

			tjSwap       = tsumiJun[x];
			tsumiJun[x]  = tsumiJun[xf];
			tsumiJun[xf] = tjSwap;
		}
		int tsumiCount = 0;

		while(jCnt)
		{
			const int jHiestY = 2; // 邪魔を積める最も高い位置

			for(x = 0; x < MAP_X_BOUND; x++)
			{
				if(m->PanelTable[x][jHiestY].Puyo == NULL) // ? まだ積み上がってない
				{
					break;
				}
			}
			if(x == MAP_X_BOUND) // ? 全て積み上がってる -> もう落とせない
			{
				break;
			}
			errorCase(1000 < tsumiCount); // anti endless-loop

			x = tsumiJun[tsumiCount++ % MAP_X_BOUND];

			for(y = 0; y < MAP_Y_BOUND; y++)
			{
				if(m->PanelTable[x][y].Puyo) // ? 頂上
				{
					break;
				}
			}
			if(jHiestY < y)
			{
				y--;

				double tx = x * PANEL_X_SIZE + m->DrawX;
				double ty = y * PANEL_Y_SIZE + m->DrawY;

				int jColor = 0;

				if(j2ColorMode)
					jColor = rndbnd(1, PUYO_COLOR_NUM - 1);

				AddPuyo(m->PanelTable[x][y].Puyo = CreatePuyo(
					jColor,
					tx,
					(double)(y - MAP_Y_BOUND) * PANEL_Y_SIZE,
					tx,
					ty
					));

				jCnt--;
			}
		}
		jCnt = cJCnt;

		if(JAMA_OOSUGI_NUM <= jCnt) // 邪魔が多い -> 打撃
		{
			CI_AddDamage(mapIndex, Pzdc.MapList[mapIndex].PlayerChar);
			PlaySe(Dc->SeUwa);
		}
		m->UIJamaCount = m->JamaCount;
		m->UIJamaSleep = 25;

		m->DoJamaProcess = 0;
		m->JamaCount -= cJCnt;
		m->JamaTimer = jCnt ? 60 : 0;

		if(j2ColorMode)
		{
			RENSA_CHECK_START(m);

			m->RensaTimer += m->JamaTimer;
			m->JamaTimer = 0;

			goto endfunc;
		}
	}
	if(m->JamaTimer)
	{
		m->JamaTimer--;
		goto endfunc;
	}
	// ----

	if(m->NeedInitRakka)
	{
		m->NeedInitAI = 1;

		// 落下初期化
		{
			m->RakkaChuu[0] = m->StockTable[0][0];
			m->RakkaChuu[1] = m->StockTable[0][1];

			m->StockTable[0][0] = m->StockTable[1][0];
			m->StockTable[0][1] = m->StockTable[1][1];
			m->StockTable[0][0]->TrueX = m->StockDrawX[0][0];
			m->StockTable[0][0]->TrueY = m->StockDrawY[0][0];
			m->StockTable[0][1]->TrueX = m->StockDrawX[0][1];
			m->StockTable[0][1]->TrueY = m->StockDrawY[0][1];

			int color;

			color = GetNextColor(mapIndex);
			m->StockTable[1][0] = CreatePuyo(color, 133.3 * color, -30.0, m->StockDrawX[1][0], m->StockDrawY[1][0]);
			color = GetNextColor(mapIndex);
			m->StockTable[1][1] = CreatePuyo(color, 133.3 * color, -30.0, m->StockDrawX[1][1], m->StockDrawY[1][1]);

			AddPuyo(m->StockTable[1][0]);
			AddPuyo(m->StockTable[1][1]);

			m->RakkaX = 2;
//			m->RakkaY = 0; // ぷよぷよっぽく出現
			m->RakkaY = 2;
			m->RakkaDir = 2;
//			m->RakkaDir = 6;
			m->SecchiTimer = 0;
			m->HoverTimer = 0;

			if(m->PanelTable[2][3].Puyo != NULL) m->RakkaY--;
			if(m->PanelTable[2][2].Puyo != NULL) m->RakkaY--;

			while(m->RakkaY < 0) m->RakkaY++; // HACK
		}
		m->NeedInitRakka = 0;
		m->AntiRakkaTimer = 30;
	}
	/*
	if(HOVER_TIMER_MAX < m->HoverTimer) // ホバリング阻止 // ちょっと乱暴な止め方なのでやめ
	{
		m->IdouDir = 0;
		m->KaitenDir = 0;
		m->Kousoku = 1;
	}
	*/
	if(m->AntiRakkaTimer)
	{
		m->Kousoku = 0;
		m->AntiRakkaTimer--;
	}

	if(m->NeedInitAI)
	{
		if(mapIndex == 1 && Pzdc.UseAI) // AIサイド && AIを使用する。
		{
			PzAI_Reset();
		}
		m->NeedInitAI = 0;
	}

	// -- 自由落下 --
	if(m->AntiRakkaTimer == 0)
	{
#define JR_SPEED    (0.01 * 0.75)
#define JR_HI_SPEED (0.75 * 0.75)

		if(m->Kousoku)
		{
			m->RakkaY += JR_HI_SPEED;

			if(m->SecchiTimer == 0) // ? 接地していない内
				m->Score++;
		}
		else
			m->RakkaY += JR_SPEED;
	}
	// ----

	// -- 音 --
//	if(m->IdouDir)   PlaySe(Dc->SeMove);
//	if(m->KaitenDir) PlaySe(Dc->SeRotate);
	// ----
	int idouDone = 0;
	int kaitenDone = 0;

	/*
		AI の場合は、移動・回転しようとして、移動・回転出来ない場合は音を出さない。

		AI は到達不可能になっても、そこに行こうとするので、見苦しい動きをする。
		音が出なければ諦観しているように見えるだろう。
	*/
	if(m->SelfIndex == 0 || Pzdc.UseAI == 0) // ? プレイヤーが人間の場合
	{
		if(m->IdouDir)   idouDone = 1;
		if(m->KaitenDir) kaitenDone = 1;
	}

	// やっぱり押しっぱなしとかうるさいので、、、
	idouDone = 0;
	kaitenDone = 0;

restart:
	int x1 = m->RakkaX;
	int y1 = (int)m->RakkaY;
	int x2 = x1;
	int y2 = y1;

	switch(m->RakkaDir)
	{
	case 2: y2++; break;
	case 4: x2--; break;
	case 6: x2++; break;
	case 8: y2--; break;
	}

	errorCase(x2 < 0);
	errorCase(MAP_X_BOUND <= x2);
	errorCase(y2 < 0);
	errorCase(MAP_Y_BOUND <= y2);

	if(m->PanelTable[x1][y1].Puyo || m->PanelTable[x2][y2].Puyo) // ? ゲームオーバー
	{
		// ボーナス・ワープ
		{
			if(kousokuAtFirst == 0 || (mapIndex == 1 && Pzdc.UseAI))
			{
				int warped = 0;

				NetworkModeAdjustSRand();

				warped |= BonusWarp(m, x1, y1, 0);
				warped |= BonusWarp(m, x2, y2, 1);

				if(warped)
				{
					PlaySe(Dc->SeErase);

					Gravitate(m);

					RENSA_CHECK_START(m);
					goto endfunc;
				}
			}
		}

		Pzdc.GameOver = 1;
		Pzdc.Winner = 1 - mapIndex;
	}

	if(m->IdouDir)
	{
		// 移動
		{
			if(m->IdouDir == -1) // 左
			{
				if(x1 && x2)
				{
					x1--;
					x2--;

					if( m->PanelTable[x1][y1].Puyo == NULL &&
						m->PanelTable[x2][y2].Puyo == NULL) {
						m->RakkaX--; idouDone = 1;
					}
				}
			}
			else if(m->IdouDir == 1) // 右
			{
				if(x1 < MAP_X_BOUND - 1 && x2 < MAP_X_BOUND - 1)
				{
					x1++;
					x2++;

					if( m->PanelTable[x1][y1].Puyo == NULL &&
						m->PanelTable[x2][y2].Puyo == NULL) {
						m->RakkaX++; idouDone = 1;
					}
				}
			}
		}
		m->IdouDir = 0;

		goto restart;
	}
	if(m->KaitenDir)
	{
		// 回転
		{
			Pzr_Dir = m->RakkaDir;
			Pzr_RotDir = m->KaitenDir; // -1 == 反時計, 1 == 時計
			Pzr_X = m->RakkaX;
			Pzr_Y = m->RakkaY;
			Pzr_Map = m;

			DoRotate();

			m->RakkaDir = Pzr_Dir;
			m->RakkaX = Pzr_X;
			m->RakkaY = Pzr_Y;

			kaitenDone = 1;
		}
		m->KaitenDir = 0;

		goto restart;
	}

	// -- 音 --
	if(idouDone)   PlaySe(Dc->SeMove);
	if(kaitenDone) PlaySe(Dc->SeRotate);
	// ----

	// ? 接地
	if(
		y1 == MAP_Y_BOUND - 1 || m->PanelTable[x1][y1 + 1].Puyo ||
		y2 == MAP_Y_BOUND - 1 || m->PanelTable[x2][y2 + 1].Puyo)
	{
		// 位置、再固定
		m->RakkaX = x1;
		m->RakkaY = (double)y1;

		m->SecchiTimer += m->Kousoku ? 3 : 1;

		if(SECCHI_TIMER_MAX < m->SecchiTimer || HOVER_TIMER_MAX < m->HoverTimer) // ? 位置、確定
		{
			// ? バグ、なんかめり込んでる。
			errorCase(m->PanelTable[x1][y1].Puyo || m->PanelTable[x2][y2].Puyo);

			m->RakkaChuu[0]->RotCnt = PUYO_ROTCNT_MAX;
			m->RakkaChuu[1]->RotCnt = PUYO_ROTCNT_MAX;
			m->RakkaChuu[0]->G_Mode = 1;
			m->RakkaChuu[1]->G_Mode = 1;

			int underFlag = y1 < y2;

			// -- 再落下、定着 --
			for(; y1 < MAP_Y_BOUND - 1; y1++) if(m->PanelTable[x1][y1 + 1].Puyo) break;
			if(m->PanelTable[x1][y1].Puyo) error();
			m->PanelTable[x1][y1].Puyo = m->RakkaChuu[0];

			// ? バグ、重なった。
			if(m->PanelTable[x2][y2].Puyo && y2) y2--;

			for(; y2 < MAP_Y_BOUND - 1; y2++) if(m->PanelTable[x2][y2 + 1].Puyo) break;
			if(m->PanelTable[x2][y2].Puyo) error();
			m->PanelTable[x2][y2].Puyo = m->RakkaChuu[1];
			// ----

			if(underFlag && y2 < y1) // ? 入れ替わり発生
			{
				Puyo_t *tmpPuyo = m->PanelTable[x1][y1].Puyo;

				m->PanelTable[x1][y1].Puyo = m->PanelTable[x2][y2].Puyo;
				m->PanelTable[x2][y2].Puyo = tmpPuyo;
			}

			// 位置、再設定
			m->PanelTable[x1][y1].Puyo->TrueX = (double)x1 * PANEL_X_SIZE + m->DrawX;
			m->PanelTable[x1][y1].Puyo->TrueY = (double)y1 * PANEL_Y_SIZE + m->DrawY;
			m->PanelTable[x2][y2].Puyo->TrueX = (double)x2 * PANEL_X_SIZE + m->DrawX;
			m->PanelTable[x2][y2].Puyo->TrueY = (double)y2 * PANEL_Y_SIZE + m->DrawY;

			// 次へ
			RENSA_CHECK_START(m); // 連鎖チェック開始
			m->DoJamaProcess = 1;
			m->NeedInitRakka = 1;

			if(Pzdc.MapList[1 - mapIndex].RensaProcessChuu) // 相手が連鎖中 -> 邪魔を落とすの保留
			{
				m->DoJamaProcess = 0;
			}

			PlaySe(Dc->SeDecide); // 音

			goto endfunc;
		}
	}
	else
	{
		int mienai =
			m->PanelTable[0][2].Puyo &&
			m->PanelTable[1][2].Puyo &&
			m->PanelTable[2][2].Puyo &&
			m->PanelTable[3][2].Puyo &&
			m->PanelTable[4][2].Puyo &&
			m->PanelTable[5][2].Puyo; // ? 見える所は全部埋まっている。

		/*
			これ以上邪魔を落とせないので、相手は攻撃出来ない。
			ホバリングで相手の自滅を待つことが出来るが、これはウザいので、阻止

			-> ホバリング阻止のため、高ペナルティ
		*/
		m->HoverTimer += m->SecchiTimer * (mienai ? 7 : 1);
		m->SecchiTimer = 0;

		/*
			ホバリング阻止について、
			移動・回転のタイミングによっては、かなり長時間、あるいは永久にホバリングが可能となる抜け道があるが、
			そのような操作は人間業では不可能と見なし、よしとする。
		*/
	}

	/*
		邪魔処理, 連鎖処理, 接地した瞬間ではない、このタイミングでなら
		必殺技を発動しても問題ない...はず
	*/

	// 必殺関係
	{
		int lastGStt = m->GoStatus;

		m->GoStatus = HISS_SCORE <= m->Score ? 1 : 0;

		if(!lastGStt && m->GoStatus) // ? Off -> On
		{
			PzefAddStarPon(m->GoX + 34, m->GoY + 15, 100);
			PlaySe(Dc->SeHissOk);
		}
	}
	if(m->Hissatsu && m->HissSleepTimer == 0) // 必殺技発動！
	{
		if(!m->GoStatus) // ? 条件不足
		{
			goto cancelled_hissatsu;
		}
		m->Score -= HISS_SCORE;

		CI_AddHiss(mapIndex, (PuzzPair_t)m->PlayerChar);
		Pzdc.GameFreezeCount = 55;
		PlaySe(Dc->SeSpCmd);

		m->DoHissatsu = 1;
		m->HissSleepTimer = 60;

cancelled_hissatsu:;
	}
	m_countDown(m->HissSleepTimer);

	// 座標の適用
	{
		m->RakkaChuu[0]->TrueX = (double)m->RakkaX;
		m->RakkaChuu[0]->TrueY = m->RakkaY;

		SetDir(m->RakkaDir);

		m->RakkaChuu[1]->TrueX = m->RakkaChuu[0]->TrueX + DirX;
		m->RakkaChuu[1]->TrueY = m->RakkaChuu[0]->TrueY + (double)DirY;

		m->RakkaChuu[0]->TrueX = m->RakkaChuu[0]->TrueX * PANEL_X_SIZE + m->DrawX;
		m->RakkaChuu[0]->TrueY = m->RakkaChuu[0]->TrueY * PANEL_Y_SIZE + m->DrawY;
		m->RakkaChuu[1]->TrueX = m->RakkaChuu[1]->TrueX * PANEL_X_SIZE + m->DrawX;
		m->RakkaChuu[1]->TrueY = m->RakkaChuu[1]->TrueY * PANEL_Y_SIZE + m->DrawY;
	}
	// 落下水平ズレ補正
	{
		if(m->RakkaDir == 4 || m->RakkaDir == 6) // ? 水平
		{
			if(abs(m->RakkaChuu[0]->RealY - m->RakkaChuu[1]->RealY) < 11.0) // ? ほどよく接近
			{
				nearize(m->RakkaChuu[1]->RealY, m->RakkaChuu[0]->RealY, 0.5);
			}
		}
	}
	for(int i = 0; i < 2; i++)
	{
		if(m->Kousoku)
		{
			m->RakkaChuu[i]->RealX = m->RakkaChuu[i]->TrueX;
			m->RakkaChuu[i]->RealY = m->RakkaChuu[i]->TrueY;
		}
		else if(Pzdc.NetworkMode)
		{
			nearize(m->RakkaChuu[i]->RealX, m->RakkaChuu[i]->TrueX, 0.95);
			nearize(m->RakkaChuu[i]->RealY, m->RakkaChuu[i]->TrueY, 0.95);
		}
	}

	// Face
	{
		int faceLv;
		int puyocnt = 0;
		int jamacnt = 0;
		int x;
		int y;

		for(y = 0; y < MAP_Y_BOUND; y++)
		for(x = 0; x < MAP_X_BOUND; x++)
		{
			if(m->PanelTable[x][y].Puyo)
			{
				puyocnt++;

				if(m->PanelTable[x][y].Puyo->Color == 0)
				{
					jamacnt++;
				}
			}
		}

		     if(puyocnt <=  4 * MAP_X_BOUND) faceLv = 0;
		else if(puyocnt <=  6 * MAP_X_BOUND) faceLv = 1;
		else if(puyocnt <=  8 * MAP_X_BOUND) faceLv = 2;
		else if(puyocnt <= 10 * MAP_X_BOUND) faceLv = 3;
		else                                 faceLv = 4;

		m->FaceLevel = faceLv;

		     if(puyocnt <=  4 * MAP_X_BOUND) faceLv = 0;
		else if(puyocnt <=  8 * MAP_X_BOUND) faceLv = 1;
		else                                 faceLv = 2;

//		ピンチレベル = faceLv; // 0 - 2

		m->PuyoSenyuu = puyocnt;
		m->JamaSenyuu = jamacnt;
	}

endfunc:;
}

static void ExecFramePuyo()
{
	for(int index = 0; index < Pzdc.PuyoCount; )
	{
		Puyo_t *i = Pzdc.PuyoList[index];

		if(i->Death)
		{
			ReleasePuyo(UnaddPuyo(index));
		}
		else
		{
			if(i->Color) // not Jama
			{
				if(
					i->G_Mode &&
					i->RealX == i->TrueX &&
					i->RealY < i->TrueY
					)
				{
					i->LastYAdd += 0.4;
					i->RealY += i->LastYAdd;

					if(i->TrueY < i->RealY)
					{
						i->RealY = i->TrueY;
						
						for(int c = Get_Rnd(2) + 1; c; c--) // カチン＝☆
						{
							double idoR = rnd() * 2.0 * PI;

							CommonEffect_t *ce = AddCommonEffect(
								Dc->PicEffeHoshi,
								0,
								i->RealX + PANEL_X_SIZE / 2,
								i->RealY + PANEL_Y_SIZE,
								rnd() * 2.0 * PI,
								0.015,
								0.8,
								cos(idoR) * 5.0,
								abs(sin(idoR) * 5.0),
								rndpm() * 0.05,
								0.0,
								1.0 / -30
								);

							ce->KasokuRate = 0.8;
						}
					}
				}
				else
				{
					if(i->G_Mode)
					{
						i->LastYAdd = 0.0;
					}
notJama:
					if(abs(i->RealX - i->TrueX) < 1.0) i->RealX = i->TrueX;
					if(abs(i->RealY - i->TrueY) < 1.0) i->RealY = i->TrueY;

					nearize(i->RealX, i->TrueX, 0.9);
					nearize(i->RealY, i->TrueY, 0.9);
				}
			}
			else // Jama
			{
				double border = i->TrueY - 2.0;

				if(border < i->RealY)
				{
					goto notJama;
				}

				i->LastYAdd += 0.75;
				i->RealY += i->LastYAdd;

				if(border < i->RealY)
				{
					Map_t *mm;
					int imm;
					int xx;
					int yy;

					for(imm = 0; imm < 2; imm++)
					{
						mm = Pzdc.MapList + imm;

						for(xx = 0; xx < MAP_X_BOUND; xx++)
						for(yy = 0; yy < MAP_Y_BOUND; yy++)
						{
							if(mm->PanelTable[xx][yy].Puyo == i)
							{
								goto fndJama;
							}
						}
					}
					error();
fndJama:
					for(int iy = yy; iy < MAP_Y_BOUND; iy++)
					{
						Puyo_t *j = mm->PanelTable[xx][iy].Puyo;

						if(!j) error();

						j->RealY = iy * PANEL_Y_SIZE + mm->DrawY + 15.0;
					}
					i->LastYAdd = 0.0;
				}
			}

			index++;
		}
	}
}

static void DrawFramePuyo()
{
	for(int index = 0; index < Pzdc.PuyoCount; index++)
	{
		Puyo_t *i = Pzdc.PuyoList[index];

		if(0 < i->RotCnt)
		{
			i->RotCnt--;

			if(0 < i->RotCnt)
			{
				double r = (double)i->RotCnt / PUYO_ROTCNT_MAX;

				r *= PI;
				r *= 0.5;

				for(int d = 0; d < 2; d++)
				{
					DppInit(
						Pzdc.Puyo[i->Color],
						i->RealX + PANEL_X_SIZE * 0.5,
						i->RealY + PANEL_Y_SIZE * 0.5,
						FALSE
						);
					DppRotate(d ? r : -r);

					SetAlpha(i->A * 0.5);
					DppDraw();
					ResetAlpha();
				}
				goto end_draw;
			}
		}

		if(0.001 < abs(i->WarpRot))
		{
			DppInit(
				Pzdc.Puyo[i->Color],
				i->RealX + PANEL_X_SIZE * 0.5,
				i->RealY + PANEL_Y_SIZE * 0.5,
				FALSE
				);
			DppRotate(i->WarpRot);

			SetAlpha(i->A);
			DppDraw();
			ResetAlpha();

			nearize(i->WarpRot, 0.0, 0.9);
			goto end_draw;
		}

		SetAlpha(i->A);
		DrawGraph((int)i->RealX, (int)i->RealY, Pzdc.Puyo[i->Color], FALSE);
		ResetAlpha();
end_draw:;
	}
}

#define DUST_Y_BORDER 700.0

static void DrawStar(int hiSpeedFlag)
{
	for(int index = 0; index < Pzdc.DustCount; index++)
	{
		Dust_t *i = Pzdc.DustList + index;

		if(i->Y < DUST_Y_BORDER)
		{
			// 移動
			{
				i->X += i->Xadd;
				i->Y += i->Yadd;
				i->Yadd += Pzdc.TokushuButtle.BigFlower ? 0.045 : 0.1;
				i->R += i->Radd;

				if(PI * 2.0 < i->R)
				{
					i->R -= PI * 2.0;
				}
			}

			// 描画
			{
				DrawRotaGraph((int)i->X, (int)i->Y, i->Zoom, i->R, i->GIdx, TRUE, FALSE);
			}
		}
	}

	// シフト
	{
		int rIndex = m_max(0, Pzdc.DustCount - 300);
		int wIndex;

		for(wIndex = rIndex; 0 < wIndex; wIndex--)
		{
			Dust_t *i = Pzdc.DustList + (wIndex - 1);

			if(i->Y < DUST_Y_BORDER)
			{
				break;
			}
		}
		while(rIndex < Pzdc.DustCount)
		{
			Dust_t *i = Pzdc.DustList + rIndex;

			if(i->Y < DUST_Y_BORDER)
			{
				if(wIndex < rIndex)
				{
					Pzdc.DustList[wIndex] = *i;
				}
				wIndex++;
			}
			rIndex++;
		}
		Pzdc.DustCount = wIndex;
	}
}

void PzMain(void)
{
	// -- Check Params --
	errorCase(Pzdc.Chara[0][0] < 0 || CHARA_MAX <= Pzdc.Chara[0][0] || Pzdc.Chara[0][0] == CHARA_NONE);
	errorCase(Pzdc.Chara[0][1] < 0 || CHARA_MAX <= Pzdc.Chara[0][1]);
	errorCase(Pzdc.Chara[1][0] < 0 || CHARA_MAX <= Pzdc.Chara[1][0] || Pzdc.Chara[1][0] == CHARA_NONE);
	errorCase(Pzdc.Chara[1][1] < 0 || CHARA_MAX <= Pzdc.Chara[1][1]);

	errorCase(Pzdc.Basho < 0 || BASHO_MAX <= Pzdc.Basho);

	errorCase(Pzdc.HissatsuType[0] < 0 || HWT_MAX <= Pzdc.HissatsuType[0]);
	errorCase(Pzdc.HissatsuType[1] < 0 || HWT_MAX <= Pzdc.HissatsuType[1]);

	errorCase(!Pzdc.BGMFileNormal);
	errorCase(!Pzdc.BGMFilePinch);

	errorCase(Pzdc.NetKansenMode && Pzdc.NetworkMode == 0); // ? 観戦モードなのに、ネットワークモード off
	// ----

	Pzdc.BGMNormal = LoadBGM(ResourcePath(Pzdc.BGMFileNormal));
	Pzdc.BGMPinch  = LoadBGM(ResourcePath(Pzdc.BGMFilePinch));

	for(int mi = 0; mi < 2; mi++)
	{
		Pzdc.LPair[mi][0] = '\0';

		for(int ci = 0; ci < 2; ci++)
		{
			int chara = Pzdc.Chara[mi][ci];
			char *lchara = LCharaList[chara];

			errorCase(strlen(lchara) != 2);

			strcpy(Pzdc.LChara[mi][ci], lchara);
			strcat(Pzdc.LPair[mi], lchara);
		}
		Pzdc.IsSolo[mi] = Pzdc.Chara[mi][1] == CHARA_NONE;
	}
	Pzdc.LBasho = LBashoList[Pzdc.Basho];

	/*
		キャラ毎に決まる画像
		パズル終了後にハンドルが無効になるので、注意！
	*/
	Dc->PicWin1P  = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Pair\\%s\\勝利.png", Pzdc.LPair[0])));
	Dc->PicWin2P  = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Pair\\%s\\勝利.png", Pzdc.LPair[1])));
	Dc->PicLose1P = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Pair\\%s\\敗北.png", Pzdc.LPair[0])));
	Dc->PicLose2P = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Pair\\%s\\敗北.png", Pzdc.LPair[1])));

	Dc->PicCI1P1Ren3 = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\三連鎖.png", Pzdc.LChara[0][0])));
	Dc->PicCI1P2Ren3 = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\三連鎖.png", Pzdc.LChara[0][1])));
	Dc->PicCI2P1Ren3 = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\三連鎖.png", Pzdc.LChara[1][0])));
	Dc->PicCI2P2Ren3 = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\三連鎖.png", Pzdc.LChara[1][1])));
	Dc->PicCI1P1Ren5 = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\五連鎖.png", Pzdc.LChara[0][0])));
	Dc->PicCI1P2Ren5 = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\五連鎖.png", Pzdc.LChara[0][1])));
	Dc->PicCI2P1Ren5 = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\五連鎖.png", Pzdc.LChara[1][0])));
	Dc->PicCI2P2Ren5 = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\五連鎖.png", Pzdc.LChara[1][1])));
	Dc->PicCI1P1Ren7 = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\最終連鎖.png", Pzdc.LChara[0][0])));
	Dc->PicCI1P2Ren7 = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\最終連鎖.png", Pzdc.LChara[0][1])));
	Dc->PicCI2P1Ren7 = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\最終連鎖.png", Pzdc.LChara[1][0])));
	Dc->PicCI2P2Ren7 = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\最終連鎖.png", Pzdc.LChara[1][1])));

	Dc->PicCI1P1Damage = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\打撃.png", Pzdc.LChara[0][0])));
	Dc->PicCI1P2Damage = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\打撃.png", Pzdc.LChara[0][1])));
	Dc->PicCI2P1Damage = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\打撃.png", Pzdc.LChara[1][0])));
	Dc->PicCI2P2Damage = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\打撃.png", Pzdc.LChara[1][1])));

	Dc->PicCIHiss1P1 = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\必殺技.png", Pzdc.LChara[0][0])));
	Dc->PicCIHiss1P2 = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\必殺技.png", Pzdc.LChara[0][1])));
	Dc->PicCIHiss2P1 = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\必殺技.png", Pzdc.LChara[1][0])));
	Dc->PicCIHiss2P2 = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\必殺技.png", Pzdc.LChara[1][1])));

	Dc->PicFace[PZPLYR_1P1][0][0] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\普通1.png", Pzdc.LChara[0][0])));
	Dc->PicFace[PZPLYR_1P1][0][1] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\普通2.png", Pzdc.LChara[0][0])));
	Dc->PicFace[PZPLYR_1P1][0][2] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\普通3.png", Pzdc.LChara[0][0])));
	Dc->PicFace[PZPLYR_1P1][1][0] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\苦しい1.png", Pzdc.LChara[0][0])));
	Dc->PicFace[PZPLYR_1P1][1][1] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\苦しい2.png", Pzdc.LChara[0][0])));
	Dc->PicFace[PZPLYR_1P1][1][2] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\苦しい3.png", Pzdc.LChara[0][0])));
	Dc->PicFace[PZPLYR_1P1][2][0] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\最悪1.png", Pzdc.LChara[0][0])));
	Dc->PicFace[PZPLYR_1P1][2][1] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\最悪2.png", Pzdc.LChara[0][0])));
	Dc->PicFace[PZPLYR_1P1][2][2] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\最悪3.png", Pzdc.LChara[0][0])));
	// - - -
	Dc->PicFace[PZPLYR_1P2][0][0] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\普通1.png", Pzdc.LChara[0][1])));
	Dc->PicFace[PZPLYR_1P2][0][1] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\普通2.png", Pzdc.LChara[0][1])));
	Dc->PicFace[PZPLYR_1P2][0][2] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\普通3.png", Pzdc.LChara[0][1])));
	Dc->PicFace[PZPLYR_1P2][1][0] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\苦しい1.png", Pzdc.LChara[0][1])));
	Dc->PicFace[PZPLYR_1P2][1][1] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\苦しい2.png", Pzdc.LChara[0][1])));
	Dc->PicFace[PZPLYR_1P2][1][2] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\苦しい3.png", Pzdc.LChara[0][1])));
	Dc->PicFace[PZPLYR_1P2][2][0] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\最悪1.png", Pzdc.LChara[0][1])));
	Dc->PicFace[PZPLYR_1P2][2][1] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\最悪2.png", Pzdc.LChara[0][1])));
	Dc->PicFace[PZPLYR_1P2][2][2] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\最悪3.png", Pzdc.LChara[0][1])));
	// - - -
	Dc->PicFace[PZPLYR_2P1][0][0] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\普通1.png", Pzdc.LChara[1][0])));
	Dc->PicFace[PZPLYR_2P1][0][1] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\普通2.png", Pzdc.LChara[1][0])));
	Dc->PicFace[PZPLYR_2P1][0][2] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\普通3.png", Pzdc.LChara[1][0])));
	Dc->PicFace[PZPLYR_2P1][1][0] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\苦しい1.png", Pzdc.LChara[1][0])));
	Dc->PicFace[PZPLYR_2P1][1][1] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\苦しい2.png", Pzdc.LChara[1][0])));
	Dc->PicFace[PZPLYR_2P1][1][2] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\苦しい3.png", Pzdc.LChara[1][0])));
	Dc->PicFace[PZPLYR_2P1][2][0] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\最悪1.png", Pzdc.LChara[1][0])));
	Dc->PicFace[PZPLYR_2P1][2][1] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\最悪2.png", Pzdc.LChara[1][0])));
	Dc->PicFace[PZPLYR_2P1][2][2] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\最悪3.png", Pzdc.LChara[1][0])));
	// - - -
	Dc->PicFace[PZPLYR_2P2][0][0] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\普通1.png", Pzdc.LChara[1][1])));
	Dc->PicFace[PZPLYR_2P2][0][1] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\普通2.png", Pzdc.LChara[1][1])));
	Dc->PicFace[PZPLYR_2P2][0][2] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\普通3.png", Pzdc.LChara[1][1])));
	Dc->PicFace[PZPLYR_2P2][1][0] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\苦しい1.png", Pzdc.LChara[1][1])));
	Dc->PicFace[PZPLYR_2P2][1][1] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\苦しい2.png", Pzdc.LChara[1][1])));
	Dc->PicFace[PZPLYR_2P2][1][2] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\苦しい3.png", Pzdc.LChara[1][1])));
	Dc->PicFace[PZPLYR_2P2][2][0] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\最悪1.png", Pzdc.LChara[1][1])));
	Dc->PicFace[PZPLYR_2P2][2][1] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\最悪2.png", Pzdc.LChara[1][1])));
	Dc->PicFace[PZPLYR_2P2][2][2] = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\Solo\\%s\\最悪3.png", Pzdc.LChara[1][1])));
	// ----

	DispMusCursor(false);

	Pzdc.MapList[0].SelfIndex = 0;
	Pzdc.MapList[1].SelfIndex = 1;
	Pzdc.MapList[0].PlayerChar = 0;
	Pzdc.MapList[1].PlayerChar = 1;

	memset(&PzdcRet, 0x00, sizeof(PzdcRet_t));

#if 1
	// 基本
	for(int i = 0; i < COLOR_STREAM_LEN; i++)
	{
		Pzdc.ColorStream[i] = rndbnd(1, PUYO_COLOR_NUM - 1);
	}

	// クラスタごとにランダム
	{
		const int clusterSize = (PUYO_COLOR_NUM - 1) * 3;

		for(int i = 0; i + clusterSize <= COLOR_STREAM_LEN; i += clusterSize)
		{
			for(int c = 0; c < clusterSize; c++)
			{
				Pzdc.ColorStream[i + c] = c % (PUYO_COLOR_NUM - 1) + 1;
			}
			Shuffle(Pzdc.ColorStream + i, clusterSize);
		}
	}
#else

	// それぞれの個数を一定にする。... あんまり意味無い？
	{
		for(int i = 0; i < COLOR_STREAM_LEN; i++)
		{
			Pzdc.ColorStream[i] =  i % (PUYO_COLOR_NUM - 1) + 1;
		}
		for(int i = 0; i < COLOR_STREAM_LEN; i++)
		{
			int j = rndbnd(i, COLOR_STREAM_LEN - 1);

			if(i < j)
			{
				int swap = Pzdc.ColorStream[i];

				Pzdc.ColorStream[i] = Pzdc.ColorStream[j];
				Pzdc.ColorStream[j] = swap;
			}
		}
	}
#endif

	// 減色モード
	{
		TokushuButtle_t *t = &Pzdc.TokushuButtle;

		if(t->ColorMax)
		{
			errorCase(t->ColorMax < 1 || PUYO_COLOR_NUM <= t->ColorMax);

			int colors[PUYO_COLOR_NUM];
			memset(colors, 0x00, sizeof(colors));

			for(int i = 0; i < t->ColorMax; i++)
			{
				for(; ; )
				{
					int cc = rndbnd(1, PUYO_COLOR_NUM - 1);

					if(!colors[cc]) {
						colors[cc] = 1;
						break;
					}
				}
			}
			int e_colors[PUYO_COLOR_NUM];
			int eci = 0;

			for(int i = 0; i < PUYO_COLOR_NUM; i++)
			{
				if(colors[i])
				{
					e_colors[eci] = i;
					eci++;
				}
			}
			errorCase(eci != t->ColorMax);

			for(int i = 0; i < COLOR_STREAM_LEN; i++)
			{
				Pzdc.ColorStream[i] = e_colors[rndbnd(0, eci - 1)];
			}
		}

		if(t->PuyoEraseNum)
		{
			Pzdc.PuyoEraseMin = t->PuyoEraseNum;
		}
	}

	Pzdc.WallPaper  = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\場所\\%s\\背景.png", Pzdc.LBasho)));
	Pzdc.FrontPaper = LoadPzPicture(ResourcePath_x(xcout("Puzzle\\場所\\%s\\枠.png", Pzdc.LBasho)));

	Pzdc.Puyo[0] = Dc->PicBlocks[0]; // LoadPzPicture(ResourcePath("ブロック\\お邪魔.png"));
	Pzdc.Puyo[1] = Dc->PicBlocks[1]; // LoadPzPicture(ResourcePath("ブロック\\黄.png"));
	Pzdc.Puyo[2] = Dc->PicBlocks[2]; // LoadPzPicture(ResourcePath("ブロック\\紫.png"));
	Pzdc.Puyo[3] = Dc->PicBlocks[3]; // LoadPzPicture(ResourcePath("ブロック\\青.png"));
	Pzdc.Puyo[4] = Dc->PicBlocks[4]; // LoadPzPicture(ResourcePath("ブロック\\赤.png"));
	Pzdc.Puyo[5] = Dc->PicBlocks[5]; // LoadPzPicture(ResourcePath("ブロック\\緑.png"));

	Pzdc.Dust[0] = Dc->PicFlowers[0]; // LoadPzPicture(ResourcePath("ブロック\\散りお邪魔.png"));
	Pzdc.Dust[1] = Dc->PicFlowers[1]; // LoadPzPicture(ResourcePath("ブロック\\散り黄.png"));
	Pzdc.Dust[2] = Dc->PicFlowers[2]; // LoadPzPicture(ResourcePath("ブロック\\散り紫.png"));
	Pzdc.Dust[3] = Dc->PicFlowers[3]; // LoadPzPicture(ResourcePath("ブロック\\散り青.png"));
	Pzdc.Dust[4] = Dc->PicFlowers[4]; // LoadPzPicture(ResourcePath("ブロック\\散り赤.png"));
	Pzdc.Dust[5] = Dc->PicFlowers[5]; // LoadPzPicture(ResourcePath("ブロック\\散り緑.png"));

	for(int i = 0; i < RENSA_MAX; i++)
	{
//		Pzdc.Rensa[i] = Dc->PicEffeHoshi;
		Pzdc.Rensa[i] = Pzdc.Puyo[i % PUYO_COLOR_NUM];
	}
	Pzdc.Rensa[0]  = LoadPzPicture(ResourcePath("フィールド\\連鎖文字\\01連鎖.png"));
	Pzdc.Rensa[1]  = LoadPzPicture(ResourcePath("フィールド\\連鎖文字\\02連鎖.png"));
	Pzdc.Rensa[2]  = LoadPzPicture(ResourcePath("フィールド\\連鎖文字\\03連鎖.png"));
	Pzdc.Rensa[3]  = LoadPzPicture(ResourcePath("フィールド\\連鎖文字\\04連鎖.png"));
	Pzdc.Rensa[4]  = LoadPzPicture(ResourcePath("フィールド\\連鎖文字\\05連鎖.png"));
	Pzdc.Rensa[5]  = LoadPzPicture(ResourcePath("フィールド\\連鎖文字\\06連鎖.png"));
	Pzdc.Rensa[6]  = LoadPzPicture(ResourcePath("フィールド\\連鎖文字\\07連鎖.png"));
	Pzdc.Rensa[7]  = LoadPzPicture(ResourcePath("フィールド\\連鎖文字\\08連鎖.png"));
	Pzdc.Rensa[8]  = LoadPzPicture(ResourcePath("フィールド\\連鎖文字\\09連鎖.png"));
	Pzdc.Rensa[9]  = LoadPzPicture(ResourcePath("フィールド\\連鎖文字\\10連鎖.png"));
	Pzdc.Rensa[10] = LoadPzPicture(ResourcePath("フィールド\\連鎖文字\\11連鎖.png"));
	Pzdc.Rensa[11] = LoadPzPicture(ResourcePath("フィールド\\連鎖文字\\12連鎖.png"));
	Pzdc.Rensa[12] = LoadPzPicture(ResourcePath("フィールド\\連鎖文字\\13連鎖.png"));
	Pzdc.Rensa[13] = LoadPzPicture(ResourcePath("フィールド\\連鎖文字\\14連鎖.png"));
	Pzdc.Rensa[14] = LoadPzPicture(ResourcePath("フィールド\\連鎖文字\\15連鎖.png"));
	Pzdc.Rensa[15] = LoadPzPicture(ResourcePath("フィールド\\連鎖文字\\16連鎖.png"));
	Pzdc.Rensa[16] = LoadPzPicture(ResourcePath("フィールド\\連鎖文字\\17連鎖.png"));
	Pzdc.Rensa[17] = LoadPzPicture(ResourcePath("フィールド\\連鎖文字\\18連鎖.png"));
	Pzdc.Rensa[18] = LoadPzPicture(ResourcePath("フィールド\\連鎖文字\\19連鎖.png"));
	Pzdc.Rensa[19] = LoadPzPicture(ResourcePath("フィールド\\連鎖文字\\20連鎖.png"));
	Pzdc.Rensa[20] = LoadPzPicture(ResourcePath("フィールド\\連鎖文字\\21連鎖.png"));
	Pzdc.Rensa[21] = LoadPzPicture(ResourcePath("フィールド\\連鎖文字\\22連鎖.png"));

	Pzdc.Go[0] = LoadPzPicture(ResourcePath("フィールド\\go_off.png"));
	Pzdc.Go[1] = LoadPzPicture(ResourcePath("フィールド\\go_on.png"));

	Pzdc.MapList[0].DrawX = 25;
	Pzdc.MapList[0].DrawY = 29 - PANEL_Y_SIZE * 2;

	Pzdc.MapList[1].DrawX = 506;
	Pzdc.MapList[1].DrawY = 29 - PANEL_Y_SIZE * 2;

	Pzdc.MapList[0].StockDrawX[0][0] = 311;
	Pzdc.MapList[0].StockDrawY[0][0] = 29;
	Pzdc.MapList[0].StockDrawX[0][1] = 311;
	Pzdc.MapList[0].StockDrawY[0][1] = 74;
	Pzdc.MapList[0].StockDrawX[1][0] = 363;
	Pzdc.MapList[0].StockDrawY[1][0] = 59;
	Pzdc.MapList[0].StockDrawX[1][1] = 363;
	Pzdc.MapList[0].StockDrawY[1][1] = 104;

	Pzdc.MapList[1].StockDrawX[0][0] = 445;
	Pzdc.MapList[1].StockDrawY[0][0] = 29;
	Pzdc.MapList[1].StockDrawX[0][1] = 445;
	Pzdc.MapList[1].StockDrawY[0][1] = 74;
	Pzdc.MapList[1].StockDrawX[1][0] = 393;
	Pzdc.MapList[1].StockDrawY[1][0] = 59;
	Pzdc.MapList[1].StockDrawX[1][1] = 393;
	Pzdc.MapList[1].StockDrawY[1][1] = 104;

	{
		Puyo_t *pyp;
		int mi;
		int ni;
		int ji;

		for(mi = 0; mi < 2; mi++)
		for(ni = 0; ni < 2; ni++)
		for(ji = 0; ji < 2; ji++)
		{
			int color = GetNextColor(mi);

			AddPuyo(pyp = CreatePuyo(color, 133.3 * color, -30.0, Pzdc.MapList[mi].StockDrawX[ni][ji], Pzdc.MapList[mi].StockDrawY[ni][ji]));
			Pzdc.MapList[mi].StockTable[ni][ji] = pyp;
		}
	}

	Pzdc.MapList[0].FaceX[0] = 314;
	Pzdc.MapList[0].FaceY[0] = 155;
	Pzdc.MapList[0].FaceX[1] = 314;
	Pzdc.MapList[0].FaceY[1] = 323;
	Pzdc.MapList[1].FaceX[0] = 419;
	Pzdc.MapList[1].FaceY[0] = 155;
	Pzdc.MapList[1].FaceX[1] = 419;
	Pzdc.MapList[1].FaceY[1] = 323;

	Pzdc.MapList[0].GoX = 314;
	Pzdc.MapList[0].GoY = 487;
	Pzdc.MapList[1].GoX = 418;
	Pzdc.MapList[1].GoY = 487;

	if(Pzdc.NetworkMode)
	{
		int syncnt = 0;

		NtSequence[0].Syn = 1;

/*
	相手の同期を確認しても、相手がこちらの Syn を確認したかは分からないので、
	観戦者との同期も同様
	確実に Syn を送りつけるため、、、
*/
#define ST_DOUKI_MAX 10

		for(int frmidx = 0; frmidx < 60 * 30; frmidx++)
		{
			SimpleDraw(Dc->PicBlackWall, 0, 0, 0);

			MyCls();
			MyPrint("");
			MyPrint_x(xcout("　同期しています ... %f / 30.0", (double)frmidx / 60));

			ExecMyPrint();
			SwapFrame();

			if(Pzdc.NetKansenMode)
			{
				NtRecvKansenSq();

				if(NtKansenSq.Syn)
				{
					if(ST_DOUKI_MAX < syncnt)
					{
						break;
					}
					syncnt++;
				}
			}
			else
			{
				NtSendSequence();
				NtRecvSequence();

				if(NtSequence[1].Syn)
				{
					if(NtKansenshaCount) // 観戦者と同期
					{
						NtKansenSq.Syn = 1;
						NtHaishin();
					}
					if(ST_DOUKI_MAX < syncnt)
					{
						break;
					}
					syncnt++;
				}
			}
		}
		clsDx();
	}
	PzStartScene();

	PlayBGM(Pzdc.BGMNormal);

	Pzdc.MapList[0].NeedInitRakka = 1;
	Pzdc.MapList[1].NeedInitRakka = 1;

	for(Pzdc.FrameCnt = 0; ; Pzdc.FrameCnt++)
	{
		if(Pzdc.GameOver)
			break;

		if(Pzdc.TokushuButtle.FlowerRain)
		{
			Pzdc.RainPct += 0.00006;
			m_range(Pzdc.RainPct, 0.0, 0.3);

			if(krnd() < Pzdc.RainPct)
			{
				if(Pzdc.TokushuButtle.FlowerRainbow)
				{
					__int64 flower_idx = Pzdc.FrameCnt / 200;

					flower_idx %= 6;
					EDEx_AddFlowerRainIdx((int)flower_idx);
				}
				else
				{
					EDEx_AddFlowerRain();
				}
			}
		}

		if( Pzdc.GameFreezeCount) {
			Pzdc.GameFreezeCount--;
			goto draw_top;
		}

		int doPause = 0;

		// プレイヤー入力
		{
			Map_t *m = Pzdc.MapList;
			Map_t *mos = Pzdc.MapList + 1;

			m->KaitenDir = 0;
			m->IdouDir = 0;
			m->Kousoku = 0;
			m->Hissatsu = 0;

			int cStatus = GetMusBtn();

			if(GetRendaHit(INP_DIR_4))
			{
				m->IdouDir = -1;
			}
			if(GetRendaHit(INP_DIR_6))
			{
				m->IdouDir = 1;
			}
			if(GetRendaHit(INP_ROT_L))
			{
				m->KaitenDir = -1;
			}
			if(GetRendaHit(INP_ROT_R))
			{
				m->KaitenDir = 1;
			}

#if LOG_ENABLED
			if(IsHit(KEY_INPUT_1))
			{
				Pzdc.GameOver = 1;
				Pzdc.Winner = 0;
			}
			if(IsHit(KEY_INPUT_2))
			{
				Pzdc.GameOver = 1;
				Pzdc.Winner = 1;
			}
			if(IsHit(KEY_INPUT_3))
			{
				Pzdc.P1Muteki = !Pzdc.P1Muteki;
			}
			if(IsHit(KEY_INPUT_4))
			{
				Pzdc.P2Muteki = !Pzdc.P2Muteki;
			}
			if(IsHit(KEY_INPUT_5))
			{
				Pzdc.MapList[0].JamaCount = 0;
				Pzdc.MapList[1].JamaCount = 0;
			}
			if(IsHit(KEY_INPUT_6))
			{
				Pzdc.MapList[0].JamaCount += 3;
			}
			if(IsHit(KEY_INPUT_7))
			{
				Pzdc.MapList[1].JamaCount += 6;
			}
			if(IsHit(KEY_INPUT_8))
			{
				Pzdc.MapList[0].Score += 5000;
			}
			if(IsHit(KEY_INPUT_9))
			{
				Pzdc.MapList[1].Score += 5000;
			}

			int pjkn_s = (int)(Pzdc.FrameCnt / 60);
			int pjkn_m = pjkn_s / 60;

			pjkn_s %= 60;

//			clsDx();
			printfDx(
				"%d:%02d %02d,%02d/%03d,%03d AI:%d,%d,%d,%02x,%d Lv4:%d CT:%.2f G:%d\n"
				"AIT:%d,%s,%f+%d+%f,%f+%d+%f T:%d %f/%f\n"
				"%03d %d/%d,%d,%d[%d]%d[%d]%d[%d]%d,%d "
				,pjkn_m
				,pjkn_s
				,Pzdc.MapList[0].SecchiTimer
				,Pzdc.MapList[1].SecchiTimer
				,Pzdc.MapList[0].HoverTimer
				,Pzdc.MapList[1].HoverTimer
				,PzAI.i.PurDir
				,PzAI.i.PurX
				,PzAI.i.HiSpeedChuu
				,PzAI.i.MovRotElaps
				,PzAI.i.MovRotCount
				,PzAI.q.Lv4.DangerMode ? 1 : 0
				,PzAI.q.ClockTower.LastHyouka
				,Pzdc.MapList[0].HissUsedCount - Pzdc.MapList[1].HissUsedCount
				// 改行
				,PzAI_DBG_SpeedLv
				,PzAI_DBG_Think
				/*
				,PzAI_DBG_hr
				,PzAI_DBG_ht
				,PzAI_DBG_hs
				,PzAI_DBG_ur
				,PzAI_DBG_ut
				,PzAI_DBG_us
				*/
				,PzAI.HissPct[0]
				,PzAI.HissJouTakasa[0]
				,PzAI.HissJouSenyuu[0]
				,PzAI.HissPct[1]
				,PzAI.HissJouTakasa[1]
				,PzAI.HissJouSenyuu[1]
				,Sys_GetFrameTaskCount()
				,Pzdc.MapList[0].RakkaY
				,Pzdc.MapList[1].RakkaY
				// 改行
				,Pzdc.DustCount
				,Pzdc.P2SecchiCounter
				,Pzdc.P2RensaCounter[0]
				,Pzdc.P2RensaCounter[1]
				,Pzdc.P2RensaCounter[2]
				,Pzdc.P2RensaCounter[3]
				,Pzdc.P2RensaCounter[4]
				,Pzdc.P2RensaCounter[5]
				,Pzdc.P2RensaCounter[6]
				,Pzdc.P2RensaCounter[7]
				,Pzdc.P2RensaCounter[8]
				,Pzdc.P2RensaCounter[9]
				);
#endif

			if(GetHit(INP_START) || cStatus == 2)
			{
				doPause = 1;
			}

			if(GetPress(INP_DIR_2))
			{
				m->Kousoku = 1;
			}
			if(GetPress(INP_BOMB))
			{
				m->Hissatsu = 1;
			}
		}

		if(Pzdc.NetworkMode)
		{
			Map_t *m = Pzdc.MapList + 0;
			int command = 0;
			int osCommand;

			if(m->IdouDir == -1) command |= 1 << NT_CMDBIT_MOV_L;
			if(m->IdouDir ==  1) command |= 1 << NT_CMDBIT_MOV_R;

			if(m->KaitenDir == -1) command |= 1 << NT_CMDBIT_ROT_L;
			if(m->KaitenDir ==  1) command |= 1 << NT_CMDBIT_ROT_R;

			if(m->Kousoku)  command |= 1 << NT_CMDBIT_HISPEED;
			if(m->Hissatsu) command |= 1 << NT_CMDBIT_BOMB;

			if(!Pzdc.NetKansenMode) NtSetCommand(Pzdc.FrameCnt + NT_DELAY, command);

			for(int millis = 0; ; millis++)
			{
				if(Pzdc.NetKansenMode)
				{
					if(doPause || NtKansenSq.EndSyn) // 観戦を中断する。
					{
						Pzdc.GameOver = 0; // 2bs
						goto endGameLoop;
					}
					NtUDPRecvCountMax = 2; // スキップしないように、且つバッファにたまらないように、、
					NtRecvKansenSq();
					NtUDPRecvCountMax = 0;

					NtSequence[0] = NtKansenSq.Sq[0];
					NtSequence[1] = NtKansenSq.Sq[1];

					NtPutBackLogSq(0, NtSequence + 0);
					NtPutBackLogSq(1, NtSequence + 1);
				}
				else
				{
					/*
					if(doPause) // 対戦を放棄する。
					{
						goto endGameLoop;
					}
					*/
					NtSendSequence();
					NtRecvSequence();
				}
				command = NtGetCommand(Pzdc.FrameCnt);
				osCommand = NtGetRecvCommand(Pzdc.FrameCnt);

				if(Pzdc.NetKansenMode)
				{
					if(command   & (1 << NT_CMDBIT_ERROR)) command   = NtGetBackLogSq(0, Pzdc.FrameCnt);
					if(osCommand & (1 << NT_CMDBIT_ERROR)) osCommand = NtGetBackLogSq(1, Pzdc.FrameCnt);

					if((command   & (1 << NT_CMDBIT_ERROR)) == 0)
					if((osCommand & (1 << NT_CMDBIT_ERROR)) == 0)
					{
						break;
					}
				}
				else
				{
					if(NtKansenshaCount) // サーバー側、配信
					{
						NtKansenSq.Sq[0] = NtSequence[0];
						NtKansenSq.Sq[1] = NtSequence[1];
						NtHaishin();
					}
					errorCase(command & (1 << NT_CMDBIT_ERROR));

					if((osCommand & (1 << NT_CMDBIT_ERROR)) == 0)
					{
						break;
					}
				}
				if(500 < millis) // 長時間経過、ちょっと画面に出す。
				{
					if(7000 < millis && Pzdc.NetKansenMode) // 観戦 timeout
					{
						goto endGameLoop;
					}
					SimpleDraw(Pzdc.WallPaper, 0, 0, 0);
					SimpleDraw(Pzdc.FrontPaper, 0, 0, 1);
					SetAlpha(0.5);
					SimpleDraw(Dc->PicBlackWall, 0, 0, 0);
					ResetAlpha();

					clsDx();
					printfDx("\n");
					printfDx("　**** ビジー状態 ****\n");
					printfDx("\n");
					printfDx("　アプリケーションが同期に失敗したか、ネットワークに問題が発生しました。\n");
					printfDx("\n");

					if(Pzdc.NetKansenMode)
					{
						printfDx("　しばらくこのままの状態が続いたら、観戦を中止します ... %f / 7.0\n", (double)millis / 1000);
						printfDx("\n");
						printfDx("　すぐに観戦を中止する場合は「スペースキー」を押して下さい。\n");
					}
					else
					{
						printfDx("　この対戦を放棄する場合は「スペースキー」を押して下さい。\n");
					}

					// -- swap screen --
					ScreenFlip();
					if(ProcessMessage() == -1)
					{
						GameTermination();
					}
					// ----
					clsDx();

					if(CheckHitKey(KEY_INPUT_SPACE))
					{
						goto endGameLoop;
					}
					if(CheckHitKey(KEY_INPUT_ESCAPE))
					{
						GameTermination();
					}
				}

				Sleep(5);
				millis += 5;
			}

			Map_t *mv[2];
			int cmdv[2];

			if(NtCommonInfo[0].SelfSide == 0)
			{
				mv[0] = Pzdc.MapList + 0;
				mv[1] = Pzdc.MapList + 1;
			}
			else
			{
				mv[0] = Pzdc.MapList + 1;
				mv[1] = Pzdc.MapList + 0;
			}
			cmdv[0] = command;
			cmdv[1] = osCommand;

			for(int pSide = 0; pSide < 2; pSide++)
			{
				command = cmdv[pSide];
				m = mv[pSide];

				// init
				m->IdouDir = 0;
				m->KaitenDir = 0;
				m->Kousoku = 0;
				m->Hissatsu = 0;

				if(command & (1 << NT_CMDBIT_MOV_L)) m->IdouDir = -1;
				if(command & (1 << NT_CMDBIT_MOV_R)) m->IdouDir =  1;

				if(command & (1 << NT_CMDBIT_ROT_L)) m->KaitenDir = -1;
				if(command & (1 << NT_CMDBIT_ROT_R)) m->KaitenDir =  1;

				if(command & (1 << NT_CMDBIT_HISPEED)) m->Kousoku = 1;
				if(command & (1 << NT_CMDBIT_BOMB)) m->Hissatsu = 1;
			}

			goto nyuuryokuEnd;
		}

		// 敵、2P 入力
		{
			Map_t *m = Pzdc.MapList + 1;

			m->KaitenDir = 0;
			m->IdouDir = 0;
			m->Kousoku = 0;
			m->Hissatsu = 0;

			if(Pzdc.UseAI) // AI
			{
				if(m->TokushuHidden)
				{
					if(krnd() < 0.01)
					{
						if(krnd() < 0.2) m->KaitenDir = krnd() < 0.5 ? -1 : 1;
						else             m->IdouDir   = krnd() < 0.5 ? -1 : 1;
					}
				}
				else
				{
					PzAI_Think();

					m->KaitenDir = PzAI.RotDir;
					m->IdouDir   = PzAI.MovDir;
					m->Kousoku   = PzAI.HiSpeed ? 1 : 0;
					m->Hissatsu  = PzAI.Hissatsu ? 1 : 0;
				}
			}
			else // 人間
			{
				if(GetRendaHitPS(1, INP_DIR_4))
				{
					m->IdouDir = -1;
				}
				if(GetRendaHitPS(1, INP_DIR_6))
				{
					m->IdouDir = 1;
				}
				if(GetRendaHitPS(1, INP_ROT_L))
				{
					m->KaitenDir = -1;
				}
				if(GetRendaHitPS(1, INP_ROT_R))
				{
					m->KaitenDir = 1;
				}
				if(GetHitPS(1, INP_START))
				{
					doPause = 1;
				}
				if(GetPressPS(1, INP_DIR_2))
				{
					m->Kousoku = 1;
				}
				if(GetPressPS(1, INP_BOMB))
				{
					m->Hissatsu = 1;
				}
			}

			errorCase(m->KaitenDir < -1 || 1 < m->KaitenDir);
			errorCase(m->IdouDir   < -1 || 1 < m->IdouDir);
			errorCase(m->Kousoku   <  0 || 1 < m->Kousoku);
			errorCase(m->Hissatsu  <  0 || 1 < m->Hissatsu);
		}
		if(doPause) // ネットワーク対戦時、ポーズ禁止
		{
			Pzdc.PauseChuu = 1;
			PauseMain();
			Pzdc.PauseChuu = 0;

			if(Pzdc.PauseSelExitGame) break;
		}
nyuuryokuEnd:

		// 常に隠しモード
		{
			int ipics[] =
			{
				PIC_HW_Hide_J,
				PIC_HW_Hide_U,
				PIC_HW_Hide_S,
				PIC_HW_Hide_GJ,
				PIC_HW_Hide_EJ,
				PIC_HW_Hide_AE,
				PIC_HW_Hide_UP,
			};

			if(Pzdc.TokushuButtle.AlwaysHidden)
			{
				if(Pzdc.FrameCnt < Pzdc.TB_Hide_NextFrameCnt) goto no_tb_hidden;
				if(Pzdc.MapList[0].TokushuHidden) goto no_tb_hidden;
				if(Pzdc.MapList[1].TokushuHidden) goto no_tb_hidden;

				NetworkModeAdjustSRand();

				int tmout = rndbnd(400, 600);
				int ipic = ipics[rndbnd(0, lengthof(ipics) - 1)];

				Pzdc.TB_Hide_NextFrameCnt = Pzdc.FrameCnt + tmout;

				for(int c = 0; c < 2; c++)
				{
					Map_t *m = Pzdc.MapList + c;

//					m->TokushuHidden = 1; // AIが混乱したりと何かありそうなので、フラグは上げない
					m->TokushuTimeout = Pzdc.TB_Hide_NextFrameCnt;

					PzefAddHissHide(c, ipic, 1, tmout);
				}
			}
no_tb_hidden:;
		}

		ExecFrameMap(0);
		ExecFrameMap(1);

		ExecFramePuyo();

		for(int c = 0; c < Pzdc.TokushuButtle.VeryFast; c++) // 高速モード
		{
			ExecFrameMap(0);
			ExecFrameMap(1);

			ExecFramePuyo();
		}

		JamaSousai(); // 旧位置

		if( 8 * MAP_X_BOUND <= Pzdc.MapList[0].PuyoSenyuu ||
			8 * MAP_X_BOUND <= Pzdc.MapList[1].PuyoSenyuu ) PlayBGM(Pzdc.BGMPinch);
		if( Pzdc.MapList[0].PuyoSenyuu <= 6 * MAP_X_BOUND &&
			Pzdc.MapList[1].PuyoSenyuu <= 6 * MAP_X_BOUND ) PlayBGM(Pzdc.BGMNormal);

		// 描画 --->

draw_top:
		DrawGraph(0, 0, Pzdc.WallPaper, 0);
		DrawFramePuyo();
		DrawGraph(0, 0, Pzdc.FrontPaper, 1);

		// 右上の文字
		{
			char *ruline = "";

//			if(!Pzdc.UseAI) ruline = "VS";
			if(Pzdc.NetworkMode) ruline = "NET";
			if(Pzdc.NetKansenMode) ruline = "VIEW";

			const int CHAR_X = 797; // 右端のx
			const int CHAR_Y = 3;
			const int CHAR_X_STEP = 16;

			for(int i = 0; ruline[i]; i++)
			{
				SimpleDraw(Dc->PicAscii[ruline[i]], CHAR_X - (strlen(ruline) - i) * CHAR_X_STEP, CHAR_Y, 1);
			}
		}

		for(int midx = 0; midx < 2; midx++)
		{
			Map_t *m = Pzdc.MapList + midx;

			for(int fidx = 0; fidx < 2; fidx++)
			{
				if(fidx == 1 && Pzdc.IsSolo[midx]) break; // ソロなので二人目はなし。

				int chara;
				int pinch;
				int anime;
				int facePic;
				int faceX;
				int faceY;

				switch(m->PlayerChar)
				{
				case 0: // 1P
					chara = fidx ? PZPLYR_1P2 : PZPLYR_1P1;
					break;
				case 1: // 2P
					chara = fidx ? PZPLYR_2P2 : PZPLYR_2P1;
					break;

				default:
					error();
				}

				     if(m->PuyoSenyuu < 4 * MAP_X_BOUND) pinch = 0;
				else if(m->PuyoSenyuu < 8 * MAP_X_BOUND) pinch = 1;
				else                                     pinch = 2;

				int ac = m->AnimeCnt[fidx];

#define ANIME_TH 100
#define ANIME_TL 90

				if(!ac)
				{
					if(rnd() < 0.0025)
					{
						ac = rnd() < 0.85 ? ANIME_TL : ANIME_TH;
					}
				}
				else
					ac--;

				     if(ANIME_TH - 3 <= ac) anime = 1;
				else if(ANIME_TH - 6 <= ac) anime = 2;
				else if(ANIME_TL     <= ac) anime = 0;
				else if(ANIME_TL - 3 <= ac) anime = 1;
				else if(ANIME_TL - 6 <= ac) anime = 2;
				else                        anime = 0;

				m->AnimeCnt[fidx] = ac;

				facePic = Dc->PicFace[chara][pinch][anime];
				faceX = m->FaceX[fidx];
				faceY = m->FaceY[fidx];

				SimpleDraw(facePic, faceX, faceY, 0);
			}
			SimpleDraw(Pzdc.Go[m->GoStatus], m->GoX, m->GoY, 1);

			// jama num
			{
				char jnline[11];
				int jnx;
				int jny;
				int jnxs;

				jnx = midx ? 525 : 43;
				jny = 6;
				jnxs = 16;

				if( m->UIJamaSleep) {
					m->UIJamaSleep--;
				}
				else
				{
					if(60 < abs(m->UIJamaCount - m->JamaCount))
					{
						double uij = (double)m->UIJamaCount;
						double j   = (double)m->JamaCount;

						nearize(uij, j, 0.9);

						m->UIJamaCount = (int)uij;
					}
					if(Pzdc.FrameCnt & 1)
					{
							 if(m->UIJamaCount < m->JamaCount) m->UIJamaCount++;
						else if(m->UIJamaCount > m->JamaCount) m->UIJamaCount--;
					}
				}
				sprintf(jnline, "%d", m->UIJamaCount);

				for(int jnidx = 0; jnline[jnidx]; jnidx++)
				{
					int chr = jnline[jnidx];

					errorCase(chr < '0' || '9' < chr);

					SimpleDraw(Dc->PicAscii[chr], jnx, jny, 1);
					jnx += jnxs;
				}
			}
			// スコア
			{
				if(m->Score != m->UIScore && Pzdc.GameFreezeCount == 0) // フリーズ中はスコアも凍る。
				{
					int scoreDiff = abs(m->Score - m->UIScore);
					int scoreAdd;

					     if(scoreDiff < 30)    scoreAdd = 1;
					else if(scoreDiff < 300)   scoreAdd = 9;
					else if(scoreDiff < 3000)  scoreAdd = 109;
					else if(scoreDiff < 30000) scoreAdd = 909;
					else                       scoreAdd = scoreDiff / 3;

					if(m->Score < m->UIScore) m->UIScore -= scoreAdd;
					else                      m->UIScore += scoreAdd;
				}
				int score = m->UIScore;
				char lscore[12];
				int lscorelen;

				sprintf(lscore, "%d", score);
				lscorelen = strlen(lscore);

				int sdX;
				int sdY;
				int sdXSpn = 16;

				if(midx == 0)
				{
					sdX = 446;
					sdY = 521 + 5;
				}
				else // 1
				{
					sdX = 493;
					sdY = 555 + 5;
				}
				sdX -= lscorelen * sdXSpn;

				for(int sdi = 0; sdi < lscorelen; sdi++)
				{
					int sdn = lscore[sdi];

					m_range(sdn, '0', '9');

					SimpleDraw(Dc->PicAscii[sdn], sdX, sdY, 1);
					sdX += sdXSpn;
				}
			}
		}

		DrawStar(Pzdc.MapList[0].Kousoku);

		SwapFrame();
	}
endGameLoop:

	if(NtKansenshaCount) // 最後の配信。決着すると配信が止まるので、決着間際の Sq を観戦者が取り損ねると、観戦者がビジーになってしまうので、、、
	{
		NtKansenSq.Sq[0] = NtSequence[0]; // 2bs
		NtKansenSq.Sq[1] = NtSequence[1]; // 2bs

		for(int lpcnt = 0; lpcnt < 7; lpcnt++) // 凍りつくので、短く
		{
			NtHaishin();
			Sleep(3);
		}
	}

	////////////////////////
	// ゲームループ終わり //
	////////////////////////

	if(Pzdc.GameOver)
	{
		int frmidx;
		double frmpct;

		if(Pzdc.NetworkMode && !Pzdc.NetKansenMode) // 勝敗の確認
		{
			int resBit;
			int osResBit;
			int syncnt = 0;

			if(Pzdc.Winner == NtCommonInfo[0].SelfSide)
			{
				resBit   = NT_CMDBIT_WIN;
				osResBit = NT_CMDBIT_LOSE;
			}
			else
			{
				resBit   = NT_CMDBIT_LOSE;
				osResBit = NT_CMDBIT_WIN;
			}
			for(frmidx = 0; ; frmidx++)
			{
				NtSetCommand(Pzdc.FrameCnt + NT_DELAY, 1 << resBit);

				NtSendSequence();
				NtRecvSequence();

				int osCommand = NtGetRecvCommand(Pzdc.FrameCnt);

				if((osCommand & (1 << NT_CMDBIT_ERROR)) == 0)
				{
					if(NtGetRecvCommand(Pzdc.FrameCnt) == (1 << osResBit))
					{
						break;
					}
					Pzdc.FrameCnt++;
				}
				errorCase(1000 < frmidx); // anti endless loop

				Sleep(5);
			}

			// こちらの結果を確実に送りつける。
			for(int lpcnt = 0; lpcnt < 7; lpcnt++)
			{
				Pzdc.FrameCnt++;
				NtSetCommand(Pzdc.FrameCnt + NT_DELAY, 1 << resBit);
			}
			for(int lpcnt = 0; lpcnt < 7; lpcnt++) // 凍りつくので、短く
			{
				NtSendSequence();
				Sleep(3);
			}
		}
		FadeOut(Dc->BGMVolume, Dc->BGMVolume / 200.0);

		if(Pzdc.TokushuButtle.GameOverAndShowStealth)
		{
			frameloop(frmidx, frmpct, 90)
			{
				for(int mi = 0; mi < 2; mi++)
				for(int x = 0; x < MAP_X_BOUND; x++)
				for(int y = 0; y < MAP_Y_BOUND; y++)
				{
					Puyo_t *p = Pzdc.MapList[mi].PanelTable[x][y].Puyo;

					if(p)
					{
						p->A += 1.0 / 40.0;
						m_range(p->A, 0.0, 1.0);
					}
				}

				SimpleDraw(Pzdc.WallPaper, 0, 0, 0);
				DrawFramePuyo();
				SimpleDraw(Pzdc.FrontPaper, 0, 0, 1);

				SwapFrame();
			}
		}

		DoEraseAll(Pzdc.MapList);
		DoEraseAll(Pzdc.MapList + 1);

		int bellWin = 0;

		if(Pzdc.NetworkMode)
		{
			if(Pzdc.Winner == NtCommonInfo[0].SelfSide)
			{
				bellWin = 1;
			}
		}
		else if(Pzdc.UseAI)
		{
			if(Pzdc.Winner == 0)
			{
				bellWin = 1;
			}
		}
		else
		{
			bellWin = 1;
		}

		if(bellWin)
		{
			PlaySe(Dc->SeBellWin);
			PlaySe(Dc->SeWin);
		}
		else
		{
			PlaySe(Dc->SeBellLose);
			PlaySe(Dc->SeLose);
		}

		switch(Pzdc.Winner)
		{
		case 0: // 左サイド (1P)
			Pzdc.GmoCharPic[0] = Dc->PicWin1P;
			Pzdc.GmoCharPic[1] = Dc->PicLose2P;

			Pzdc.GmoMojiPic[0] = Dc->PicWin;
			Pzdc.GmoMojiPic[1] = Dc->PicLose;
			break;

		case 1: // 右サイド (2P or CPU)
			Pzdc.GmoCharPic[0] = Dc->PicLose1P;
			Pzdc.GmoCharPic[1] = Dc->PicWin2P;

			Pzdc.GmoMojiPic[0] = Dc->PicLose;
			Pzdc.GmoMojiPic[1] = Dc->PicWin;
			break;

		default:
			error();
		}

#define GMO_MAP_UNDER_Y 569
#define GMO_LOSE_CHAR_YSIZE 540
		for(int gci = 0; gci < 2; gci++)
		{
			if(gci == Pzdc.Winner) // Winner Side
			{
				Pzdc.GmoCharX[gci] = gci == 0 ? 0 : 402;
				Pzdc.GmoCharDestY[gci] = 0;

				Pzdc.GmoMojiTrueX[gci] = Pzdc.GmoCharX[gci]     + 227;
				Pzdc.GmoMojiTrueY[gci] = Pzdc.GmoCharDestY[gci] +  13;
			}
			else // Lose Side
			{
				Pzdc.GmoCharX[gci] = Pzdc.MapList[gci].DrawX;
				Pzdc.GmoCharDestY[gci] = GMO_MAP_UNDER_Y - GMO_LOSE_CHAR_YSIZE;

				Pzdc.GmoMojiTrueX[gci] = Pzdc.MapList[gci].DrawX;
				Pzdc.GmoMojiTrueY[gci] = Pzdc.MapList[gci].DrawY + PANEL_X_SIZE * 2;
			}
			Pzdc.GmoCharY[gci] = Pzdc.GmoCharDestY[gci] + rndbnd(400, 600);
		}

		// 旧位置
		/*
		Pzdc.GmoMojiTrueX[0] = 50;
		Pzdc.GmoMojiTrueY[0] = 75;
		Pzdc.GmoMojiTrueX[1] = 530;
		Pzdc.GmoMojiTrueY[1] = 75;
		*/

		Pzdc.GmoMojiX[0] = Get_Rnd(100) / 10.0 -  50.0;
		Pzdc.GmoMojiX[1] = Get_Rnd(100) / 10.0 + 850.0;
		Pzdc.GmoMojiY[0] = Get_Rnd(100) / 10.0 + 600.0;
		Pzdc.GmoMojiY[1] = Get_Rnd(100) / 10.0 + 600.0;

		// -- 文字の裏位置とか --
		int hwin_trg = Pzdc.Winner;
		errorCase(hwin_trg < 0 || 1 < hwin_trg);
		double hwin_x = Pzdc.GmoMojiX[hwin_trg];
		double hwin_y = Pzdc.GmoMojiY[hwin_trg];

		int tslose_trg = 1 - Pzdc.Winner;
		errorCase(tslose_trg < 0 || 1 < tslose_trg);
		double tslose_x = Pzdc.GmoMojiTrueX[tslose_trg];
		double tslose_y = Pzdc.GmoMojiTrueY[tslose_trg];
		double tslose_a = 0.0;
		// ----

		double ca = 0.0;

//		frameloop(frmidx, frmpct, 300)
		for(frmidx = 0; ; frmidx++)
		{
			if(90 < frmidx)
			{
				if(GetMusBtn()) break;
				if(GetPress(INP_ROT_DECIDE)) break;
				if(GetPress(INP_START)) break;
			}
			if(Pzdc.NetKansenMode)
			{
				NtRecvKansenSq();

				if(NtKansenSq.EndSyn)
				{
					break;
				}
			}

			DrawGraph(0, 0, Pzdc.WallPaper, 0);
			DrawGraph(0, 0, Pzdc.FrontPaper, 1);

#define GMO_CHAR_Y_K 0.9
#define GMO_CHAR_Y_MGN 0.05
			for(int gci = 0; gci < 2; gci++)
			{
				nearize_adj(Pzdc.GmoCharY[gci], Pzdc.GmoCharDestY[gci], GMO_CHAR_Y_K, GMO_CHAR_Y_MGN);
			}

			for(int gci = 0; gci < 2; gci++)
			{
				double g = 1.3;
				double k = 0.95;

				if(gci == Pzdc.Winner)
				{
					double kyo = m_pow(Pzdc.GmoMojiX[gci] - Pzdc.GmoMojiTrueX[gci]) + m_pow(Pzdc.GmoMojiY[gci] - Pzdc.GmoMojiTrueY[gci]);

					kyo = sqrt(kyo);
					kyo -= 5.0;
					kyo /= 100.0;
					m_range(kyo, 0.0, 1.0);

					g *= kyo;

					k = 1.0 - k;
//					k *= kyo;
					k = 1.0 - k;

					/*
					if(nearthan(
						Pzdc.GmoMojiX[gci],
						Pzdc.GmoMojiY[gci],
						Pzdc.GmoMojiTrueX[gci],
						Pzdc.GmoMojiTrueY[gci],
						30.0
						))
					{
						g = 0.001;
						k = 0.999;
					}
					*/

					if(krnd() < 0.1)
					{
						const int life = 20;

						CutIn_t *i = PzefAddEffect(
							Pzdc.GmoMojiX[gci] + rnd() * 168,
							Pzdc.GmoMojiY[gci] + rnd() * 316,
							0.0,
							0.0,
							0.3,
							rndpm() * 0.1,
							0.1,
							life,
							Dc->PicEffeHoshi
							);

						i->A = 0.5;
						i->Aa = -i->A / life;
					}
				}

				Pzdc.GmoMojiX[gci] += Pzdc.GmoMojiXAdd[gci];
				Pzdc.GmoMojiXAdd[gci] += (Pzdc.GmoMojiX[gci] < Pzdc.GmoMojiTrueX[gci] ? 1 : -1) * g;
				Pzdc.GmoMojiXAdd[gci] *= k;

				Pzdc.GmoMojiY[gci] += Pzdc.GmoMojiYAdd[gci];
				Pzdc.GmoMojiYAdd[gci] += (Pzdc.GmoMojiY[gci] < Pzdc.GmoMojiTrueY[gci] ? 1 : -1) * g;
				Pzdc.GmoMojiYAdd[gci] *= k;
			}

			ca += 0.015;
			m_range(ca, 0.0, 1.0);

			for(int gci = 0; gci < 2; gci++)
			{
				SetAlpha(ca);
				SimpleDraw(Pzdc.GmoCharPic[gci], (int)Pzdc.GmoCharX[gci], (int)Pzdc.GmoCharY[gci], 1);
				ResetAlpha();
			}

			// -- 文字の裏 --
			{
				SimpleDraw(Dc->PicWinWall, (int)hwin_x, (int)hwin_y, 1);

				SetAlpha(tslose_a);
				SimpleDraw(Dc->PicLoseWall, (int)tslose_x, (int)tslose_y, 1);
				ResetAlpha();

				nearize(hwin_x, Pzdc.GmoMojiX[hwin_trg], 0.5);
				nearize(hwin_y, Pzdc.GmoMojiY[hwin_trg], 0.5);

				tslose_a += 0.005;
				m_range(tslose_a, 0.0, 1.0);
			}

			for(int gci = 0; gci < 2; gci++)
			{
				SimpleDraw(Pzdc.GmoMojiPic[gci], (int)Pzdc.GmoMojiX[gci], (int)Pzdc.GmoMojiY[gci], 1);
			}

			DrawStar(0);
			SwapFrame();

#if LOG_ENABLED
			printfDx(
				"%I64d (%I64d) %d %d\n"
				"0:%05d,%f\n"
				"1:%05d,%f\n"
				"2:%05d,%f\n"
				"3:%05d,%f\n"
				"4:%05d,%f\n"
				"5:%05d,%f\n"
				"6:%05d,%f\n"
				"7:%05d,%f\n"
				"8:%05d,%f\n"
				"9:%05d,%f\n"
				,Pzdc.FrameCnt / 60
				,Pzdc.FrameCnt
				,Sys_GetFrameTaskCount()
				,Pzdc.P2SecchiCounter
				,Pzdc.P2RensaCounter[0], (double)Pzdc.P2RensaCounter[0] / Pzdc.P2SecchiCounter
				,Pzdc.P2RensaCounter[1], (double)Pzdc.P2RensaCounter[1] / Pzdc.P2SecchiCounter
				,Pzdc.P2RensaCounter[2], (double)Pzdc.P2RensaCounter[2] / Pzdc.P2SecchiCounter
				,Pzdc.P2RensaCounter[3], (double)Pzdc.P2RensaCounter[3] / Pzdc.P2SecchiCounter
				,Pzdc.P2RensaCounter[4], (double)Pzdc.P2RensaCounter[4] / Pzdc.P2SecchiCounter
				,Pzdc.P2RensaCounter[5], (double)Pzdc.P2RensaCounter[5] / Pzdc.P2SecchiCounter
				,Pzdc.P2RensaCounter[6], (double)Pzdc.P2RensaCounter[6] / Pzdc.P2SecchiCounter
				,Pzdc.P2RensaCounter[7], (double)Pzdc.P2RensaCounter[7] / Pzdc.P2SecchiCounter
				,Pzdc.P2RensaCounter[8], (double)Pzdc.P2RensaCounter[8] / Pzdc.P2SecchiCounter
				,Pzdc.P2RensaCounter[9], (double)Pzdc.P2RensaCounter[9] / Pzdc.P2SecchiCounter
				);
#endif
		}
		if(NtKansenshaCount) // 観戦者のゲームオーバー画面を終わらせる。
		{
			NtKansenSq.EndSyn = 1;

			for(int lpcnt = 0; lpcnt < 10; lpcnt++)
			{
				NtHaishin();
			}
		}
	}
	clsDx();
	PzEndScene();

	// フレームタスク＝カットインが残っているうちに Pzdc を開放すると落ちる。
	// フレームタスクは別モジュール、カットインは Pzdc なので、、、
	{
		Pzdc.GameIsEnded = 1;

		for(int f = 0 ; f < 900; f++) // １５秒あれば消えるだろう、、、
		{
			SimpleDraw(Dc->PicBlackWall, 0, 0, 0);
			SwapFrame();

			int ftc = Sys_GetFrameTaskCount();

#if LOG_ENABLED
			printfDx("%03d %d ", f, ftc);
#endif

			if(ftc == 0)
			{
				break;
			}
		}
	}
	StopBGM();

	UnloadBGM(Pzdc.BGMNormal);
	UnloadBGM(Pzdc.BGMPinch);

	PzdcRet.GameOver = Pzdc.GameOver;
	PzdcRet.Winner = Pzdc.Winner;

	// -- Cleanup --
	UnloadPzPictureAll();
	memset(&Pzdc, 0x00, sizeof(DataCenter_t));

	DispMusCursor(true);
	// ----
}

// 以下、Tools

int PzutlGetTakasa(Map_t *m, int x)
{
	errorCase(!m);
	errorCase(x < 0 || MAP_X_BOUND <= x);

	int y;

	for(y = 0; y < MAP_Y_BOUND; y++)
	{
		if(m->PanelTable[x][y].Puyo != NULL)
		{
			break;
		}
	}
	y = MAP_Y_BOUND - y;

	errorCase(y < 0 || MAP_Y_BOUND < y);
	return y;
}
int PzutlGetTakasaMin(Map_t *m)
{
	int min_h = BORDER_OF_NUMERIC;

	for(int x = 0; x < MAP_X_BOUND; x++)
	{
		int h = PzutlGetTakasa(m, x);
		min_h = m_min(min_h, h);
	}
	return min_h;
}
