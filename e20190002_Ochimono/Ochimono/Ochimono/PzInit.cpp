#include "StdInc.h"

void PzInit(void)
{
	PzdcRet.PauseSelExitApp = 0;
	Pzdc.PuyoEraseMin = 4;

	// -- Params (Default) --
	Pzdc.Chara[0][0] = CHARA_I;
	Pzdc.Chara[0][1] = CHARA_NONE;
	Pzdc.Chara[1][0] = CHARA_I;
	Pzdc.Chara[1][1] = CHARA_NONE;

	Pzdc.Basho = BASHO_SUNFLOWER;

	Pzdc.HissatsuType[0] = HWT_I;
	Pzdc.HissatsuType[1] = HWT_I;

	Pzdc.UseAI = 1;

	Pzdc.BGMFileNormal = RESRC_BGM_DESERT;
	Pzdc.BGMFilePinch  = RESRC_BGM_PINCH01;
	// ----

	PzAI_Init();
}
void PzFnlz(void)
{
	PzAI_Fnlz();
}
