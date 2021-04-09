#include "StdInc.h"

#define TITLE_X 0
#define TITLE_Y 16

#define ITEM_X_SIZE 108
#define ITEM_Y_SIZE 328
#define ITEM_MAX 6

enum
{
	IPL_X1,
	IPL_Y1,
	IPL_X2,
	IPL_Y2,
	IPL_IPIC,
	IPL_IPIC_FOCUS,
	IPL_HIDE,

	IPL_MAX, // == len
};

static int ItemPosList[ITEM_MAX][IPL_MAX];
static double ItemA = 0.0;

static void InitItemPosList(void)
{
	ItemPosList[0][IPL_X1] =  40;
	ItemPosList[1][IPL_X1] = 165;
	ItemPosList[2][IPL_X1] = 286;
	ItemPosList[3][IPL_X1] = 406;
	ItemPosList[4][IPL_X1] = 527;
	ItemPosList[5][IPL_X1] = 650;

	ItemPosList[0][IPL_Y1] = 63;
	ItemPosList[1][IPL_Y1] = 63;
	ItemPosList[2][IPL_Y1] = 63;
	ItemPosList[3][IPL_Y1] = 63;
	ItemPosList[4][IPL_Y1] = 63;
	ItemPosList[5][IPL_Y1] = 63;

	for(int index = 0; index < ITEM_MAX; index++)
	{
		ItemPosList[index][IPL_X2] = ItemPosList[index][IPL_X1] + ITEM_X_SIZE;
		ItemPosList[index][IPL_Y2] = ItemPosList[index][IPL_Y1] + ITEM_Y_SIZE;

		int ipic = -1;
		int ipicf = -1;
		int ihide = 0;
		switch(index)
		{
		case 0:
			ipic  = PIC_RS_GJ;
			ipicf = PIC_RS_GJFocus;
			ihide = SettInfo.RouteOpenList[ROL_A] == 0;
			break;
		case 1:
			ipic  = PIC_RS_IP;
			ipicf = PIC_RS_IPFocus;
			ihide = SettInfo.RouteOpenList[ROL_A] == 0;
			break;
		case 2:
			ipic  = PIC_RS_GP;
			ipicf = PIC_RS_GPFocus;
			ihide = SettInfo.RouteOpenList[ROL_B] == 0;
			break;
		case 3:
			ipic  = PIC_RS_IJ;
			ipicf = PIC_RS_IJFocus;
			ihide = SettInfo.RouteOpenList[ROL_B] == 0;
			break;
		case 4:
			ipic  = PIC_RS_GI;
			ipicf = PIC_RS_GIFocus;
			ihide = SettInfo.RouteOpenList[ROL_C] == 0;
			break;
		case 5:
			ipic  = PIC_RS_PJ;
			ipicf = PIC_RS_PJFocus;
			ihide = SettInfo.RouteOpenList[ROL_C] == 0;
			break;

		default:
			error();
		}
		if(ihide)
		{
			ipic  = PIC_RS_Hide;
			ipicf = PIC_RS_HideFocus;
		}
		ItemPosList[index][IPL_IPIC]       = ipic;
		ItemPosList[index][IPL_IPIC_FOCUS] = ipicf;
		ItemPosList[index][IPL_HIDE]       = ihide;
	}
}
void ScRotSenProcInit(void)
{
	InitItemPosList(); // 不要？
}
void ScRotSenProcFnlz(void)
{
	// noop
}
int ScRouteSentakuMode;
int ScCurrItem; // -1: no-select

void ScInitByScenarioStart(void)
{
	ScRouteSentakuMode = 0;
	ScCurrItem = -1;
	ItemA = 0.0;

	GC_LoadRange(PIC_RS_Start, PIC_RS_End);

	InitItemPosList(); // Hide が変化するので、毎回初期化すること。
}
void ScRotSenDrawFrame(void)
{
	if(ScRouteSentakuMode) ItemA += 0.03;
	else                   ItemA -= 0.03;
	m_range(ItemA, 0.0, 1.0);

	SetAlpha(ItemA);
	SimpleDraw(GC_Pic(PIC_RS_Title), TITLE_X, TITLE_Y, 1);

	for(int index = 0; index < ITEM_MAX; index++)
	{
		SimpleDraw(
			GC_Pic(ItemPosList[index][index == ScCurrItem ? IPL_IPIC_FOCUS : IPL_IPIC]),
			ItemPosList[index][IPL_X1],
			ItemPosList[index][IPL_Y1],
			1
			);
	}
	ResetAlpha();
}
int ScRotSenExecFrame(int in_decide, int in_dir4, int in_dir6) // ret : 決定した。
{
	int mx;
	int my;
	GetMusPos(&mx, &my);

	// init
	ScProcDc.RouteSelected = SC_ROUTE_NONE;
	ScCurrItem = -1;

	for(int index = 0; index < ITEM_MAX; index++)
	{
		if(isinside(mx, my,
			ItemPosList[index][IPL_X1],
			ItemPosList[index][IPL_Y1],
			ItemPosList[index][IPL_X2],
			ItemPosList[index][IPL_Y2]
			))
		{
			ScCurrItem = index;
		}
	}

	int moved = 0;
	if(in_dir4)
	{
		ScCurrItem--;
		moved = 1;
	}
	if(in_dir6)
	{
		ScCurrItem++;
		moved = 1;
	}
	if(moved)
	{
		m_range(ScCurrItem, 0, ITEM_MAX - 1);
		SetMusPos(
			ItemPosList[ScCurrItem][IPL_X2] - 1,
			ItemPosList[ScCurrItem][IPL_Y2] - 1
			);
	}

	if(in_decide)
	{
		if(ScCurrItem != -1 && ItemPosList[ScCurrItem][IPL_HIDE] == 0) // ? 選択されている。&& HIDE ではない。
		{
			ScProcDc.RouteSelected = ScCurrItem + 1;
			return 1;
		}
	}
	return 0;
}
