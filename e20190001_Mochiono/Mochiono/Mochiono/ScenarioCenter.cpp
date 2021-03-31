#include "StdInc.h"

char *Scc_EventList[SCC_EVENT_MAX];

void Scc_ProcInit(void)
{
	int fp = DFOpen(ResourcePath("ƒVƒiƒŠƒI\\Scenario"));

	for(int index = 0; index < SCC_EVENT_MAX; index++)
	{
		Scc_EventList[index] = strx(DFReadLine(fp));
	}
	DFClose(fp);
}
void Scc_ProcFnlz(void)
{
	// noop
}

int Scc_GetEventIndex(char *eventLine)
{
	for(int index = 0; index < SCC_EVENT_MAX; index++)
	{
		if(!strcmp(eventLine, Scc_EventList[index]))
		{
			return index;
		}
	}
	printfDx("%s\n", eventLine);

	error();
	return -1; // dummy
}
char *Scc_GetEvent(int index)
{
	errorCase(index < 0 || SCC_EVENT_MAX <= index);
	return Scc_EventList[index];
}

void Scc_SetPuzzleParams(char *puzzTitle)
{
	// init ret
	Pzdc.Chara[0][0] = CHARA_NONE;
	Pzdc.Chara[0][1] = CHARA_NONE;
	Pzdc.Chara[1][0] = CHARA_NONE;
	Pzdc.Chara[1][1] = CHARA_NONE;

	// 1. GJ
	if(!strcmp(puzzTitle, "P_JU"))
	{
		Pzdc.Chara[0][0] = CHARA_J;
		Pzdc.Chara[1][0] = CHARA_U;

		Pzdc.HissatsuType[0] = HWT_J;
		Pzdc.HissatsuType[1] = HWT_U;

		Pzdc.Basho = BASHO_FOREST;
		Pzdc.BGMFileNormal = RESRC_BGM_CAFE;

		PzAI_SetStdParams(1, PzThink_Lv1, 0.2, 0, 0.5, 0.3, 0, 0.0);
		goto endbuttle;
	}
	if(!strcmp(puzzTitle, "P_GC"))
	{
		Pzdc.Chara[0][0] = CHARA_G;
		Pzdc.Chara[1][0] = CHARA_C;

		Pzdc.HissatsuType[0] = HWT_G;
		Pzdc.HissatsuType[1] = HWT_C;

		Pzdc.Basho = BASHO_JINJA;
		Pzdc.BGMFileNormal = RESRC_BGM_SHIKI;

		PzAI_SetStdParams(2, PzThink_Lv1, 0.2, 0, 0.5, 0.3, 0, 0.0);
		goto endbuttle;
	}
	if(!strcmp(puzzTitle, "P_GJEC"))
	{
		Pzdc.Chara[0][0] = CHARA_G;
		Pzdc.Chara[0][1] = CHARA_J;
		Pzdc.Chara[1][0] = CHARA_E;
		Pzdc.Chara[1][1] = CHARA_C;

		Pzdc.HissatsuType[0] = HWT_GJ;
		Pzdc.HissatsuType[1] = HWT_EC;

		Pzdc.Basho = BASHO_JINJA;
		Pzdc.BGMFileNormal = RESRC_BGM_MITSU;

		PzAI_SetStdParams(2, PzThink_Lv2, 0.3, 0, 0.5, 0.5, 0, 0.0);
		goto endbuttle;
	}

	// 2. IP
	if(!strcmp(puzzTitle, "P_PF"))
	{
		Pzdc.Chara[0][0] = CHARA_P;
		Pzdc.Chara[1][0] = CHARA_F;

		Pzdc.HissatsuType[0] = HWT_P;
		Pzdc.HissatsuType[1] = HWT_F;

		Pzdc.Basho = BASHO_FLOWER;
		Pzdc.BGMFileNormal = RESRC_BGM_CAFE;

		PzAI_SetStdParams(1, PzThink_Lv1, 0.2, 0, 0.5, 0.3, 0, 0.0);
		goto endbuttle;
	}
	if(!strcmp(puzzTitle, "P_IR"))
	{
		Pzdc.Chara[0][0] = CHARA_I;
		Pzdc.Chara[1][0] = CHARA_R;

		Pzdc.HissatsuType[0] = HWT_I;
		Pzdc.HissatsuType[1] = HWT_R;

		Pzdc.Basho = BASHO_LAKE;
		Pzdc.BGMFileNormal = RESRC_BGM_POLP;

		PzAI_SetStdParams(2, PzThink_Lv1, 0.2, 0, 0.5, 0.3, 0, 0.0);
		goto endbuttle;
	}
	if(!strcmp(puzzTitle, "P_PISR"))
	{
		Pzdc.Chara[0][0] = CHARA_P;
		Pzdc.Chara[0][1] = CHARA_I;
		Pzdc.Chara[1][0] = CHARA_S;
		Pzdc.Chara[1][1] = CHARA_R;

		Pzdc.HissatsuType[0] = HWT_PI;
		Pzdc.HissatsuType[1] = HWT_SR;

		Pzdc.Basho = BASHO_LAKE;
		Pzdc.BGMFileNormal = RESRC_BGM_POLP;

		PzAI_SetStdParams(2, PzThink_Lv2, 0.3, 0, 0.5, 0.5, 0, 0.0);
		goto endbuttle;
	}

	// 3. GP
	if(!strcmp(puzzTitle, "P_PA"))
	{
		Pzdc.Chara[0][0] = CHARA_P;
		Pzdc.Chara[1][0] = CHARA_A;

		Pzdc.HissatsuType[0] = HWT_P;
		Pzdc.HissatsuType[1] = HWT_A;

		Pzdc.Basho = BASHO_FOREST;
		Pzdc.BGMFileNormal = RESRC_BGM_TEIEN;

		PzAI_SetStdParams(2, PzThink_Lv2, 0.3, 0, 0.6, 0.5, 0, 0.0);
		goto endbuttle;
	}
	if(!strcmp(puzzTitle, "P_GF"))
	{
		Pzdc.Chara[0][0] = CHARA_G;
		Pzdc.Chara[1][0] = CHARA_F;

		Pzdc.HissatsuType[0] = HWT_G;
		Pzdc.HissatsuType[1] = HWT_F;

		Pzdc.Basho = BASHO_DESERT;
		Pzdc.BGMFileNormal = RESRC_BGM_TEIEN;

		PzAI_SetStdParams(3, PzThink_Lv2, 0.3, 0, 0.6, 0.5, 0, 0.0);
		goto endbuttle;
	}
	if(!strcmp(puzzTitle, "P_PU"))
	{
		Pzdc.Chara[0][0] = CHARA_P;
		Pzdc.Chara[1][0] = CHARA_U;

		Pzdc.HissatsuType[0] = HWT_P;
		Pzdc.HissatsuType[1] = HWT_U;

		Pzdc.Basho = BASHO_SUNFLOWER;
		Pzdc.BGMFileNormal = RESRC_BGM_SHIKI;

		PzAI_SetStdParams(3, PzThink_Lv3, 0.4, 0, 0.6, 0.5, 0, 0.0);
		goto endbuttle;
	}
	if(!strcmp(puzzTitle, "P_GS"))
	{
		Pzdc.Chara[0][0] = CHARA_G;
		Pzdc.Chara[1][0] = CHARA_S;

		Pzdc.HissatsuType[0] = HWT_G;
		Pzdc.HissatsuType[1] = HWT_S;

		Pzdc.Basho = BASHO_LAKE;
		Pzdc.BGMFileNormal = RESRC_BGM_3DAYS;

		PzAI_SetStdParams(4, PzThink_Lv3, 0.4, 0, 0.6, 0.5, 0, 0.0);
		goto endbuttle;
	}
	if(!strcmp(puzzTitle, "P_GPFS"))
	{
		Pzdc.Chara[0][0] = CHARA_G;
		Pzdc.Chara[0][1] = CHARA_P;
		Pzdc.Chara[1][0] = CHARA_F;
		Pzdc.Chara[1][1] = CHARA_S;

		Pzdc.HissatsuType[0] = HWT_GP;
		Pzdc.HissatsuType[1] = HWT_FS;

		Pzdc.Basho = BASHO_LAKE;
		Pzdc.BGMFileNormal = RESRC_BGM_3DAYS;

		PzAI_SetStdParams(4, PzThink_Lv4, 0.5, 0, 0.6, 0.8, 0, 0.0);
		goto endbuttle;
	}

	// 4. IP
	if(!strcmp(puzzTitle, "P_JR"))
	{
		Pzdc.Chara[0][0] = CHARA_J;
		Pzdc.Chara[1][0] = CHARA_R;

		Pzdc.HissatsuType[0] = HWT_J;
		Pzdc.HissatsuType[1] = HWT_R;

		Pzdc.Basho = BASHO_LAKE;
		Pzdc.BGMFileNormal = RESRC_BGM_3DAYS;

		PzAI_SetStdParams(2, PzThink_Lv2, 0.3, 0, 0.6, 0.5, 0, 0.0);
		goto endbuttle;
	}
	if(!strcmp(puzzTitle, "P_IC"))
	{
		Pzdc.Chara[0][0] = CHARA_I;
		Pzdc.Chara[1][0] = CHARA_C;

		Pzdc.HissatsuType[0] = HWT_I;
		Pzdc.HissatsuType[1] = HWT_C;

		Pzdc.Basho = BASHO_JINJA;
		Pzdc.BGMFileNormal = RESRC_BGM_SHIKI;

		PzAI_SetStdParams(3, PzThink_Lv2, 0.3, 0, 0.6, 0.5, 0, 0.0);
		goto endbuttle;
	}
	if(!strcmp(puzzTitle, "P_IE"))
	{
		Pzdc.Chara[0][0] = CHARA_I;
		Pzdc.Chara[1][0] = CHARA_E;

		Pzdc.HissatsuType[0] = HWT_I;
		Pzdc.HissatsuType[1] = HWT_E;

		Pzdc.Basho = BASHO_SEA;
		Pzdc.BGMFileNormal = RESRC_BGM_3DAYS;

		PzAI_SetStdParams(3, PzThink_Lv3, 0.4, 0, 0.6, 0.5, 0, 0.0);
		goto endbuttle;
	}
	if(!strcmp(puzzTitle, "P_JA"))
	{
		Pzdc.Chara[0][0] = CHARA_J;
		Pzdc.Chara[1][0] = CHARA_A;

		Pzdc.HissatsuType[0] = HWT_J;
		Pzdc.HissatsuType[1] = HWT_A;

		Pzdc.Basho = BASHO_NIGHT;
		Pzdc.BGMFileNormal = RESRC_BGM_MITSU;

		PzAI_SetStdParams(4, PzThink_Lv3, 0.4, 0, 0.6, 0.5, 0, 0.0);
		goto endbuttle;
	}
	if(!strcmp(puzzTitle, "P_IJAE"))
	{
		Pzdc.Chara[0][0] = CHARA_I;
		Pzdc.Chara[0][1] = CHARA_J;
		Pzdc.Chara[1][0] = CHARA_A;
		Pzdc.Chara[1][1] = CHARA_E;

		Pzdc.HissatsuType[0] = HWT_IJ;
		Pzdc.HissatsuType[1] = HWT_AE;

		Pzdc.Basho = BASHO_NIGHT;
		Pzdc.BGMFileNormal = RESRC_BGM_EVE;

		PzAI_SetStdParams(4, PzThink_Lv4, 0.5, 0, 0.6, 0.8, 0, 0.0);
		goto endbuttle;
	}

	// 5. GI
	if(!strcmp(puzzTitle, "P_GA"))
	{
		Pzdc.Chara[0][0] = CHARA_G;
		Pzdc.Chara[1][0] = CHARA_A;

		Pzdc.HissatsuType[0] = HWT_G;
		Pzdc.HissatsuType[1] = HWT_A;

		Pzdc.Basho = BASHO_JINJA;
		Pzdc.BGMFileNormal = RESRC_BGM_POLP;

		PzAI_SetStdParams(2, PzThink_Lv3, 0.4, 0, 0.6, 0.5, 0, 0.0);
		goto endbuttle;
	}
	if(!strcmp(puzzTitle, "P_IS"))
	{
		Pzdc.Chara[0][0] = CHARA_I;
		Pzdc.Chara[1][0] = CHARA_S;

		Pzdc.HissatsuType[0] = HWT_I;
		Pzdc.HissatsuType[1] = HWT_S;

		Pzdc.Basho = BASHO_SEA;
		Pzdc.BGMFileNormal = RESRC_BGM_CAFE;

		PzAI_SetStdParams(3, PzThink_Lv4, 0.5, 0, 0.6, 0.6, 0, 0.0);
		goto endbuttle;
	}
	if(!strcmp(puzzTitle, "P_GIFE"))
	{
		Pzdc.Chara[0][0] = CHARA_G;
		Pzdc.Chara[0][1] = CHARA_I;
		Pzdc.Chara[1][0] = CHARA_F;
		Pzdc.Chara[1][1] = CHARA_E;

		Pzdc.HissatsuType[0] = HWT_GI;
		Pzdc.HissatsuType[1] = HWT_FE;

		Pzdc.Basho = BASHO_FLOWER;
		Pzdc.BGMFileNormal = RESRC_BGM_STORY;
		Pzdc.BGMFilePinch  = RESRC_BGM_D;

		PzAI_SetStdParams(4, PzThink_Lv4, 0.5, 0, 0.6, 0.8, 0, 0.0);
		goto endbuttle;
	}
	if(!strcmp(puzzTitle, "P_GIUP"))
	{
		Pzdc.Chara[0][0] = CHARA_G;
		Pzdc.Chara[0][1] = CHARA_I;
		Pzdc.Chara[1][0] = CHARA_U;
		Pzdc.Chara[1][1] = CHARA_P;

		Pzdc.HissatsuType[0] = HWT_GI;
		Pzdc.HissatsuType[1] = HWT_UP;

		Pzdc.Basho = BASHO_FOREST;
		Pzdc.BGMFileNormal = RESRC_BGM_DANCE;
		Pzdc.BGMFilePinch  = RESRC_BGM_D;

		PzAI_SetStdParams(5, PzThink_Lv4, 0.6, 0, 0.6, 1.0, 0, 0.0);
		goto endbuttle;
	}
	if(!strcmp(puzzTitle, "P_GIEJ"))
	{
		Pzdc.Chara[0][0] = CHARA_G;
		Pzdc.Chara[0][1] = CHARA_I;
		Pzdc.Chara[1][0] = CHARA_E;
		Pzdc.Chara[1][1] = CHARA_J;

		Pzdc.HissatsuType[0] = HWT_GI;
		Pzdc.HissatsuType[1] = HWT_EJ;

		Pzdc.Basho = BASHO_NIGHT;
		Pzdc.BGMFileNormal = RESRC_BGM_STORY;
		Pzdc.BGMFilePinch  = RESRC_BGM_D;

		PzAI_SetStdParams(5, PzThink_ClockTower, 0.6, 0, 0.6, 1.0, 0, 0.0);
		goto endbuttle;
	}

	// 6. GI
	if(!strcmp(puzzTitle, "P_PE"))
	{
		Pzdc.Chara[0][0] = CHARA_P;
		Pzdc.Chara[1][0] = CHARA_E;

		Pzdc.HissatsuType[0] = HWT_P;
		Pzdc.HissatsuType[1] = HWT_E;

		Pzdc.Basho = BASHO_NIGHT;
		Pzdc.BGMFileNormal = RESRC_BGM_EVE;

		PzAI_SetStdParams(2, PzThink_Lv3, 0.4, 0, 0.6, 0.5, 0, 0.0);
		goto endbuttle;
	}
	if(!strcmp(puzzTitle, "P_JC"))
	{
		Pzdc.Chara[0][0] = CHARA_J;
		Pzdc.Chara[1][0] = CHARA_C;

		Pzdc.HissatsuType[0] = HWT_J;
		Pzdc.HissatsuType[1] = HWT_C;

		Pzdc.Basho = BASHO_LAKE;
		Pzdc.BGMFileNormal = RESRC_BGM_SHIKI;

		PzAI_SetStdParams(3, PzThink_Lv4, 0.5, 0, 0.6, 0.6, 0, 0.0);
		goto endbuttle;
	}
	if(!strcmp(puzzTitle, "P_PJIR"))
	{
		Pzdc.Chara[0][0] = CHARA_P;
		Pzdc.Chara[0][1] = CHARA_J;
		Pzdc.Chara[1][0] = CHARA_I;
		Pzdc.Chara[1][1] = CHARA_R;

		Pzdc.HissatsuType[0] = HWT_PJ;
		Pzdc.HissatsuType[1] = HWT_IR;

		Pzdc.Basho = BASHO_DESERT;
		Pzdc.BGMFileNormal = RESRC_BGM_CAFE;

		PzAI_SetStdParams(4, PzThink_Lv4, 0.5, 0, 0.6, 0.8, 0, 0.0);
		goto endbuttle;
	}
	if(!strcmp(puzzTitle, "P_PJUA"))
	{
		Pzdc.Chara[0][0] = CHARA_P;
		Pzdc.Chara[0][1] = CHARA_J;
		Pzdc.Chara[1][0] = CHARA_U;
		Pzdc.Chara[1][1] = CHARA_A;

		Pzdc.HissatsuType[0] = HWT_PJ;
		Pzdc.HissatsuType[1] = HWT_UA;

		Pzdc.Basho = BASHO_SUNFLOWER;
		Pzdc.BGMFileNormal = RESRC_BGM_DANCE;
		Pzdc.BGMFilePinch  = RESRC_BGM_D;

		PzAI_SetStdParams(5, PzThink_Lv4, 0.6, 0, 0.6, 1.0, 0, 0.0);
		goto endbuttle;
	}
	if(!strcmp(puzzTitle, "P_PJAE"))
	{
		Pzdc.Chara[0][0] = CHARA_P;
		Pzdc.Chara[0][1] = CHARA_J;
		Pzdc.Chara[1][0] = CHARA_A;
		Pzdc.Chara[1][1] = CHARA_E;

		Pzdc.HissatsuType[0] = HWT_PJ;
		Pzdc.HissatsuType[1] = HWT_AE;

		Pzdc.Basho = BASHO_JINJA;
		Pzdc.BGMFileNormal = RESRC_BGM_SUI;
		Pzdc.BGMFilePinch  = RESRC_BGM_D;

		PzAI_SetStdParams(5, PzThink_ClockTower, 0.6, 0, 0.6, 1.0, 0, 0.0);
		goto endbuttle;
	}
	error();

endbuttle:;
}
