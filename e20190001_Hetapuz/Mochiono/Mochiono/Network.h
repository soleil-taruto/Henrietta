// ----

#define NT_DELAY 3 // dealy frame cnt, 1 ～

// ----

typedef struct NtCommonInfo_st
{
	int SockHandle;

	int Syn;
	int AckSyn;
	int Ack;

	/*
		0 : 1P, 左, サーバー側
		1 : 2P, 右, クライアント側
	*/
	int SelfSide; // 0 or 1

	char BuiltStamp[BUILT_STAMP_LEN + 1];
	int Seed; // サーバー優先

	// Session-Check
	int Credentials;
	int OSCredentials; // -1 == 未定義 no-check

	// 自分側
	char Name[NT_NAME_LENMAX + 1];
	uchar IP[4];
	int Port;

	// 接続先
	uchar SendIP[4];
	int SendPort;

	int Chara;
	int Basho; // サーバー優先

	TokushuButtle_t TokushuButtle;
	int TBEnable;
}
NtCommonInfo_t;

enum
{
	NT_CMDBIT_MOV_L,
	NT_CMDBIT_MOV_R,

	NT_CMDBIT_ROT_L,
	NT_CMDBIT_ROT_R,

	NT_CMDBIT_HISPEED,
	NT_CMDBIT_BOMB,

	NT_CMDBIT_WIN,
	NT_CMDBIT_LOSE,

	NT_CMDBIT_ERROR,
};

#define NT_SQ_LEN 20

typedef struct NtSequence_st
{
	int Syn;

	/*
		[0] ～ [NT_SQ_LEN - 1]
		新 ～ 旧
	*/
	int CmdSq[NT_SQ_LEN];
	__int64 SqFrame; // frame of CmdSq[0], 0 ～
}
NtSequence_t;

extern NtCommonInfo_t NtCommonInfo[2];
extern NtSequence_t NtSequence[2];

#define NT_KANSEN_MAX 30

extern uchar NtKansenshaIPList[NT_KANSEN_MAX][4];
extern int NtKansenshaPortList[NT_KANSEN_MAX];
extern int NtKansenshaCount;

typedef struct NtKansenRequest_st
{
	char BuiltStamp[BUILT_STAMP_LEN + 1];
	int Port;
}
NtKansenRequest_t;

typedef struct NtKansenOkRet_st
{
	int Ok;
	NtCommonInfo_t Info[2];
}
NtKansenOkRet_t;

typedef struct NtKansenSq_st
{
	int Syn;
	NtSequence_t Sq[2];
	int EndSyn;
}
NtKansenSq_t;

extern NtKansenRequest_t NtKansenRequest;
extern NtKansenOkRet_t NtKansenOkRet;
extern NtKansenSq_t NtKansenSq;

void NtProcInit(void);
void NtProcFnlz(void);

void NtInit(int portno);
void NtFnlz(void);

extern uchar NtUDPRecvIP[4];
extern int NtUDPRecvCountMax;
extern int NtLastUDPRecvCount;

void NtEscapeSendIPPort(void);
void NtUnescapeSendIPPort(void);
void NtSetSendIPPort(uchar ip[4], int port);

void NtUDPRecvBuffClear(void);

void NtSendCommonInfo(void);
void NtRecvCommonInfo(void);

void NtSendSequence(void);
void NtRecvSequence(void);

void NtSetCommand(__int64 absFrame, int command);
int NtGetCommand(__int64 absFrame);
int NtGetRecvCommand(__int64 absFrame);

void NtSendKansenRequest(void);
void NtRecvKansenRequest(void);

void NtSendKansenOkRet(void);
void NtRecvKansenOkRet(void);

void NtSendKansenSq(void);
void NtRecvKansenSq(void);
void NtHaishin(void);

void NtPutBackLogSq(int mapIndex, NtSequence_t *i);
int NtGetBackLogSq(int mapIndex, __int64 absFrame);
