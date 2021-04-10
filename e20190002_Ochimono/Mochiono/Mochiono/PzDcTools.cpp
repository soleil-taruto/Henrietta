#include "StdInc.h"

Puyo_t *TakePuyo(void)
{
	Puyo_t **pp;

	errorCase(PUYO_SQ_SIZE <= Pzdc.PuyoStockIndex);
	pp = Pzdc.PuyoStock + Pzdc.PuyoStockIndex;

	if(!*pp)
		*pp = Pzdc.PuyoSequence + Pzdc.PuyoStockIndex;
	else
		memset(*pp, 0x00, sizeof(Puyo_t)); // as memCalloc()

	Pzdc.PuyoStockIndex++;
	return *pp;
}
void GivePuyo(Puyo_t *p)
{
	errorCase(!Pzdc.PuyoStockIndex);

	Pzdc.PuyoStockIndex--;
	Pzdc.PuyoStock[Pzdc.PuyoStockIndex] = p;
}

Puyo_t *CreatePuyo(int color, double realX, double realY, double trueX, double trueY)
{
	Puyo_t *i = TakePuyo();

	i->Color = color;
	i->A = 1.0;
	i->RotCnt = 0;

	i->RealX = realX;
	i->RealY = realY;

	i->TrueX = trueX;
	i->TrueY = trueY;

	i->Erase = 0;
	i->Death = 0;

	i->LastXAdd = 0.0;
	i->LastYAdd = 0.0;

	return i;
}
void ReleasePuyo(Puyo_t *i)
{
	GivePuyo(i);
}

int AddPuyo(Puyo_t *i)
{
	if(PUYO_NUMMAX <= Pzdc.PuyoCount) error();

	Pzdc.PuyoList[Pzdc.PuyoCount] = i;
	Pzdc.PuyoCount++;

	return Pzdc.PuyoCount - 1;
}
Puyo_t *UnaddPuyo(int index)
{
	Puyo_t *i;

	if(Pzdc.PuyoCount <= index) error();

	i = Pzdc.PuyoList[index];

	Pzdc.PuyoCount--;
	Pzdc.PuyoList[index] = Pzdc.PuyoList[Pzdc.PuyoCount];

	return i;
}

int GetNextColor(int mapidx)
{
	int color = Pzdc.ColorStream[Pzdc.MapList[mapidx].ColorStreamIndex];

	Pzdc.MapList[mapidx].ColorStreamIndex = (Pzdc.MapList[mapidx].ColorStreamIndex + 1) % COLOR_STREAM_LEN;

	return color;
}

int GetPuyoSenyuu(Map_t *map)
{
	int count = 0;

	for(int x = 0; x < MAP_X_BOUND; x++)
	for(int y = 0; y < MAP_Y_BOUND; y++)
	{
		if(map->PanelTable[x][y].Puyo) count++;
	}
	return count;
}
double GetPuyoSenyuuRit(Map_t *map)
{
	return (double)GetPuyoSenyuu(map) / (MAP_X_BOUND * MAP_Y_BOUND);
}
