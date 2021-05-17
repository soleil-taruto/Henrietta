#include "StdInc.h"

NtCommonInfo_t NtCommonInfo[2]; // [Send, Recv]
NtSequence_t NtSequence[2]; // [Send, Recv]

/*
	常にサーバーが配信者になる。
	従って Pzdc.NetworkMode != 0 && Pzdc.NetKansenMode == 0 && NtCommonInfo[0].PlayerSide == 0 であるときのみ NtKansennshaCount == 0 ～ NT_KANSEN_MAX になる。
	それ以外の場合は NtKansenshaCount == 0 である。
*/
uchar NtKansenshaIPList[NT_KANSEN_MAX][4];
int NtKansenshaPortList[NT_KANSEN_MAX];
int NtKansenshaCount;

NtKansenRequest_t NtKansenRequest;
NtKansenOkRet_t NtKansenOkRet;
NtKansenSq_t NtKansenSq;

#define BLSQ_MAX (60 * 60 * 60 * 24)

static int *BackLogSq[2];

void NtProcInit(void)
{
	// noop
}
void NtProcFnlz(void)
{
	// noop
}

static void CheckEndian(void)
{
	int a = 1;
	errorCase(!*(uchar *)&a); // ? not little endian
}
void NtInit(int portno)
{
	/*
		同じサイズだとお互いを同じと見なす、、ような気がする。
	*/
	{
		int sizes[] = {
			sizeof(NtCommonInfo_t),
			sizeof(NtSequence_t),
			sizeof(NtKansenRequest_t),
			sizeof(NtKansenOkRet_t),
			sizeof(NtKansenSq_t),
		};
		int i;
		int j;

		for(i =     0; i < lengthof(sizes); i++)
		for(j = i + 1; j < lengthof(sizes); j++)
		{
			errorCase(sizes[i] == sizes[j]);
		}
	}

	CheckEndian();

	memset(NtCommonInfo + 0, 0x00, sizeof(NtCommonInfo_t));
	memset(NtCommonInfo + 1, 0x00, sizeof(NtCommonInfo_t));
	memset(NtSequence + 0, 0x00, sizeof(NtSequence_t));
	memset(NtSequence + 1, 0x00, sizeof(NtSequence_t));

	NtCommonInfo[0].SockHandle = MakeUDPSocket(portno);
	errorCase(NtCommonInfo[0].SockHandle == -1);

	NtKansenshaCount = 0;

	memset(&NtKansenRequest, 0x00, sizeof(NtKansenRequest_t));
	memset(&NtKansenOkRet, 0x00, sizeof(NtKansenOkRet_t));
	memset(&NtKansenSq, 0x00, sizeof(NtKansenSq_t));
}
void NtFnlz(void)
{
	NtUDPRecvBuffClear();
	NtKansenshaCount = 0;

	memFree(BackLogSq[0]);
	memFree(BackLogSq[1]);
	BackLogSq[0] = NULL;
	BackLogSq[1] = NULL;

	DeleteUDPSocket(NtCommonInfo[0].SockHandle);
}

#define UDPBLOCK_SIZEMAX 66000
#define CRED_SIZE 4
#define CRC_SIZE 4
#define UDPEXTEND_SIZE (CRED_SIZE + CRC_SIZE)

static uchar UDPBlockTmp[UDPBLOCK_SIZEMAX + UDPEXTEND_SIZE];
uchar NtUDPRecvIP[4];
int NtUDPRecvCountMax;
int NtLastUDPRecvCount; // == 0 : 無限

static void PutExtend(uchar *block, int blockSize)
{
	block[blockSize + 0] = 0xff & NtCommonInfo[0].Credentials / 0x1000000;
	block[blockSize + 1] = 0xff & NtCommonInfo[0].Credentials / 0x10000;
	block[blockSize + 2] = 0xff & NtCommonInfo[0].Credentials / 0x100;
	block[blockSize + 3] = 0xff & NtCommonInfo[0].Credentials / 0x1;

	PutCRC(block, blockSize + CRED_SIZE);
}
static int CheckExtend(uchar *block, int blockSize)
{
	if(CheckCRC(block, blockSize + CRED_SIZE))
	{
		if(NtCommonInfo[0].OSCredentials != -1) // ? OSC 有効
		{
			return
				block[blockSize + 0] == (0xff & NtCommonInfo[0].OSCredentials / 0x1000000) &&
				block[blockSize + 1] == (0xff & NtCommonInfo[0].OSCredentials / 0x10000) &&
				block[blockSize + 2] == (0xff & NtCommonInfo[0].OSCredentials / 0x100) &&
				block[blockSize + 3] == (0xff & NtCommonInfo[0].OSCredentials / 0x1); // ? Recv-CRED == OSC
		}
		return 1;
	}
	return 0;
}

static int P_SendUDP(int handle, IPDATA ip, int port, void *block, int size)
{
	int retval = NetWorkSendUDP(handle, ip, port, block, size);

//	LogWrite_x(xcout("SendUDP: %d [%d.%d.%d.%d] %d %d -> %d", handle, ip.d1, ip.d2, ip.d3, ip.d4, port, size, retval)); // test
//	LogWrite("SendUDP_Block", block, size); // test

	return retval;
}
static int P_RecvUDP(int handle, IPDATA &ip, int &port, void *block, int size)
{
	int retval = NetWorkRecvUDP(handle, &ip, &port, block, size, 0);

//	LogWrite_x(xcout("RecvUDP: %d [%d.%d.%d.%d] %d %d -> %d", handle, ip.d1, ip.d2, ip.d3, ip.d4, port, size, retval)); // test

//	if(0 <= retval)
//		LogWrite("RecvUDP_Block", block, retval); // test

	return retval;
}
static void UDPSendBlock(void *block, int blockSize)
{
	IPDATA ip;

	errorCase(block == NULL);
	errorCase(blockSize < 1); // ゼロバイト送ることないだろ、、、
	errorCase(UDPBLOCK_SIZEMAX < blockSize);

	memcpy(UDPBlockTmp, block, blockSize);
	PutExtend(UDPBlockTmp, blockSize);

	ip.d1 = NtCommonInfo[0].SendIP[0];
	ip.d2 = NtCommonInfo[0].SendIP[1];
	ip.d3 = NtCommonInfo[0].SendIP[2];
	ip.d4 = NtCommonInfo[0].SendIP[3];

	int retval = P_SendUDP(NtCommonInfo[0].SockHandle, ip, NtCommonInfo[0].SendPort, UDPBlockTmp, blockSize + UDPEXTEND_SIZE);

	ExtraErrorInfo.RetVal = retval;
	errorCase(retval < 0);
}
static int UDPRecvBlock(void *block, int blockSize) // ret : ? 受信した。
{
	IPDATA ip;
	int port;
	int retval;
	int recved = 0;

	errorCase(block == NULL);
	errorCase(blockSize < 1); // ゼロバイト受信することないだろ、、、
	errorCase(UDPBLOCK_SIZEMAX < blockSize);

	NtLastUDPRecvCount = 0;

	for(int lpcnt = 0; lpcnt < 99990000; lpcnt++) // HACK: 上限適当。低いと問題アリ、そもそも上限を決めることに意味があるのか？
	{
//LogWrite("lpcnt", lpcnt); // カウンタが上がり続けるようなら NetWorkRecvUDP() の最後のオプションのバグを疑え！

		retval = CheckNetWorkRecvUDP(NtCommonInfo[0].SockHandle);
		errorCase(retval < 0);

		if(retval == 0) // ? もう無い
		{
			break;
		}
		retval = P_RecvUDP(NtCommonInfo[0].SockHandle, ip, port, UDPBlockTmp, UDPBLOCK_SIZEMAX + UDPEXTEND_SIZE);
//		retval = NetWorkRecvUDP(NtCommonInfo[0].SockHandle, &ip, &port, UDPBlockTmp, UDPBLOCK_SIZEMAX + UDPEXTEND_SIZE, 0); // Peek == 0 で消去する。マニュアルと逆！
//		retval = NetWorkRecvUDP(NtCommonInfo[0].SockHandle, &ip, &port, UDPBlockTmp, UDPBLOCK_SIZEMAX + UDPEXTEND_SIZE, 1); // 直った。-> 3.07aでデグレってやがる。

		ExtraErrorInfo.RetVal = retval;
//		errorCase(retval < 0); // エラーは時々起こる！？ ... 多分、関係の無い UDP パケットが飛んでくる。

		if(retval == blockSize + UDPEXTEND_SIZE && CheckExtend(UDPBlockTmp, blockSize)) // ? 成功
		{
			NtUDPRecvIP[0] = ip.d1;
			NtUDPRecvIP[1] = ip.d2;
			NtUDPRecvIP[2] = ip.d3;
			NtUDPRecvIP[3] = ip.d4;

			memcpy(block, UDPBlockTmp, blockSize); // 受信データを反映

			NtLastUDPRecvCount++;
			recved = 1;

			if(NtUDPRecvCountMax && NtUDPRecvCountMax <= NtLastUDPRecvCount)
			{
				break;
			}
		}
	}
	return recved;
}
void NtUDPRecvBuffClear(void)
{
	char dummy;

	NtUDPRecvCountMax = 0; // 2bs

	UDPRecvBlock(&dummy, 1);
}

static int IPPortEscaped;
static uchar EscIP[4];
static int EscPort;

void NtEscapeSendIPPort(void)
{
	errorCase(IPPortEscaped);

	memcpy(EscIP, NtCommonInfo[0].SendIP, 4);
	EscPort = NtCommonInfo[0].SendPort;

	IPPortEscaped = 1;
}
void NtUnescapeSendIPPort(void)
{
	errorCase(!IPPortEscaped);

	memcpy(NtCommonInfo[0].SendIP, EscIP, 4);
	NtCommonInfo[0].SendPort = EscPort;

	IPPortEscaped = 0;
}
void NtSetSendIPPort(uchar ip[4], int port)
{
	memcpy(NtCommonInfo[0].SendIP, ip, 4);
	NtCommonInfo[0].SendPort = port;
}

void NtSendCommonInfo(void)
{
	UDPSendBlock(NtCommonInfo + 0, sizeof(NtCommonInfo_t));
}
static void AdjustCommonInfo(NtCommonInfo_t *i)
{
	m_range(i->SelfSide, 0, 1);

	line2domain_len(i->BuiltStamp, BUILT_STAMP_LEN);
	line2domain_len(i->Name, NT_NAME_LENMAX);

	if(!i->Name)
		strcpy(i->Name, NT_DEF_NAME);

	m_range(i->Port, NT_MIN_PORT, NT_MAX_PORT);
	m_range(i->SendPort, NT_MIN_PORT, NT_MAX_PORT);

	m_range(i->Chara, 0, CORRECT_PAIR_MAX - 1);
	m_range(i->Basho, 0, BASHO_MAX);

	line2domain_len(i->TokushuButtle.Title, TB_TITLE_LENMAX);
}
void NtRecvCommonInfo(void)
{
	if(UDPRecvBlock(NtCommonInfo + 1, sizeof(NtCommonInfo_t)))
	{
		AdjustCommonInfo(NtCommonInfo + 1);
	}
}

void NtSendSequence(void)
{
	UDPSendBlock(NtSequence + 0, sizeof(NtSequence_t));
}
static void AdjustSequence(NtSequence_t *i)
{
	// noop
}
void NtRecvSequence(void)
{
	if(UDPRecvBlock(NtSequence + 1, sizeof(NtSequence_t)))
	{
		AdjustSequence(NtSequence + 1);
	}
}

#define ABS2REL_FRMIDX(absFrame, sqFrame) \
	((sqFrame) - (absFrame))

#define REL2ABS_FRMIDX(sqFrame, sqIndex) \
	((sqFrame) - (sqIndex))

void NtSetCommand(__int64 absFrame, int command)
{
	NtSequence_t *i = NtSequence + 0;

	errorCase(absFrame < 0);
	errorCase(i->SqFrame < 0);

//	errorCase(BORDER_OF_NUMERIC < absFrame); // 2bs
//	errorCase(BORDER_OF_NUMERIC < i->SqFrame); // 2bs

	int sqIndex = (int)ABS2REL_FRMIDX(absFrame, i->SqFrame);

	if(sqIndex < 0)
	{
		for(int widx = NT_SQ_LEN - 1; 0 < widx; widx--)
		{
			int ridx = widx + sqIndex;
			int cmd = 0;

			if(0 <= ridx)
			{
				cmd = i->CmdSq[ridx];
			}
			i->CmdSq[widx] = cmd;
		}
		i->CmdSq[0] = command;

		i->SqFrame = absFrame;
	}
	else
	{
		errorCase(NT_SQ_LEN <= sqIndex); // 古過ぎる。

		i->CmdSq[sqIndex] = command;
	}
}
static int GetCommand(int dataIndex, __int64 absFrame)
{
	NtSequence_t *i = NtSequence + dataIndex;

	errorCase(absFrame < 0);
//	errorCase(BORDER_OF_NUMERIC < absFrame); // 2bs

	int sqIndex = (int)ABS2REL_FRMIDX(absFrame, i->SqFrame);

	if(sqIndex < 0 || NT_SQ_LEN <= sqIndex) // ? 範囲外
	{
		return 1 << NT_CMDBIT_ERROR;
	}
	return i->CmdSq[sqIndex];
}
int NtGetCommand(__int64 absFrame)
{
	return GetCommand(0, absFrame);
}
int NtGetRecvCommand(__int64 absFrame)
{
	return GetCommand(1, absFrame);
}

/*
	ここから下で USPSend(Recv)Block() する場合、

	送信側は SendIP, SendPort を設定する。
	受信側は OSCredentails を無効 (-1) にすることで、送信者が Credentails を偽装する必要はなくなる。
*/

// 観戦モード用 ...

void NtSendKansenRequest(void)
{
	UDPSendBlock(&NtKansenRequest, sizeof(NtKansenRequest_t));
}
static void AdjustKansenRequest(NtKansenRequest_t *i)
{
	line2domain_len(i->BuiltStamp, BUILT_STAMP_LEN);
	m_range(i->Port, NT_MIN_PORT, NT_MAX_PORT);
}
void NtRecvKansenRequest(void)
{
	if(UDPRecvBlock(&NtKansenRequest, sizeof(NtKansenRequest_t)))
	{
		AdjustKansenRequest(&NtKansenRequest);
	}
}

void NtSendKansenOkRet(void)
{
	UDPSendBlock(&NtKansenOkRet, sizeof(NtKansenOkRet_t));
}
static void AdjustKansenOkRet(NtKansenOkRet_t *i)
{
	AdjustCommonInfo(i->Info + 0);
	AdjustCommonInfo(i->Info + 1);
}
void NtRecvKansenOkRet(void)
{
	if(UDPRecvBlock(&NtKansenOkRet, sizeof(NtKansenOkRet_t)))
	{
		AdjustKansenOkRet(&NtKansenOkRet);
	}
}

void NtSendKansenSq(void)
{
	UDPSendBlock(&NtKansenSq, sizeof(NtKansenSq_t));
}
static void AdjustKansenSq(NtKansenSq_t *i)
{
	AdjustSequence(i->Sq + 0);
	AdjustSequence(i->Sq + 1);
}
void NtRecvKansenSq(void)
{
	if(UDPRecvBlock(&NtKansenSq, sizeof(NtKansenSq_t)))
	{
		AdjustKansenSq(&NtKansenSq);
	}
}
void NtHaishin(void)
{
	NtEscapeSendIPPort();

	for(int i = 0; i < NtKansenshaCount; i++)
	{
		NtSetSendIPPort(NtKansenshaIPList[i], NtKansenshaPortList[i]);
		NtSendKansenSq();
	}
	NtUnescapeSendIPPort();
}

void NtPutBackLogSq(int mapIndex, NtSequence_t *i)
{
	errorCase(mapIndex < 0 || 1 < mapIndex);
	errorCase(i == NULL);

	if(!BackLogSq[0]) // ? not inited -> init
	{
		BackLogSq[0] = (int *)memAlloc(BLSQ_MAX * sizeof(int));
		BackLogSq[1] = (int *)memAlloc(BLSQ_MAX * sizeof(int));

		for(int frmidx = 0; frmidx < BLSQ_MAX; frmidx++)
		{
			BackLogSq[0][frmidx] = 1 << NT_CMDBIT_ERROR;
			BackLogSq[1][frmidx] = 1 << NT_CMDBIT_ERROR;
		}
	}

	for(int sqidx = 0; sqidx < NT_SQ_LEN; sqidx++)
	{
		__int64 absFrame = REL2ABS_FRMIDX(i->SqFrame, sqidx);

		if(0 <= absFrame && absFrame < BLSQ_MAX) // ? not out-of-range
		{
			BackLogSq[mapIndex][(int)absFrame] = i->CmdSq[sqidx];
		}
	}
}
int NtGetBackLogSq(int mapIndex, __int64 absFrame)
{
	errorCase(mapIndex < 0 || 1 < mapIndex);
	errorCase(absFrame < 0);

	if(BackLogSq[mapIndex]) // ? inited
	{
		if(0 <= absFrame && absFrame < BLSQ_MAX) // ? not out-of-range
		{
			return BackLogSq[mapIndex][(int)absFrame];
		}
	}
	return 1 << NT_CMDBIT_ERROR;
}
