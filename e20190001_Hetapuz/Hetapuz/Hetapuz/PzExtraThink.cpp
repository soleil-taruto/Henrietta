#include "StdInc.h"

__int64 PzThink_CT_SleepEndFrm;

static void RequestColor(int color1, int color2)
{
	if(color2 == -1) color2 = rndbnd(1, PUYO_COLOR_NUM - 1);

	errorCase(color1 < 1 || PUYO_COLOR_NUM <= color1);
	errorCase(color2 < 1 || PUYO_COLOR_NUM <= color2);

	Map_t *m   = Pzdc.MapList + 1;
	Map_t *mos = Pzdc.MapList + 0;

	if(m->ColorStreamIndex < mos->ColorStreamIndex) // ? 相手の方が先行している。-> シーケンスが、明らかに違ってしまう。
	{
		return;
	}
	if(COLOR_STREAM_LEN - 2 < m->ColorStreamIndex) // ? 書き込むスペース不足
	{
		return;
	}
	if(Pzdc.TokushuButtle.ColorMax != 0) // ? 減色モード
	{
		return;
	}

	// 連発防止
	{
		if(Pzdc.FrameCnt < PzThink_CT_SleepEndFrm)
		{
			return;
		}
		PzThink_CT_SleepEndFrm = Pzdc.FrameCnt + 60 * 2;
	}

	if(krnd() < 0.5)
	{
		int swap = color1;

		color1 = color2;
		color2 = swap;
	}
	Pzdc.ColorStream[m->ColorStreamIndex + 0] = color1;
	Pzdc.ColorStream[m->ColorStreamIndex + 1] = color2;
}

static double CT_CheckJoudanTsumiJoutai(PzABST_Case_t *i) // ret : < 0.0 == 連鎖不可
{
	double h = 0.0;
	int reach = 0;

	for(int x = 1; x < MAP_X_BOUND - 1; x++)
	{
		int uc = PzAI_CaseTable[x - 1][13];
		int cs[3];

		cs[0] = PzAI_CaseTable[x][10];
		cs[1] = PzAI_CaseTable[x][ 9];
		cs[2] = PzAI_CaseTable[x][ 8];

		if(uc == -1) // ? 左下が空間
		{
			if(cs[0] == 0 && cs[1] == 0 && cs[2] == 0) return -1.0; // 右上が全部お邪魔で埋まっている。
		}
		else // ? 左下、積んでいる。
		{
			if(uc == cs[0] || uc == cs[1] || uc == cs[2]) // ? 左下と同じ色がある。
			{
				h += 100.0;
				reach++;
			}
			else if(cs[2] != -1) // ? 左下と違う色で埋まっている。
			{
				return -1.0;
			}
			else if(cs[1] != -1) // ? 左下と違う色で埋まりそう。
			{
				RequestColor(uc, -1);
			}
		}
	}
	int jr = reach == MAP_X_BOUND - 2; // ? 上リーチ
	int ur;

	{
		int x;

		for(x = 0; x < MAP_X_BOUND; x++)
		{
			if(PzAI_CaseTable[x][11] == -1) // ? 空間
			{
				break;
			}
		}
		ur = x == MAP_X_BOUND; // ? 下リーチ
	}

	{
		int cs[2];
		int ks[2];

		cs[0] = PzAI_CaseTable[MAP_X_BOUND - 1][10];
		cs[1] = PzAI_CaseTable[MAP_X_BOUND - 1][ 9];

		ks[0] = PzAI_CaseTable[MAP_X_BOUND - 1][11];
		ks[1] = PzAI_CaseTable[MAP_X_BOUND - 2][11];

		if(jr && ur) // ? リーチ
		{
			h += 0.01; // リーチフラグ的な

			if(cs[0] == ks[0] && cs[1] == ks[1]) // ? 起爆
			{
				return 990000.0;
			}
			if(cs[0] == -1) // ? 起爆待ち
			{
				RequestColor(ks[0], ks[1]);
			}
		}
		if(cs[0] != -1) // ? 起爆が埋まっている。
		{
			return -1.0;
		}
	}
	if(1 <= i->RensaCount) // ? 消しちゃってる。
	{
		return -1.0;
	}
	return h;
}
static double CT_CheckTsumiJoutai(PzABST_Case_t *i) // ret : < 0.0 == 連鎖不可
{
	double h = 0.0;
	double jh;
	double kh;
	double mh;

	PzAI_MakeCaseTable(i);

	for(int x = 0; x < MAP_X_BOUND; x++)
	{
		int cs[3];

		cs[0] = PzAI_CaseTable[x][13];
		cs[1] = PzAI_CaseTable[x][12];
		cs[2] = PzAI_CaseTable[x][11];

		if(cs[0] == 0) return -1.0; // お邪魔
		if(cs[1] == 0) return -1.0; // お邪魔
		if(cs[2] == 0) return -1.0; // お邪魔

		if(cs[0] != -1) // 1段目は積んでいる。
		{
			if(cs[1] != -1 && cs[0] != cs[1]) return -1.0; // 2段目に違う色を積んでいる。
			if(cs[2] != -1 && cs[0] != cs[2]) return -1.0; // 3段目に違う色を積んでいる。
		}

		if(cs[0] != -1) h += 300.0;
		if(cs[1] != -1) h += 600.1;
		if(cs[2] != -1) h += 900.3;
	}
	for(int x = 0; x < MAP_X_BOUND - 1; x++)
	{
		int cs[2];

		cs[0] = PzAI_CaseTable[x + 0][13];
		cs[1] = PzAI_CaseTable[x + 1][13];

		if(cs[0] != -1 && cs[1] != -1) // ? 両方積んでいる。
		{
			if(cs[0] == cs[1]) // ? 隣同士同じ色を積んでいる。
			{
				return -1.0;
			}
		}
	}
	// 裏の支援
	{
		for(int x = 0; x < MAP_X_BOUND - 1; x++)
		{
			int cn[2];
			int cp[2];

			cn[0] = PzAI_CaseTable[x + 0][13];
			cn[1] = PzAI_CaseTable[x + 1][13];
			cp[0] = PzAI_CaseTable[x + 0][12];
			cp[1] = PzAI_CaseTable[x + 1][12];

			if(cn[0] != -1 && cn[1] != -1 && cp[0] == -1 && cp[1] == -1 && krnd() < 0.05)
			{
				RequestColor(cn[0], cn[1]);
			}
		}
		for(int x = 0; x < MAP_X_BOUND; x++)
		{
			int cs[3];
			int ss[2];

			cs[0] = PzAI_CaseTable[x][13];
			cs[1] = PzAI_CaseTable[x][12];
			cs[2] = PzAI_CaseTable[x][11];
			ss[0] = 1;
			ss[1] = 1;

			if(0 < x)               ss[0] = PzAI_CaseTable[x - 1][11];
			if(x < MAP_X_BOUND - 1) ss[1] = PzAI_CaseTable[x + 1][11];

			if(ss[0] != -1 && ss[1] != -1 && cs[0] != -1 && cs[2] == -1 && krnd() < 0.05) // 谷
			{
				if(cs[1] == -1)
				{
					RequestColor(cs[0], cs[0]);
				}
				else
				{
					RequestColor(cs[0], -1);
				}
			}
		}
	}
	jh = CT_CheckJoudanTsumiJoutai(i);

	PzAI_LRTurnCaseTable();

	kh = CT_CheckJoudanTsumiJoutai(i);
	mh = m_max(jh, kh);

	if(mh < 0.0)
	{
		return -1.0;
	}
	return h + mh;
}
static double CT_Hyouka;

static double ClockTower(PzABST_Case_t *i)
{
	double h = CT_CheckTsumiJoutai(i);

	CT_Hyouka = m_max(CT_Hyouka, h);

	if(h < 0.0) // ? 連鎖不可
	{
		h = PzThink_Lv4_Hyouka(i);
	}
	else
	{
		h += 990000.0 + PzThink_Lv4_Hyouka(i);
	}
	return h;
}
void PzThink_ClockTower(void)
{
	CT_Hyouka = -2.0;
	PzAI_Abstracter(ClockTower);
	PzAI.q.ClockTower.LastHyouka = CT_Hyouka;
}
