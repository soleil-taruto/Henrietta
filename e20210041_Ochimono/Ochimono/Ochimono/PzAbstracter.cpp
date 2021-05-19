#include "StdInc.h"

PzABST_Result_t PzABST_Result;

// DoSecchi() ret:
static int WorkTable[MAP_X_BOUND][MAP_Y_BOUND]; // -1: 空, 0: 邪魔, 1～: 各色
static int WSeccX[2];
static int WSeccY[2];
static int WSeccColor[2];

// MakeRelateMap() ret:
static int RelateMap[MAP_X_BOUND][MAP_Y_BOUND];
static int RelCount;

static int GetWTColor(int x, int y, int outColor)
{
	if(x < 0 || MAP_X_BOUND <= x) return outColor;
	if(y < 0 || MAP_Y_BOUND <= y) return outColor;

	return WorkTable[x][y];
}
static int GetSecchiY(int x)
{
	int y;

	for(y = 1; y < MAP_Y_BOUND; y++)
	{
		if(WorkTable[x][y] != -1) // ? 空ではない
		{
			break;
		}
	}
	return y - 1;
}
static void DoSecchi(int x, int dir, int color1, int color2)
{
	int x1;
	int x2;
	int y1;
	int y2;

	SetDir(dir);
	x1 = x;
	x2 = x + DirX;

	if(DirY == 1)
	{
		DoSecchi(x, 8, color2, color1);
		return;
	}
	errorCase(x1 < 0 || MAP_X_BOUND <= x1);
	errorCase(x2 < 0 || MAP_X_BOUND <= x2);

	y1 = GetSecchiY(x1); WorkTable[x1][y1] = color1;
	y2 = GetSecchiY(x2); WorkTable[x2][y2] = color2;

	WSeccX[0] = x1;
	WSeccX[1] = x2;
	WSeccY[0] = y1;
	WSeccY[1] = y2;
	WSeccColor[0] = color1;
	WSeccColor[1] = color2;
}
static void MRM_Check(int x, int y, int color)
{
	// ? 枠外
	if( x < 0 || MAP_X_BOUND <= x ||
		y < 0 || MAP_Y_BOUND <= y ) return;

	if(RelateMap[x][y]) // ? もう見た。
		return;

	if(WorkTable[x][y] != color) // ? 色が違う
		return;

	RelateMap[x][y] = 1;
	RelCount++;

	MRM_Check(x - 1, y,     color);
	MRM_Check(x + 1, y,     color);
	MRM_Check(x,     y - 1, color);
	MRM_Check(x,     y + 1, color);
}
static void MakeRelateMap(int startX, int startY)
{
	memset(RelateMap, 0x00, sizeof(RelateMap));
	RelCount = 0;

	int startColor = WorkTable[startX][startY];

	if(startColor <= 0) // ? 空 || 邪魔 -> 消えないもの
	{
		return;
	}
	MRM_Check(startX, startY, startColor);
}
static void ERJC_Check(int x, int y)
{
	// ? 枠外
	if( x < 0 || MAP_X_BOUND <= x ||
		y < 0 || MAP_Y_BOUND <= y ) return;

	if(WorkTable[x][y] == 0) // ? 邪魔
	{
		RelateMap[x][y] = 1;
	}
}
static void ExpandRelateJamaCells(void)
{
	for(int x = 0; x < MAP_X_BOUND; x++)
	for(int y = 0; y < MAP_Y_BOUND; y++)
	{
		if(RelateMap[x][y] && WorkTable[x][y] != 0) // ? 消える予定 && 邪魔ではない
		{
			ERJC_Check(x - 1, y    );
			ERJC_Check(x + 1, y    );
			ERJC_Check(x,     y - 1);
			ERJC_Check(x,     y + 1);
		}
	}
}
static int ERC_EraseCount;
static int ERC_JEraseCount;
static void EraseRelateCells(void)
{
	ERC_EraseCount  = 0;
	ERC_JEraseCount = 0;

	for(int x = 0; x < MAP_X_BOUND; x++)
	for(int y = 0; y < MAP_Y_BOUND; y++)
	{
		if(RelateMap[x][y]) // ? 消えるもの
		{
			if(WorkTable[x][y] == 0) // ? 邪魔
				ERC_JEraseCount++;

			WorkTable[x][y] = -1;
			ERC_EraseCount++;
		}
	}
}
static void DoReSecchi(void)
{
	for(int x = 0; x < MAP_X_BOUND; x++)
	{
		int wy = MAP_Y_BOUND - 1;

		for(int y = MAP_Y_BOUND - 1; 0 <= y; y--)
		{
			if(WorkTable[x][y] != -1)
			{
				WorkTable[x][wy] = WorkTable[x][y];
				wy--;
			}
		}
		for(; 0 <= wy; wy--)
		{
			WorkTable[x][wy] = -1;
		}
	}
}
static void Simulation(int x, int dir)
{
	PzABST_Result_t *i = &PzABST_Result;
	PzABST_Case_t *c = &(i->CaseTable[x][dir]);

	c->PurX = x;
	c->PurDir = dir;

	if( dir != 2 &&
		dir != 4 &&
		dir != 6 &&
		dir != 8
		)
		return;

	if(x == 0 && dir == 4) return;
	if(x == 5 && dir == 6) return;

	c->Enable = 1;

	memcpy(WorkTable, i->Table, sizeof(i->Table));
	DoSecchi(x, dir, i->RakkaColors[0], i->RakkaColors[1]);

	c->SecchiX[0] = WSeccX[0];
	c->SecchiX[1] = WSeccX[1];
	c->SecchiY[0] = WSeccY[0];
	c->SecchiY[1] = WSeccY[1];

	c->SecchiTakasa[0] = MAP_Y_BOUND - WSeccY[0];
	c->SecchiTakasa[1] = MAP_Y_BOUND - WSeccY[1];

	c->Takasa = m_max(c->SecchiTakasa[0], c->SecchiTakasa[1]);

	c->SecchiColor[0] = WSeccColor[0];
	c->SecchiColor[1] = WSeccColor[1];

	for(; ; )
	{
		int erased = 0;

		for(int ix = 0; ix < MAP_X_BOUND; ix++)
		for(int iy = 0; iy < MAP_Y_BOUND; iy++)
		{
			// 初回は落としたブロックのみ調べる。
			if(c->RensaCount == 0 &&
				!(ix == c->SecchiX[0] && iy == c->SecchiY[0]) &&
				!(ix == c->SecchiX[1] && iy == c->SecchiY[1])
				)
				continue;

			MakeRelateMap(ix, iy);

			if(Pzdc.PuyoEraseMin <= RelCount)
			{
				ExpandRelateJamaCells();
				EraseRelateCells();
				c->EraseCount  += ERC_EraseCount;
				c->JEraseCount += ERC_JEraseCount;
				erased = 1;
			}
			if(c->RensaCount == 0)
			{
				c->RelateCount += RelCount;
			}
		}
		if(erased == 0)
		{
			break;
		}
		DoReSecchi();
		c->RensaCount++;
	}

	// Extra --->

	// 露出チェック
	{
		int roColors[PUYO_COLOR_NUM];

		memset(roColors, 0x00, sizeof(roColors));

		for(int ix = 0; ix < MAP_X_BOUND; ix++)
		{
			int iy;

			for(iy = 0; iy < MAP_Y_BOUND; iy++)
			{
				if(WorkTable[ix][iy] != -1)
				{
					break;
				}
			}
			if(iy == 0) // ? 完全に積み上がってる。
			{
				errorCase(ix < 0 || MAP_X_BOUND <= ix);
				errorCase(WorkTable[ix][0] == -1);
				continue;
			}
			iy--;

			errorCase(ix < 0 || MAP_X_BOUND <= ix);
			errorCase(iy < 0 || MAP_Y_BOUND <= iy);
			errorCase(WorkTable[ix][iy] != -1);

			int tnrcs[3];

			tnrcs[0] = GetWTColor(ix - 1, iy,     -1); // 左
			tnrcs[1] = GetWTColor(ix + 1, iy,     -1); // 右
			tnrcs[2] = GetWTColor(ix,     iy + 1, -1); // 下

			for(int ic = 0; ic < 3; ic++)
			{
				int tnrc = tnrcs[ic];

				errorCase(tnrc < -1 || PUYO_COLOR_NUM <= tnrc);

				if(tnrc != -1)
					roColors[tnrc] = 1;
			}
		}
		for(int ic = 1; ic < PUYO_COLOR_NUM; ic++) // 邪魔は数えない！
		{
			if(roColors[ic])
			{
				c->ExtraData.RoCount++;
			}
		}
	}

	/*
		ブロック毎の繋がりを評価
		連鎖時はブロックが動くので、意味のない値になるので注意！
		0 == 邪魔、空
		1 == 孤立
	*/
	MakeRelateMap(c->SecchiX[0], c->SecchiY[0]);
	c->ExtraData.RelSecchi[0] = RelCount;
	MakeRelateMap(c->SecchiX[1], c->SecchiY[1]);
	c->ExtraData.RelSecchi[1] = RelCount;

	// 縦・横並び評価
	{
		int tatenum = 0;
		int yokonum = 0;

		for(int ix = 0; ix < MAP_X_BOUND; ix++)
		for(int iy = 0; iy < MAP_Y_BOUND; iy++)
		{
			int koko = WorkTable[ix][iy];

			if(0 < koko) // ? カラー
			{
				if(ix < MAP_X_BOUND - 1 && koko == WorkTable[ix + 1][iy])
				{
					yokonum++;
				}
				if(iy < MAP_Y_BOUND - 1 && koko == WorkTable[ix][iy + 1])
				{
					tatenum++;
				}
			}
		}

		c->ExtraData.TateRelCnt = tatenum;
		c->ExtraData.YokoRelCnt = yokonum;
	}
}
void PzAbstracter(int mapIndex)
{
	errorCase(mapIndex < 0 || 1 < mapIndex);

	PzABST_Result_t *i = &PzABST_Result;
	Map_t *map = Pzdc.MapList + mapIndex;

	memset(&PzABST_Result, 0x00, sizeof(PzABST_Result_t));

	for(int x = 0; x < MAP_X_BOUND; x++)
	for(int y = 0; y < MAP_Y_BOUND; y++)
	{
		Puyo_t *puyo = map->PanelTable[x][y].Puyo;
		int color = -1;

		if(puyo != NULL)
		{
			color = puyo->Color;

			if(color == 0) // ? 邪魔
				i->JamaCount++;
			else
				i->PuyoCount++;
		}
		errorCase(color < -1 || PUYO_COLOR_NUM <= color);

		i->Table[x][y] = color;
	}
	errorCase(map->RakkaChuu[0] == NULL);
	errorCase(map->RakkaChuu[1] == NULL);

	i->RakkaColors[0] = map->RakkaChuu[0]->Color;
	i->RakkaColors[1] = map->RakkaChuu[1]->Color;

	for(int x = 0; x < MAP_X_BOUND; x++)
	for(int d = 0; d < 10; d++)
	{
		Simulation(x, d);
	}

	// Set i->Takasa*
	{
		int tmin =  2100000000;
		int tmax = -2100000000;

		for(int x = 0; x < MAP_X_BOUND; x++)
		{
			int y;
			for(y = 0; y < MAP_Y_BOUND; y++)
			{
				if(i->Table[x][y] != -1)
				{
					break;
				}
			}
			int t = MAP_Y_BOUND - y;

			tmin = m_min(tmin, t);
			tmax = m_max(tmax, t);

			i->TakasaList[x] = t;
		}
		i->TakasaMin = tmin;
		i->TakasaMax = tmax;
	}
}
void PzAllHyouka(double (*funcHyouka)(PzABST_Case_t *))
{
	for(int x = 0; x < MAP_X_BOUND; x++)
	for(int d = 0; d < 10; d++)
	{
		PzABST_Case_t *i = &(PzABST_Result.CaseTable[x][d]);

		if(i->Enable)
		{
			i->Hyouka = funcHyouka(i);
		}
	}
}
void PzGetMaxHyouka(void)
{
	int first = 1;
	double maxHyouka;
	int purX   = -1;
	int purDir = -1;

	for(int x = 0; x < MAP_X_BOUND; x++)
	for(int d = 0; d < 10; d++)
	{
		PzABST_Case_t *i = &(PzABST_Result.CaseTable[x][d]);

		if(i->Enable)
		{
			if(first || maxHyouka < i->Hyouka)
			{
				first = 0;
				maxHyouka = i->Hyouka;
				purX   = x;
				purDir = d;
			}
		}
	}

	PzABST_Result.PurX   = purX;
	PzABST_Result.PurDir = purDir;
}
