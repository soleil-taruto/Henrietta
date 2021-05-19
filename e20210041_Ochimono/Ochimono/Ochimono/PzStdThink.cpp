#include "StdInc.h"

static double Lv0(PzABST_Case_t *i)
{
	return PzAI_WorstOfNext(i);
}
void PzThink_Lv0(void)
{
	PzAI_Abstracter(Lv0);
}

static double Lv1(PzABST_Case_t *i)
{
	double h = PzAI_BestOfNext(i);

	if(Pzdc.FrameCnt < 60 * 60 * 3)
	{
		if(2 <= i->RensaCount) h -= 990000.0;
	}
	else // timeout -> •‰‚¯‚é•ûŒü‚Ö
	{
		h = PzAI_WorstOfNext(i);
	}
	return h;
}
void PzThink_Lv1(void)
{
	PzAI_Abstracter(Lv1);
}

static double Lv2(PzABST_Case_t *i)
{
	double h = PzAI_BestOfNext(i);

	if(Pzdc.FrameCnt < 60 * 60 * 5)
	{
		if(3 <= i->RensaCount) h -= 990000.0;
	}
	else // timeout -> •‰‚¯‚é•ûŒü‚Ö
	{
		h = PzAI_WorstOfNext(i);
	}
	return h;
}
void PzThink_Lv2(void)
{
	PzAI_Abstracter(Lv2);
}

static double Lv3(PzABST_Case_t *i)
{
	return PzAI_BestOfNext(i);
}
void PzThink_Lv3(void)
{
	PzAI_Abstracter(Lv3);
}

static double Lv4(PzABST_Case_t *i)
{
	double h = PzAI_BestOfNext(i);

	if(PzAI_IsAnzen())
	{
		PzAI.q.Lv4.DangerMode = 0;
	}
	if(PzAI_IsDanger())
	{
		PzAI.q.Lv4.DangerMode = 1;
	}
	if(PzAI.q.Lv4.DangerMode == 0)
	{
		if(i->RensaCount == 1)
		{
			h -= 990000.0;
		}
	}
	return h;
}
void PzThink_Lv4(void)
{
	PzAI_Abstracter(Lv4);
}
double PzThink_Lv4_Hyouka(PzABST_Case_t *i)
{
	return Lv4(i);
}
