#include "StdInc.h"

char *LCharaList[CHARA_MAX] =
{
	"--",
	"à…",
	"âp",
	"êº",
	"íÜ",
	"ì∆",
	"ìÏ",
	"ì˙",
	"ïÅ",
	"ïß",
	"ïƒ",
	"òI",
};
char *LBashoList[BASHO_MAX] =
{
	"Ç–Ç‹ÇÌÇËî®",
	"â‘î®",
	"äC",
	"åŒ",
	"çªîô",
	"êX",
	"ê_é–",
	"ñÈ",
};
int CorrectPairs[CORRECT_PAIR_MAX][2] =
{
	{ CHARA_I, CHARA_NONE },
	{ CHARA_E, CHARA_NONE },
	{ CHARA_S, CHARA_NONE },
	{ CHARA_C, CHARA_NONE },
	{ CHARA_G, CHARA_NONE },
	{ CHARA_R, CHARA_NONE },
	{ CHARA_J, CHARA_NONE },
	{ CHARA_P, CHARA_NONE },
	{ CHARA_F, CHARA_NONE },
	{ CHARA_A, CHARA_NONE },
	{ CHARA_U, CHARA_NONE },
	{ CHARA_I, CHARA_R },
	{ CHARA_I, CHARA_J },
	{ CHARA_E, CHARA_C },
	{ CHARA_E, CHARA_J },
	{ CHARA_S, CHARA_R },
	{ CHARA_G, CHARA_I },
	{ CHARA_G, CHARA_J },
	{ CHARA_G, CHARA_P },
	{ CHARA_P, CHARA_I },
	{ CHARA_P, CHARA_J },
	{ CHARA_F, CHARA_E },
	{ CHARA_F, CHARA_S },
	{ CHARA_A, CHARA_E },
	{ CHARA_U, CHARA_P },
	{ CHARA_U, CHARA_A },
};
