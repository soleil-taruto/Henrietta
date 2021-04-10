int LoadPicture(char *file);
void UnloadPicture(int pic);

int LoadPzPicture(char *file);
void UnloadPzPictureAll(void);

void GetPictureSize(int pic, int *xp, int *yp);

void SetAlpha(double a, int kasanMode = 0);
void ResetAlpha(void);

void SimpleDraw(int pic, int x, int y, int enable_trans);
void SimpleDrawCenter(int pic, int x, int y, int enable_trans);

typedef struct DrawPictureParam_st
{
	int Pic;
	double X;
	double Y;
	double Xlt;
	double Ylt;
	double Xrt;
	double Yrt;
	double Xrb;
	double Yrb;
	double Xlb;
	double Ylb;
	int TransEnable;
}
DrawPictureParam_t;
extern DrawPictureParam_t Dpp;

void DppInit(int pic, double x, double y, int enable_trans);
void DppDraw(void);
void DppRotate(double rot);
void DppZoom(double zoom);

void SetAntiAlias(int mode);