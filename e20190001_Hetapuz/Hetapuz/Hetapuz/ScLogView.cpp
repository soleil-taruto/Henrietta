#include "StdInc.h"

#define LINE_MAX 65536
#define LINELEN_MAX 1024

#define DRAW_X 15
#define DRAW_Y 10
#define DRAW_Y_STEP 24

#define DISP_X_BOUND 35
#define DISP_Y_BOUND 24

static char **InitLines(void)
{
	char **lines = (char **)memAlloc(LINE_MAX * sizeof(char *));
	int index;

	for(index = 0; index < LINE_MAX; index++)
	{
		lines[index] = (char *)memAlloc(LINELEN_MAX + 1);
		strcpy(lines[index], "初期化エラー！");
	}
	return lines;
}
static char **Lines;
static int LineCount;

static void AddOneLine(char *line)
{
	errorCase(LINE_MAX <= LineCount);
	errorCase(LINELEN_MAX < strlen(line));

	strcpy(Lines[LineCount], line);

	LineCount++;
}
static void AddLine(char *line) // line : 全角のみ
{
	const int divcnt = DISP_X_BOUND * 2;
	char *p = line;

	while(divcnt < strlen(p))
	{
		char *ep = p + divcnt;
		int echr = *ep;

		*ep = '\0';
		AddOneLine(p);
		*ep = echr;

		p = ep;
	}
	AddOneLine(p);
}
static void LoadLines(int staidx, int endaidx)
{
	LineCount = 0;

	for(int index = staidx; index < endaidx; index++)
	{
		ScAction_t *a = Scdc->Scenario.ActLst + index;

		char *line = xcout("%s%s", a->CharaName, a->Message);
		char *p = line;

		if(*p)
		{
			for(; ; )
			{
				char *ep = strchr(p, '\n');

				if(!ep)
					break;

				*ep = '\0';
				AddLine(p);
				p = ep + 1;
			}
			if(*p)
				AddLine(p);

			AddLine("");
		}
		memFree(line);
	}
}

static char *GetLine(int index)
{
	return 0 <= index && index < LineCount ? Lines[index] : "";
}

void ScLogView(void (*funcDrawWall)(void))
{
	int endaidx = Scdc->ActionIndex; // 表示する最後のページの次のページ

	if(endaidx == 0)
		return;

	if(!Lines)
		Lines = InitLines();

	LoadLines(0, endaidx);
	Dc->BlackCurtain = 0.8;

	// font
	LDrawRGB = 0x00ffffcc;

	int currIndex = m_max(0, LineCount - DISP_Y_BOUND);
	int koroSleep = 5;

	for(; ; )
	{
		funcDrawWall();
		ExecFrameCurtain();

		for(int row = 0; row < DISP_Y_BOUND; row++)
		{
			LDraw(GetLine(currIndex + row),
				DRAW_X,
				DRAW_Y + DRAW_Y_STEP * row
				);
		}
		SwapFrame();

		int koro = GetMouseWheelRotVol();

		if( koroSleep) {
			koroSleep--;
			koro = 0;
		}
		int cStatus = GetClick();

		if(GetHit(INP_DIR_4) || GetRendaHit(INP_DIR_8) || 0 < koro) // 上へ
		{
			currIndex--;
		}
		if(GetHit(INP_DIR_6) || GetRendaHit(INP_DIR_2) || koro < 0) // 下へ
		{
			currIndex++;
		}

		if(m_max(0, LineCount - DISP_Y_BOUND) < currIndex)
		{
			break;
		}
		currIndex = m_max(currIndex, 0);

		if(GetHit(INP_ROT_DECIDE) || GetHit(INP_ROT_CANCEL) || cStatus)
		{
			break;
		}
	}
	Dc->BlackCurtain = 0.0;
}
