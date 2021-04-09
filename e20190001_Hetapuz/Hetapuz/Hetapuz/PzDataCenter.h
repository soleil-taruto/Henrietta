typedef struct CutIn_st
{
	int Alive;
	int MapIndex;
	int Side; // 0 == 1P, 1 == 2P
	int Pic;

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

	int FrameCount;
	double PrvRnd;

	union {
		struct {
			int Jumped;
		}
		Rensa3;
		struct {
			int MachiCount;
		}
		Rensa7;
		struct {
			int MachiCount;
		}
		Damage;
		struct {
			struct
			{
				int Count;
				int MapIndex;
			}
			JamaCountInfo;

			double EndX;
			double EndY;

			double Speed;
			double GuruR;
			double GuruRa;
			double GuruHan;
		}
		JamaStar;
		struct {
			int Color;

			double DestX;
			double DestY;
		}
		HissJamaStar;
		struct {
			int TimeoutFrame;
		}
		HissHide;
	}
	q;

	int KasanMode;
}
CutIn_t;

#define CUTIN_NUMMAX 8192

typedef struct Puyo_st
{
	int Color; // 0 ～ PUYO_COLOR_NUM - 1
		// 0 == おじゃま
	double A;
	int RotCnt;
	double WarpRot;

	// 画面上の位置
	double RealX;
	double RealY;

	// 内部的な位置
	double TrueX;
	double TrueY;

	int Erase;
	int Death;

	double LastXAdd;
	double LastYAdd;

	// 2012追加
	int G_Mode;
}
Puyo_t;

#define PUYO_ROTCNT_MAX 13

typedef struct Panel_st
{
	Puyo_t *Puyo; // NULL == Empty
}
Panel_t;

#define MAP_X_BOUND 6
#define MAP_Y_BOUND (12 + 2)

#define PANEL_X_SIZE 45
#define PANEL_Y_SIZE 45

#define SECCHI_TIMER_MAX 80
#define HOVER_TIMER_MAX 360

typedef struct Map_st
{
	int SelfIndex;  // 0 == 左, 1 == 右
	int PlayerChar; // 0 == 左, 1 == 右

	int DrawX;
	int DrawY;

	Panel_t PanelTable[MAP_X_BOUND][MAP_Y_BOUND];

	int ForceSleepTimer;
	int AntiRakkaTimer;

	int DoHissatsu;
	int HissUsedCount;
	int HissSleepTimer;

	// -- 連鎖 --
	int DoRensaProcess;
	int RensaProcessChuu;

	int RensaCount;
	int RensaTimer;
	// ----

	// -- 邪魔落下 --
	int DoJamaProcess;

	int JamaCount;
	int JamaTimer;

	int UIJamaCount;
	int UIJamaSleep;

	int Jama2ColorMode;
	// ----

	sint KaitenDir; // -1: 反時計, 1: 時計
	sint IdouDir; // -1: Left, 1: Right
	int Kousoku; // 高速落下
	int Hissatsu; // 必殺技発動！

	// -- 落下 --
	int NeedInitRakka;
	int NeedInitAI;

	Puyo_t *RakkaChuu[2]; // 0: 軸, 1: 振り回される方
	int RakkaX;
	double RakkaY;
	int RakkaDir; // 2648
	int SecchiTimer; // SECCHI_TIMER_MAX で、タイムアウトとする。
	int HoverTimer; // HOVER_TIMER_MAX で、強制接地させる。
	// ----

	Puyo_t *StockTable[2][2]; // (0: Next, 1: その次), (0: 軸, 1: 振り回される方)
	int StockDrawX[2][2];
	int StockDrawY[2][2];
	int ColorStreamIndex;

	// -- 顔グラ --
	int FaceX[2];
	int FaceY[2];
	int FaceLevel;  // 0 - 4
	int PuyoSenyuu; // 0 - (MAP_X_BOUND * MAP_Y_BOUND)
	int JamaSenyuu; // 0 - (MAP_X_BOUND * MAP_Y_BOUND)

	int AnimeCnt[2];
	// ----

	int GoX;
	int GoY;
	int GoStatus; // 0 - 1

	int Score;
	int UIScore;

	int TokushuRakka; // -1: 禁止, 1: 常に高速
	int TokushuHidden;
	__int64 TokushuTimeout; // タイムアウトするフレーム
}
Map_t;

typedef struct Dust_st
{
	int GIdx;
	double Zoom;
	double X;
	double Y;
	double Xadd;
	double Yadd;
	double R;
	double Radd;
}
Dust_t;

#define COLOR_STREAM_LEN 1000000

#define PUYO_COLOR_NUM 6
#define PUYO_NUMMAX 200
#define PUYO_SQ_SIZE 1024

#define DUST_PIC_NUM PUYO_COLOR_NUM
#define DUST_NUMMAX 3000

#define RENSA_MAX 99
#define FACE_CNT 5

#define TB_TITLE_LENMAX 15

typedef struct TokushuButtle_st
{
	char Title[TB_TITLE_LENMAX + 1];
	int ColorMax; // 0: 無効
	int Stealth[PUYO_COLOR_NUM]; // 1 == 隠す
	int StealthDisable[2]; // [1P, 2P]
	int GameOverAndShowStealth;
	int FlowerRain;
	int FlowerRainbow;
	int StarRain;
	int JamaExpand;
	int ScoreExpand;
	int AlwaysHidden;
	int VeryFast; // 0: 無効, 1: 高速, 2: 超高速
	int PuyoEraseNum; // 0: 無効
	int BigFlower;
}
TokushuButtle_t;

#define DBG_2P_RENSA_COUNTER_MAX 10

typedef struct DataCenter_st
{
	// -- Params --
	int Chara[2][2]; // [1P, 2P][1人目, 2人目] == CHARA_*
	int Basho;       // BASHO_*
	int HissatsuType[2]; // [1P, 2P] == HWT_*

	int UseAI; // AI を使用する。

	char *BGMFileNormal;
	char *BGMFilePinch;

	TokushuButtle_t TokushuButtle;

	int NetworkMode;
	int NetKansenMode; // not 0 のとき NetworkMode も立ち上げること！
	// ----

	int BGMNormal;
	int BGMPinch;

	char LChara[2][2][3]; // "xP"
	char LPair[2][5]; // "1P2P"
	char *LBasho;
	int IsSolo[2];

	Puyo_t PuyoSequence[PUYO_SQ_SIZE];
	Puyo_t *PuyoStock[PUYO_SQ_SIZE];
	int PuyoStockIndex;

	int PuyoEraseMin;

	__int64 FrameCnt;
	uchar ColorStream[COLOR_STREAM_LEN];

	double SS_Rot;
	double SS_Zoom;
	double SS_Alpha;
	double SS_Takasa;
	double SS_Rakka;
	int SS_Hane;

	int PauseChuu;
	int PauseSelExitGame;

	int GameOver;
	int GameFreezeCount;
	int Winner; // 0 == Left, 1 == Right
	int GameIsEnded; // ゲーム終了後、カットインなどの終了待ちに入るとき立ち上げる。

	// -- 画像 --
	int WallPaper;
	int FrontPaper;
	int Puyo[PUYO_COLOR_NUM];
	int Dust[DUST_PIC_NUM];
	int Rensa[RENSA_MAX]; // [0] == 1連鎖 ...
//	int Face[FACE_CNT];
	int Go[2];
	// ----

	Dust_t DustList[DUST_NUMMAX];
	int DustCount;

	Puyo_t *PuyoList[PUYO_NUMMAX];
	int PuyoCount;

	CutIn_t CutInList[CUTIN_NUMMAX];
	int CutInIndex;

	Map_t MapList[2]; // 0: Left (Player), 1: Right (Enemy)

	// -- GameOver --
	double GmoCharX[2];
	double GmoCharY[2];
	double GmoCharDestY[2];
	int GmoCharPic[2];

	double GmoMojiX[2];
	double GmoMojiXAdd[2];
	double GmoMojiTrueX[2];
	double GmoMojiY[2];
	double GmoMojiYAdd[2];
	double GmoMojiTrueY[2];
	int GmoMojiPic[2];
	// ----

	double RainPct;
	__int64 TB_Hide_NextFrameCnt;
	int P1Muteki;
	int P2Muteki;

	// -- DEBUG --
	int P2RensaCounter[DBG_2P_RENSA_COUNTER_MAX];
	int P2SecchiCounter;
	// ----
}
DataCenter_t;

extern DataCenter_t Pzdc;

typedef struct PzdcRet_st
{
	int GameOver; // 0: ゲームを中断した。1: 決着した。
	int Winner; // 0: 1P勝利, 1: 1P敗北

	int PauseSelExitApp;
}
PzdcRet_t;

extern PzdcRet_t PzdcRet;
