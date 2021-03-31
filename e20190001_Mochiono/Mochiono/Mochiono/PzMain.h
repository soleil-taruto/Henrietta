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
#define RESRC_BGM_3DAYS "Puzzle\\音\\3days.mp3"
#define RESRC_BGM_TEIEN "Puzzle\\音\\BGM_永遠色五色_庭園.mp3"
#define RESRC_BGM_CAFE  "Puzzle\\音\\cafe.mp3"
#define RESRC_BGM_STORY "Puzzle\\音\\cover_story.mp3"      // -> D
#define RESRC_BGM_DANCE "Puzzle\\音\\dance_in_the_sky.mp3" // -> D
#define RESRC_BGM_EVE   "Puzzle\\音\\eve.mp3"
#define RESRC_BGM_SHIKI "Puzzle\\音\\k_sikisai.mp3"
#define RESRC_BGM_MITSU "Puzzle\\音\\mitukoi.mp3"
#define RESRC_BGM_POLP  "Puzzle\\音\\polp.mp3"
#define RESRC_BGM_SUI   "Puzzle\\音\\suikyou.mp3" // -> D
#define RESRC_BGM_D     "Puzzle\\音\\d.mp3"
#define RESRC_BGM_NIISAN "Puzzle\\音\\チョイとソコ行く変な兄さん.mp3"
// ----

int PzutlGetTakasa(Map_t *m, int x);
int PzutlGetTakasaMin(Map_t *m);
