#include "StdInc.h"

#define VAL_SIGNATURE 0x6cca

#define STRFILE_SAVEDATA "SaveData.dat"
#define LINE_LENMAX 1024

static FILE *SdFp;
static int ForWrite;

static void CloseSaveData(void);
static void OpenSaveData(int forWrite)
{
	CloseSaveData(); // 2bs

	SdFp = fopen(STRFILE_SAVEDATA, forWrite ? "wt" : "rt");
	errorCase(!SdFp);

	ForWrite = forWrite;
}
static void CloseSaveData(void)
{
	if(SdFp)
	{
		fclose(SdFp);
		SdFp = NULL;
	}
}
static char *ReadLine(void)
{
	static char line[LINE_LENMAX + 1];
	int index;

	errorCase(!SdFp);
	errorCase(ForWrite);

	for(index = 0; index < LINE_LENMAX; index++)
	{
		int chr = fgetc(SdFp);

		if(chr == '\n' || chr == EOF)
		{
			break;
		}
		line[index] = chr;
	}
	line[index] = '\0';
	return line;
}
static char *ReadLine_LenMax(int lenmax)
{
	char *line = ReadLine();

	errorCase(lenmax < 0);
	errorCase(LINE_LENMAX < lenmax);

	line[lenmax] = '\0';
	return line;
}
static __int64 Line2Int64(char *line, __int64 minval, __int64 maxval)
{
	__int64 value = 0;
	int vsign = 1;
	int index;

	for(index = 0; line[index]; index++)
	{
		int chr = line[index];

		if(chr == '-')
		{
			vsign = -1;
		}
		if('0' <= chr && chr <= '9')
		{
			value *= 10;
			value += chr - '0';
		}
	}
	value *= vsign;

	m_range(value, minval, maxval);
	return value;
}
static __int64 ReadInt64(__int64 minval, __int64 maxval)
{
	return Line2Int64(ReadLine(), minval, maxval);
}
static int ReadInt(int minval, int maxval)
{
	return (int)ReadInt64((__int64)minval, (__int64)maxval);
}
static void WriteLine(char *line)
{
	fprintf(SdFp, "%s\n", line);
}
static void WriteInt64(__int64 value)
{
if(!SdFp) return; // HACK

	errorCase(!SdFp);
	errorCase(!ForWrite);

	fprintf(SdFp, "%I64d\n", value);
}
static void WriteInt(int value)
{
	WriteInt64((__int64)value);
}
static double ReadDouble(double seido, double minval, double maxval) // 0.0 < seido <= 1.0
{
	errorCase(seido <= 0.0 || 1.0 < seido);

	double value = (double)ReadInt64(-9220000000000000000, 9220000000000000000) * seido;

	m_range(value, minval, maxval);
	return value;
}
static void WriteDouble(double value, double seido) // 0.0 < seido <= 1.0
{
	errorCase(seido <= 0.0 || 1.0 < seido);

	WriteInt64((__int64)(value / seido));
}

Settings_t SettInfo;

void InitSettings(void)
{
	memset(&SettInfo, 0x00, sizeof(Settings_t));

	SettInfo.BGMVolume = 0.45;
	SettInfo.SeVolume  = 0.45;

	SettInfo.MessSpeed = 0.2;
	SettInfo.AutoSpeed = 0.2;

	for(int index = 0; index < SAVEDATA_MAX; index++)
	{
		strcpy(SettInfo.SaveDataList[index].UIText,   SAVEDATA_EMPTY_UITEXT);
		strcpy(SettInfo.SaveDataList[index].Scenario, SAVEDATA_EMPTY_SCENARIO);
	}
	InitInput();
	Input2SettInfo();

	SettInfo.RouteOpenList[ROL_A] = 1; // 最初から開通
	SettInfo.RouteOpenList[ROL_B] = 0; // ROL_A クリアで開通
	SettInfo.RouteOpenList[ROL_C] = 0; // ROL_B クリアで開通

	SettInfo.ScreenZoom = 1.0;
}
void LoadSettings(void)
{
	InitSettings();

	if(!_access(STRFILE_SAVEDATA, 0)) // ? セーブデータあり
	{
		OpenSaveData(0);

		if(ReadInt(0, 0x7fffffff) != VAL_SIGNATURE) // ? セーブデータ初期状態 (無い状態と同じ)
			goto noSaveData;

		SettInfo.FullScreenMode = ReadInt(0, 1);
		SettInfo.BGMVolume = ReadDouble(0.0001, 0.0, 1.0);
		SettInfo.SeVolume  = ReadDouble(0.0001, 0.0, 1.0);

		for(int pSide = 0; pSide < 2; pSide++)
		for(int kp = 0; kp < 2; kp++)
		{
			SettInfo.KeySettInfos[pSide][kp].RotL  = ReadInt(0, 0x7fffffff);
			SettInfo.KeySettInfos[pSide][kp].RotR  = ReadInt(0, 0x7fffffff);
			SettInfo.KeySettInfos[pSide][kp].Dir2  = ReadInt(0, 0x7fffffff);
			SettInfo.KeySettInfos[pSide][kp].Dir4  = ReadInt(0, 0x7fffffff);
			SettInfo.KeySettInfos[pSide][kp].Dir6  = ReadInt(0, 0x7fffffff);
			SettInfo.KeySettInfos[pSide][kp].Dir8  = ReadInt(0, 0x7fffffff);
			SettInfo.KeySettInfos[pSide][kp].Bomb  = ReadInt(0, 0x7fffffff);
			SettInfo.KeySettInfos[pSide][kp].Pause = ReadInt(0, 0x7fffffff);
		}

		SettInfo.MessSpeed = ReadDouble(0.0001, 0.0, 1.0);
		SettInfo.AutoSpeed = ReadDouble(0.0001, 0.0, 1.0);
		SettInfo.Midokumotobasu = ReadInt(0, 1);

		for(int index = 0; index < SAVEDATA_MAX; index++)
		{
			strcpy(SettInfo.SaveDataList[index].UIText, ReadLine());
			strcpy(SettInfo.SaveDataList[index].Scenario, ReadLine());
			SettInfo.SaveDataList[index].ActionIndex = ReadInt(0, BORDER_OF_NUMERIC);
		}
		SettInfo.KidokuCount = ReadInt(0, KIDOKU_MAX);

		for(int index = 0; index < SettInfo.KidokuCount; index++)
		{
			char *line = ReadLine();

			if(!*line) // ? == ""
				line = "ERROR:Scenario-Title";

			strcpy(SettInfo.KidokuList[index].Scenario, line);
			SettInfo.KidokuList[index].HiestActionIndex = ReadInt(0, BORDER_OF_NUMERIC);
		}

		SettInfo.RouteOpenList[ROL_A] = ReadInt(0, 1);
		SettInfo.RouteOpenList[ROL_B] = ReadInt(0, 1);
		SettInfo.RouteOpenList[ROL_C] = ReadInt(0, 1);

		for(int index = 0; index < ED_MAX; index++)
		{
			SettInfo.EndingWatchedList[index] = ReadInt(0, 1);
		}

		SettInfo.NtInited = ReadInt(0, 1);
		SettInfo.NtRecvPort = ReadInt(NT_MIN_PORT, NT_MAX_PORT);
		strcpy(SettInfo.NtName, ReadLine_LenMax(NT_NAME_LENMAX));

		for(int index = 0; index < NT_TOMODACHI_NUM; index++)
		{
			NtTomodachiInfo_t *dachi = SettInfo.NtTomodachiList + index;

			strcpy(dachi->Name, ReadLine_LenMax(NT_NAME_LENMAX));
			dachi->IP[0] = ReadInt(0, 255);
			dachi->IP[1] = ReadInt(0, 255);
			dachi->IP[2] = ReadInt(0, 255);
			dachi->IP[3] = ReadInt(0, 255);
			dachi->Port = ReadInt(NT_MIN_PORT, NT_MAX_PORT);
		}
		SettInfo.NtKansenOk = ReadInt(0, 1);

		SettInfo.ScreenZoom = ReadDouble(0.01, 0.1, 10.0);
		CloseSaveData();
	}
	else // セーブデータなし
	{
noSaveData:
		1;
//		InitSettings();
	}

	if(!SettInfo.NtInited) {
		SettInfo.NtInited = 1;
		SettInfo.NtRecvPort = NT_DEF_PORT;
		strcpy(SettInfo.NtName, NT_DEF_NAME);

		for(int index = 0; index < NT_TOMODACHI_NUM; index++)
		{
			NtTomodachiInfo_t *dachi = SettInfo.NtTomodachiList + index;

			strcpy(dachi->Name, NT_DEF_NAME);
			dachi->IP[0] = NT_DEF_IP1;
			dachi->IP[1] = NT_DEF_IP2;
			dachi->IP[2] = NT_DEF_IP3;
			dachi->IP[3] = NT_DEF_IP4;
			dachi->Port = NT_DEF_PORT;
		}
	}

	SettInfo2Other();
}
void SaveSettings(void)
{
	Other2SettInfo();
	OpenSaveData(1);

	WriteInt(VAL_SIGNATURE);

	WriteInt(SettInfo.FullScreenMode);
	WriteDouble(SettInfo.BGMVolume, 0.0001);
	WriteDouble(SettInfo.SeVolume,  0.0001);

	for(int pSide = 0; pSide < 2; pSide++)
	for(int kp = 0; kp < 2; kp++)
	{
		WriteInt(SettInfo.KeySettInfos[pSide][kp].RotL);
		WriteInt(SettInfo.KeySettInfos[pSide][kp].RotR);
		WriteInt(SettInfo.KeySettInfos[pSide][kp].Dir2);
		WriteInt(SettInfo.KeySettInfos[pSide][kp].Dir4);
		WriteInt(SettInfo.KeySettInfos[pSide][kp].Dir6);
		WriteInt(SettInfo.KeySettInfos[pSide][kp].Dir8);
		WriteInt(SettInfo.KeySettInfos[pSide][kp].Bomb);
		WriteInt(SettInfo.KeySettInfos[pSide][kp].Pause);
	}

	WriteDouble(SettInfo.MessSpeed, 0.0001);
	WriteDouble(SettInfo.AutoSpeed, 0.0001);
	WriteInt(SettInfo.Midokumotobasu);

	for(int index = 0; index < SAVEDATA_MAX; index++)
	{
		WriteLine(SettInfo.SaveDataList[index].UIText);
		WriteLine(SettInfo.SaveDataList[index].Scenario);
		WriteInt(SettInfo.SaveDataList[index].ActionIndex);
	}
	WriteInt(SettInfo.KidokuCount);

	for(int index = 0; index < SettInfo.KidokuCount; index++)
	{
		WriteLine(SettInfo.KidokuList[index].Scenario);
		WriteInt(SettInfo.KidokuList[index].HiestActionIndex);
	}

	WriteInt(SettInfo.RouteOpenList[ROL_A] ? 1 : 0);
	WriteInt(SettInfo.RouteOpenList[ROL_B] ? 1 : 0);
	WriteInt(SettInfo.RouteOpenList[ROL_C] ? 1 : 0);

	for(int index = 0; index < ED_MAX; index++)
	{
		WriteInt(SettInfo.EndingWatchedList[index] ? 1 : 0);
	}

	WriteInt(SettInfo.NtInited ? 1 : 0);
	WriteInt(SettInfo.NtRecvPort);
	WriteLine(SettInfo.NtName);

	for(int index = 0; index < NT_TOMODACHI_NUM; index++)
	{
		NtTomodachiInfo_t *dachi = SettInfo.NtTomodachiList + index;

		WriteLine(dachi->Name);
		WriteInt(dachi->IP[0]);
		WriteInt(dachi->IP[1]);
		WriteInt(dachi->IP[2]);
		WriteInt(dachi->IP[3]);
		WriteInt(dachi->Port);
	}
	WriteInt(SettInfo.NtKansenOk ? 1 : 0);

	SettInfo.ScreenZoom = GetWindowSizeExtendRate(); // 2013.1.16 ADD
	WriteDouble(SettInfo.ScreenZoom, 0.01);
	CloseSaveData();
}

void Input2SettInfo(void)
{
	for(int pSide = 0; pSide < 2; pSide++)
	for(int kp = 0; kp < 2; kp++)
	{
		SettInfo.KeySettInfos[pSide][kp].Dir2  = GetMap(&(InputList[pSide][INP_DIR_2]), kp);
		SettInfo.KeySettInfos[pSide][kp].Dir4  = GetMap(&(InputList[pSide][INP_DIR_4]), kp);
		SettInfo.KeySettInfos[pSide][kp].Dir6  = GetMap(&(InputList[pSide][INP_DIR_6]), kp);
		SettInfo.KeySettInfos[pSide][kp].Dir8  = GetMap(&(InputList[pSide][INP_DIR_8]), kp);
		SettInfo.KeySettInfos[pSide][kp].RotL  = GetMap(&(InputList[pSide][INP_ROT_L]), kp);
		SettInfo.KeySettInfos[pSide][kp].RotR  = GetMap(&(InputList[pSide][INP_ROT_R]), kp);
//		SettInfo.KeySettInfos[pSide][kp].Bomb  = GetMap(&(InputList[pSide][INP_BOMB]),  kp);
		SettInfo.KeySettInfos[pSide][kp].Pause = GetMap(&(InputList[pSide][INP_START]), kp);
	}
}
void SettInfo2Input(void)
{
	for(int pSide = 0; pSide < 2; pSide++)
	for(int kp = 0; kp < 2; kp++)
	{
		SetMap(&(InputList[pSide][INP_DIR_2]), kp, SettInfo.KeySettInfos[pSide][kp].Dir2);
		SetMap(&(InputList[pSide][INP_DIR_4]), kp, SettInfo.KeySettInfos[pSide][kp].Dir4);
		SetMap(&(InputList[pSide][INP_DIR_6]), kp, SettInfo.KeySettInfos[pSide][kp].Dir6);
		SetMap(&(InputList[pSide][INP_DIR_8]), kp, SettInfo.KeySettInfos[pSide][kp].Dir8);
		SetMap(&(InputList[pSide][INP_ROT_L]), kp, SettInfo.KeySettInfos[pSide][kp].RotL);
		SetMap(&(InputList[pSide][INP_ROT_R]), kp, SettInfo.KeySettInfos[pSide][kp].RotR);
//		SetMap(&(InputList[pSide][INP_BOMB]),  kp, SettInfo.KeySettInfos[pSide][kp].Bomb);
		SetMap(&(InputList[pSide][INP_START]), kp, SettInfo.KeySettInfos[pSide][kp].Pause);
	}
}
void Other2SettInfo(void)
{
	Input2SettInfo();

	if(Dc == NULL) return;

	SettInfo.BGMVolume = Dc->BGMVolume;
	SettInfo.SeVolume  = Dc->SeVolume;
}
void SettInfo2Other(void)
{
	SettInfo2Input();

	if(Dc == NULL) return;

	Dc->BGMVolume = SettInfo.BGMVolume;
	Dc->SeVolume  = SettInfo.SeVolume;
}

int GetKidokuHiestActionIndex(char *scnrTitle)
{
	errorCase(!scnrTitle);
	errorCase(!*scnrTitle); // "" は無いはず。

	for(int index = 0; index < SettInfo.KidokuCount; index++)
	{
		Kidoku_t *i = (Kidoku_t *)SettInfo.KidokuList + index;

		if(!strcmp(i->Scenario, scnrTitle))
		{
			return i->HiestActionIndex;
		}
	}
	return 0; // 無いってことは未読
}
void SetKidokuHiestActionIndex(char *scnrTitle, int actionIndex)
{
	errorCase(!scnrTitle);
	errorCase(!*scnrTitle); // "" は無いはず。
	errorCase(actionIndex < 0);

	for(int index = 0; ; index++)
	{
		errorCase(KIDOKU_MAX <= index); // もう i は無い

		Kidoku_t *i = (Kidoku_t *)SettInfo.KidokuList + index;

		if(SettInfo.KidokuCount <= index) // i は最後の有効な要素の次
		{
			// -- 追加 --
			strcpy(i->Scenario, scnrTitle);
			i->HiestActionIndex = actionIndex;
			SettInfo.KidokuCount++;
			// ----

			break;
		}
		if(!strcmp(i->Scenario, scnrTitle)) // ? 発見
		{
			i->HiestActionIndex = actionIndex; // 更新
			break;
		}
	}
}
