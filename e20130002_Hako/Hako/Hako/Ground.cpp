#include "all.h"

Gnd_t Gnd;

static autoList<int> *ReadIntLines_D6(int etcId)
{
	autoList<uchar> *fileData = GetEtcRes()->GetHandle(etcId);
	autoList<char *> *lines = readLines(fileData);
	autoList<int> *values = new autoList<int>();

	for(int index = 0; index < lines->GetCount(); index++)
	{
		char *line = lines->GetElement(index);
		int value = atoi(line);

		errorCase(!m_isRange(value, 100000, 999999)); // ? 6桁ではない。

		values->AddElement(value);
	}
	delete fileData->Ecdysis(); // 空にする。
	releaseList(lines, (void (*)(char *))memFree);
	return values;
}
void Gnd_INIT(void)
{
	memset(&Gnd, 0x00, sizeof(Gnd_t));

	Gnd.EL = new taskList();
	Gnd.PrimaryPadId = -1;

	// SaveData -->

	Gnd.RealScreen_W = SCREEN_W;
	Gnd.RealScreen_H = SCREEN_H;

	Gnd.ScreenDraw_L = 0;
	Gnd.ScreenDraw_T = 0;
	Gnd.ScreenDraw_W = SCREEN_W;
	Gnd.ScreenDraw_H = SCREEN_H;

	Gnd.MusicVolume = 0.5;
	Gnd.SEVolume = 0.5;

	Gnd.PadBtnId.Dir_2 = SNWPB_DIR_2;
	Gnd.PadBtnId.Dir_4 = SNWPB_DIR_4;
	Gnd.PadBtnId.Dir_6 = SNWPB_DIR_6;
	Gnd.PadBtnId.Dir_8 = SNWPB_DIR_8;
	Gnd.PadBtnId.A = SNWPB_A1;
	Gnd.PadBtnId.B = SNWPB_B1;
	Gnd.PadBtnId.C = SNWPB_A2;
	Gnd.PadBtnId.D = SNWPB_B2;
	Gnd.PadBtnId.E = SNWPB_A3;
	Gnd.PadBtnId.F = SNWPB_B3;
	Gnd.PadBtnId.L = SNWPB_L;
	Gnd.PadBtnId.R = SNWPB_R;
	Gnd.PadBtnId.Pause = SNWPB_DSTART;
	Gnd.PadBtnId.Start = SNWPB_USTART;

	// app -->

	for(int index = 0; index < FLOOR_NUM; index++)
		Gnd.BestTimeList[index] = -1;

	Gnd.D6_NotPrimePower_101 = ReadIntLines_D6(ETC_NOT_PRIME_POWER_101);
	Gnd.D6_NotPrimePower_201 = ReadIntLines_D6(ETC_NOT_PRIME_POWER_201);
	Gnd.D6_NotPrimePower_301 = ReadIntLines_D6(ETC_NOT_PRIME_POWER_301);
	Gnd.D6_NotPrimePower_401 = ReadIntLines_D6(ETC_NOT_PRIME_POWER_401);
	Gnd.D6_NotPrimePower_501 = ReadIntLines_D6(ETC_NOT_PRIME_POWER_501);
	Gnd.D6_Prime             = ReadIntLines_D6(ETC_PRIME);
	Gnd.D6_PrimePower_101    = ReadIntLines_D6(ETC_PRIME_POWER_101);
	Gnd.D6_PrimePower_201    = ReadIntLines_D6(ETC_PRIME_POWER_201);
	Gnd.D6_PrimePower_301    = ReadIntLines_D6(ETC_PRIME_POWER_301);
	Gnd.D6_PrimePower_401    = ReadIntLines_D6(ETC_PRIME_POWER_401);
	Gnd.D6_PrimePower_501    = ReadIntLines_D6(ETC_PRIME_POWER_501);
}
void Gnd_FNLZ(void)
{
	delete Gnd.EL;
	ReleaseSubScreen(Gnd.MainScreen);

	// app -->

	delete Gnd.D6_NotPrimePower_101;
	delete Gnd.D6_NotPrimePower_201;
	delete Gnd.D6_NotPrimePower_301;
	delete Gnd.D6_NotPrimePower_401;
	delete Gnd.D6_NotPrimePower_501;
	delete Gnd.D6_Prime;
	delete Gnd.D6_PrimePower_101;
	delete Gnd.D6_PrimePower_201;
	delete Gnd.D6_PrimePower_301;
	delete Gnd.D6_PrimePower_401;
	delete Gnd.D6_PrimePower_501;
}

// ---- SaveData ----

#define SAVE_FILE "SaveData.dat"
#define SAVEDATA_SIGNATURE "Hako_SaveData_v20210309"

static autoList<uchar> *SaveData;
static int SDIndex;

static char *SD_ReadLine(void) // ret: bind
{
	static char *line;

	memFree(line);
	line = readLine(SaveData, SDIndex);

	if(!line)
		line = strx("");

	return line;
}
static int SD_ReadBoolean(void)
{
	return atoi(SD_ReadLine()) ? 1 : 0;
}
static int SD_ReadInt(int minval, int maxval, int defval = 0)
{
	char *line = SD_ReadLine();

	if(!*line)
		return defval;

	int value = atoi(line);
	m_range(value, minval, maxval);
	return value;
}
static double SD_ReadDouble(double minval, double maxval, int denom)
{
	errorCase(denom < 1);

	double value = (double)atoi(SD_ReadLine()) / denom;
	m_range(value, minval, maxval);
	return value;
}
static void SD_ReadBitList(bitList *dest)
{
	dest->Clear();

	char *line = SD_ReadLine();

	for(int index = 0; line[index]; index++)
	{
		switch(line[index])
		{
		case '0':
//			dest->PutBit(index, 0); // 不要
			break;

		case '1':
			dest->PutBit(index, 1);
			break;

		default:
			error();
		}
	}
}

static void SD_WriteLine(char *line)
{
	writeLine(SaveData, line);
}
static void SD_WriteLine_x(char *line)
{
	SD_WriteLine(line);
	memFree(line);
}
static void SD_WriteInt(int value)
{
	SD_WriteLine_x(xcout("%d", value));
}
static void SD_WriteDouble(double value, int denom)
{
	SD_WriteInt(m_d2i(value * denom));
}
static void SD_WriteBitList(bitList *src)
{
	autoList<char> *buff = new autoList<char>();

	for(int index = 0; index < src->GetList_DIRECT()->GetCount() * (int)sizeof(uint) * CHAR_BIT; index++) // HACK: 最終ビット位置を返すメソッド
	{
		buff->AddElement(src->RefBit(index) ? '1' : '0');
	}
	buff->AddElement('\0');
	SD_WriteLine_x(buff->UnbindBuffer());
	delete buff;
}

static void ResolveOverlapPadBtn(void)
{
	int *pPadBtns[] =
	{
		&Gnd.PadBtnId.Dir_2,
		&Gnd.PadBtnId.Dir_4,
		&Gnd.PadBtnId.Dir_6,
		&Gnd.PadBtnId.Dir_8,
		&Gnd.PadBtnId.A,
		&Gnd.PadBtnId.B,
		&Gnd.PadBtnId.C,
		&Gnd.PadBtnId.D,
		&Gnd.PadBtnId.E,
		&Gnd.PadBtnId.F,
		&Gnd.PadBtnId.L,
		&Gnd.PadBtnId.R,
		&Gnd.PadBtnId.Pause,
		&Gnd.PadBtnId.Start,
	};

	for(int index = 1; index < lengthof(pPadBtns); index++)
	{
recheck:
		if(*pPadBtns[index] != -1) // ? 割り当てアリ
		{
			for(int b = 0; b < index; b++)
			{
				if(*pPadBtns[b] == *pPadBtns[index])
				{
					*pPadBtns[index] = (*pPadBtns[index] + 1) % PAD_BUTTON_MAX;
					goto recheck;
				}
			}
		}
	}
}
void ImportSaveData(void)
{
	if(!accessible(SAVE_FILE))
		return;

	SaveData = readFile(SAVE_FILE);

#if LOG_ENABLED == 0
	if(!Jammer(SaveData, 0)) // ? セーブデータ破損
	{
		delete SaveData;
		return;
	}
#endif
	SDIndex = 0;

	if(strcmp(SD_ReadLine(), SAVEDATA_SIGNATURE) != 0) // ? セーブデータ破損
	{
		LOGPOS();
		delete SaveData;
		return;
	}

	// Item {

	Gnd.RealScreen_W = SD_ReadInt(SCREEN_W, SCREEN_W_MAX);
	Gnd.RealScreen_H = SD_ReadInt(SCREEN_H, SCREEN_H_MAX);

	Gnd.ScreenDraw_L = SD_ReadInt(0, SCREEN_W_MAX / 2);
	Gnd.ScreenDraw_T = SD_ReadInt(0, SCREEN_H_MAX / 2);
	Gnd.ScreenDraw_W = SD_ReadInt(SCREEN_H, SCREEN_W_MAX);
	Gnd.ScreenDraw_H = SD_ReadInt(SCREEN_H, SCREEN_H_MAX);

	Gnd.MusicVolume = SD_ReadDouble(0.0, 1.0, IMAX);
	Gnd.SEVolume = SD_ReadDouble(0.0, 1.0, IMAX);

	Gnd.PadBtnId.Dir_2 = SD_ReadInt(-1, PAD_BUTTON_MAX - 1);
	Gnd.PadBtnId.Dir_4 = SD_ReadInt(-1, PAD_BUTTON_MAX - 1);
	Gnd.PadBtnId.Dir_6 = SD_ReadInt(-1, PAD_BUTTON_MAX - 1);
	Gnd.PadBtnId.Dir_8 = SD_ReadInt(-1, PAD_BUTTON_MAX - 1);
	Gnd.PadBtnId.A = SD_ReadInt(-1, PAD_BUTTON_MAX - 1);
	Gnd.PadBtnId.B = SD_ReadInt(-1, PAD_BUTTON_MAX - 1);
	Gnd.PadBtnId.C = SD_ReadInt(-1, PAD_BUTTON_MAX - 1);
	Gnd.PadBtnId.D = SD_ReadInt(-1, PAD_BUTTON_MAX - 1);
	Gnd.PadBtnId.E = SD_ReadInt(-1, PAD_BUTTON_MAX - 1);
	Gnd.PadBtnId.F = SD_ReadInt(-1, PAD_BUTTON_MAX - 1);
	Gnd.PadBtnId.L = SD_ReadInt(-1, PAD_BUTTON_MAX - 1);
	Gnd.PadBtnId.R = SD_ReadInt(-1, PAD_BUTTON_MAX - 1);
	Gnd.PadBtnId.Pause = SD_ReadInt(-1, PAD_BUTTON_MAX - 1);
	Gnd.PadBtnId.Start = SD_ReadInt(-1, PAD_BUTTON_MAX - 1);

	// app -->

	Gnd.UnclearedFloorIndex = SD_ReadInt(0, FLOOR_NUM);
	Gnd.GraphicalMode = SD_ReadInt(0, GM_MAX - 1);
	Gnd.RecordingMode = SD_ReadBoolean();

	for(int index = 0; index < FLOOR_NUM; index++)
		Gnd.BestTimeList[index] = SD_ReadInt(-1, IMAX, -1);

	Gnd.ShowMouseCursorMode = SD_ReadBoolean();

	// }

	delete SaveData;

	ResolveOverlapPadBtn();
}
void ExportSaveData(void)
{
	SaveData = new autoList<uchar>();

	SD_WriteLine(SAVEDATA_SIGNATURE);

	// Item {

	SD_WriteInt(Gnd.RealScreen_W);
	SD_WriteInt(Gnd.RealScreen_H);

	SD_WriteInt(Gnd.ScreenDraw_L);
	SD_WriteInt(Gnd.ScreenDraw_T);
	SD_WriteInt(Gnd.ScreenDraw_W);
	SD_WriteInt(Gnd.ScreenDraw_H);

	SD_WriteDouble(Gnd.MusicVolume, IMAX);
	SD_WriteDouble(Gnd.SEVolume, IMAX);

	SD_WriteInt(Gnd.PadBtnId.Dir_2);
	SD_WriteInt(Gnd.PadBtnId.Dir_4);
	SD_WriteInt(Gnd.PadBtnId.Dir_6);
	SD_WriteInt(Gnd.PadBtnId.Dir_8);
	SD_WriteInt(Gnd.PadBtnId.A);
	SD_WriteInt(Gnd.PadBtnId.B);
	SD_WriteInt(Gnd.PadBtnId.C);
	SD_WriteInt(Gnd.PadBtnId.D);
	SD_WriteInt(Gnd.PadBtnId.E);
	SD_WriteInt(Gnd.PadBtnId.F);
	SD_WriteInt(Gnd.PadBtnId.L);
	SD_WriteInt(Gnd.PadBtnId.R);
	SD_WriteInt(Gnd.PadBtnId.Pause);
	SD_WriteInt(Gnd.PadBtnId.Start);

	// app -->

	SD_WriteInt(Gnd.UnclearedFloorIndex);
	SD_WriteInt(Gnd.GraphicalMode);
	SD_WriteInt(Gnd.RecordingMode);

	for(int index = 0; index < FLOOR_NUM; index++)
		SD_WriteInt(Gnd.BestTimeList[index]);

	SD_WriteInt(Gnd.ShowMouseCursorMode);

	// }

#if LOG_ENABLED == 0
	errorCase(!Jammer(SaveData, 1));
#endif

	writeFile_cx(SAVE_FILE, SaveData);
}
