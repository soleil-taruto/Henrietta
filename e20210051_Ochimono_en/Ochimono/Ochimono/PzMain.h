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

// -- �� --
#define RESRC_BGM_SEA       "Puzzle\\��\\MusMus-BGM-109_muon-100-100.mp3"
#define RESRC_BGM_DESERT    "Puzzle\\��\\MusMus-CT-NAVAO-21_muon-100-100.mp3"
#define RESRC_BGM_FLOWER    "Puzzle\\��\\MusMus-BGM-087_muon-100-100.mp3"
#define RESRC_BGM_NIGHT     "Puzzle\\��\\MusMus-BGM-030_muon-100-100.mp3"
#define RESRC_BGM_FOREST    "Puzzle\\��\\MusMus-BGM-074_muon-100-100.mp3"
#define RESRC_BGM_SUNFLOWER "Puzzle\\��\\MusMus-BGM-009_muon-100-100.mp3"
#define RESRC_BGM_LAKE      "Puzzle\\��\\MusMus-BGM-040_muon-100-100.mp3"
#define RESRC_BGM_JINJA     "Puzzle\\��\\MusMus-BGM-118_muon-100-100.mp3"
#define RESRC_BGM_PINCH01   "Puzzle\\��\\Pinch01.mp3" // �s�g�p
#define RESRC_BGM_PINCH02   "Puzzle\\��\\Pinch02.mp3" // �s�g�p
#define RESRC_BGM_NOTUSED01 "Puzzle\\��\\NotUsed01.mp3" // �s�g�p
#define RESRC_BGM_NOTUSED02 "Puzzle\\��\\NotUsed02.mp3" // �s�g�p
// ----

int PzutlGetTakasa(Map_t *m, int x);
int PzutlGetTakasaMin(Map_t *m);
