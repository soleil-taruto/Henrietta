#include "StdInc.h"

// -- Params --
int SLSaveMode;
void (*SLSaveFunc)(int index);
// ----

// -- Returns --
int SLDoLoad;
int SLLoadIndex;
// ----

#define ITEM_X_SIZE 375
#define ITEM_Y_SIZE 30

// [][x, y]
static int ItemPosList[][2] =
{
	{  17, 132 },
	{  17, 170 },
	{  17, 209 },
	{  17, 246 },
	{  17, 284 },
	{  17, 323 },
	{  17, 361 },
	{  17, 399 },
	{  17, 438 },
	{ 406, 132 },
	{ 406, 170 },
	{ 406, 209 },
	{ 406, 246 },
	{ 406, 284 },
	{ 406, 323 },
	{ 406, 361 },
	{ 406, 399 },
	{ 406, 438 },
};
static int IPicWall;

#define COLOR_SAVE_FOCUS 0x00cc4411
#define COLOR_SAVE_STD   0x00332211

#define COLOR_LOAD_FOCUS 0x001111cc
#define COLOR_LOAD_STD   0x00111133
#define COLOR_LOAD_EMPTY 0x00000011

static int Confirm(int saveMode) // ret : する。
{
//	SwapFrame(); // キー入力の更新のため

	int retval = 0;

	SetAlpha(0.5);
	SimpleDraw(Dc->PicBlackWall, 0, 0, 0);
	ResetAlpha();

	SimpleDraw(GC_Pic(saveMode ? PIC_SL_SaveSuruka : PIC_SL_LoadSuruka), 202, 192, 1);

#define CONF_YES_X 249
#define CONF_YES_Y 318
#define CONF_NO_X 443
#define CONF_NO_Y 318
#define CONF_BTN_X_SIZE 130
#define CONF_BTN_Y_SIZE 30

	for(; ; )
	{
		int currSelect = -1; // -1: none, 0: はい, 1: いいえ

		int mx;
		int my;
		GetMusPos(&mx, &my);

		int cStatus;
		cStatus = GetClick();

		if(isinside(mx, my, CONF_YES_X, CONF_YES_Y, CONF_YES_X + CONF_BTN_X_SIZE, CONF_YES_Y + CONF_BTN_Y_SIZE))
		{
			currSelect = 0;
		}
		if(isinside(mx, my, CONF_NO_X, CONF_NO_Y, CONF_NO_X + CONF_BTN_X_SIZE, CONF_NO_Y + CONF_BTN_Y_SIZE))
		{
			currSelect = 1;
		}

		SimpleDraw(GC_Pic(currSelect == 0 ? PIC_SL_SuruFocus   : PIC_SL_Suru),   CONF_YES_X, CONF_YES_Y, 0);
		SimpleDraw(GC_Pic(currSelect == 1 ? PIC_SL_ShinaiFocus : PIC_SL_Shinai), CONF_NO_X,  CONF_NO_Y,  0);

		SwapFrame();

		if(GetHit(INP_DIR_2) || GetHit(INP_DIR_4) || GetHit(INP_DIR_6) || GetHit(INP_DIR_8))
		{
			if(currSelect)
			{
				mx = CONF_YES_X + CONF_BTN_X_SIZE;
				my = CONF_YES_Y + CONF_BTN_Y_SIZE;
			}
			else
			{
				mx = CONF_NO_X + CONF_BTN_X_SIZE;
				my = CONF_NO_Y + CONF_BTN_Y_SIZE;
			}
			SetMusPos(mx, my);
		}
		if(GetHit(INP_ROT_DECIDE) || cStatus == 1)
		{
			retval = currSelect == 0;
			break;
		}
		if(GetHit(INP_ROT_CANCEL) || cStatus == 2)
		{
			break;
		}
	}
	SwapFrame(); // キー入力のクリアのため
	return retval;
}
void SaveLoad(void)
{
	IPicWall = SLSaveMode ? PIC_SL_SaveWall : PIC_SL_LoadWall;

	int modoruX = 568;
	int modoruY = 509;
	int modoruXSize = 1;
	int modoruYSize = 1;

	// Font
	LDrawRGB = SLSaveMode ? COLOR_SAVE_STD : COLOR_LOAD_STD;
	SetFontSize(20);
	SetFontThickness(3);
//	ChangeFontType(DX_FONTTYPE_NORMAL);
	ChangeFontType(DX_FONTTYPE_ANTIALIASING);

	// init ret
	SLDoLoad = 0;
	SLLoadIndex = -1;

	GC_LoadRange(PIC_SL_Start, PIC_SL_End);
	GetGraphSize(GC_Pic(PIC_SL_Modoru), &modoruXSize, &modoruYSize);

	Dc->BCPct = 1.0;
	Dc->BlackCurtain = 0.0;
	WaitToCurtain(GC_Pic(IPicWall));

	int currIndex = -1; // -1: no-selected
	int modoruFocus = 0;

	for(; ; )
	{
		int cStatus = GetClick();

		if(GetHit(INP_ROT_DECIDE) || cStatus == 1)
		{
			if(modoruFocus)
				break;

			if(currIndex != -1)
			{
				if(SLSaveMode)
				{
					if(Confirm(1))
					{
						SLSaveFunc(currIndex);
					}
				}
				else // Load
				{
					if(strcmp(SettInfo.SaveDataList[currIndex].Scenario, SAVEDATA_EMPTY_SCENARIO)) // 空ではない
					{
						if(Confirm(0))
						{
							SLDoLoad = 1;
							SLLoadIndex = currIndex;
							break;
						}
					}
				}
			}
		}
		if(GetHit(INP_ROT_CANCEL) || cStatus == 2)
		{
			break;
		}

		int moved = 0;

		if(GetRendaHit(INP_DIR_2))
		{
			currIndex++;
			moved = 1;
		}
		if(GetRendaHit(INP_DIR_8))
		{
			currIndex--;
			moved = 1;
		}
		if(GetRendaHit(INP_DIR_4) || GetRendaHit(INP_DIR_6))
		{
			currIndex = (currIndex + 9) % SAVEDATA_MAX;
			moved = 1;
		}
		if(moved)
		{
			m_range(currIndex, 0, SAVEDATA_MAX);

			if(currIndex == SAVEDATA_MAX)
			{
				currIndex = -1;
				SetMusPos(modoruX + modoruXSize - 1, modoruY + modoruYSize - 1);
			}
			else
			{
				SetMusPos(
					ItemPosList[currIndex][0] + ITEM_X_SIZE - 1,
					ItemPosList[currIndex][1] + ITEM_Y_SIZE - 1
					);
			}
		}

		int mx;
		int my;
		GetMusPos(&mx, &my);

		currIndex = -1;

		for(int index = 0; index < SAVEDATA_MAX; index++)
		{
			int ii = isinside(
				mx,
				my,
				ItemPosList[index][0],
				ItemPosList[index][1],
				ItemPosList[index][0] + ITEM_X_SIZE,
				ItemPosList[index][1] + ITEM_Y_SIZE
				);

			if(ii)
			{
				currIndex = index;
			}
		}
		modoruFocus = 0;
		if(isinside(mx, my, modoruX, modoruY, modoruX + modoruXSize, modoruY + modoruYSize))
		{
			modoruFocus = 1;
		}
		SimpleDraw(GC_Pic(IPicWall), 0, 0, 0);

		for(int index = 0; index < SAVEDATA_MAX; index++)
		{
			int cc = -1;

			if(SLSaveMode)
			{
				cc = COLOR_SAVE_STD;
				if(index == currIndex) cc = COLOR_SAVE_FOCUS;
			}
			else
			{
				cc = COLOR_LOAD_STD;
				if(!strcmp(SettInfo.SaveDataList[currIndex].Scenario, SAVEDATA_EMPTY_SCENARIO)) cc = COLOR_LOAD_EMPTY;
				if(index == currIndex) cc = COLOR_LOAD_FOCUS;
			}
			LDrawRGB = cc;
			LDraw(SettInfo.SaveDataList[index].UIText, ItemPosList[index][0], ItemPosList[index][1]);
		}
		// 戻るボタン
		{
			int ipic = modoruFocus ? PIC_SL_Modoru : PIC_SL_ModoruFocus;
			int x = modoruX;
			int y = modoruY;

			if(modoruFocus)
			{
				x++;
				y++;
			}
			SimpleDraw(GC_Pic(ipic), x, y, 1);
		}
		SwapFrame();
	}
	Dc->BlackCurtain = 1.0;
	WaitToCurtain(GC_Pic(IPicWall));
	Dc->BlackCurtain = 0.0;
	Dc->BCPct = 0.0;

	GC_UnloadRange(PIC_SL_Start, PIC_SL_End);
}
