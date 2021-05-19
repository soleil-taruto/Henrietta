typedef struct PzABST_Case_st
{
	int Enable; // X, Dir が有り得るか
	int PurX;
	int PurDir;

	int SecchiX[2];
	int SecchiY[2];
	int SecchiTakasa[2];
	int SecchiColor[2];

	int Takasa;      // 1 ～ MAP_Y_BOUND   接地したブロックの高い方の高さ
	int RelateCount; // 2 ～               接地時に何個つながるか、最悪それぞれ孤立して 1 + 1 -> 2
	int RensaCount;  // 0 ～               連鎖回数
	int EraseCount;  // 0 ～               消えるブロック数 (邪魔を含む)
	int JEraseCount; // 0 ～               消える邪魔の数

	struct
	{
		int RoCount;      // 0 ～ PUYO_COLOR_NUM   露出する色の数
		int RelSecchi[2]; // 0 ～                  接地側の繋がりの数, 0 == 邪魔・空, 1 == 孤立 (連鎖発生時は当てにならない)

		int TateRelCnt;
		int YokoRelCnt;
	}
	ExtraData;

	double Hyouka;
}
PzABST_Case_t;

typedef struct PzABST_Result_st
{
	int Table[MAP_X_BOUND][MAP_Y_BOUND]; // (-1) ～ (PUYO_COLOR_NUM - 1) == (-1: 空, 0 - PUYO_COLOR_NUM: 各色 (0 == 邪魔))
	int RakkaColors[2]; // 1 ～ (PUYO_COLOR_NUM - 1)
	int PuyoCount; // Jama 除く
	int JamaCount;

	PzABST_Case_t CaseTable[MAP_X_BOUND][10]; // [X][Dir]

	int TakasaList[MAP_X_BOUND];
	int TakasaMin;
	int TakasaMax;

	// -- PzGetMaxHyouka() Results --
	int PurX;
	int PurDir;
	// ----
}
PzABST_Result_t;

extern PzABST_Result_t PzABST_Result;

void PzAbstracter(int mapIndex);
void PzAllHyouka(double (*funcHyouka)(PzABST_Case_t *));
void PzGetMaxHyouka(void);
