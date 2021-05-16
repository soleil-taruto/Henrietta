#include "StdInc.h"

static void DrawSimpleWall(double a)
{
	SimpleDraw(Dc->PicBlackWall, 0, 0, 0);

	SetAlpha(a);
	SimpleDraw(Pzdc.WallPaper, 0, 0, 1);
	SimpleDraw(Pzdc.FrontPaper, 0, 0, 1);
	ResetAlpha();
}

static void SS_DrawReady(void)
{
	DppInit(
		Dc->PicReady,
		400 + sin(Pzdc.SS_Rot) * Pzdc.SS_Zoom,
		300 + cos(Pzdc.SS_Rot) * Pzdc.SS_Zoom,
		1
		);
	DppRotate(Pzdc.SS_Rot);
	DppDraw();

	Pzdc.SS_Rot *= 0.85;
	nearize(Pzdc.SS_Zoom, 1.0, 0.85);
}
static void SS_DrawReady2(void)
{
	DppInit(
		Dc->PicReady,
		400,
		300 - Pzdc.SS_Takasa,
		1
		);
	DppDraw();

	Pzdc.SS_Takasa -= Pzdc.SS_Rakka;
	Pzdc.SS_Rakka += 3.5;

	if(Pzdc.SS_Takasa < 0.0)
	{
		Pzdc.SS_Takasa = 0.0;
		Pzdc.SS_Rakka *= 0.4; // K
		Pzdc.SS_Rakka = abs(Pzdc.SS_Rakka) * -1.0;
		Pzdc.SS_Hane++;

		if(5 <= Pzdc.SS_Hane)
		{
			Pzdc.SS_Rakka = 0.0;
			Pzdc.SS_Takasa = 0.0;
		}
	}
}
static int SS_DrawReadyFar(void *dummy)
{
	if(Pzdc.SS_Alpha <= 0.0)
		return 0;

	SetAlpha(Pzdc.SS_Alpha);
	DppInit(Dc->PicReady, 400, 300, 1);
	DppRotate(Pzdc.SS_Rot);
	DppZoom(Pzdc.SS_Zoom);
	DppDraw();
	ResetAlpha();

	Pzdc.SS_Rot += 0.9;
	Pzdc.SS_Zoom += 0.15;
	Pzdc.SS_Alpha -= 0.02;

	return 1;
}

void PzStartScene(void)
{
	int frmidx;
	double frmpct;

	Pzdc.SS_Rot = 100.0;
	Pzdc.SS_Zoom = 800.0;
	Pzdc.SS_Alpha = 1.0;
	Pzdc.SS_Takasa = 600.0;
	Pzdc.SS_Rakka = 0.0;
	Pzdc.SS_Hane = 0;

	void (*func_dr)(void) = Get_Rnd(1) ? SS_DrawReady : SS_DrawReady2;

	StopBGM();

	PlaySe(Dc->SeReady);
//	PlaySe(Dc->SeJReady); // îpé~

	frameloop(frmidx, frmpct, 30)
	{
		DrawSimpleWall(frmpct);
		func_dr();
		SwapFrame();
	}
	frameloop(frmidx, frmpct, 60)
	{
		DrawSimpleWall(1.0);
		func_dr();
		SwapFrame();
	}

	// SS_* Faróp
	Pzdc.SS_Rot = 0.0;
	Pzdc.SS_Zoom = 1.0;

	PlaySe(Dc->SeStart);
//	PlaySe(Dc->SeGo); // îpé~

	AddFrameTask(SS_DrawReadyFar, NULL);
}
void PzEndScene(void)
{
	int frmidx;
	double frmpct;

	frameloop(frmidx, frmpct, 55)
	{
		DrawSimpleWall(1.0 - frmpct);
		SwapFrame();
	}
	StopBGM();
}
