// -- PzDE* --
extern double PzDECenterX;
extern double PzDECenterY;
extern int PzDESeOff;
extern int PzDEEffectOff;
extern int PzDEEraseCount;
extern int PzDEJEraseCount;
extern int PzDEDeadXList[MAP_X_BOUND * MAP_Y_BOUND];
extern int PzDEDeadYList[MAP_X_BOUND * MAP_Y_BOUND];
extern int PzDEDeadColorList[MAP_X_BOUND * MAP_Y_BOUND];
extern int PzDEDeadCnt;
// ----
void PzEx_DoErase(Map_t *m);

void PzMain(void);

// -- 音 --
#define RESRC_BGM_SEA       "Puzzle\\音\\MusMus-BGM-109_muon-100-100.mp3"
#define RESRC_BGM_DESERT    "Puzzle\\音\\MusMus-CT-NAVAO-21_muon-100-100.mp3"
#define RESRC_BGM_FLOWER    "Puzzle\\音\\MusMus-BGM-087_muon-100-100.mp3"
#define RESRC_BGM_NIGHT     "Puzzle\\音\\MusMus-BGM-030_muon-100-100.mp3"
#define RESRC_BGM_FOREST    "Puzzle\\音\\MusMus-BGM-074_muon-100-100.mp3"
#define RESRC_BGM_SUNFLOWER "Puzzle\\音\\MusMus-BGM-009_muon-100-100.mp3"
#define RESRC_BGM_LAKE      "Puzzle\\音\\MusMus-BGM-040_muon-100-100.mp3"
#define RESRC_BGM_JINJA     "Puzzle\\音\\MusMus-BGM-118_muon-100-100.mp3"
#define RESRC_BGM_PINCH01   "Puzzle\\音\\Pinch01.mp3" // 不使用
#define RESRC_BGM_PINCH02   "Puzzle\\音\\Pinch02.mp3" // 不使用
#define RESRC_BGM_NOTUSED01 "Puzzle\\音\\NotUsed01.mp3" // 不使用
#define RESRC_BGM_NOTUSED02 "Puzzle\\音\\NotUsed02.mp3" // 不使用
// ----

int PzutlGetTakasa(Map_t *m, int x);
int PzutlGetTakasaMin(Map_t *m);
