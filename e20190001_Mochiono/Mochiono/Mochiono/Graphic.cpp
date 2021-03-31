#include "StdInc.h"

int LoadPicture(char *file)
{
	int pic = LoadGraph(file);

if(pic == -1) printfDx("%d: %s\n", pic, file);

	errorCase(pic == -1);
	return pic;
}
void UnloadPicture(int pic)
{
	DeleteGraph(pic);
}

#define P2_SIZE 1024

static int P2List[P2_SIZE];
static int P2Index;

int LoadPzPicture(char *file)
{
	int pic = LoadPicture(file);

	errorCase(P2_SIZE <= P2Index);

	P2List[P2Index++] = pic;
	return pic;
}
void UnloadPzPictureAll(void)
{
	while(P2Index)
	{
		P2Index--;
		UnloadPicture(P2List[P2Index]);
	}
}

void GetPictureSize(int pic, int *xp, int *yp)
{
	if(GetGraphSize(pic, xp, yp) != 0)
	{
		error();
	}
}

void SetAlpha(double a, int kasanMode) // a : 0.0 - 1.0 == 透明 - 不透明
{
	int pal = (int)(a * 255);

	errorCase(pal < 0 || 255 < pal);

	if(SetDrawBlendMode(kasanMode ? DX_BLENDMODE_ADD : DX_BLENDMODE_ALPHA, pal) != 0)
	{
		error();
	}
}
void ResetAlpha(void)
{
	if(SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0) != 0)
	{
		error();
	}
}

void SimpleDraw(int pic, int x, int y, int enable_trans)
{
	if(DrawGraph(x, y, pic, enable_trans) != 0)
	{
		error();
	}
}
void SimpleDrawCenter(int pic, int x, int y, int enable_trans)
{
	int xs;
	int ys;

	GetPictureSize(pic, &xs, &ys);

	SimpleDraw(pic, x - xs / 2, y - ys / 2, enable_trans);
}

DrawPictureParam_t Dpp;
static int AntiAilasEnable;

void DppInit(int pic, double x, double y, int enable_trans) // enable_trans : 0 == 不透明, 1 == 透明色有効
{
	int xsize;
	int ysize;

	GetPictureSize(pic, &xsize, &ysize);

	Dpp.Pic = pic;
	Dpp.X = x;
	Dpp.Y = y;
	Dpp.Xlt = -xsize / 2.0;
	Dpp.Ylt = -ysize / 2.0;
	Dpp.Xrt =  xsize / 2.0;
	Dpp.Yrt = -ysize / 2.0;
	Dpp.Xrb =  xsize / 2.0;
	Dpp.Yrb =  ysize / 2.0;
	Dpp.Xlb = -xsize / 2.0;
	Dpp.Ylb =  ysize / 2.0;
	Dpp.TransEnable = enable_trans;
}
void DppDraw(void)
{
	int retval;

	if(AntiAilasEnable)
	{
		retval = DrawModiGraphF(
			(float)(Dpp.X + Dpp.Xlt),
			(float)(Dpp.Y + Dpp.Ylt),
			(float)(Dpp.X + Dpp.Xrt),
			(float)(Dpp.Y + Dpp.Yrt),
			(float)(Dpp.X + Dpp.Xrb),
			(float)(Dpp.Y + Dpp.Yrb),
			(float)(Dpp.X + Dpp.Xlb),
			(float)(Dpp.Y + Dpp.Ylb),
			Dpp.Pic,
			Dpp.TransEnable
			);
	}
	else
	{
		retval = DrawModiGraph(
			(int)(Dpp.X + Dpp.Xlt),
			(int)(Dpp.Y + Dpp.Ylt),
			(int)(Dpp.X + Dpp.Xrt),
			(int)(Dpp.Y + Dpp.Yrt),
			(int)(Dpp.X + Dpp.Xrb),
			(int)(Dpp.Y + Dpp.Yrb),
			(int)(Dpp.X + Dpp.Xlb),
			(int)(Dpp.Y + Dpp.Ylb),
			Dpp.Pic,
			Dpp.TransEnable
			);
	}
	errorCase(retval != 0);
}

#define Rotate(x, y, rot) \
	do { \
	double xx = x * cos(rot) - y * sin(rot); \
	double yy = x * sin(rot) + y * cos(rot); \
	x = xx; \
	y = yy; \
	} while(0)

void DppRotate(double rot) // rot : PI == 180度, 時計回り
{
	Rotate(Dpp.Xlt, Dpp.Ylt, rot);
	Rotate(Dpp.Xrt, Dpp.Yrt, rot);
	Rotate(Dpp.Xrb, Dpp.Yrb, rot);
	Rotate(Dpp.Xlb, Dpp.Ylb, rot);
}
void DppZoom(double zoom)
{
	Dpp.Xlt *= zoom;
	Dpp.Ylt *= zoom;
	Dpp.Xrt *= zoom;
	Dpp.Yrt *= zoom;
	Dpp.Xrb *= zoom;
	Dpp.Yrb *= zoom;
	Dpp.Xlb *= zoom;
	Dpp.Ylb *= zoom;
}

void SetAntiAlias(int mode) // mode: ジャギー対策 0:なし(デフォ), 1:あり
{
	AntiAilasEnable = mode;
	SetDrawMode(mode ? DX_DRAWMODE_BILINEAR : DX_DRAWMODE_NEAREST);
}
