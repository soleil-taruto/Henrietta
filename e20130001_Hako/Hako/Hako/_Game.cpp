#include "all.h"

GDc_t GDc;

void GameInit(void)
{
	zeroclear(&GDc);

	GDc.Map = new autoTable<MCell_t *>(CreateMCell, ReleaseMCell);
	GDc.EnemyList = new autoList<Enemy_t *>();

	GDc.Meteor.EL = new taskList();
	GDc.Meteor.WallScreen = CreateSubScreen(SCREEN_W, SCREEN_H);
	GDc.Meteor.ExWallScreen = CreateSubScreen(SCREEN_W, SCREEN_H);
	GDc.Meteor.FieldScreen = CreateSubScreen(SCREEN_W, SCREEN_H, 1);

	GDc.DWS_KnownValues = new autoList<int>();
}
void GameFnlz(void)
{
	memFree(GDc.MapBmpFile);

	if(!GDc.MapBmpImageUnreleaseFlag)
		delete GDc.MapBmpImage;

	delete GDc.MapBmp;
	delete GDc.Rnd;
	delete GDc.Map;
	releaseList(GDc.EnemyList, ReleaseEnemy);

	delete GDc.Meteor.EL;
	ReleaseSubScreen(GDc.Meteor.WallScreen);
	ReleaseSubScreen(GDc.Meteor.ExWallScreen);
	ReleaseSubScreen(GDc.Meteor.FieldScreen);

	delete GDc.DWS_KnownValues;

	zeroclear(&GDc);
}

// ----

char *GetGraphicalModeCaption(int grphMd)
{
	static char *grphMdTxts[] =
	{
		"ïWèÄ",
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
