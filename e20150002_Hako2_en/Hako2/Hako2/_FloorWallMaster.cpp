#include "all.h"

void FloorWallMasterMain(void)
{
#if LOG_ENABLED
	printfDx("カメラ位置：%d,%d\n", m_d2i(CameraX), m_d2i(CameraY));
	printfDx("プレイヤー：%d,%d\n", m_d2i(GDc.Player.X), m_d2i(GDc.Player.Y));
	printfDx("PlayerBr,EnemyBr,ArkanoidBr,PataBr,CookieBr: %.3f,%.3f,%.3f,%.3f,%.3f\n", PlayerBr, EnemyBr, ArkanoidBr, PataBr, CookieBr);
#endif

	// デフォルト {

	GDc.KabeDrawFunc = KabeDraw_Default;
	GDc.KabePlayerBr = 1.0;
	GDc.KabeEnemyBr = 1.0;
	GDc.KabeArkanoidBr = 1.0;
	GDc.KabePataBr = 1.0;
	GDc.KabeCookieBr = 1.0;
	GDc.KabeBr_A = 1.0;
	GDc.KabeBr_R = 1.0;
	GDc.KabeBr_G = 0.7;
	GDc.KabeBr_B = 0.1;

	// }

	if(GDc.FloorIndex == 0)
	{
		SetKabe_01();
	}
	else if(GDc.FloorIndex == 1)
	{
		SetKabe_02();
	}
	else if(GDc.FloorIndex == 2)
	{
		SetKabe_03();
	}
	// ここへ追加..
	else
	{
		error();
	}
}
