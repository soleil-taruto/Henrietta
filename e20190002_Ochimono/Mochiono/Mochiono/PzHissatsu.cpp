#include "StdInc.h"

char *LHissatsuTypes[HWT_MAX] =
{
	"HWT_TEST_01",
	"HWT_TEST_02",
	"HWT_TEST_03",

	"HWT_G",
	"HWT_I",
	"HWT_J",
	"HWT_P",
	"HWT_E",
	"HWT_A",
	"HWT_U",
	"HWT_C",
	"HWT_F",
	"HWT_S",
	"HWT_R",

	"HWT_GI",
	"HWT_PJ",
	"HWT_GJ",
	"HWT_IJ",
	"HWT_PI",
	"HWT_GP",
	"HWT_EJ",
	"HWT_AE",
	"HWT_SR",
	"HWT_UP",
	"HWT_EC",
	"HWT_IR",
	"HWT_UA",
	"HWT_FS",
	"HWT_FE",
};

static void InitJSDest(int mapIndex, int jCnt)
{
	PzefInitJSDest(mapIndex, jCnt, Pzdc.MapList[0].JamaCount, Pzdc.MapList[1].JamaCount);
}
static void AddHissJamaBomb(int mapIndex, int x, int y, int color, int jamaSendMode)
{
	const int JAMA_STAR_NUM = 7;

	PzefGetCellCenter(mapIndex, x, y);
	double sx = PzefX;
	double sy = PzefY;

	PzefGetJSDest();
	double ex = PzefX;
	double ey = PzefY;

	PzefAddHissJamaStar(sx, sy, ex, ey, color, jamaSendMode ? JAMA_STAR_NUM : 0);
}
/*
	m->PanelTable[][].Puyo->Erase != 0 を消す
*/
static void DoEraseAndHissEffect(Map_t *m, int jamaSendMode)
{
	PzDEEffectOff = 1;
	PzEx_DoErase(m);
	PzDEEffectOff = 0;

	int mapIndex = m->SelfIndex;
	int jCnt = PzDEDeadCnt;

	InitJSDest(mapIndex, jCnt);

	for(int index = 0; index < PzDEDeadCnt; index++)
	{
		int x = PzDEDeadXList[index];
		int y = PzDEDeadYList[index];

		AddHissJamaBomb(m->SelfIndex, x, y, PzDEDeadColorList[index], jamaSendMode);
	}
	if(jamaSendMode)
	{
		Pzdc.MapList[1 - mapIndex].JamaCount += jCnt;
		Pzdc.MapList[1 - mapIndex].UIJamaSleep = 30;
	}
}
static void JamaPut(int mapIndex, int numJama)
{
	Pzdc.MapList[0].JamaCount = 0;
	Pzdc.MapList[1].JamaCount = 0;

	Pzdc.MapList[mapIndex].JamaCount = numJama;
}
static void JamaToColor(int mapIndex)
{
	Pzdc.MapList[mapIndex].Jama2ColorMode = 1;
}
static void SetTokushuRakka(int mapIndex, int rakka, int frmcnt) // rakka: -1 or 1, frmcnt: 0～
{
	errorCase(mapIndex < 0 || 1 < mapIndex);
	errorCase(rakka != -1 && rakka != 1);
	errorCase(frmcnt < 0);

	Map_t *m = Pzdc.MapList + mapIndex;

	m->TokushuRakka = rakka;
	m->TokushuTimeout = Pzdc.FrameCnt + frmcnt;
}
static void SetTokushuHidden(int mapIndex, int frmcnt)
{
	errorCase(mapIndex < 0 || 1 < mapIndex);
	errorCase(frmcnt < 0);

	Map_t *m = Pzdc.MapList + mapIndex;

	m->TokushuHidden = 1;
	m->TokushuTimeout = Pzdc.FrameCnt + frmcnt;
}

// タイプ別Erase --->

static int Table[MAP_X_BOUND][MAP_Y_BOUND]; // -1: 空, 0 - (PUYO_COLOR_NUM - 1): 有り
static int TableHi[MAP_X_BOUND];
static int TableHiestY; // 一番高いところに積んであるブロックの Y 座標 (0 ～ (MAP_Y_BOUND - 1)), 何も無いとき == (MAP_Y_BOUND - 1)

static void InitTable(int mapIndex)
{
	for(int x = 0; x < MAP_X_BOUND; x++)
	for(int y = 0; y < MAP_Y_BOUND; y++)
	{
		Puyo_t *p = Pzdc.MapList[mapIndex].PanelTable[x][y].Puyo;
		int color = -1;

		if(p)
			color = p->Color;

		Table[x][y] = color;
	}
	int hiest = 0;

	for(int x = 0; x < MAP_X_BOUND; x++)
	{
		int y;

		for(y = 0; y < MAP_Y_BOUND; y++)
		{
			if(Table[x][y] != -1)
				break;
		}
		TableHi[x] = MAP_Y_BOUND - y;
		errorCase(TableHi[x] < 0 || MAP_Y_BOUND < TableHi[x]); // 0 ～ MAP_Y_BOUND

		hiest = m_max(hiest, TableHi[x]);
	}
	TableHiestY = MAP_Y_BOUND - hiest;

	if( TableHiestY == MAP_Y_BOUND)
		TableHiestY--;

	errorCase(TableHiestY < 0 || MAP_Y_BOUND <= TableHiestY); // 0 ～ (MAP_Y_BOUND - 1)
}
static int IsErasedRow(int mapIndex, int y) // ret : ? 消された行 || 何もない行
{
	for(int x = 0; x < MAP_X_BOUND; x++)
	{
		Puyo_t *p = Pzdc.MapList[mapIndex].PanelTable[x][y].Puyo;

		if(p && p->Erase == 0)
		{
			return 0;
		}
	}
	return 1;
}
static int GetRndEraseRow(int mapIndex)
{
	int bY = rndbnd(TableHiestY, MAP_Y_BOUND - 1);

	for(int p = 0; p < MAP_Y_BOUND; p++)
	{
		int y = (bY + p) % MAP_Y_BOUND;

		if(!IsErasedRow(mapIndex, y))
		{
			return y;
		}
	}
	return bY;
}
static int IsExistColor(int mapIndex, int color)
{
	for(int x = 0; x < MAP_X_BOUND; x++)
	for(int y = 0; y < MAP_Y_BOUND; y++)
	{
		Puyo_t *p = Pzdc.MapList[mapIndex].PanelTable[x][y].Puyo;

		if(Table[x][y] == color && p && p->Erase == 0)
		{
			return 1;
		}
	}
	return 0;
}
static int GetRndEraseColor(int mapIndex)
{
	int bC = rndbnd(0, PUYO_COLOR_NUM - 1);

	for(int p = 0; p < PUYO_COLOR_NUM; p++)
	{
		int color = (bC + p) % PUYO_COLOR_NUM;

		if(IsExistColor(mapIndex, color))
		{
			return color;
		}
	}
	return bC;
}

static void EraseRow(int mapIndex, int y) // y : -1 == RND
{
	InitTable(mapIndex);

	if(y == -1)
		y = GetRndEraseRow(mapIndex);

	errorCase(y < 0 || MAP_Y_BOUND <= y);

	for(int x = 0; x < MAP_X_BOUND; x++)
	{
		Puyo_t *p = Pzdc.MapList[mapIndex].PanelTable[x][y].Puyo;

		if(p)
			p->Erase = 1;
	}
}
static void EraseColumn(int mapIndex, int x) // ランダム無し！
{
	InitTable(mapIndex);
	errorCase(x < 0 || MAP_X_BOUND <= x);

	for(int y = 0; y < MAP_Y_BOUND; y++)
	{
		Puyo_t *p = Pzdc.MapList[mapIndex].PanelTable[x][y].Puyo;

		if(p)
			p->Erase = 1;
	}
}
static void EraseColor(int mapIndex, int color, int halfmode) // color : -1 == RND
{
	InitTable(mapIndex);

	if(color == -1)
		color = GetRndEraseColor(mapIndex);

	int ecnt = 0;

	for(int x = 0; x < MAP_X_BOUND; x++)
	for(int y = 0; y < MAP_Y_BOUND; y++)
	{
		Puyo_t *p = Pzdc.MapList[mapIndex].PanelTable[x][y].Puyo;

		if(p && p->Color == color)
		{
			ecnt++;
			p->Erase = halfmode ? ecnt % 2 : 1;
		}
	}
}

// <---

/*
	視覚エフェクト:
		PzefAddHissatsu(mi);
		PzefAddHissHide(mi, PIC_HW_Hide_*, sec * 60);
		PzefAddHissKirara(mi);
*/
void PzHissatsu(int mapIndex, int hissatsuType)
{
	errorCase(mapIndex < 0 || 1 < mapIndex);
//	errorCase(hissatsuType < 0 || HWT_MAX <= hissatsuType);

	switch(hissatsuType)
	{
	case HWT_TEST_01:
	case HWT_TEST_02:
	case HWT_TEST_03:
		error();

	case HWT_G:
		EraseColumn(mapIndex, 2);
		EraseColumn(mapIndex, 3);
		DoEraseAndHissEffect(Pzdc.MapList + mapIndex, 0);

		PzefAddHissatsu(mapIndex);
		break;

	case HWT_I:
		SetTokushuRakka(1 - mapIndex, -1, 60 * 20);

		PzefAddHissHide(1 - mapIndex, (1 - mapIndex) ? PIC_HW_Slow_R : PIC_HW_Slow_L, 0, 60 * 20);
		break;

	case HWT_J:
		SetTokushuHidden(1 - mapIndex, 60 * 30);

		PzefAddHissHide(1 - mapIndex, PIC_HW_Hide_J, 1, 60 * 30);
		break;

	case HWT_P:
		EraseColor(mapIndex, -1, 0);
		EraseColor(mapIndex, -1, 0);
		DoEraseAndHissEffect(Pzdc.MapList + mapIndex, 0);

		PzefAddHissatsu(mapIndex);
		break;

	case HWT_E:
		SetTokushuRakka(1 - mapIndex, 1, 60 * 20);

		PzefAddHissHide(1 - mapIndex, (1 - mapIndex) ? PIC_HW_Fast_R : PIC_HW_Fast_L, 0, 60 * 20);
		break;

	case HWT_A:
		JamaPut(1 - mapIndex, 12);
		JamaToColor(1 - mapIndex);

		PzefAddHissKirara(1 - mapIndex);
		break;

	case HWT_U:
		SetTokushuHidden(1 - mapIndex, 60 * 30);

		PzefAddHissHide(1 - mapIndex, PIC_HW_Hide_U, 1, 60 * 30);
		break;

	case HWT_C:
		EraseColor(mapIndex, 1, 0);
		EraseColor(mapIndex, 4, 0);
		DoEraseAndHissEffect(Pzdc.MapList + mapIndex, 0);

		PzefAddHissatsu(mapIndex);
		break;

	case HWT_F:
		EraseColor(mapIndex, 0, 1);
		DoEraseAndHissEffect(Pzdc.MapList + mapIndex, 0);

		PzefAddHissatsu(mapIndex);
		break;

	case HWT_S:
		SetTokushuHidden(1 - mapIndex, 60 * 30);

		PzefAddHissHide(1 - mapIndex, PIC_HW_Hide_S, 1, 60 * 30);
		break;

	case HWT_R:
		EraseRow(mapIndex, 12);
		EraseRow(mapIndex, 13);
		DoEraseAndHissEffect(Pzdc.MapList + mapIndex, 0);

		PzefAddHissatsu(mapIndex);
		break;

	case HWT_GI:
		EraseColumn(mapIndex, 2);
		EraseColumn(mapIndex, 3);
		DoEraseAndHissEffect(Pzdc.MapList + mapIndex, 1);

		PzefAddHissatsu(mapIndex);
		break;

	case HWT_PJ:
		EraseColor(mapIndex, -1, 0);
		DoEraseAndHissEffect(Pzdc.MapList + mapIndex, 1);

		PzefAddHissatsu(mapIndex);
		break;

	case HWT_GJ:
		EraseColumn(mapIndex, 2);
		EraseColumn(mapIndex, 3);
		DoEraseAndHissEffect(Pzdc.MapList + mapIndex, 0);
		SetTokushuHidden(1 - mapIndex, 60 * 30);

		PzefAddHissatsu(mapIndex);
		PzefAddHissHide(1 - mapIndex, PIC_HW_Hide_GJ, 1, 60 * 30);
		break;
		
	case HWT_IJ:
		EraseRow(mapIndex, -1);
		EraseRow(mapIndex, -1);
		DoEraseAndHissEffect(Pzdc.MapList + mapIndex, 0);
		SetTokushuRakka(1 - mapIndex, -1, 60 * 20);

		PzefAddHissatsu(mapIndex);
		PzefAddHissHide(1 - mapIndex, (1 - mapIndex) ? PIC_HW_Slow_R : PIC_HW_Slow_L, 0, 60 * 20);
		break;

	case HWT_PI:
		EraseColor(mapIndex, -1, 0);
		DoEraseAndHissEffect(Pzdc.MapList + mapIndex, 0);
		SetTokushuRakka(1 - mapIndex, 1, 60 * 20);

		PzefAddHissatsu(mapIndex);
		PzefAddHissHide(1 - mapIndex, (1 - mapIndex) ? PIC_HW_Fast_R : PIC_HW_Fast_L, 0, 60 * 20);
		break;

	case HWT_GP:
		EraseRow(mapIndex, -1);
		EraseColor(mapIndex, -1, 0);
		DoEraseAndHissEffect(Pzdc.MapList + mapIndex, 1);

		PzefAddHissatsu(mapIndex);
		break;

	case HWT_EJ:
		SetTokushuHidden(1 - mapIndex, 60 * 30);
		SetTokushuRakka(1 - mapIndex, 1, 60 * 30);

		PzefAddHissHide(1 - mapIndex, PIC_HW_Hide_U, 1, 60 * 30);
		PzefAddHissHide(1 - mapIndex, (1 - mapIndex) ? PIC_HW_Fast_R : PIC_HW_Fast_L, 0, 60 * 30);
		break;

	case HWT_AE:
		JamaPut(1 - mapIndex, 12);
		JamaToColor(1 - mapIndex);
		SetTokushuHidden(1 - mapIndex, 60 * 30);

		PzefAddHissKirara(1 - mapIndex);
		PzefAddHissHide(1 - mapIndex, PIC_HW_Hide_AE, 1, 60 * 30);
		break;

	case HWT_SR:
		EraseRow(mapIndex, 12);
		EraseRow(mapIndex, 13);
		DoEraseAndHissEffect(Pzdc.MapList + mapIndex, 1);

		PzefAddHissatsu(mapIndex);
		break;

	case HWT_UP:
		EraseColor(mapIndex, -1, 0);
		EraseColor(mapIndex, -1, 0);
		DoEraseAndHissEffect(Pzdc.MapList + mapIndex, 0);
		SetTokushuHidden(1 - mapIndex, 60 * 30);

		PzefAddHissHide(1 - mapIndex, PIC_HW_Hide_UP, 1, 60 * 30);
		break;

	case HWT_EC:
		EraseColor(mapIndex, 1, 0);
		EraseColor(mapIndex, 4, 0);
		DoEraseAndHissEffect(Pzdc.MapList + mapIndex, 0);
		SetTokushuRakka(1 - mapIndex, 1, 60 * 20);

		PzefAddHissatsu(mapIndex);
		PzefAddHissHide(1 - mapIndex, (1 - mapIndex) ? PIC_HW_Fast_R : PIC_HW_Fast_L, 0, 60 * 20);
		break;

	case HWT_IR:
		EraseRow(mapIndex, 12);
		EraseRow(mapIndex, 13);
		DoEraseAndHissEffect(Pzdc.MapList + mapIndex, 0);
		SetTokushuRakka(1 - mapIndex, -1, 60 * 20);

		PzefAddHissatsu(mapIndex);
		PzefAddHissHide(1 - mapIndex, (1 - mapIndex) ? PIC_HW_Slow_R : PIC_HW_Slow_L, 0, 60 * 20);
		break;

	case HWT_UA:
		EraseColor(mapIndex, -1, 0);
		DoEraseAndHissEffect(Pzdc.MapList + mapIndex, 1);
		JamaToColor(1 - mapIndex);

		PzefAddHissatsu(mapIndex);
		break;

	case HWT_FS:
		EraseColor(mapIndex, 0, 1);
		DoEraseAndHissEffect(Pzdc.MapList + mapIndex, 1);

		PzefAddHissatsu(mapIndex);
		break;

	case HWT_FE:
		EraseColor(mapIndex, 0, 1);
		DoEraseAndHissEffect(Pzdc.MapList + mapIndex, 0);
		SetTokushuRakka(1 - mapIndex, 1, 60 * 20);

		PzefAddHissatsu(mapIndex);
		PzefAddHissHide(1 - mapIndex, (1 - mapIndex) ? PIC_HW_Slow_R : PIC_HW_Slow_L, 0, 60 * 20);
		break;

	default:
		error();
	}
	PlaySe(Dc->SeBellHiss);
}
