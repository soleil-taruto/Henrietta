#include "StdInc.h"

#define STD_CURTAIN 0.5
//#define STD_CURTAIN 0.9
#define STD_WALL (Dc->PicTitle)

typedef struct TaisenInfo_st
{
	int Chara[2]; // [1P, 2P] == 0 ～ (CORRECT_PAIR_MAX - 1)
	int Basho; // 0 ～ BASHO_MAX
	int UseAI;
	int AITsuyosa;

	TokushuButtle_t TokushuButtle;
}
TaisenInfo_t;
TaisenInfo_t TaisenInfo;

void Taisen_ProcInit(void)
{
	TaisenInfo.Chara[0] = 16;
	TaisenInfo.Chara[1] = 20;
	TaisenInfo.Basho = BASHO_FLOWER;
	TaisenInfo.AITsuyosa = 8;
}
void Taisen_ProcFnlz(void)
{
	// noop
}

static void InputLine(char *buffer, int lenmax, char *defLine, char *prompt)
{
	uchar pressmap[256];
	uchar lastpressmap[256];
	uchar hitmap[256];

	if(!strcmp(buffer, defLine))
	{
		buffer[0] = '\0';
	}
	IH_LastPressed = 1;

//	memset(pressmap, 0x00, 256);
	memset(lastpressmap, 0x00, 256);
	memset(hitmap, 0x00, 256);

	for(int frmcnt = 0; ; frmcnt++, frmcnt %= BORDER_OF_NUMERIC)
	{
		int cStatus = GetClick();
		int chr = '\0';

		memset(pressmap, 0x00, 256);

		if(IsPress(KEY_INPUT_A)) pressmap['A'] = 1;
		if(IsPress(KEY_INPUT_B)) pressmap['B'] = 1;
		if(IsPress(KEY_INPUT_C)) pressmap['C'] = 1;
		if(IsPress(KEY_INPUT_D)) pressmap['D'] = 1;
		if(IsPress(KEY_INPUT_E)) pressmap['E'] = 1;
		if(IsPress(KEY_INPUT_F)) pressmap['F'] = 1;
		if(IsPress(KEY_INPUT_G)) pressmap['G'] = 1;
		if(IsPress(KEY_INPUT_H)) pressmap['H'] = 1;
		if(IsPress(KEY_INPUT_I)) pressmap['I'] = 1;
		if(IsPress(KEY_INPUT_J)) pressmap['J'] = 1;
		if(IsPress(KEY_INPUT_K)) pressmap['K'] = 1;
		if(IsPress(KEY_INPUT_L)) pressmap['L'] = 1;
		if(IsPress(KEY_INPUT_M)) pressmap['M'] = 1;
		if(IsPress(KEY_INPUT_N)) pressmap['N'] = 1;
		if(IsPress(KEY_INPUT_O)) pressmap['O'] = 1;
		if(IsPress(KEY_INPUT_P)) pressmap['P'] = 1;
		if(IsPress(KEY_INPUT_Q)) pressmap['Q'] = 1;
		if(IsPress(KEY_INPUT_R)) pressmap['R'] = 1;
		if(IsPress(KEY_INPUT_S)) pressmap['S'] = 1;
		if(IsPress(KEY_INPUT_T)) pressmap['T'] = 1;
		if(IsPress(KEY_INPUT_U)) pressmap['U'] = 1;
		if(IsPress(KEY_INPUT_V)) pressmap['V'] = 1;
		if(IsPress(KEY_INPUT_W)) pressmap['W'] = 1;
		if(IsPress(KEY_INPUT_X)) pressmap['X'] = 1;
		if(IsPress(KEY_INPUT_Y)) pressmap['Y'] = 1;
		if(IsPress(KEY_INPUT_Z)) pressmap['Z'] = 1;

		if(IsPress(KEY_INPUT_PERIOD) || IsPress(KEY_INPUT_DECIMAL)) pressmap['.'] = 1;

		if(IsPress(KEY_INPUT_0) || IsPress(KEY_INPUT_NUMPAD0)) pressmap['0'] = 1;
		if(IsPress(KEY_INPUT_1) || IsPress(KEY_INPUT_NUMPAD1)) pressmap['1'] = 1;
		if(IsPress(KEY_INPUT_2) || IsPress(KEY_INPUT_NUMPAD2)) pressmap['2'] = 1;
		if(IsPress(KEY_INPUT_3) || IsPress(KEY_INPUT_NUMPAD3)) pressmap['3'] = 1;
		if(IsPress(KEY_INPUT_4) || IsPress(KEY_INPUT_NUMPAD4)) pressmap['4'] = 1;
		if(IsPress(KEY_INPUT_5) || IsPress(KEY_INPUT_NUMPAD5)) pressmap['5'] = 1;
		if(IsPress(KEY_INPUT_6) || IsPress(KEY_INPUT_NUMPAD6)) pressmap['6'] = 1;
		if(IsPress(KEY_INPUT_7) || IsPress(KEY_INPUT_NUMPAD7)) pressmap['7'] = 1;
		if(IsPress(KEY_INPUT_8) || IsPress(KEY_INPUT_NUMPAD8)) pressmap['8'] = 1;
		if(IsPress(KEY_INPUT_9) || IsPress(KEY_INPUT_NUMPAD9)) pressmap['9'] = 1;

		for(int i = 0; i < 64; i++)
		{
			((uint *)hitmap)[i] = ((uint *)pressmap)[i] & ~((uint *)lastpressmap)[i];
		}
		for(int i = 0; i < 256; i++)
		{
			if(hitmap[i])
			{
				chr = i;
				break;
			}
		}
		memcpy(lastpressmap, pressmap, 256);

		if(chr == 'V' && (IsPress(KEY_INPUT_LCONTROL) || IsPress(KEY_INPUT_RCONTROL))) // CTRL+V -> 貼り付け
		{
			char *line = GetClipLine();

			if(line)
			{
				if(*line)
				{
					line2domain_len(line, lenmax);
					strcpy(buffer, line);
				}
				memFree(line);
			}
		}
		else if(chr)
		{
			if(strlen(buffer) < (uint)lenmax)
			{
				char *p = strchr(buffer, '\0');

				p[0] = chr;
				p[1] = '\0';
			}
		}
		else if(IsHit(KEY_INPUT_RETURN) || IsHit(KEY_INPUT_NUMPADENTER) || cStatus)
		{
			break;
		}
		else if(IsHit(KEY_INPUT_BACK))
		{
			if(*buffer)
			{
				strchr(buffer, '\0')[-1] = '\0';
			}
		}
		else if(IsHit(KEY_INPUT_DELETE))
		{
			*buffer = '\0';
		}

		SimpleDraw(STD_WALL, 0, 0, 0);
		ExecFrameCurtain();

		MyCls();
		MyPrint("");
		MyPrint_x(xcout("　%s", prompt));
		MyPrint("");
		MyPrint_x(xcout("　> %s%c", buffer, (frmcnt / 20) & 1 ? '_' : ' '));
		MyPrint("");

		ExecMyPrint();
		SwapFrame();
	}

	if(!*buffer)
	{
		strcpy(buffer, defLine);
	}
}
static int InputValue(int minval, int maxval, char *nameOfValue, int defValue)
{
	char buffer[12];
	char *ltmp;
	int value;

	buffer[0] = '\0';
	InputLine(buffer, sizeof(buffer) - 1, "", ltmp = xcout("%sを入力して下さい (%d～%d) :", nameOfValue, minval , maxval));
	memFree(ltmp);

	value = atoi(buffer);

	if(value < minval || maxval < value)
	{
		value = defValue;
	}
	return value;
}

// メニュー共通 --->

#define ITEM_MAX 50

static char *Title;
static char *Items[ITEM_MAX];
static int ItemCount;
static int ItemTsumero;

static void MenuSetTitle(char *title)
{
	Title = title;
}
static void MenuAddItem(char *item)
{
	errorCase(ITEM_MAX <= ItemCount);

	Items[ItemCount] = item;
	ItemCount++;
}
static int Menu(int lastSel)
{
	errorCase(!Title);
	errorCase(!ItemCount);
	errorCase(lastSel < 0 || ItemCount <= lastSel);

	PlayBGM(Dc->BGM_Title);

	int currIndex = lastSel;
	int musCurrIndex = -1; // -1 == not-selected

	for(; ; )
	{
		SimpleDraw(STD_WALL, 0, 0, 0);
		ExecFrameCurtain();

		MyCls();
		MyPrint("");
		MyPrint_x(xcout("　**** %s ****", Title));
		MyPrint("");

		int mx;
		int my;
		GetMusPos(&mx, &my);

		const int MENU_ITEM_Y_SPAN = 20;

		musCurrIndex = my / MENU_ITEM_Y_SPAN;
		musCurrIndex -= 3;

		if(!ItemTsumero) musCurrIndex /= 2;

		if(musCurrIndex < 0 || ItemCount <= musCurrIndex)
			musCurrIndex = -1;

		for(int index = 0; index < ItemCount; index++)
		{
			char *lbox;

			if(index == musCurrIndex)
			{
				lbox = "◇";
				if(index == currIndex) lbox = "◆";
			}
			else
			{
				lbox = "□";
				if(index == currIndex) lbox = "■";
			}
			MyPrint_x(xcout("　%s　%s", lbox, Items[index]));

			if(!ItemTsumero)
				MyPrint("");
		}
		ExecMyPrint();
		SwapFrame();

		int cStatus = GetClick();

		for(int pSide = 0; pSide < 2; pSide++)
		{
			if(GetHitPS(pSide, INP_ROT_DECIDE))
			{
				goto end_menu;
			}
			if(cStatus == 1)
			{
				if(musCurrIndex != -1)
				{
					currIndex = musCurrIndex;
					errorCase(currIndex < 0 || ItemCount <= currIndex);
					goto end_menu;
				}
			}
			if(cStatus == 2)
			{
				currIndex = ItemCount - 1;
				goto end_menu;
			}
			if(GetHitPS(pSide, INP_ROT_CANCEL))
			{
				int cancelidx = ItemCount - 1;

				if(currIndex == cancelidx)
				{
					goto end_menu;
				}
				currIndex = cancelidx;
			}
			if(GetHitPS(pSide, INP_BOMB)) // ランダム選択用
			{
				currIndex = rndbnd(0, ItemCount - 1);
			}
			if(GetRendaHitPS(pSide, INP_DIR_4) || GetRendaHitPS(pSide, INP_DIR_8)) // 上へ
			{
				currIndex--;
			}
			if(GetRendaHitPS(pSide, INP_DIR_2) || GetRendaHitPS(pSide, INP_DIR_6)) // 下へ
			{
				currIndex++;
			}
			m_range(currIndex, 0, ItemCount - 1);
		}
	}
end_menu:

	clsDx();

	// init
	Title = NULL;
	ItemCount = 0;
	ItemTsumero = 0;

	return currIndex;
}

// <---

static void TaisenEx(void)
{
	TaisenInfo_t *i = &TaisenInfo;

	errorCase(i->Chara[0] < 0 || CORRECT_PAIR_MAX <= i->Chara[0]);
	errorCase(i->Chara[1] < 0 || CORRECT_PAIR_MAX <= i->Chara[1]);
	errorCase(i->Basho < 0 || BASHO_MAX <= i->Basho);

	PzInit();

	Pzdc.Chara[0][0] = CorrectPairs[i->Chara[0]][0];
	Pzdc.Chara[0][1] = CorrectPairs[i->Chara[0]][1];
	Pzdc.Chara[1][0] = CorrectPairs[i->Chara[1]][0];
	Pzdc.Chara[1][1] = CorrectPairs[i->Chara[1]][1];

	for(int pSide = 0; pSide < 2; pSide++)
	{
		int h = HWT_I;

		switch(i->Chara[pSide])
		{
		case  0: h = HWT_I; break;
		case  1: h = HWT_E; break;
		case  2: h = HWT_S; break;
		case  3: h = HWT_C; break;
		case  4: h = HWT_G; break;
		case  5: h = HWT_R; break;
		case  6: h = HWT_J; break;
		case  7: h = HWT_P; break;
		case  8: h = HWT_F; break;
		case  9: h = HWT_A; break;
		case 10: h = HWT_U; break;
		case 11: h = HWT_IR; break;
		case 12: h = HWT_IJ; break;
		case 13: h = HWT_EC; break;
		case 14: h = HWT_EJ; break;
		case 15: h = HWT_SR; break;
		case 16: h = HWT_GI; break;
		case 17: h = HWT_GJ; break;
		case 18: h = HWT_GP; break;
		case 19: h = HWT_PI; break;
		case 20: h = HWT_PJ; break;
		case 21: h = HWT_FE; break;
		case 22: h = HWT_FS; break;
		case 23: h = HWT_AE; break;
		case 24: h = HWT_UP; break;
		case 25: h = HWT_UA; break;

		default:
			error();
		}
		Pzdc.HissatsuType[pSide] = h;
	}

	Pzdc.Basho = i->Basho;

	switch(i->Basho)
	{
	case BASHO_SUNFLOWER:
		Pzdc.BGMFileNormal = RESRC_BGM_SHIKI;
		break;

	case BASHO_FLOWER:
		Pzdc.BGMFileNormal = RESRC_BGM_CAFE;
		break;

	case BASHO_SEA:
		Pzdc.BGMFileNormal = RESRC_BGM_3DAYS;
		break;

	case BASHO_LAKE:
		Pzdc.BGMFileNormal = RESRC_BGM_POLP;
		break;

	case BASHO_DESERT:
		Pzdc.BGMFileNormal = RESRC_BGM_TEIEN;
		break;

	case BASHO_FOREST:
		Pzdc.BGMFileNormal = RESRC_BGM_DANCE;
		Pzdc.BGMFilePinch  = RESRC_BGM_D;
		break;

	case BASHO_JINJA:
		Pzdc.BGMFileNormal = RESRC_BGM_SUI;
		Pzdc.BGMFilePinch  = RESRC_BGM_D;
		break;

	case BASHO_NIGHT:
		Pzdc.BGMFileNormal = RESRC_BGM_STORY;
		Pzdc.BGMFilePinch  = RESRC_BGM_D;
		break;

	default:
		error();
	}
	Pzdc.UseAI = i->UseAI;

	switch(i->AITsuyosa)
	{
	case 0: PzAI_SetStdParams(1, PzThink_Lv0, 0.000, 0, 0.000, 0.0001, 0, 0.000); break;
	case 1: PzAI_SetStdParams(1, PzThink_Lv1, 0.000, 0, 0.000, 0.0001, 0, 0.000); break;
	case 2: PzAI_SetStdParams(2, PzThink_Lv1, 0.000, 0, 0.000, 0.050,  9, 0.700); break;
	case 3: PzAI_SetStdParams(2, PzThink_Lv2, 0.000, 0, 0.000, 0.050,  9, 0.700); break;
	case 4: PzAI_SetStdParams(3, PzThink_Lv2, 0.000, 0, 0.000, 0.222,  9, 0.700); break;
	case 5: PzAI_SetStdParams(3, PzThink_Lv3, 0.050, 9, 0.700, 0.222,  9, 0.700); break;
	case 6: PzAI_SetStdParams(4, PzThink_Lv3, 0.050, 9, 0.700, 0.888,  9, 0.700); break;
	case 7: PzAI_SetStdParams(4, PzThink_Lv4, 0.050, 9, 0.700, 0.888,  9, 0.700); break;
	case 8: PzAI_SetStdParams(5, PzThink_Lv4, 0.777, 9, 0.700, 1.000, 10, 0.500); break;
	case 9: PzAI_SetStdParams(5, PzThink_ClockTower, 0.777, 9, 0.700, 1.000, 10, 0.500); break;

	default:
		error();
	}
	Pzdc.TokushuButtle = i->TokushuButtle;

	PzMain();
	PzFnlz();

	if(PzdcRet.PauseSelExitApp)
	{
		termination(0);
	}
	for(int frmidx = 0; frmidx < 30; frmidx++) // ノイズ対策 ... 意味無い？
	{
		SwapFrame();
	}
}
static void Taisen(void)
{
	Dc->BlackCurtain = 1.0;
	WaitToCurtain(STD_WALL);
	Dc->BlackCurtain = 0.0;
	Dc->BCPct = 0.0;

	TaisenEx();

	Dc->BCPct = 1.0;
	Dc->BlackCurtain = STD_CURTAIN;
}

static void SetColorStealth(TokushuButtle_t *t, int num)
{
	errorCase(num < 0);
	errorCase(PUYO_COLOR_NUM - 1 < num);

	Bulldoze(t->Stealth + 1, num, 1);
	Shuffle(t->Stealth + 1, PUYO_COLOR_NUM - 1);
}
static void (*ST_Perform)(void);

static void SpecialTaisen(void)
{
	int lastSel = 0;

	TaisenInfo_t *i = &TaisenInfo;
	TokushuButtle_t *t = &i->TokushuButtle;

	for(; ; )
	{
		MenuSetTitle("エキストラ");

		ItemTsumero = 1;

		MenuAddItem("２色");
		MenuAddItem("３色");
		MenuAddItem("お邪魔ステルス");
		MenuAddItem("お邪魔以外ステルス");
		MenuAddItem("２色ステルス");
		MenuAddItem("３色ステルス");
		MenuAddItem("２色＋お邪魔ステルス");
		MenuAddItem("３色＋お邪魔ステルス");
		MenuAddItem("全部ステルス");
		MenuAddItem("ステルスなハンデ戦【１Ｐ上級者／２Ｐ初心者】");
		MenuAddItem("ステルスなハンデ戦【１Ｐ初心者／２Ｐ上級者】");
		MenuAddItem("エンディング");
		MenuAddItem("エンディング(ボーダー)");
		MenuAddItem("ビッグフラワー");
		MenuAddItem("お邪魔×３");
		MenuAddItem("お邪魔×５");
		MenuAddItem("お邪魔×７７７");
		MenuAddItem("スコア×３");
		MenuAddItem("スコア×５");
		MenuAddItem("スコア×７７７");
		MenuAddItem("？個で消える");
		MenuAddItem("常に隠し");
		MenuAddItem("高速");
		MenuAddItem("もっと高速");
		MenuAddItem("もっともっと高速");
		MenuAddItem("戻る");

		memset(t, 0x00, sizeof(TokushuButtle_t)); // 特殊対戦設定をクリア

		char *tbTitle = "untitle";

		switch(lastSel = Menu(lastSel))
		{
		case 0: // 2色
			tbTitle = "2Colors";
			t->ColorMax = 2;
			break;

		case 1: // 3色
			tbTitle = "3Colors";
			t->ColorMax = 3;
			break;

		case 2: // ステルス - お邪魔
			tbTitle = "Stealth-J";
			t->Stealth[0] = 1;
			t->GameOverAndShowStealth = 1;
			break;

		case 3: // ステルス - カラー
			tbTitle = "Stealth-C";
			SetColorStealth(t, PUYO_COLOR_NUM - 1);
			t->GameOverAndShowStealth = 1;
			break;

		case 4: // ステルス - 2色
			tbTitle = "Stealth-2C";
			SetColorStealth(t, 2);
			t->GameOverAndShowStealth = 1;
			break;

		case 5: // ステルス - 3色
			tbTitle = "Stealth-3C";
			SetColorStealth(t, 3);
			t->GameOverAndShowStealth = 1;
			break;

		case 6: // ステルス - 2色 + お邪魔
			tbTitle = "Stealth-J2C";
			SetColorStealth(t, 2);
			t->Stealth[0] = 1;
			t->GameOverAndShowStealth = 1;
			break;

		case 7: // ステルス - 3色 + お邪魔
			tbTitle = "Stealth-J3C";
			SetColorStealth(t, 3);
			t->Stealth[0] = 1;
			t->GameOverAndShowStealth = 1;
			break;

		case 8: // ステルス - ALL
			tbTitle = "Stealth";
			SetColorStealth(t, PUYO_COLOR_NUM - 1);
			t->Stealth[0] = 1;
			t->GameOverAndShowStealth = 1;
			break;

		case 9: // ステルス - ALL (1P only)
			tbTitle = "Stealth-1P";
			SetColorStealth(t, PUYO_COLOR_NUM - 1);
			t->Stealth[0] = 1;
			t->StealthDisable[1] = 1;
			t->GameOverAndShowStealth = 1;
			break;

		case 10: // ステルス - ALL (2P only)
			tbTitle = "Stealth-2P";
			SetColorStealth(t, PUYO_COLOR_NUM - 1);
			t->Stealth[0] = 1;
			t->StealthDisable[0] = 1;
			t->GameOverAndShowStealth = 1;
			break;

		case 11: // 花降らし
			tbTitle = "FlowerRain";
			t->FlowerRain = 1;
			break;

		case 12: // 花降らし (虹)
			tbTitle = "BorderRain";
			t->FlowerRain = 1;
			t->FlowerRainbow = 1;
			break;

		case 13: // 散り花、大
			tbTitle = "BigFlower";
			t->BigFlower = 1;
			break;

		case 14: // お邪魔 * 3
			tbTitle = "Jx3";
			t->JamaExpand = 3;
			break;

		case 15: // お邪魔 * 5
			tbTitle = "Jx5";
			t->JamaExpand = 5;
			break;

		case 16: // お邪魔 * 777
			tbTitle = "Jx777";
			t->JamaExpand = 777;
			break;

		case 17: // スコア * 3
			tbTitle = "Sx3";
			t->ScoreExpand = 3;
			break;

		case 18: // スコア * 5
			tbTitle = "Sx5";
			t->ScoreExpand = 5;
			break;

		case 19: // スコア * 777
			tbTitle = "Sx777";
			t->ScoreExpand = 777;
			break;

		case 20: // ｎ個消え
			{
				MenuSetTitle("？個で消える");

				MenuAddItem("２個で消える");
				MenuAddItem("３個で消える");
				MenuAddItem("４個で消える");
				MenuAddItem("５個で消える");
				MenuAddItem("６個で消える");
				MenuAddItem("７個で消える");
				MenuAddItem("８個で消える");
				MenuAddItem("９個で消える");
				MenuAddItem("１ダースで消える");
				MenuAddItem("１ダースで消える(２色)");
				MenuAddItem("１ダースで消える(３色)");

				switch(Menu(2))
				{
				case 0: tbTitle = "Erase-2"; t->PuyoEraseNum = 2; break;
				case 1: tbTitle = "Erase-3"; t->PuyoEraseNum = 3; break;
				case 2: tbTitle = "Erase-4"; t->PuyoEraseNum = 4; break;
				case 3: tbTitle = "Erase-5"; t->PuyoEraseNum = 5; break;
				case 4: tbTitle = "Erase-6"; t->PuyoEraseNum = 6; break;
				case 5: tbTitle = "Erase-7"; t->PuyoEraseNum = 7; break;
				case 6: tbTitle = "Erase-8"; t->PuyoEraseNum = 8; break;
				case 7: tbTitle = "Erase-9"; t->PuyoEraseNum = 9; break;
				case 8:
					tbTitle = "Erase-12";
					t->PuyoEraseNum = 12;
					break;

				case 9:
					tbTitle = "Erase-12-2C";
					t->PuyoEraseNum = 12;
					t->ColorMax = 2;
					break;

				case 10:
					tbTitle = "Erase-12-3C";
					t->PuyoEraseNum = 12;
					t->ColorMax = 3;
					break;

				default:
					error();
				}
			}
			break;

		case 21: // 常に隠し
			tbTitle = "Hidden";
			t->AlwaysHidden = 1;
			break;

		case 22: // 高速
			tbTitle = "HiSpeed";
			t->VeryFast = 1;
			break;

		case 23: // 高速x2
			tbTitle = "ExHiSpeed";
			t->VeryFast = 2;
			break;

		case 24: // 高速x3
			tbTitle = "ExExHiSpeed";
			t->VeryFast = 5;
			break;

		case 25: // 戻る
			goto end_menu;

		default:
			error();
		}
		errorCase(TB_TITLE_LENMAX < strlen(tbTitle));
		strcpy(t->Title, tbTitle);

		( ST_Perform ? ST_Perform : Taisen )();
	}
end_menu:;
}

static int CharaSelect(int lastSel)
{
	MenuSetTitle("キャラクタまたはペアを選んで下さい");

	ItemTsumero = 1;

	// キャラ
	MenuAddItem("イタリア");
	MenuAddItem("イギリス");
	MenuAddItem("スペイン");
	MenuAddItem("中国");
	MenuAddItem("ドイツ");
	MenuAddItem("ロマーノ");
	MenuAddItem("日本");
	MenuAddItem("プロイセン");
	MenuAddItem("フランス");
	MenuAddItem("アメリカ");
	MenuAddItem("ロシア");

	// ペア
	MenuAddItem("イタリア・ロマーノ");
	MenuAddItem("イタリア・日本");
	MenuAddItem("イギリス・中国");
	MenuAddItem("イギリス・日本");
	MenuAddItem("スペイン・ロマーノ");
	MenuAddItem("ドイツ・イタリア");
	MenuAddItem("ドイツ・日本");
	MenuAddItem("ドイツ・プロイセン");
	MenuAddItem("プロイセン・イタリア");
	MenuAddItem("プロイセン・日本");
	MenuAddItem("フランス・イギリス");
	MenuAddItem("フランス・スペイン");
	MenuAddItem("アメリカ・イギリス");
	MenuAddItem("ロシア・プロイセン");
	MenuAddItem("ロシア・アメリカ");

	return Menu(lastSel);
}
static int BashoSelect(int lastSel)
{
	MenuSetTitle("場所を選んで下さい");

	MenuAddItem("ひまわり畑");
	MenuAddItem("花畑");
	MenuAddItem("海");
	MenuAddItem("湖");
	MenuAddItem("砂漠");
	MenuAddItem("森");
	MenuAddItem("神社");
	MenuAddItem("夜");

	return Menu(lastSel);
}
static int AITsuyosaSelect(int lastSel)
{
	MenuSetTitle("ＡＩの強さを選んで下さい (レベル０が最弱)");

	MenuAddItem("レベル０");
	MenuAddItem("レベル１");
	MenuAddItem("レベル２");
	MenuAddItem("レベル３");
	MenuAddItem("レベル４");
	MenuAddItem("レベル５");
	MenuAddItem("レベル６");
	MenuAddItem("レベル７");
	MenuAddItem("レベル８");
	MenuAddItem("レベル８±α");

	return Menu(lastSel);
}

static void AIToTaisen(void)
{
	int lastSel = 0;

	TaisenInfo.UseAI = 1;

	for(; ; )
	{
		MenuSetTitle("１人で対戦");

		MenuAddItem("対戦スタート");
		MenuAddItem("１Ｐ(自分)キャラ変更");
		MenuAddItem("２Ｐ(ＡＩ)キャラ変更");
		MenuAddItem("場所変更");
		MenuAddItem("強さ変更");
		MenuAddItem("エキストラ");
		MenuAddItem("戻る");

		switch(lastSel = Menu(lastSel))
		{
		case 0:
			Taisen();
			break;

		case 1:
			TaisenInfo.Chara[0] = CharaSelect(TaisenInfo.Chara[0]);
			break;

		case 2:
			TaisenInfo.Chara[1] = CharaSelect(TaisenInfo.Chara[1]);
			break;

		case 3:
			TaisenInfo.Basho = BashoSelect(TaisenInfo.Basho);
			break;

		case 4:
			TaisenInfo.AITsuyosa = AITsuyosaSelect(TaisenInfo.AITsuyosa);
			break;

		case 5:
			SpecialTaisen();
			break;

		case 6:
			goto end_menu;

		default:
			error();
		}
	}
end_menu:;
}
static void DachiToTaisen(void)
{
	int lastSel = 0;

	TaisenInfo.UseAI = 0;

	for(; ; )
	{
		MenuSetTitle("２人で対戦");

		MenuAddItem("対戦スタート");
		MenuAddItem("１Ｐ(左側)キャラ変更");
		MenuAddItem("２Ｐ(右側)キャラ変更");
		MenuAddItem("場所変更");
		MenuAddItem("エキストラ");
		MenuAddItem("戻る");

		switch(lastSel = Menu(lastSel))
		{
		case 0:
			Taisen();
			break;

		case 1:
			TaisenInfo.Chara[0] = CharaSelect(TaisenInfo.Chara[0]);
			break;

		case 2:
			TaisenInfo.Chara[1] = CharaSelect(TaisenInfo.Chara[1]);
			break;

		case 3:
			TaisenInfo.Basho = BashoSelect(TaisenInfo.Basho);
			break;

		case 4:
			SpecialTaisen();
			break;

		case 5:
			goto end_menu;

		default:
			error();
		}
	}
end_menu:;
}

// ==== ==== ====
// ==== 設定 ====
// ==== ==== ====

static void P_ChangeFullScreenMode(void)
{
	SettInfo.FullScreenMode = !SettInfo.FullScreenMode;

	int w;
	int h;

	if(SettInfo.FullScreenMode)
	{
		w = GetSystemMetrics(SM_CXSCREEN);
		h = GetSystemMetrics(SM_CYSCREEN);
	}
	else
	{
		w = 800;
		h = 600;
	}
	SetGraphMode(w, h, 32);

	// スクリーンモード変更に伴う、各種再初期化
	{
		DispMusCursor(true);

		DcInit_LoadAllPictureSafe();
//		Pub_InitPicKeyList(); // PicKeyListを使わないので呼び出し不要
		GC_ProcInitReset();
		EDEx_ScreenModeChanged();
		SFS_Reset();

		SetDrawScreen(DX_SCREEN_BACK);

		Pub_MainScreen = -1;
	}

	AdjustScreenPosition(w, h);
	SetDrawScreen_LTWH(SettInfo.FullScreenMode);
}
static int P_SPK_InputKey(void)
{
	int keyidx;

	for(; ; )
	{
		keyidx = Pub_GetPressKeyIndex();

		if(keyidx != -1)
			break;

		SimpleDraw(STD_WALL, 0, 0, 0);
		ExecFrameCurtain();

		MyCls();
		MyPrint("");
		MyPrint("　機能を割り当てるキーを押して下さい...");
		MyPrint("");

		ExecMyPrint();
		SwapFrame();
	}
	return Pub_GetUseKey(keyidx);
}
static void P_SettingPlayerKey(int plIndex)
{
	int lastSel = 0;

	for(; ; )
	{
		{
		static char *tmp;
		memFree(tmp);
		MenuSetTitle(tmp = xcout("プレイヤー%sのキー割り当て", ZenInt(plIndex + 1)));
		}

		MenuAddItem("上キー");
		MenuAddItem("下キー");
		MenuAddItem("左キー");
		MenuAddItem("右キー");
		MenuAddItem("左回転キー(決定キー)");
		MenuAddItem("右回転キー(キャンセルキー)");
		MenuAddItem("戻る");

		switch(lastSel = Menu(lastSel))
		{
		case 0: SettInfo.KeySettInfos[KSIIDX_P1][KSIIDX_KB].Dir8 = P_SPK_InputKey(); break;
		case 1: SettInfo.KeySettInfos[KSIIDX_P1][KSIIDX_KB].Dir2 = P_SPK_InputKey(); break;
		case 2: SettInfo.KeySettInfos[KSIIDX_P1][KSIIDX_KB].Dir4 = P_SPK_InputKey(); break;
		case 3: SettInfo.KeySettInfos[KSIIDX_P1][KSIIDX_KB].Dir6 = P_SPK_InputKey(); break;
		case 4: SettInfo.KeySettInfos[KSIIDX_P1][KSIIDX_KB].RotL = P_SPK_InputKey(); break;
		case 5: SettInfo.KeySettInfos[KSIIDX_P1][KSIIDX_KB].RotR = P_SPK_InputKey(); break;

		case 6:
			goto end_menu;

		default:
			error();
		}

		SettInfo2Input();
		RefreshInput(); // 今押したキーが入力にならないように。
	}
end_menu:
	;
}
static void P_SettingVolume(double &varVolume, int bgmFlag)
{
	int lastSel = 0;

	for(; ; )
	{
		{
		static char *tmp;
		memFree(tmp);
		MenuSetTitle(tmp = xcout("%s音量 ( %03d )", bgmFlag ? "ＢＧＭ" : "ＳＥ", (int)(varVolume * 100.0 + 0.5)));
		}

		MenuAddItem("音量を上げる");
		MenuAddItem("音量を下げる");
		MenuAddItem("戻る");

		int iVol = (int)(varVolume * 20.0 + 0.5);

		switch(lastSel = Menu(lastSel))
		{
		case 0: iVol++; break;
		case 1: iVol--; break;

		case 2:
			goto end_menu;

		default:
			error();
		}
		m_range(iVol, 0, 20);
		varVolume = iVol / 20.0;
	}
end_menu:
	;
}
static void P_Setting(void)
{
	int lastSel = 0;

	for(; ; )
	{
		MenuSetTitle("設定");

		MenuAddItem(SettInfo.FullScreenMode ? "ウィンドウモードに戻す" : "フルスクリーンモードに変更する");
		MenuAddItem("プレイヤー１のキー割り当て");
		MenuAddItem("プレイヤー２のキー割り当て");
		MenuAddItem("ＰＡＤ１のボタンの割り当て");
		MenuAddItem("ＰＡＤ２のボタンの割り当て");
		MenuAddItem("ＢＧＭ音量");
		MenuAddItem("ＳＥ音量");
		MenuAddItem("戻る");

		switch(lastSel = Menu(lastSel))
		{
		case 0:
			P_ChangeFullScreenMode();
			break;

		case 1:
			P_SettingPlayerKey(KSIIDX_P1);
			break;

		case 2:
			P_SettingPlayerKey(KSIIDX_P2);
			break;

		case 3:
			Pub_PadButtonConfig(0);
			break;

		case 4:
			Pub_PadButtonConfig(1);
			break;

		case 5:
			P_SettingVolume(Dc->BGMVolume, 1);
			break;

		case 6:
			P_SettingVolume(Dc->SeVolume, 0);
			break;

		case 7:
			goto end_menu;

		default:
			error();
		}
	}
end_menu:
	;
}

// ==== ==== ====
// ==== ==== ====
// ==== ==== ====

void NetworkTaisen(void);

void TestTaisen(void)
{
	int lastSel = 0;

	Dc->BlackCurtain = STD_CURTAIN;
	Dc->BCPct = Dc->BlackCurtain; // 背景カーテンを一瞬でセットする。

	for(int c = 0; c < 20; c++)
	{
		SimpleDraw(STD_WALL, 0, 0, 0);
		ExecFrameCurtain();
		SwapFrame();
	}
	for(; ; )
	{
		MenuSetTitle("ＹＴ－ＯＣＨＩＭＯＮＯ");

		MenuAddItem("１人で対戦");
		MenuAddItem("２人で対戦");
		MenuAddItem("ネットワーク対戦");
		MenuAddItem("設定");
		MenuAddItem("ゲームを終わる");

		switch(lastSel = Menu(lastSel))
		{
		case 0:
			AIToTaisen();
			break;

		case 1:
			DachiToTaisen();
			break;

		case 2:
			NetworkTaisen();
			break;

		case 3:
			P_Setting();
			break;

		case 4:
			goto end_menu;

		default:
			error();
		}
	}
end_menu:

	Dc->BlackCurtain = 0.0;
}

////////////////////////////
// 以下、ネットワーク対戦 //
////////////////////////////

static int IsKnownKansenshaIP(uchar ip[4])
{
	for(int index = 0; index < NtKansenshaCount; index++)
	{
		if(!memcmp(ip, NtKansenshaIPList[index], 4))
		{
			return 1;
		}
	}
	return 0;
}

#define TntCls() \
	SimpleDraw(STD_WALL, 0, 0, 0); \
	ExecFrameCurtain(); \
	MyCls(); \
	MyPrint(""); \
	MyPrint("　**** ネットワークに接続しています ****"); \
	MyPrint("")

static void NetworkCommonInit(NtCommonInfo_t *i)
{
//	i->SockHandle; // 既に初期化

	i->Syn = 1;
	i->AckSyn = 0;
	i->Ack = 0;

//	i->SelfSide; // 個別
	strcpy(i->BuiltStamp, GetBuiltStamp());
	i->Seed = (int)time(NULL);

	i->Credentials = i->Seed;
	i->OSCredentials = -1;

	strcpy(i->Name, SettInfo.NtName);
//	i->IP; // 最初の受信で初期化
	i->Port = SettInfo.NtRecvPort;

//	i->SendIP; // 個別
//	i->SendPort; // 個別

//	i->Chara; // 個別
//	i->Basho; // 個別
}
static void ServerWaitForClient(void)
{
	NtInit(SettInfo.NtRecvPort);

	NtCommonInfo_t *i = NtCommonInfo + 0;
	NtCommonInfo_t *osi = NtCommonInfo + 1;

	NetworkCommonInit(i);

	i->SelfSide = 0;
	i->Chara = TaisenInfo.Chara[0];
	i->Basho = TaisenInfo.Basho;

	char ignorename[NT_NAME_LENMAX + 1];
	ignorename[0] = '\0';

restart:
	NtUDPRecvBuffClear();
	osi->Syn = 0;

	for(int frmidx = 0; ; frmidx++)
	{
		int frmsec = frmidx / 60;

		TntCls();
		MyPrint_x(xcout("　接続を待っています ... %d:%02d", frmsec / 60, frmsec % 60));
		MyPrint("");
		MyPrint("　中断するにはキャンセルキー(ボタン)を押して下さい。");

		ExecMyPrint();
		SwapFrame();
		int cStatus = GetClick();

		if(GetHit(INP_ROT_CANCEL) || cStatus == 2)
		{
			goto endfunc;
		}
		NtRecvCommonInfo();

		if(osi->Syn) // ? 接続アリ
		{
			if(strcmp(i->BuiltStamp, osi->BuiltStamp)) // ? BuiltStamp 不一致
			{}
			else if(!strcmp(ignorename, osi->Name)) // ? 無視する相手に一致
			{}
			else
			{
				break;
			}
		}
	}
	i->TokushuButtle = osi->TokushuButtle;
	i->TBEnable = osi->TBEnable;

	for(int frmidx = 0; ; frmidx++)
	{
		char *osname = osi->Name;
		char *extraInfo;

		if(i->TBEnable)
			extraInfo = xcout("(エキストラ:%s)", i->TokushuButtle.Title);
		else
			extraInfo = strx("");

		TntCls();
		MyPrint_x(xcout("　%sさんがあなたとの対戦を希望しています。%s", osname, extraInfo));
		memFree(extraInfo);

		MyPrint("");
		MyPrint("　　対戦する → 決定キー(ボタン)");
		MyPrint("");
		MyPrint("　　無視する → キャンセルキー(ボタン)");

		ExecMyPrint();
		SwapFrame();
		int cStatus = GetClick();

		if(GetHit(INP_ROT_DECIDE) || cStatus == 1)
		{
			break;
		}
		if(GetHit(INP_ROT_CANCEL) || cStatus == 2)
		{
			strcpy(ignorename, osname);
			goto restart;
		}
		if(frmidx % (60 * 5) == 0)
		{
			PlaySe(Dc->SeIkuyo);
		}
	}
	i->AckSyn = 1;
	i->OSCredentials = osi->Credentials;

	i->IP[0] = osi->SendIP[0];
	i->IP[1] = osi->SendIP[1];
	i->IP[2] = osi->SendIP[2];
	i->IP[3] = osi->SendIP[3];

	i->SendIP[0] = NtUDPRecvIP[0];
	i->SendIP[1] = NtUDPRecvIP[1];
	i->SendIP[2] = NtUDPRecvIP[2];
	i->SendIP[3] = NtUDPRecvIP[3];
	i->SendPort = osi->Port;

	for(int frmidx = 0; ; frmidx++)
	{
		TntCls();
		MyPrint_x(xcout("　接続中 ... %d", i->Ack));
		MyPrint("");
		MyPrint("　中断するにはキャンセルキー(ボタン)を押して下さい。");

		ExecMyPrint();
		SwapFrame();
		int cStatus = GetClick();

		if(GetHit(INP_ROT_CANCEL) || cStatus == 2)
		{
			goto endfunc;
		}
		if(frmidx % 100 == 10)
		{
			NtSendCommonInfo();
		}
		NtRecvCommonInfo();

		if(osi->AckSyn)
		{
			i->Ack = 1;
		}
		if(osi->Ack)
		{
			break;
		}
	}

	if(SettInfo.NtKansenOk)
	{
		int bk_osCred = NtCommonInfo[0].OSCredentials;

		NtCommonInfo[0].OSCredentials = -1;
		NtUDPRecvCountMax = 1;

		for(int frmidx = 0; frmidx < 60 * 3; frmidx++)
		{
			TntCls();
			MyPrint_x(xcout("　観戦エントリー受け付け中 ... %d", NtKansenshaCount));
			ExecMyPrint();
			SwapFrame();

			for(int lpcnt = 0; lpcnt < 10000; lpcnt++)
			{
				NtRecvKansenRequest();

				if(NtLastUDPRecvCount == 0)
					break;

				NtKansenRequest_t *i = &NtKansenRequest;

				if(strcmp(i->BuiltStamp, GetBuiltStamp())) // ? BuiltStamp 不一致
				{}
				else if(IsKnownKansenshaIP(NtUDPRecvIP)) // ? 既に登録してある。
				{}
				else if(NT_KANSEN_MAX <= NtKansenshaCount) // ? overflow
				{}
				else
				{
					// 登録
					memcpy(NtKansenshaIPList[NtKansenshaCount], NtUDPRecvIP, 4);
					NtKansenshaPortList[NtKansenshaCount] = i->Port;
					NtKansenshaCount++;
				}
			}
		}
		NtCommonInfo[0].OSCredentials = bk_osCred;
		NtUDPRecvCountMax = 0;

		for(int frmidx = 0; frmidx < 60 * 2; frmidx++)
		{
			TntCls();
			MyPrint_x(xcout("　観戦者を招待しています ... %d", NtKansenshaCount));

			// test
			/*
			for(int i = 0; i < NtKansenshaCount; i++)
			{
				printfDx(
					"%u.%u.%u.%u:%d\n"
					,NtKansenshaIPList[i][0]
					,NtKansenshaIPList[i][1]
					,NtKansenshaIPList[i][2]
					,NtKansenshaIPList[i][3]
					,NtKansenshaPortList[i]
					);
			}
			*/

			ExecMyPrint();
			SwapFrame();

			NtEscapeSendIPPort();

			NtKansenOkRet.Ok = 1;
			NtKansenOkRet.Info[0] = NtCommonInfo[0];
			NtKansenOkRet.Info[1] = NtCommonInfo[1];

			for(int i = 0; i < NtKansenshaCount; i++)
			{
				NtSetSendIPPort(NtKansenshaIPList[i], NtKansenshaPortList[i]);
				NtSendKansenOkRet();
			}
			NtUnescapeSendIPPort();
		}
		// 同期するようになったので、不要
		/*
		for(int frmidx = 0; frmidx < 60 * 5; frmidx++)
		{
			TntCls();
			printfDx("　観戦ウェイト中 ... %f / 5.0\n", (double)frmidx / 60);
			SwapFrame();
		}
		*/
	}
	clsDx();
//	TntCls();
//	printfDx("　同期しています ...\n");
//	SwapFrame();

	TaisenInfo_t *t = &TaisenInfo;

	t->Chara[0] = i->Chara;
	t->Chara[1] = osi->Chara;
	t->Basho = i->Basho;
	t->UseAI = 0; // 2bs

	t->TokushuButtle = i->TokushuButtle;

	Pzdc.NetworkMode = 1;
	Init_Rnd(i->Seed);

	Taisen(); // サーバーモード

	memset(&t->TokushuButtle, 0x00, sizeof(TokushuButtle_t)); // 設定した側ではないので、ゼロに戻す。

endfunc:
	NtFnlz();
}
static void ClientWaitForServer(NtTomodachiInfo_t *dachi, int tbEnable)
{
	NtInit(SettInfo.NtRecvPort);

	NtCommonInfo_t *i = NtCommonInfo + 0;
	NtCommonInfo_t *osi = NtCommonInfo + 1;

	NetworkCommonInit(i);

	i->SelfSide = 1;
	i->Chara = TaisenInfo.Chara[1];
	i->Basho = BASHO_SUNFLOWER; // dummy

	i->SendIP[0] = dachi->IP[0];
	i->SendIP[1] = dachi->IP[1];
	i->SendIP[2] = dachi->IP[2];
	i->SendIP[3] = dachi->IP[3];
	i->SendPort = dachi->Port;

	i->TokushuButtle = TaisenInfo.TokushuButtle;
	i->TBEnable = tbEnable;

	NtUDPRecvBuffClear();
	osi->Syn = 0;

	for(int frmidx = 0; ; frmidx++)
	{
		int frmsec = frmidx / 60;

		TntCls();
		MyPrint_x(xcout("　応答を待っています ... %d:%02d", frmsec / 60, frmsec % 60));
		MyPrint("");
		MyPrint("　中断するにはキャンセルキー(ボタン)を押して下さい。");

		ExecMyPrint();
		SwapFrame();
		int cStatus = GetClick();

		if(GetHit(INP_ROT_CANCEL) || cStatus == 2)
		{
			goto endfunc;
		}
		if(frmidx % 100 == 10)
		{
			NtSendCommonInfo();
		}
		NtRecvCommonInfo();

		if(osi->Syn)
		if(osi->SelfSide == 0) // ? 相手はサーバーである。自分に接続出来てしまう問題への対策
		{
			break;
		}
	}
	i->AckSyn = 1;
	i->OSCredentials = osi->Credentials;

	i->IP[0] = osi->SendIP[0];
	i->IP[1] = osi->SendIP[1];
	i->IP[2] = osi->SendIP[2];
	i->IP[3] = osi->SendIP[3];

	strcpy(dachi->Name, osi->Name);

	for(int frmidx = 0; ; frmidx++)
	{
		TntCls();
		MyPrint("　接続中 ...");
		MyPrint("");
		MyPrint("　中断するにはキャンセルキー(ボタン)を押して下さい。");

		ExecMyPrint();
		SwapFrame();
		int cStatus = GetClick();

		if(GetHit(INP_ROT_CANCEL) || cStatus == 2)
		{
			goto endfunc;
		}
		if(frmidx % 100 == 10)
		{
			NtSendCommonInfo();
		}
		NtRecvCommonInfo();

		if(osi->AckSyn)
		{
			break;
		}
	}
	i->Ack = 1;

	for(int i = 0; i < 100; i++) // HACK: 受信を確認できないが、確実に Ack を送る必要がある。
	{
		TntCls();
		MyPrint("　送信中 ...");
		ExecMyPrint();
		SwapFrame();

		NtSendCommonInfo();
	}
	clsDx();
//	TntCls();
//	printfDx("　同期しています ...\n");
//	SwapFrame();

	TaisenInfo_t *t = &TaisenInfo;

	t->Chara[0] = osi->Chara;
	t->Chara[1] = i->Chara;
	t->Basho = osi->Basho;
	t->UseAI = 0; // 2bs

	Pzdc.NetworkMode = 1;
	Init_Rnd(osi->Seed);

	Taisen(); // クライアントモード

endfunc:
	NtFnlz();
}
static void ClientKansenWait(NtTomodachiInfo_t *dachi)
{
	NtCommonInfo_t *i = NtCommonInfo + 0;
	NtCommonInfo_t *osi = NtCommonInfo + 1;

restart:
	NtInit(SettInfo.NtRecvPort);
	NtSetSendIPPort(dachi->IP, dachi->Port);
	NtCommonInfo[0].OSCredentials = -1;

	for(int frmidx = 0; ; frmidx++)
	{
		int frmsec = frmidx / 60;

		TntCls();
		MyPrint_x(xcout("　観戦エントリー中 ... %d:%02d", frmsec / 60, frmsec % 60));
		MyPrint("");
		MyPrint("　中断するにはキャンセルキー(ボタン)を押して下さい。");

		ExecMyPrint();
		SwapFrame();
		int cStatus = GetClick();

		if(GetHit(INP_ROT_CANCEL) || cStatus == 2)
		{
			goto endfunc;
		}
		if(frmidx % 60 == 0 ||
			frmidx ==  5 ||
			frmidx == 10 ||
			frmidx == 15
			)
		{
			strcpy(NtKansenRequest.BuiltStamp, GetBuiltStamp());
			NtKansenRequest.Port = SettInfo.NtRecvPort;

			NtSendKansenRequest();
		}
		NtRecvKansenOkRet();

		if(NtKansenOkRet.Ok) // ? エントリー成功
		{
			int bk_sh = NtCommonInfo[0].SockHandle;

			NtCommonInfo[0] = NtKansenOkRet.Info[0];
			NtCommonInfo[1] = NtKansenOkRet.Info[1];

			/*
				注意 ... CI[0] 側 SockHandle, OSCredentials は上書きする！
			*/
			NtCommonInfo[0].SockHandle = bk_sh;
			NtCommonInfo[0].OSCredentials = NtCommonInfo[0].Credentials;
			break;
		}
	}
	clsDx();

	TaisenInfo_t *t = &TaisenInfo;

	t->Chara[0] = i->Chara;
	t->Chara[1] = osi->Chara;
	t->Basho = i->Basho;
	t->UseAI = 0; // 2bs

	t->TokushuButtle = i->TokushuButtle;

	Pzdc.NetworkMode = 1;
	Pzdc.NetKansenMode = 1;
	Init_Rnd(i->Seed);

	Taisen(); // 観戦モード

	memset(&t->TokushuButtle, 0x00, sizeof(TokushuButtle_t)); // 設定した側ではないので、ゼロに戻す。

	for(int frmidx = 0; frmidx < 90; frmidx++)
	{
		TntCls();
		MyPrint_x(xcout("　観戦エントリーを再開します ... %f / 1.5", (double)frmidx / 60));
		MyPrint("");
		MyPrint("　中断するにはキャンセルキー(ボタン)を押して下さい。");

		ExecMyPrint();
		SwapFrame();
		int cStatus = GetClick();

		if(GetHit(INP_ROT_CANCEL) || cStatus == 2)
		{
			goto endfunc;
		}
	}
	NtFnlz();
	goto restart;

endfunc:
	NtFnlz();
}

// --------------------
// ここからメニュー ...
// --------------------

static void NetworkServer_P2(void)
{
	int lastSel = 0;

	for(; ; )
	{
		MenuSetTitle("サーバーモード");

		MenuAddItem("待ち受け開始");
		MenuAddItem("キャラ変更");
		MenuAddItem("場所変更");
		MenuAddItem("戻る");

		switch(lastSel = Menu(lastSel))
		{
		case 0:
			ServerWaitForClient();
			break;

		case 1:
			TaisenInfo.Chara[0] = CharaSelect(TaisenInfo.Chara[0]);
			break;

		case 2:
			TaisenInfo.Basho = BashoSelect(TaisenInfo.Basho);
			break;

		case 3:
			goto end_menu;

		default:
			error();
		}
	}
end_menu:;
}
static void NetworkServer(void)
{
	int lastSel = 0;
	char *item_p = NULL;
	char *item_n = NULL;
	char *item_k = NULL;

	for(; ; )
	{
		MenuSetTitle("サーバーモード／通信の設定");

		memFree(item_p);
		memFree(item_n);
		memFree(item_k);

		MenuAddItem("この設定で通信する");
		MenuAddItem(item_p = xcout("待ち受けポート番号の変更 : [%d]", SettInfo.NtRecvPort));
		MenuAddItem(item_n = xcout("ニックネームの変更 : %s", SettInfo.NtName));
		MenuAddItem(item_k = xcout("あなたの対戦を他のユーザーが観戦することを許可%s", SettInfo.NtKansenOk ? "する" : "しない"));
		MenuAddItem("戻る");

		switch(lastSel = Menu(lastSel))
		{
		case 0:
			NetworkServer_P2();
			break;

		case 1:
			SettInfo.NtRecvPort = InputValue(NT_MIN_PORT, NT_MAX_PORT, "待ち受けポート番号", NT_DEF_PORT);
			break;

		case 2:
			InputLine(SettInfo.NtName, NT_NAME_LENMAX, NT_DEF_NAME, "あなたのニックネームを入力して下さい (この名前は公開されます) :");
			break;

		case 3:
			SettInfo.NtKansenOk = !SettInfo.NtKansenOk;
			break;

		case 4:
			goto end_menu;

		default:
			error();
		}
	}
end_menu:

	memFree(item_p);
	memFree(item_n);
	memFree(item_k);
}

static NtTomodachiInfo_t *NCP3_Dachi;

static void NCP3_Perform(void)
{
	ClientWaitForServer(NCP3_Dachi, 1);
}
static void NetworkClient_P3(NtTomodachiInfo_t *dachi)
{
	int lastSel = 0;

	for(; ; )
	{
		MenuSetTitle("クライアントモード");

		MenuAddItem("接続開始");
		MenuAddItem("接続開始 (エキストラ)");
		MenuAddItem("キャラ変更");
		MenuAddItem("観戦モード");
		MenuAddItem("戻る");

		switch(lastSel = Menu(lastSel))
		{
		case 0:
			ClientWaitForServer(dachi, 0);
			break;

		case 1:
			{
				NCP3_Dachi = dachi;

				ST_Perform = NCP3_Perform;
				SpecialTaisen();
				ST_Perform = NULL;

				NCP3_Dachi = NULL;
			}
			break;

		case 2:
			TaisenInfo.Chara[1] = CharaSelect(TaisenInfo.Chara[1]);
			break;

		case 3:
			ClientKansenWait(dachi);
			break;

		case 4:
			goto end_menu;

		default:
			error();
		}
	}
end_menu:;
}
static void NetworkClient_P2(NtTomodachiInfo_t *dachi)
{
	int lastSel = 0;
	char *item_a = NULL;
	char *item_p = NULL;
	char *item_q = NULL;
	char *item_n = NULL;

	for(; ; )
	{
		MenuSetTitle("クライアントモード／通信の設定");

		memFree(item_a);
		memFree(item_p);
		memFree(item_q);
		memFree(item_n);

		MenuAddItem("この設定で通信する");
		MenuAddItem(item_a = xcout("接続先ＩＰアドレスの変更 : %u.%u.%u.%u", dachi->IP[0], dachi->IP[1], dachi->IP[2], dachi->IP[3]));
		MenuAddItem(item_p = xcout("接続先ポート番号の変更 : [%d]", dachi->Port));
		MenuAddItem(item_q = xcout("こちらのポート番号の変更 : [%d]", SettInfo.NtRecvPort));
		MenuAddItem(item_n = xcout("あなたのニックネームの変更 : %s", SettInfo.NtName));
		MenuAddItem("戻る");

		switch(lastSel = Menu(lastSel))
		{
		case 0:
			NetworkClient_P3(dachi);
			break;

		case 1:
			{
				char buffer[16];

				sprintf(buffer, "%u.%u.%u.%u", dachi->IP[0], dachi->IP[1], dachi->IP[2], dachi->IP[3]);
				InputLine(buffer, sizeof(buffer) - 1, "", "接続先ＩＰアドレスを入力して下さい :");
				*(uint *)dachi->IP = 0;

				for(int index = 0; index < 4; index++)
				{
					char *lval = strtok(index ? NULL : buffer, ".");

					if(!lval)
						break;

					int val = atoi(lval);
					m_range(val, 0, 255);

					dachi->IP[index] = val;
				}
				if(!*(uint *)dachi->IP)
				{
					dachi->IP[0] = NT_DEF_IP1;
					dachi->IP[1] = NT_DEF_IP2;
					dachi->IP[2] = NT_DEF_IP3;
					dachi->IP[3] = NT_DEF_IP4;
				}
			}
			break;

		case 2:
			dachi->Port = InputValue(NT_MIN_PORT, NT_MAX_PORT, "接続先ポート番号を入力して下さい :", NT_DEF_PORT);
			break;

		case 3:
			SettInfo.NtRecvPort = InputValue(NT_MIN_PORT, NT_MAX_PORT, "こちらのポート番号", NT_DEF_PORT);
			break;

		case 4:
			InputLine(SettInfo.NtName, NT_NAME_LENMAX, NT_DEF_NAME, "あなたのニックネームを入力して下さい (この名前は相手に通知します) :");
			break;

		case 5:
			goto end_menu;

		default:
			error();
		}
	}
end_menu:

	memFree(item_a);
	memFree(item_p);
	memFree(item_q);
	memFree(item_n);
}
static void NetworkClient(void)
{
	int lastSel = 0;
	char *items[NT_TOMODACHI_NUM];

	memset(&items, 0x00, sizeof(items));

	for(; ; )
	{
		MenuSetTitle("クライアントモード／接続先の選択");

		ItemTsumero = 1;

		for(int index = 0; index < NT_TOMODACHI_NUM; index++)
		{
			NtTomodachiInfo_t *dachi = SettInfo.NtTomodachiList + index;

			memFree(items[index]);

			items[index] = xcout(
				"%02d: %s (%u.%u.%u.%u:%d)"
				,index + 1
				,dachi->Name
				,dachi->IP[0]
				,dachi->IP[1]
				,dachi->IP[2]
				,dachi->IP[3]
				,dachi->Port
				);

			MenuAddItem(items[index]);
		}
		MenuAddItem("戻る");

		int selidx = Menu(lastSel);
		lastSel = selidx;

		if(0 <= selidx && selidx < NT_TOMODACHI_NUM)
		{
			NetworkClient_P2(SettInfo.NtTomodachiList + selidx);
			goto continue_loop;
		}
		selidx -= NT_TOMODACHI_NUM;

		switch(selidx)
		{
		case 0:
			goto end_menu;

		default:
			error();
		}
continue_loop:;
	}
end_menu:

	for(int index = 0; index < NT_TOMODACHI_NUM; index++)
	{
		memFree(items[index]);
	}
}
static void NetworkTaisen(void)
{
	int lastSel = 0;

	for(; ; )
	{
		MenuSetTitle("ネットワーク対戦");

		MenuAddItem("サーバーを立てる");
		MenuAddItem("ＩＰアドレスとポート番号を指定してサーバーに接続する(クライアントモード)");
		MenuAddItem("戻る");

		switch(lastSel = Menu(lastSel))
		{
		case 0:
			NetworkServer();
			break;

		case 1:
			NetworkClient();
			break;

		case 2:
			goto end_menu;

		default:
			error();
		}
	}
end_menu:;
}
