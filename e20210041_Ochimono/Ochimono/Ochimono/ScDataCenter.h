typedef struct ScCharaInfo_st
{
	int PrevPic; // -1 : なし
	int Pic;     // -1 : なし
	double A; // 0.0 - 1.0
	int SleepCnt;

	int DrawX;
	int DrawY;

	double YureFar;
	double YureX;
	double YureY;
}
ScCharaInfo_t;

enum
{
	SC_MESSBTN_SAVE,
	SC_MESSBTN_LOAD,
	SC_MESSBTN_AUTO,
	SC_MESSBTN_SKIP,
	SC_MESSBTN_MENU,

	SC_MESSBTN_MAX, // == length
};

typedef struct ScDataCenter_st
{
	// -- Params --
	char *ScnrTitle; // ex. "プロローグ"

	int JumpActionIndex;
	// ----

	// -- Results --
	int DoReload; // ロードした、再実行して下さい。ロード先は、SL* にあり
	int ReturnBackToTitle;
	// ----

	char ScnrFile[2048];
	ScScenario_t Scenario;

	int ActionIndex;

	int PicSuperWall; // -1 : なし
	double WallA; // 0.0 - 1.0
	int PicWall; // -1 : なし (黒)
	// int WallForDisp

	double MessA;
	int MessForDisp;

	ScCharaInfo_t CharaList[4];

	int SelectButton; // -1: no-select, 0 - (SC_MESSBTN_MAX - 1)

	int IPicButtonList[SC_MESSBTN_MAX];
	int IPicFocusButtonList[SC_MESSBTN_MAX];
	int ButtonPosList[SC_MESSBTN_MAX][2]; // [][x, y]
	int ButtonXSize;
	int ButtonYSize;
}
ScDataCenter_t;

extern ScDataCenter_t *Scdc;

enum
{
	SC_ROUTE_NONE,
	SC_ROUTE_GJ,
	SC_ROUTE_IP,
	SC_ROUTE_GP,
	SC_ROUTE_IJ,
	SC_ROUTE_GI,
	SC_ROUTE_PJ,
};
typedef struct ScProcDc_st
{
	int RouteSelected; // == SC_ROUTE_*
}
ScProcDc_t;

extern ScProcDc_t ScProcDc;
