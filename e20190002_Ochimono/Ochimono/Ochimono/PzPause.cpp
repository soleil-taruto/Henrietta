#include "StdInc.h"

#define PAUSE_CURTAIN 0.666
#define SELITEMNUM 4

#define SELRX 120
#define SELRY 20

static int SelItemX[SELITEMNUM];
static int SelItemY[SELITEMNUM];

void PauseMain()
{
	SelItemX[0] = 400;
	SelItemY[0] = 210;
	SelItemX[1] = 400;
	SelItemY[1] = 270;
	SelItemX[2] = 400;
	SelItemY[2] = 330;
	SelItemX[3] = 400;
	SelItemY[3] = 390;

	DispMusCursor(true);

	SwapFrame();
	Dc->BlackCurtain = PAUSE_CURTAIN;

	for(; ; )
	{
		int currsel = -1;

		for(int selidx = 0; selidx < SELITEMNUM; selidx++)
		{
			int mx;
			int my;

			GetMusPos(&mx, &my);

			if( abs(SelItemX[selidx] - mx) < SELRX &&
				abs(SelItemY[selidx] - my) < SELRY )
			{
				currsel = selidx;
			}
		}

		DrawGraph(0, 0, Pzdc.WallPaper, 0);
		ExecFrameCurtain();

		for(int selidx = 0; selidx < SELITEMNUM; selidx++)
		{
			DppInit(Dc->PicPauseItem[selidx][selidx == currsel ? 1 : 0], SelItemX[selidx], SelItemY[selidx], 1);
			DppDraw();
		}

		int movecursel = 0;
		if(GetRendaHit(INP_DIR_4) || GetRendaHit(INP_DIR_8)) // ã‚Ö
		{
			currsel--;
			movecursel = 1;
		}
		if(GetRendaHit(INP_DIR_2) || GetRendaHit(INP_DIR_6)) // ‰º‚Ö
		{
			currsel++;
			movecursel = 1;
		}
		if(movecursel)
		{
			m_range(currsel, 0, SELITEMNUM -1);
			SetMusPos(SelItemX[currsel] + SELRX - 1, SelItemY[currsel] + SELRY - 1);
		}

		if(GetHit(INP_START) || GetHit(INP_ROT_CANCEL))
		{
			break;
		}
		if(GetHit(INP_DECIDE) || GetHit(INP_ROT_DECIDE) || (GetClick() & 1))
		{
			if(currsel == 0) // Return to Game
			{
				break;
			}
			else if(currsel == 1) // Exit App
			{
				Pzdc.PauseSelExitGame = 1;
				PzdcRet.PauseSelExitApp = 1;
				break;
			}
			else if(currsel == 2) // Exit Game
			{
				Pzdc.PauseSelExitGame = 1;
				break;
			}
			else if(currsel == 3) // Settings
			{
				SwapFrame();
				Dc->BlackCurtain = 1.0;
				WaitToCurtain(Pzdc.WallPaper);

				Dc->BlackCurtain = 0.0;
				Dc->BCPct = 0.0;

				DoConfigPz();

				Dc->BlackCurtain = PAUSE_CURTAIN;
				Dc->BCPct = 1.0;
			}
		}
		SwapFrame();
	}
	Dc->BlackCurtain = 0.0;
	SwapFrame();

	DispMusCursor(false);
}
