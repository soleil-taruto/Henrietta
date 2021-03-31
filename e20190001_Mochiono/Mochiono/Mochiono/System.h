#define FT_LAYER_MIN (-2)
#define FT_LAYER_MAX ( 2)

void AddFrameTaskLayer(int (*func)(void *), void *info, int layer);
void AddFrameTask(int (*func)(void *), void *info);
int Sys_GetFrameTaskCount(void);
void ExecFrameTasks(void);
extern int EFCDone;
void ExecFrameCurtain(void);
void WaitToCurtain(int picWall);

extern int Sys_EscapeDisable;
extern int Sys_EscapePressed;

void SFS_Reset(void);
void SwapFrameScreen(void);
void SwapFrame(void);

int IsPress(int key);
extern int IH_LastPressed;
extern int IH_Pressed;
int IsHit(int key);
int IsRenda(int key);

int GetPressPad(int padNo);
extern int IH_PadLastPressed;
extern int IH_PadPressed;
int GetHitPad(int padNo);

void SetMusPos(int xp, int yp);
void GetMusPos(int *xp, int *yp);
int GetMusBtn(void);
int GetClick(void);

int IsMusIn(int x1, int x2, int y1, int y2);
int IsMusNear(int x, int y, int r);

void DispMusCursor(int doDisp);

char *ResourcePath(char *innerPath);
char *x_ResourcePath(char *innerPath);
char *ResourcePath_x(char *innerPath);

extern int LDrawRGB;
void LDraw(char *ljchr, int x, int y);

void PlayDouga(char *file);
void GameTermination(void);

#define BUILT_STAMP_LEN 20

char *GetBuiltStamp(void);

typedef struct CommonEffect_st
{
	int Pic;
	int KasanMode;
	double X;
	double Y;
	double R;
	double Z;
	double A;
	double Xa;
	double Ya;
	double Ra;
	double Za;
	double Aa;
	double Xaa;
	double Yaa;
	double Raa;
	double Zaa;
	double Aaa;
	// <-- Init by Params

	int FrmCnt;

	int LifeCount; // 0 == –³Œø
	double DeadAa; // 0 -> 1/30

	double KasokuRate; // 0.0 == –³Œø
}
CommonEffect_t;
CommonEffect_t *AddCommonEffect(
	int pic,
	int kasanMode,
	double x = 400.0,
	double y = 300.0,
	double r = 0.0,
	double z = 1.0,
	double a = 1.0,
	double xa = 0.0,
	double ya = 0.0,
	double ra = 0.0,
	double za = 0.0,
	double aa = 0.0,
	double xaa = 0.0,
	double yaa = 0.0,
	double raa = 0.0,
	double zaa = 0.0,
	double aaa = 0.0
	);

extern int ProcFrmCnt;