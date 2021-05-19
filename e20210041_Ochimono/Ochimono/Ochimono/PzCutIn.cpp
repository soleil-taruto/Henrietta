#include "StdInc.h"

#define STD_SPEED_RT 0.8
#define HISS_SPEED_RT 0.7

static CutIn_t *TakeCutIn(int mapIndex, int side)
{
	for(int r_index = 0; r_index < CUTIN_NUMMAX; r_index++)
	{
		int index = (Pzdc.CutInIndex + r_index) % CUTIN_NUMMAX;

		CutIn_t *i = Pzdc.CutInList + index;

		if(i->Alive == 0)
		{
			memset(i, 0x00, sizeof(CutIn_t));

			i->Alive = 1;
			i->MapIndex = mapIndex;
			i->Side = side;
			i->Pic = Dc->PicCIStar; // Def

			i->X = -1000.0;
			i->Y = -1000.0;
			i->R = 0.0;
			i->Z = 1.0;
			i->A = 1.0;

			i->PrvRnd = rnd();

			Pzdc.CutInIndex = index + 1;
			return i;
		}
	}
	error();
	return NULL; // Dummy
}
static void ActCutIn(CutIn_t *i)
{
	return; // カットイン_廃止

	i->X += i->Xa;
	i->Y += i->Ya;
	i->R += i->Ra;
	i->Z += i->Za;
	i->A += i->Aa;

	m_range(i->Z, 0.0001, 1000.0);
	m_range(i->A, 0.0, 1.0);

	i->Xa += i->Xaa;
	i->Ya += i->Yaa;
	i->Ra += i->Raa;
	i->Za += i->Zaa;
	i->Aa += i->Aaa;

	// 描画
	{
	DppInit(i->Pic, i->X, i->Y, 1);
	DppRotate(i->R);
	DppZoom(i->Z);
	SetAlpha(i->A, i->KasanMode);
	DppDraw();
	ResetAlpha();
	}
}
CutIn_t *CIEx_TakeCutIn(int mapIndex, int side)
{
	return TakeCutIn(mapIndex, side);
}
void CIEx_ActCutIn(CutIn_t *i)
{
	ActCutIn(i);
}

static int ActStar(void *vi)
{
	CutIn_t *i = (CutIn_t *)vi;

	ActCutIn(i);

	if(Get_Rnd(100) == 1) // 跳ね返り
	{
		i->Ya = -abs(i->Ya);
		i->Ya *= 0.15; // K
	}

	if(700.0 < i->Y)
	{
		i->Alive = 0;
	}
	return i->Alive;
}
static CutIn_t *AddStar(void)
{
	CutIn_t *i = TakeCutIn(0, 0);

	i->X = Get_Rnd(8000) * 0.1;
	i->Y = - 30.0 - Get_Rnd(1000) * 0.3;
	i->R = 0.0;
	i->Z = (Get_Rnd(1000) / 1000.0) * 0.25 + 0.25;
	i->A = (Get_Rnd(1000) / 1000.0) * 0.5 + 0.5;

	i->Xa = ((Get_Rnd(1000) / 1000.0) - 0.5) * 7.0;
	i->Ya = ((Get_Rnd(1000) / 1000.0) - 0.0) * -7.5;
	i->Ra = ((Get_Rnd(1000) / 1000.0) - 0.5) * 0.5;

	i->Yaa = 0.4;
//	i->Yaa = 0.45;

	AddFrameTask(ActStar, i);
	return i;
}
static void AddStar2(int mapIndex, int num)
{
	for(int c = 0; c < num; c++)
	{
		CutIn_t *i = AddStar();

		i->X *= 0.5;
		i->X += mapIndex ? 400 : 0;
	}
}
void CIEx_AddStar(int num)
{
	for(int c = 0; c < num; c++)
	{
		AddStar();
	}
}

static int Rensa3(void *vi)
{
	CutIn_t *i = (CutIn_t *)vi;

	if(i->FrameCount == 0) // Init
	{
		// noop
	}
	i->FrameCount++;

	if(i->q.Rensa3.Jumped == 0 && 400.0 < i->Y) {
		i->q.Rensa3.Jumped = 1;

		i->Ya = -abs(i->Ya);
		i->Ya *= 0.5;
	}

	ActCutIn(i);
	return i->Alive = i->Y < 700.0;
}
static int Rensa5(void *vi)
{
	CutIn_t *i = (CutIn_t *)vi;

	if(i->FrameCount == 0) // Init
	{
		i->R = -0.75;
		i->Ra = 0.015 * STD_SPEED_RT;

		i->Z = 0.333;
		i->Za = 0.035 * STD_SPEED_RT;

		i->A = 0.1;
		i->Aa = 0.04 * STD_SPEED_RT;
	}
	i->FrameCount++;

	if(i->A == 1.0) {
		i->A = 1.0;

		i->Za = -0.003 * STD_SPEED_RT;
		i->Aa = -0.015 * STD_SPEED_RT;
	}

	ActCutIn(i);
	return i->Alive = i->A != 0.0;
}
static int Rensa7(void *vi)
{
	CutIn_t *i = (CutIn_t *)vi;

	if(i->FrameCount == 0) // Init
	{
		i->X = i->MapIndex ? 641 : 160;
		i->Y = 200;
		i->Ya = 3.0 * STD_SPEED_RT;

		i->A = 0.1;
		i->Aa = 0.04 * STD_SPEED_RT;
	}
	i->FrameCount++;

	if(i->q.Rensa7.MachiCount)
	{
		i->q.Rensa7.MachiCount--;
		return 1;
	}
	if(1.0 <= i->A)
	{
		i->Aa = -0.03 * STD_SPEED_RT;
	}

	ActCutIn(i);
	return i->Alive = i->A != 0.0;
}

void CI_AddCutIn(int rensanum, int mapIndex, int side) // side : 0 == 1P, 1 == 2P
{
	return; // カットイン_廃止

	errorCase(mapIndex < 0 && 1 < mapIndex);
	errorCase(side     < 0 && 1 < side);

	switch(rensanum)
	{
	case 3:
		{
			AddStar2(mapIndex, 40);

			CutIn_t *ia = TakeCutIn(mapIndex, side);
			CutIn_t *ib = TakeCutIn(mapIndex, side);

			switch(side)
			{
			case 0: // 1P
				ia->Pic = Dc->PicCI1P1Ren3;
				ib->Pic = Dc->PicCI1P2Ren3;
				break;

			case 1: // 2P
				ia->Pic = Dc->PicCI2P1Ren3;
				ib->Pic = Dc->PicCI2P2Ren3;
				break;

			default:
				error();
			}

			ia->X = 125;
			ib->X = 275;

			ia->Y = -10.0;
			ib->Y = -150.0;
			ia->Yaa = 1.0 * HISS_SPEED_RT;
			ib->Yaa = 1.0 * HISS_SPEED_RT;

			if(mapIndex == 1)
			{
				ia->X += 400;
				ib->X += 400;
			}

			if(Pzdc.IsSolo[mapIndex])
			{
				ia->X = (ia->X + ib->X) / 2.0;

				AddFrameTask(Rensa3, ia);
				ib->Alive = 0;
			}
			else
			{
				AddFrameTask(Rensa3, ia);
				AddFrameTask(Rensa3, ib);
			}
		}
		break;
	case 5:
		{
			AddStar2(mapIndex, 120);

			CutIn_t *ia = TakeCutIn(mapIndex, side);
			CutIn_t *ib = TakeCutIn(mapIndex, side);

			switch(side)
			{
			case 0: // 1P
				ia->Pic = Dc->PicCI1P1Ren5;
				ib->Pic = Dc->PicCI1P2Ren5;
				break;
			case 1: // 2P
				ia->Pic = Dc->PicCI2P1Ren5;
				ib->Pic = Dc->PicCI2P2Ren5;
				break;

			default:
				error();
			}
			ia->X = 125;
			ia->Y = 450;

			ib->X = 275;
			ib->Y = 150;

			if(mapIndex == 1)
			{
				ia->X += 400;
				ib->X += 400;
			}
			
			if(Pzdc.IsSolo[mapIndex])
			{
				ia->X = (ia->X + ib->X) / 2.0;
				ia->Y = (ia->Y + ib->Y) / 2.0;

				AddFrameTask(Rensa5, ia);
				ib->Alive = 0;
			}
			else
			{
				AddFrameTask(Rensa5, ia);
				AddFrameTask(Rensa5, ib);
			}
		}
		break;
	case 7:
		{
			CutIn_t *ia = TakeCutIn(mapIndex, side);
			CutIn_t *ib = TakeCutIn(mapIndex, side);

			switch(side)
			{
			case 0: // 1P
				ia->Pic = Dc->PicCI1P1Ren7;
				ib->Pic = Dc->PicCI1P2Ren7;
				break;
			case 1: // 2P
				ia->Pic = Dc->PicCI2P1Ren7;
				ib->Pic = Dc->PicCI2P2Ren7;
				break;

			default:
				error();
			}

			ib->q.Rensa7.MachiCount = (int)(30.0 / STD_SPEED_RT);

			if(Pzdc.IsSolo[mapIndex])
			{
				AddFrameTask(Rensa7, ia);
				ib->Alive = 0;
			}
			else
			{
				AddFrameTask(Rensa7, ia);
				AddFrameTask(Rensa7, ib);
			}
		}
		break;

	default:
		error();
	}
}

static int ActDamage(void *vi)
{
	CutIn_t *i = (CutIn_t *)vi;

	if(i->FrameCount == 0) // Init
	{
		if(i->MapIndex)
		{
			i->X += 400.0;
		}

		i->Xa = 7.5;
		i->Ya = 1.0;
		i->Yaa = 0.75;

//		i->Z = 0.5;

		i->A = 0.1;
		i->Aa = 0.1;

		i->FrameCount = 1;
	}

	if( i->q.Damage.MachiCount) {
		i->q.Damage.MachiCount--;
		return 1;
	}
	i->FrameCount++;

	if(i->FrameCount == 20)
	{
		i->Ya = -abs(i->Ya);
		i->Ya *= 0.5;
	}
	if(i->FrameCount == 40)
	{
		i->Aa = -0.1;
	}

	ActCutIn(i);
	return i->Alive = i->FrameCount < 50;
}
void CI_AddDamage(int mapIndex, int side)
{
	return; // カットイン_廃止

	errorCase(mapIndex < 0 && 1 < mapIndex);

	CutIn_t *ia = TakeCutIn(mapIndex, side);
	CutIn_t *ib = TakeCutIn(mapIndex, side);

	switch(side)
	{
	case 0: // 1P
		ia->Pic = Dc->PicCI1P1Damage;
		ib->Pic = Dc->PicCI1P2Damage;
		break;
	case 1: // 2P
		ia->Pic = Dc->PicCI2P1Damage;
		ib->Pic = Dc->PicCI2P2Damage;
		break;

	default:
		error();
	}
	ia->X = -10.0;
	ib->X = 0.0;

	ia->Y = 50.0;
	ib->Y = 350.0;

	ib->q.Damage.MachiCount = 5;

	if(Pzdc.IsSolo[mapIndex])
	{
		ia->X = (ia->X + ib->X) / 2.0;
		ia->Y = (ia->Y + ib->Y) / 2.0;

		AddFrameTask(ActDamage, ia);
		ib->Alive = 0;
	}
	else
	{
		AddFrameTask(ActDamage, ia);
		AddFrameTask(ActDamage, ib);
	}
}

static int ActJamaStar(void *vi)
{
	CutIn_t *i = (CutIn_t *)vi;

	double diffX = i->q.JamaStar.EndX - i->X;
	double diffY = i->q.JamaStar.EndY - i->Y;
	double remfar = sqrt(diffX * diffX + diffY * diffY);

	double r = m_min(remfar * 1.333, i->FrameCount * 10.0);
	r *= i->q.JamaStar.GuruHan;

	double bkX = i->X;
	double bkY = i->Y;

	i->X += sin(i->q.JamaStar.GuruR) * r;
	i->Y += cos(i->q.JamaStar.GuruR) * r;
	i->q.JamaStar.GuruR += i->q.JamaStar.GuruRa;

	i->FrameCount++;

	ActCutIn(i);

	i->X = bkX;
	i->Y = bkY;

	nearize(i->X, i->q.JamaStar.EndX, i->q.JamaStar.Speed);
	nearize(i->Y, i->q.JamaStar.EndY, i->q.JamaStar.Speed);

	if(remfar < 30.0)
	{
		int jcnt = i->q.JamaStar.JamaCountInfo.Count;
		int midx = i->q.JamaStar.JamaCountInfo.MapIndex;

		Pzdc.MapList[midx].JamaCount += jcnt;

		i->Alive = 0;
		return 0;
	}
	return 1;
}
void CI_AddJamaStar(int destJamaCount, int destMapIndex, double startX, double startY, double endX, double endY)
{
	CutIn_t *i = TakeCutIn(0, 0);

	i->Pic = Dc->PicEffeKagaya;

	i->X = startX;
	i->Y = startY;
	i->Z = rnd() * 0.15 + 0.1;
	i->Ra = rndpm() * 0.333;
	i->A = rnd() * 0.5 + 0.5;

	i->q.JamaStar.JamaCountInfo.Count = destJamaCount;
	i->q.JamaStar.JamaCountInfo.MapIndex = destMapIndex;

	i->q.JamaStar.EndX = endX;
	i->q.JamaStar.EndY = endY;

	i->q.JamaStar.Speed   = rnd() * 0.05 + 0.9;
	i->q.JamaStar.GuruR   = rndpm() * 3.0;
	i->q.JamaStar.GuruRa  = rndpm() * 0.25;
	i->q.JamaStar.GuruHan = rnd() * 1.25;

	AddFrameTask(ActJamaStar, i);
}

#define HISS_SPEED 165.0
#define HISS_SLOW_BRIT 0.075;
#define HISS_SLOW_ST_FRM ((int)( 7.0 / HISS_SPEED_RT))
#define HISS_SLOW_ED_FRM ((int)(25.0 / HISS_SPEED_RT))

static int ActHiss(void *vi)
{
	CutIn_t *i = (CutIn_t *)vi;

	if(i->FrameCount == HISS_SLOW_ST_FRM)   i->Xa *= HISS_SLOW_BRIT;
	if(i->FrameCount == HISS_SLOW_ED_FRM) { i->Xa /= HISS_SLOW_BRIT; Dc->BlackCurtain = 0.0; }

	ExecFrameCurtain();
	ActCutIn(i);

	i->FrameCount++;
	i->Alive = i->FrameCount < HISS_SLOW_ED_FRM || (-400 < i->X && i->X < 1200);

	if(i->Alive == 0)
	{
//		Dc->BlackCurtain = 0.0;
	}
	return i->Alive;
}
void CI_AddHiss(int mapIndex, PuzzPair_t pairSide)
{
	CutIn_t *i;
	PuzzPlayer_t hcs[2];
	int solomode = Pzdc.IsSolo[mapIndex];

	switch(pairSide)
	{
	case PZPAIR_1P:
		hcs[0] = PZPLYR_1P1;
		hcs[1] = PZPLYR_1P2;
		break;
	case PZPAIR_2P:
		hcs[0] = PZPLYR_2P1;
		hcs[1] = PZPLYR_2P2;
		break;

	default:
		error();
	}
	for(int cnt = 0; cnt < 2; cnt++)
	{
		if(cnt == 1 && solomode)
		{
			break;
		}
		i = TakeCutIn(0, 0);

		switch(hcs[cnt])
		{
		case PZPLYR_1P1: i->Pic = Dc->PicCIHiss1P1; break;
		case PZPLYR_1P2: i->Pic = Dc->PicCIHiss1P2; break;
		case PZPLYR_2P1: i->Pic = Dc->PicCIHiss2P1; break;
		case PZPLYR_2P2: i->Pic = Dc->PicCIHiss2P2; break;

		default:
			error();
		}
		i->X = -800.0;
		i->Y = 150.0;
		i->Xa = (int)((HISS_SPEED - 5.0) * HISS_SPEED_RT);

		if(cnt == 1 || (solomode && mapIndex == 1))
		{
			i->X = 800.0 - i->X;
			i->Y = 450.0;
			i->Xa = -i->Xa;
		}
		if(solomode)
		{
			i->Y = 300.0;
		}
		AddFrameTask(ActHiss, i);
	}
	int dirmode = 0; // -1: 常に左方向, 1: 常に右方向

	if(solomode)
	{
		if(mapIndex == 0) dirmode =  1;
		else              dirmode = -1;
	}
	for(int cnt = 0; cnt < 400; cnt++)
	{
		int backmode = cnt < 250;

		i = TakeCutIn(0, pairSide);
		i->Pic = rnd() < 0.5 ? Dc->PicEffeHana : Dc->PicEffeHoshi;
//		i->Pic = Dc->PicEffeHana;
//		i->Pic = Dc->PicEffeHoshi;

		double y = rnd();

		y *= y;
		y *= y;
		y *= y;
		y *= backmode ? 400.0 : 50.0;

		i->X = rnd() * -800.0 * 2.0;
		i->Y = y;
		i->Z = rnd() * 0.1 + 0.1;
		i->A = rnd() * 0.5 + 0.5;

		i->Xa = HISS_SPEED * HISS_SPEED_RT + rndpm() * 100.0;
		i->Ra = rnd();

		if(solomode)
		{
			i->Y /= 2.0;
			if(krnd() < 0.5) i->Y = -i->Y;
			i->Y += 150.0;
		}

		int revmode = 0;
		if(krnd() < 0.5)
		{
			i->Y = 600.0 - i->Y;
			revmode = 1;
		}
		if(dirmode == -1) revmode = 1;
		else              revmode = 0;
		if(revmode)
		{
			i->X = 800.0 - i->X;
			i->Xa = -i->Xa;
			i->Ra = -i->Ra;
		}
		AddFrameTaskLayer(ActHiss, i, backmode ? -1 : 1);
	}
	Dc->BlackCurtain = 0.9;
}

static int ActKirari(void *vi)
{
	// 使ってない。
}
void CI_AddKirari(double x, double y, int pic)
{
	// 使ってない。
}
