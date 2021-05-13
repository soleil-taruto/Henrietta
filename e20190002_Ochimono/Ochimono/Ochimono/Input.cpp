#include "StdInc.h"

Input_t InputList[2][INP_END]; // [1P, 2P][]

void InitInput(void)
{
	InputList[0][INP_DIR_2].Map_Key = KEY_INPUT_DOWN;
	InputList[0][INP_DIR_4].Map_Key = KEY_INPUT_LEFT;
	InputList[0][INP_DIR_6].Map_Key = KEY_INPUT_RIGHT;
	InputList[0][INP_DIR_8].Map_Key = KEY_INPUT_UP;
	InputList[0][INP_START].Map_Key = KEY_INPUT_SPACE;
	InputList[0][INP_DECIDE].Map_Key = KEY_INPUT_RETURN;
	InputList[0][INP_CANCEL].Map_Key = KEY_INPUT_DELETE;
	InputList[0][INP_ROT_L].Map_Key = KEY_INPUT_Z;
	InputList[0][INP_ROT_R].Map_Key = KEY_INPUT_X;
//	InputList[0][INP_BOMB].Map_Key = KEY_INPUT_C;

	InputList[0][INP_DIR_2].Map_Pad = PAD_INPUT_DOWN;
	InputList[0][INP_DIR_4].Map_Pad = PAD_INPUT_LEFT;
	InputList[0][INP_DIR_6].Map_Pad = PAD_INPUT_RIGHT;
	InputList[0][INP_DIR_8].Map_Pad = PAD_INPUT_UP;
	InputList[0][INP_START].Map_Pad = PAD_INPUT_START;
	InputList[0][INP_DECIDE].Map_Pad = 0; // PAD_INPUT_A;
	InputList[0][INP_CANCEL].Map_Pad = 0; // PAD_INPUT_B;
	InputList[0][INP_ROT_L].Map_Pad = PAD_INPUT_A;
	InputList[0][INP_ROT_R].Map_Pad = PAD_INPUT_B;
//	InputList[0][INP_BOMB].Map_Pad = PAD_INPUT_C;

	// Map_Pad は 0 でも問題ない。使用しないボタンは 0 のままで、、

	// ----
	// 2P
	InputList[1][INP_DIR_2].Map_Key = KEY_INPUT_NUMPAD2;
	InputList[1][INP_DIR_4].Map_Key = KEY_INPUT_NUMPAD1;
	InputList[1][INP_DIR_6].Map_Key = KEY_INPUT_NUMPAD3;
	InputList[1][INP_DIR_8].Map_Key = KEY_INPUT_NUMPAD5;

	// -- 固定なのでコピー --
	InputList[1][INP_START].Map_Key  = InputList[0][INP_START].Map_Key;
	InputList[1][INP_DECIDE].Map_Key = InputList[0][INP_DECIDE].Map_Key;
	InputList[1][INP_CANCEL].Map_Key = InputList[0][INP_CANCEL].Map_Key;
	// ----

	InputList[1][INP_ROT_L].Map_Key = KEY_INPUT_NUMPAD4;
	InputList[1][INP_ROT_R].Map_Key = KEY_INPUT_NUMPAD6;
//	InputList[1][INP_BOMB].Map_Key  = KEY_INPUT_NUMPAD0;
}
int GetMap(Input_t *i, int forPad)
{
	return forPad ? i->Map_Pad : i->Map_Key;
}
void SetMap(Input_t *i, int forPad, int map)
{
	*(forPad ? &i->Map_Pad : &i->Map_Key) = map;
}

#define RENDA_DELAY_1ST 14
#define RENDA_DELAY 4

void RefreshInput(void)
{
	for(int pSide = 0; pSide < 2; pSide++)
	for(int index = 0; index < INP_END; index++)
	{
		Input_t *i = InputList[pSide] + index;

		int k = CheckHitKey(i->Map_Key);
		int p = GetJoypadInputState(pSide == 0 ? DX_INPUT_PAD1 : DX_INPUT_PAD2) & i->Map_Pad;

		if(!GetActiveFlag()) // ? ウィンドウが非アクティブ
		{
			k = 0;
			p = 0;
		}

		// -- 特別 --
		if(index == INP_ROT_DECIDE)
		{
			k = k || CheckHitKey(KEY_INPUT_RETURN);
			k = k || CheckHitKey(KEY_INPUT_NUMPADENTER);
		}
		// ----

		i->Press = k || p;
		i->Hit = i->PressCount == 0 && i->Press;
		i->RendaHit = 0;

		if(i->Press)
		{
			int rc = i->PressCount;

			if(RENDA_DELAY_1ST <= rc)
			{
				rc -= RENDA_DELAY_1ST;
				rc %= RENDA_DELAY;
			}
			i->RendaHit = !rc;
		}

		if(i->Press)
			i->PressCount++;
		else
			i->PressCount = 0;
	}
}

int GetInput(int pSide, int index, int mode)
{
	errorCase(pSide < 0 || 1 < pSide);
	errorCase(index < 0 || INP_END <= index);

	Input_t *i = InputList[pSide] + index;
	int ret;

	switch(mode)
	{
	case 'P': ret = i->Press;    break;
	case 'H': ret = i->Hit;      break;
	case 'R': ret = i->RendaHit; break;

	default:
		error();
	}
	return ret;
}

int GetPressPS(int pSide, int index)
{
	return GetInput(pSide, index, 'P');
}
int GetHitPS(int pSide, int index)
{
	return GetInput(pSide, index, 'H');
}
int GetRendaHitPS(int pSide, int index)
{
	return GetInput(pSide, index, 'R');
}

int GetPress(int index)
{
	return GetPressPS(0, index);
}
int GetHit(int index)
{
	return GetHitPS(0, index);
}
int GetRendaHit(int index)
{
	return GetRendaHitPS(0, index);
}
