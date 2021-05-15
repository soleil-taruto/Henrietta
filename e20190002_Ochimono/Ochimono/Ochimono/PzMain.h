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
#define RESRC_BGM_SEA       "Puzzle\\��\\3days.mp3"
#define RESRC_BGM_DESERT    "Puzzle\\��\\BGM_�i���F�ܐF_�뉀.mp3"
#define RESRC_BGM_FLOWER    "Puzzle\\��\\cafe.mp3"
#define RESRC_BGM_NIGHT     "Puzzle\\��\\cover_story.mp3"
#define RESRC_BGM_FOREST    "Puzzle\\��\\dance_in_the_sky.mp3"
#define RESRC_BGM_EVE       "Puzzle\\��\\eve.mp3"
#define RESRC_BGM_SUNFLOWER "Puzzle\\��\\k_sikisai.mp3"
#define RESRC_BGM_MITSU     "Puzzle\\��\\mitukoi.mp3"
#define RESRC_BGM_LAKE      "Puzzle\\��\\polp.mp3"
#define RESRC_BGM_JINJA     "Puzzle\\��\\suikyou.mp3"
#define RESRC_BGM_PINCH01   "Puzzle\\��\\�`���C�ƃ\�R�s���ςȌZ����.mp3"
#define RESRC_BGM_PINCH02   "Puzzle\\��\\d.mp3"
// ----

int PzutlGetTakasa(Map_t *m, int x);
int PzutlGetTakasaMin(Map_t *m);
