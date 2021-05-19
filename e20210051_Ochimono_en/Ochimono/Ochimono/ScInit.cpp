#include "StdInc.h"

void ScProcInit(void)
{
	Scdc = (ScDataCenter_t *)memAlloc(sizeof(ScDataCenter_t));
}
void ScProcFnlz(void)
{
	// memFree(Scdc);
}

void ScInit(void)
{
	memset(Scdc, 0x00, sizeof(ScDataCenter_t));

	Scdc->PicSuperWall = -1;
	Scdc->PicWall = Dc->PicBlackWall;
//	Scdc->PicWall = -1;

	Scdc->MessA = 0.0;
	Scdc->MessForDisp = 1;

	Scdc->CharaList[0].PrevPic = -1;
	Scdc->CharaList[1].PrevPic = -1;
	Scdc->CharaList[2].PrevPic = -1;
	Scdc->CharaList[3].PrevPic = -1;

	Scdc->CharaList[0].Pic = -1;
	Scdc->CharaList[1].Pic = -1;
	Scdc->CharaList[2].Pic = -1;
	Scdc->CharaList[3].Pic = -1;

	Scdc->CharaList[0].SleepCnt = 20;
	Scdc->CharaList[1].SleepCnt = 20;
	Scdc->CharaList[2].SleepCnt = 20;
	Scdc->CharaList[3].SleepCnt = 20;

	int hx = 442 / 2;

	Scdc->CharaList[0].DrawX = 130 - hx;
	Scdc->CharaList[1].DrawX = 310 - hx;
	Scdc->CharaList[2].DrawX = 490 - hx;
	Scdc->CharaList[3].DrawX = 670 - hx;

	Scdc->CharaList[0].DrawY = 0;
	Scdc->CharaList[1].DrawY = 0;
	Scdc->CharaList[2].DrawY = 0;
	Scdc->CharaList[3].DrawY = 0;

	// -- Button --
	Scdc->IPicButtonList[0] = PIC_SCSys_Save;
	Scdc->IPicButtonList[1] = PIC_SCSys_Load;
	Scdc->IPicButtonList[2] = PIC_SCSys_Auto;
	Scdc->IPicButtonList[3] = PIC_SCSys_Skip;
	Scdc->IPicButtonList[4] = PIC_SCSys_Menu;

	Scdc->IPicFocusButtonList[0] = PIC_SCSys_SaveFocus;
	Scdc->IPicFocusButtonList[1] = PIC_SCSys_LoadFocus;
	Scdc->IPicFocusButtonList[2] = PIC_SCSys_AutoFocus;
	Scdc->IPicFocusButtonList[3] = PIC_SCSys_SkipFocus;
	Scdc->IPicFocusButtonList[4] = PIC_SCSys_MenuFocus;

	Scdc->ButtonPosList[0][0] = 420;
	Scdc->ButtonPosList[1][0] = 484;
	Scdc->ButtonPosList[2][0] = 547;
	Scdc->ButtonPosList[3][0] = 610;
	Scdc->ButtonPosList[4][0] = 673;

	Scdc->ButtonPosList[0][1] = 396;
	Scdc->ButtonPosList[1][1] = 396;
	Scdc->ButtonPosList[2][1] = 396;
	Scdc->ButtonPosList[3][1] = 396;
	Scdc->ButtonPosList[4][1] = 396;

	Scdc->ButtonXSize = 57;
	Scdc->ButtonYSize = 31;
	// ----
}
void ScFnlz(void)
{
	// noop
}
