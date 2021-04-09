typedef struct PzAI_st
{
	// -- Params --
	int RndMovRotMax; // 0～ == RndMovRotMax < i.MovRotCount から目標位置を目指す。
	int MovRotMax;    // 0～ == MovRotMax < i.MovRotCount から目標位置を諦める。
	int MovRotSleep;  // 0～ == 最後の移動・回転から何もしないでいるフレーム数

	double MovRotPct;  // 0.0～1.0
	double HiSpeedPct; // 0.0～1.0

	// [通常時, 打ち返し時]
	double HissPct[2];       // 0.0～1.0
	double HissJouSenyuu[2]; // 0.0～1.0       == 必殺発動に必要なフィールド占有率
	int HissJouTakasa[2];    // 0～MAP_Y_BOUND == 必殺発動に必要な最大の高さ

	/*
		落下位置決定のために実行する。
		実行直前に PzAbstracter() を実行する。
	*/
	void (*Think)(void);
	// ----

	// -- PzAI_Think() Results --
	int MovDir; // -1～1 == 左方向, 移動なし, 右方向
	int RotDir; // -1～1 == 反時計, 移動なし, 時計
	int HiSpeed;
	int Hissatsu;
	// ----

	// PzAI_Reset() 毎に初期化
	struct
	{
		int HiSpeedChuu;
		int MovRotCount;
		int MovRotElaps;

		/*
		struct
		{
			int Inited;
			int MovDir;
			int MovRemain;
		}
		QuickEscape;
		*/

		int Inited;
		int Pinch;

		// -- PzAI.Think() Results --
		int PurX;
		int PurDir;
		// ----
	}
	i;

	// 思考ルーチン別領域
	union {
		struct {
			int DangerMode;
		}
		Lv4;
		struct {
			double LastHyouka;
		}
		ClockTower;
	}
	q;
}
PzAI_t;

extern PzAI_t PzAI;

void PzAI_DefThink(void);

void PzAI_Init(void);
void PzAI_Fnlz(void);
void PzAI_Reset(void);
void PzAI_Think(void);

// -- DEBUG --
extern int PzAI_DBG_SpeedLv;
extern char *PzAI_DBG_Think;
/*
extern double PzAI_DBG_hr;
extern int    PzAI_DBG_ht;
extern double PzAI_DBG_hs;
extern double PzAI_DBG_ur;
extern int    PzAI_DBG_ut;
extern double PzAI_DBG_us;
*/
// ----

void PzAI_SetStdParams(int speedLevel, void (*funcThink)(void), double hr, int ht, double hs, double ur, int ut, double us);

// 以下、PzAI.Think() 用 util

void PzAI_Abstracter(double (*funcHyouka)(PzABST_Case_t *));

int PzAI_IsAnzen(void);
int PzAI_IsDanger(void);
double PzAI_BestOfNext(PzABST_Case_t *i);
double PzAI_WorstOfNext(PzABST_Case_t *i);

extern int PzAI_CaseTable[MAP_X_BOUND][MAP_Y_BOUND];

void PzAI_MakeCaseTable(PzABST_Case_t *i);
void PzAI_LRTurnCaseTable(void);
