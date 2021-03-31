#include "StdInc.h"

int DirRotClock(int dir)
{
	switch(dir)
	{
	case 2: dir = 4; break;
	case 4: dir = 8; break;
	case 6: dir = 2; break;
	case 8: dir = 6; break;

	default:
		error();
	}
	return dir;
}
int DirRotHanClock(int dir)
{
	return DirRotClock(DirRotClock(DirRotClock(dir)));
}
int DirRot(int dir, int rotDir)
{
	switch(rotDir)
	{
	case -1: dir = DirRotHanClock(dir); break;
	case  1: dir = DirRotClock(dir);    break;

	default:
		error();
	}
	return dir;
}

int DirX;
int DirY;
void SetDir(int dir)
{
	DirX = 0;
	DirY = 0;

	switch(dir)
	{
	case 2: DirY++; break;
	case 4: DirX--; break;
	case 6: DirX++; break;
	case 8: DirY--; break;

	default:
		error();
	}
}

int PairX[2];
int PairY[2];
void SetPairPos(int x, int y, int dir)
{
	PairX[0] = x;
	PairY[0] = y;

	SetDir(dir);

	PairX[1] = x + DirX;
	PairY[1] = y + DirY;
}

int Pzr_Dir;
int Pzr_RotDir; // -1 == 反時計, 1 == 時計
int Pzr_X;
double Pzr_Y;
Map_t *Pzr_Map;

static Puyo_t *GetPuyo(int x, int y)
{
	if(
		0 <= x && x < MAP_X_BOUND &&
		0 <= y && y < MAP_Y_BOUND
		)
	{
		return Pzr_Map->PanelTable[x][y].Puyo;
	}
	return (Puyo_t *)"Dummy";
}
void DoRotate(void)
{
	int jx = Pzr_X;
	int jy = (int)Pzr_Y;

	Pzr_Dir = DirRot(Pzr_Dir, Pzr_RotDir);
	SetPairPos(jx, jy, Pzr_Dir);

	if(GetPuyo(PairX[1], PairY[1])) // ? めり込む
	{
		if(Pzr_Dir == 4) // ? 左
		{
			if(GetPuyo(PairX[1] + 2, PairY[1]) == NULL)
			{
				Pzr_X++;
				goto endfunc;
			}
		}
		else if(Pzr_Dir == 6) // ? 右
		{
			if(GetPuyo(PairX[1] - 2, PairY[1]) == NULL)
			{
				Pzr_X--;
				goto endfunc;
			}
		}
		else
			goto adjust_tate;

		Pzr_Dir = DirRot(Pzr_Dir, Pzr_RotDir);
	}
adjust_tate:

	// このへんてきとー
	while(-10.0 < PairY[0])
	{
		SetPairPos(Pzr_X, (int)Pzr_Y, Pzr_Dir);

		if( !GetPuyo(PairX[0], PairY[0]) &&
			!GetPuyo(PairX[1], PairY[1])) break;

		Pzr_Y = (int)Pzr_Y - 1;
	}
	for(; ; )
	{
		SetPairPos(Pzr_X, (int)Pzr_Y, Pzr_Dir);

		if( 0 <= PairY[0] &&
			0 <= PairY[1]) break;

		Pzr_Y = (int)Pzr_Y + 1;
	}

endfunc:
	// check ...
	SetPairPos(Pzr_X, (int)Pzr_Y, Pzr_Dir);

	errorCase(PairX[0] < 0);
	errorCase(MAP_X_BOUND <= PairX[0]);
	errorCase(PairY[0] < 0);
	errorCase(MAP_Y_BOUND <= PairY[0]);

	errorCase(PairX[1] < 0);
	errorCase(MAP_X_BOUND <= PairX[1]);
	errorCase(PairY[1] < 0);
	errorCase(MAP_Y_BOUND <= PairY[1]);
}
