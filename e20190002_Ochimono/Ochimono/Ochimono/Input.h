enum
{
	INP_DIR_2,
	INP_DIR_4,
	INP_DIR_6,
	INP_DIR_8,
	INP_START,
	INP_DECIDE,
	INP_CANCEL,
	INP_ROT_L,
	INP_ROT_R,
//	INP_BOMB, // •KŽE‹Z_”pŽ~

	INP_END,
};

#define INP_ROT_DECIDE INP_ROT_L
#define INP_ROT_CANCEL INP_ROT_R

typedef struct Input_st
{
	int Map_Key;
	int Map_Pad;

	int Press;
	int PressCount;

	int Hit;
	int RendaHit;
}
Input_t;

extern Input_t InputList[2][INP_END];

void InitInput(void);
int GetMap(Input_t *i, int forPad);
void SetMap(Input_t *i, int forPad, int map);
void RefreshInput(void);

int GetInput(int pSide, int index, int mode);
int GetPressPS(int pSide, int index);
int GetHitPS(int pSide, int index);
int GetRendaHitPS(int pSide, int index);

int GetPress(int index);
int GetHit(int index);
int GetRendaHit(int index);
