typedef enum GC_PIC_et
{
	PIC_Start,

	PIC_SCSys_Start,
	PIC_SCSys_MessageWindow,
	PIC_SCSys_Save,
	PIC_SCSys_SaveFocus,
	PIC_SCSys_Load,
	PIC_SCSys_LoadFocus,
	PIC_SCSys_Auto,
	PIC_SCSys_AutoFocus,
	PIC_SCSys_Skip,
	PIC_SCSys_SkipFocus,
	PIC_SCSys_Menu,
	PIC_SCSys_MenuFocus,
	PIC_SCSys_End,

	PIC_RS_Start,
	PIC_RS_Title,
	PIC_RS_Hide,
	PIC_RS_HideFocus,
	PIC_RS_GJ,
	PIC_RS_GJFocus,
	PIC_RS_IP,
	PIC_RS_IPFocus,
	PIC_RS_GP,
	PIC_RS_GPFocus,
	PIC_RS_IJ,
	PIC_RS_IJFocus,
	PIC_RS_GI,
	PIC_RS_GIFocus,
	PIC_RS_PJ,
	PIC_RS_PJFocus,
	PIC_RS_End,

	PIC_SL_Start,
	PIC_SL_SaveWall,
	PIC_SL_LoadWall,
	PIC_SL_Modoru,
	PIC_SL_ModoruFocus,
	PIC_SL_SaveSuruka,
	PIC_SL_LoadSuruka,
	PIC_SL_Suru,
	PIC_SL_SuruFocus,
	PIC_SL_Shinai,
	PIC_SL_ShinaiFocus,
	PIC_SL_End,

	PIC_HW_Start,
	PIC_HW_Hiss_L,
	PIC_HW_Hiss_R,
	PIC_HW_Slow_L,
	PIC_HW_Slow_R,
	PIC_HW_Fast_L,
	PIC_HW_Fast_R,
	PIC_HW_Hide_J,
	PIC_HW_Hide_U,
	PIC_HW_Hide_S,
	PIC_HW_Hide_GJ,
	PIC_HW_Hide_EJ,
	PIC_HW_Hide_AE,
	PIC_HW_Hide_UP,
	PIC_HW_End,

	PIC_End,

	PIC_MAX, // ’·‚³—p
}
GC_PIC_t;

extern char *GC_PicFiles[PIC_MAX];
extern int GC_PicList[PIC_MAX];

void GC_ProcInitReset(void);
void GC_ProcInit(void);
void GC_ProcFnlz(void);

void GC_Load(int index);
void GC_Unload(int index);
void GC_LoadRange(int minidx, int maxidx);
void GC_UnloadRange(int minidx, int maxidx);
void GC_UnloadAll(void);

int GC_Pic(int index);
int GC_LPic(int index);

#define GC_NMPIC_MAX 128

extern char *GC_NmPicNames[GC_NMPIC_MAX];
extern char *GC_NmPicFiles[GC_NMPIC_MAX];
extern int GC_NmPicList[GC_NMPIC_MAX];
extern int GC_NmPicIndex;

void GC_NmPicLoad(char *name);
int GC_IsNmPicLoaded(char *name);
void GC_NmPicUnloadAll(void);
int GC_NmPic(char *name);

#define GC_NMBGM_MAX 128

extern char *GC_NmBgmNames[GC_NMBGM_MAX];
extern int GC_NmBgmList[GC_NMBGM_MAX];
extern int GC_NmBgmCount;

void GC_NmBgmLoadAll(void);
int GC_NmBgm(char *name);
