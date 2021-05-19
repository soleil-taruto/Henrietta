#include "StdInc.h"

int PzefActEffect(void *vi)
{
	CutIn_t *i = (CutIn_t *)vi;

	CIEx_ActCutIn(i);

	i->FrameCount--;
	if(i->FrameCount < 0)
	{
		i->Alive = 0;
	}
	if(Pzdc.GameIsEnded)
	{
		i->A -= 1.0 / (i->PrvRnd * 150.0 + 30.0);
		i->Alive = 0.0 < i->A;
	}
	return i->Alive;
}
CutIn_t *PzefAddEffectLayer(double x, double y, double xa, double ya, double g, double rSpeed, double z, int lifeFrmCnt, int pic, int layer)
{
	CutIn_t *i = CIEx_TakeCutIn(0, 0);

	i->X = x;
	i->Y = y;
	i->Xa = xa;
	i->Ya = ya;
	i->Xaa = 0.0;
	i->Yaa = g;

	i->R = 0.0;
	i->Ra = rSpeed;
	i->Raa = 0.0;

	i->Z = z;
	i->Za = 0.0;
	i->Zaa = 0.0;

	i->FrameCount = lifeFrmCnt;
	i->Pic = pic;

	AddFrameTaskLayer(PzefActEffect, i, layer);
	return i;
}
CutIn_t *PzefAddEffect(double x, double y, double xa, double ya, double g, double rSpeed, double z, int lifeFrmCnt, int pic)
{
	return PzefAddEffectLayer(x, y, xa, ya, g, rSpeed, z, lifeFrmCnt, pic, 0);
}
void PzefAddStarPon(double x, double y, int num)
{
#define STAR_LIFE 150

	for(int cnt = 0; cnt < num; cnt++)
	{
		CutIn_t *i = PzefAddEffect(
			x,
			y,
			rndpm() * 5.0,
			-rnd() * 4.5,
			rnd() * 0.7,
			rnd() * 1.0 + 2.0,
			rnd() * 0.1 + 0.075,
			STAR_LIFE,
			Dc->PicEffeKagaya
			);

		i->Aa = -1.0 / rndbnd(1, STAR_LIFE);
		i->KasanMode = 1;
	}
}

double PzefX;
double PzefY;

void PzefGetCellPos(int mapIndex, int x, int y)
{
	errorCase(mapIndex < 0 || 1 < mapIndex);

	PzefX = Pzdc.MapList[mapIndex].DrawX + (double)x * PANEL_X_SIZE;
	PzefY = Pzdc.MapList[mapIndex].DrawY + (double)y * PANEL_Y_SIZE;
}
void PzefGetCellCenter(int mapIndex, int x, int y)
{
	PzefGetCellPos(mapIndex, x, y);

	PzefX += PANEL_X_SIZE / 2;
	PzefY += PANEL_Y_SIZE / 2;
}

int PzefJSDest; // -1 : Both, 0-1 : L or R

void PzefInitJSDest(int mapIndex, int jCnt, int jCntL, int jCntR)
{
	errorCase(mapIndex < 0 || 1 < mapIndex);
	errorCase(jCnt < 0);
	errorCase(jCntL < 0);
	errorCase(jCntR < 0);

	int jCnt1 = mapIndex == 0 ? jCntL : jCntR;
	int jCnt2 = mapIndex == 0 ? jCntR : jCntL;
	int dest;

	if(jCnt1)
	{
		if(jCnt1 < jCnt)
		{
			dest = -1;
		}
		else
		{
			dest = mapIndex;
		}
	}
	else
	{
		dest = 1 - mapIndex;
	}
	PzefJSDest = dest;
}

#define JSDEST_X1 ( 43 + 16)
#define JSDEST_Y1    6
#define JSDEST_X2 (525 + 16)
#define JSDEST_Y2    6

void PzefGetJSDest(void)
{
	int dest = PzefJSDest;

	if(dest == -1)
		dest = rndbnd(0, 1);

	errorCase(dest < 0 || 1 < dest);

	if(dest == 0) // L
	{
		PzefX = JSDEST_X1;
		PzefY = JSDEST_Y1;
	}
	else // R
	{
		PzefX = JSDEST_X2;
		PzefY = JSDEST_Y2;
	}
}

static int ActHissJamaStar(void *vi)
{
	CutIn_t *i = (CutIn_t *)vi;

	if(krnd() < 0.05) // ‚«‚ç‚«‚ç
	{
		int kiraColor;

//		kiraColor = rndbnd(0, PUYO_COLOR_NUM - 1);
		kiraColor = i->q.HissJamaStar.Color;

		errorCase(kiraColor < 0 || PUYO_COLOR_NUM <= kiraColor);

#define HJS_KIRA_LIFE 30
		i = PzefAddEffect(
			i->X,
			i->Y,
			rndpm() * 3.0,
			rndpm() * 3.0,
			rnd() * 1.5 + 0.5,
			rnd() * 1.25 + 0.1,
			0.2,
			HJS_KIRA_LIFE,
			Pzdc.Dust[kiraColor]
			);
		i->Za = 0.1;
		i->Aa = -1.0 / HJS_KIRA_LIFE;
	}

	i->FrameCount--;
	if(i->FrameCount == 0)
	{
		i->Xa = 0.0;
		i->Ya = 0.0;
		i->Xaa = 0.0;
		i->Yaa = 0.0;
	}
	if(i->FrameCount < 0)
	{
		nearize(i->X, i->q.HissJamaStar.DestX, 0.8);
		nearize(i->Y, i->q.HissJamaStar.DestY, 0.8);
	}
	CIEx_ActCutIn(i);

	if(nearthan(i->X, i->Y, i->q.HissJamaStar.DestX, i->q.HissJamaStar.DestY, 30.0))
	{
		PzefAddStarPon(i->X, i->Y, 3);
		i->Alive = 0;
	}
	return i->Alive;
}
void PzefAddHissJamaStar(double startX, double startY, double destX, double destY, int color, int num)
{
	errorCase(color < 0 || PUYO_COLOR_NUM <= color);

	CutIn_t *i;

#define HJS_BIG_LIFE 45

	i = PzefAddEffectLayer(startX, startY, 0.0, 0.0, 0.0, rnd() * 0.25 + 0.1, 0.01, HJS_BIG_LIFE, Dc->PicCIHissStar, 1);
	i->Za = 0.05;
	i->Aa = -1.0 / HJS_BIG_LIFE;

	for(int cnt = 0; cnt < num; cnt++)
	{
		i = CIEx_TakeCutIn(0, 0);

		double moveKakudo = rnd() * PI * 2.0;
		double moveSokudo = rnd() * 2.0 + 7.0;

		int teitaiFrmCnt = rndbnd(5, 20);

		i->X = startX;
		i->Y = startY;
		i->Xa = sin(moveKakudo) * moveSokudo;
		i->Ya = cos(moveKakudo) * moveSokudo;
		i->Xaa = -i->Xa / teitaiFrmCnt;
		i->Yaa = -i->Ya / teitaiFrmCnt;

		i->Z = rnd() * 0.05 + 0.15;
		i->Ra = rndpm() * 0.25;
		i->A = rnd() * 0.3 + 0.7;

		i->FrameCount = teitaiFrmCnt;
//		i->Pic = krnd() < 0.5 ? Dc->PicEffeHana : Dc->PicEffeHoshi;
//		i->Pic = Dc->PicEffeHana;
		i->Pic = Dc->PicEffeHoshi;
//		i->Pic = Dc->PicEffeKagaya;

		i->q.HissJamaStar.Color = color;
		i->q.HissJamaStar.DestX = destX;
		i->q.HissJamaStar.DestY = destY;

		AddFrameTask(ActHissJamaStar, i);
	}
}

static int ActHissatsu(void *vi)
{
	CutIn_t *i = (CutIn_t *)vi;

	if(Pzdc.PauseChuu)
		i->FrameCount += 3; // ‚‘¬

	i->FrameCount++;
	int frmidx = i->FrameCount;
	double frmpct;

	double a = 0.0;

	frameif(frmidx, frmpct, 20)
	{
		a = frmpct;
		goto end_frameif;
	}
	frameif(frmidx, frmpct, 30)
	{
		a = 1.0;
		goto end_frameif;
	}
	frameif(frmidx, frmpct, 250)
	{
		a = 1.0 - frmpct;
		goto end_frameif;
	}
	i->Alive = 0;
end_frameif:

	SetAlpha(a);
	SimpleDraw(i->Pic, (int)i->X, (int)i->Y, 1);
	ResetAlpha();

	return i->Alive;
}
void PzefAddHissatsu(int mapIndex)
{
	CutIn_t *i = CIEx_TakeCutIn(mapIndex, mapIndex);

	i->X = mapIndex ? 400 : 0;
	i->Y = 0;
	i->Pic = GC_LPic(mapIndex ? PIC_HW_Hiss_R : PIC_HW_Hiss_L);

	AddFrameTask(ActHissatsu, i);
}

static int ActHissHide(void *vi)
{
	CutIn_t *i = (CutIn_t *)vi;

	if(Pzdc.GameOver || Pzdc.GameIsEnded)
		i->q.HissHide.TimeoutFrame = 0;

	if(!Pzdc.PauseChuu)
		i->FrameCount++;

	int frmidx = i->FrameCount;
	double frmpct;

	double a = 0.0;

	int frmcnt_1st = 90;
	int timeout = i->q.HissHide.TimeoutFrame - frmcnt_1st;

	if(timeout < 0)
		timeout = 0;

	frameif(frmidx, frmpct, frmcnt_1st)
	{
		a = frmpct;
		goto end_frameif;
	}
	frameif(frmidx, frmpct, timeout)
	{
		a = 1.0;
		goto end_frameif;
	}
	frameif(frmidx, frmpct, 20)
	{
		a = 1.0 - frmpct;
		goto end_frameif;
	}
	i->Alive = 0;
end_frameif:

	if(Pzdc.PauseChuu)
		a = 0.0;

	SetAlpha(a);

	if(a < 1.0)
	{
		for(int c = 0; c < 3; c++)
		{
			double k = 1.0 - a;
			double r = rnd() * PI * 2.0;

			k *= 100.0;
			k *= rnd() * 0.5 + 0.5;

			SimpleDraw(i->Pic, (int)(i->X + sin(r) * k), (int)(i->Y + cos(r) * k), 1);
		}
	}
	else
	{
		SimpleDraw(i->Pic, (int)i->X, (int)i->Y, 1);
	}
	ResetAlpha();

	return i->Alive;
}
void PzefAddHissHide(int mapIndex, int ipic, int kyoutsuuPicMode, int framecnt)
{
	CutIn_t *i = CIEx_TakeCutIn(mapIndex, mapIndex);

	i->X = 0;
	i->Y = 0;
	i->Pic = GC_LPic(ipic);

	if(mapIndex)
	{
#if 0
		if(kyoutsuuPicMode) i->X = Pzdc.MapList[1].DrawX - Pzdc.MapList[0].DrawX;
		else                i->X = 400;
#else
		i->X = 400;
#endif
	}

	i->q.HissHide.TimeoutFrame = framecnt;

	AddFrameTask(ActHissHide, i);
}

static void Kirara(double x, double y)
{	
	CutIn_t *i;

	PzefAddStarPon(x, y, 2);

	for(int c = 0; c < 1; c++)
	{
		const int LIFE = 30;

		i = PzefAddEffect(x, y, rndpm() * 3.5, rndpm() * 3.0, 0.5, rnd() * 0.5 + 0.25, 1.0, LIFE, Pzdc.Dust[rndbnd(1, DUST_PIC_NUM - 1)]);
		i->Aa = -1.0 / LIFE;
	}
}
static int ActKirara(void *vi)
{
	CutIn_t *i = (CutIn_t *)vi;

	i->FrameCount++;
	int frmidx = i->FrameCount;
	double frmpct;

	frameif(frmidx, frmpct, 60)
	{
		double k = frmpct * 130.0;

		Kirara(i->X + k, i->Y);
		Kirara(i->X - k, i->Y);

		goto end_frameif;
	}
	i->Alive = 0;
end_frameif:

	return i->Alive;
}
void PzefAddHissKirara_XY(int mapIndex, double x, double y)
{
	CutIn_t *i = CIEx_TakeCutIn(mapIndex, mapIndex);

	i->X = x;
	i->Y = y;

	AddFrameTask(ActKirara, i);
}
void PzefAddHissKirara(int mapIndex)
{
	PzefAddHissKirara_XY(mapIndex, Pzdc.MapList[mapIndex].DrawX + PANEL_X_SIZE * MAP_X_BOUND / 2, 30);
}

void PzefAddPuyoWarp(double x, double y, int color)
{
	errorCase(color < 0 || PUYO_COLOR_NUM <= color);

	const int LIFE = 25;

	const int fnum = 8;
	const double speed = 7.0;
	const double speedSpan = 13.0;

	for(int f = 1; f <= fnum; f++)
	{
		double r = (double)f / (fnum + 1);

		r *= PI;

		double rx = cos(r);
		double ry = sin(r);
		double sp = speed + rnd() * speedSpan;

		CutIn_t *i = PzefAddEffect(x, y, sp * rx, sp * ry, 0.0, rndpm() * 0.1, 0.3 + rnd() * 0.7, LIFE, Pzdc.Dust[color]);

		i->Xaa = -((sp * rx) / LIFE);
		i->Yaa = -((sp * ry) / LIFE);
		i->Aa = -(1.0 / LIFE);
	}

	// OLD
	/*
	const int LIFE = 25;

	const double zmin = 0.5;
	const double zspn = 1.5;

	const int fnum = 4;
	const double speed = 7.0;

	for(int f = 1; f <= fnum; f++)
	{
		double r = (double)f / (fnum + 1);

		r *= PI;

		double rx = cos(r);
		double ry = sin(r);

		CutIn_t *i = PzefAddEffect(x, y, speed * rx, speed * ry, 0.0,  0.2, zmin,        LIFE, Pzdc.Dust[color]);
		CutIn_t *j = PzefAddEffect(x, y, speed * rx, speed * ry, 0.0, -0.2, zmin + zspn, LIFE, Pzdc.Dust[color]);

		i->Aa = -(1.0 / LIFE);
		j->Aa = -(1.0 / LIFE);

		i->Za = +(zspn / LIFE);
		j->Za = -(zspn / LIFE);
	}
	*/

	// OLD
	/*
	const int LIFE = 35;

	const double speed = 4.2;
	const double rSpeed = 1.0;

	for(int lr = 0; lr < 2; lr++)
	{
		CutIn_t *i = PzefAddEffect(x, y, speed * (lr ? -1 : 1), speed, 0.0,  rSpeed, 0.3, LIFE, Dc->PicEffeKagaya);
		CutIn_t *j = PzefAddEffect(x, y, speed * (lr ? -1 : 1), speed, 0.0, -rSpeed, 0.3, LIFE, Dc->PicEffeKagaya);

		i->Aa = -(1.0 / LIFE);
		j->Aa = -(1.0 / LIFE);

		i->Raa = -(rSpeed / LIFE);
		j->Raa =  (rSpeed / LIFE);
	}
	*/
}
