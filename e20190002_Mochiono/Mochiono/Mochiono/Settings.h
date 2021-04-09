typedef struct KeySettings_st
{
	int RotL;
	int RotR;
	int Dir2;
	int Dir4;
	int Dir6;
	int Dir8;
	int Bomb;
	int Pause;
}
KeySettings_t;

enum {
	KSIIDX_P1,
	KSIIDX_P2,
};
enum {
	KSIIDX_KB,
	KSIIDX_PAD,
};

typedef struct SaveData_st
{
	char UIText[1024 + 128];
	char Scenario[1024 + 128]; // == SAVEDATA_EMPTY_SCENARIO : データ無し
	int ActionIndex;
}
SaveData_t;

// シナリオ関連 --->

#define SAVEDATA_MAX 18
#define SAVEDATA_EMPTY_UITEXT "----/--/-- --:--:-- (未使用)"
#define SAVEDATA_EMPTY_SCENARIO "no-data"

typedef struct Kidoku_st
{
	char Scenario[2048];
	int HiestActionIndex; // まだ読んでいない最初のページ
}
Kidoku_t;

#define KIDOKU_MAX 2048

enum
{
	ROL_A,
	ROL_B,
	ROL_C,

	ROL_MAX, // == len
};

// <---

// ネットワーク関連 --->

#define NT_NAME_LENMAX 15

typedef struct NtTomodachiInfo_st
{
	char Name[NT_NAME_LENMAX + 1];
	uchar IP[4];
	int Port;
}
NtTomodachiInfo_t;

#define NT_TOMODACHI_NUM 25
//#define NT_TOMODACHI_NUM 30 // old

#define NT_DEF_NAME "unknown"
#define NT_DEF_IP1 127
#define NT_DEF_IP2 0
#define NT_DEF_IP3 0
#define NT_DEF_IP4 1
#define NT_DEF_PORT 59999

#define NT_MIN_PORT 1
#define NT_MAX_PORT 0xffff

// <---

typedef struct Settings_st
{
	int FullScreenMode;

	double BGMVolume; // 0.0 - 1.0
	double SeVolume;  // 0.0 - 1.0

	KeySettings_t KeySettInfos[2][2]; // [1P, 2P][KB, Pad]

	double MessSpeed; // 0.0 - 1.0
	double AutoSpeed; // 0.0 - 1.0

	int Midokumotobasu;

	SaveData_t SaveDataList[SAVEDATA_MAX];

	Kidoku_t KidokuList[KIDOKU_MAX];
	int KidokuCount;

	int RouteOpenList[ROL_MAX];
	int EndingWatchedList[ED_MAX];

	int NtInited;
	int NtRecvPort;
	char NtName[NT_NAME_LENMAX + 1];
	NtTomodachiInfo_t NtTomodachiList[NT_TOMODACHI_NUM];
	int NtKansenOk;

	double ScreenZoom; // 0.1 - 1.0(Def) - 10.0
}
Settings_t;

extern Settings_t SettInfo;

void InitSettings(void);
void LoadSettings(void);
void SaveSettings(void);

void Input2SettInfo(void);
void SettInfo2Input(void);

void Other2SettInfo(void);
void SettInfo2Other(void);

int GetKidokuHiestActionIndex(char *scnrTitle);
void SetKidokuHiestActionIndex(char *scnrTitle, int actionIndex);
