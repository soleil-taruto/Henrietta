#include "all.h"

GDc_t GDc;

void GameInit(void)
{
	zeroclear(&GDc);

	GDc.Map = new autoTable<MCell_t *>(CreateMCell, ReleaseMCell);
	GDc.EnemyList = new autoList<Enemy_t *>();
	GDc.EnemyList_Add = new autoList<Enemy_t *>();
	GDc.EnemyList_Saved = new autoList<Enemy_t *>();

	GDc.Meteor.EL = new taskList();
	GDc.Meteor.WallScreen = CreateSubScreen(SCREEN_W, SCREEN_H);
	GDc.Meteor.ExWallScreen = CreateSubScreen(SCREEN_W, SCREEN_H);
	GDc.Meteor.FieldScreen = CreateSubScreen(SCREEN_W, SCREEN_H, 1);

	// デフォルト(念のための初期化、使われないはず！) {

	GDc.KabeDrawFunc = KabeDraw_Default;
	GDc.KabePlayerBr = 1.0;
	GDc.KabeEnemyBr = 1.0;
	GDc.KabeArkanoidBr = 1.0;
	GDc.KabePataBr = 1.0;
	GDc.KabeCookieBr = 1.0;
	GDc.KabeBr_A = 1.0;
	GDc.KabeBr_R = 1.0;
	GDc.KabeBr_G = 1.0;
	GDc.KabeBr_B = 1.0;

	// }

	PostInitKabe();
}
void GameFnlz(void)
{
	memFree(GDc.MapBmpFile);

	if(!GDc.MapBmpImageUnreleaseFlag)
		delete GDc.MapBmpImage;

	delete GDc.MapBmp;
	delete GDc.Rnd;
//	delete GDc.PlayRnd;
	delete GDc.Map;
	releaseList(GDc.EnemyList, ReleaseEnemy);
	releaseList(GDc.EnemyList_Add, ReleaseEnemy);
	releaseList(GDc.EnemyList_Saved, ReleaseEnemy);

	delete GDc.Meteor.EL;
	ReleaseSubScreen(GDc.Meteor.WallScreen);
	ReleaseSubScreen(GDc.Meteor.ExWallScreen);
	ReleaseSubScreen(GDc.Meteor.FieldScreen);

	zeroclear(&GDc);

	UnloadAllPicResHandle(); // 背景とかめちゃくちゃデカいので！
}

// ----

void PostInitKabe(void)
{
	GDc.BackKabeDrawFunc = NULL;
	GDc.FrontKabeDrawFunc = GDc.KabeDrawFunc;
	GDc.FrontKabe_A = 1.0;

	for(int c = 0; c < KABE_BR_MAX; c++)
	{
		GDc.KabeBrList_A[c] = GDc.KabeBr_A;
		GDc.KabeBrList_R[c] = GDc.KabeBr_R;
		GDc.KabeBrList_G[c] = GDc.KabeBr_G;
		GDc.KabeBrList_B[c] = GDc.KabeBr_B;
	}
}
char *GetGraphicalModeCaption(int grphMd)
{
	static char *grphMdTxts[] =
	{
		"標準",
		"LEGACY",
		"STONE",
		"WOOD",
		"WATER",
		"FOREST",
		"FOREST EX",
		"METEOR",
		"METEOR EX",
		"EARTHQUAKER",
		"GHOST",
	};

	errorCase(lengthof(grphMdTxts) != GM_MAX); // 2bs
	errorCase(grphMd < 0 || GM_MAX <= grphMd);

	return grphMdTxts[grphMd];
}

double PlayerBr = 1.0;
double EnemyBr = 1.0;
double ArkanoidBr = 1.0;
double PataBr = 1.0;
double CookieBr = 1.0; // クッキーと大回転
