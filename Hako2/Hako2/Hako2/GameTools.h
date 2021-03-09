extern int sc_numer; // 0 - sc_denom
extern int sc_denom; // 1 - *
extern double sc_rate; // 0.0 - 1.0

void sceneEnter(void);
void sceneLeave(void);

void adjustSelectIndex(int &selectIndex, int selectMax);

/*
ex.
	forscene(100)
	{
		// sc_numer == (0 - 100), 101 ‰ñˆ—‚³‚ê‚éB
		EachFrame();
	}
	sceneLeave();
*/
// frameMax: (1 ` *)
#define forscene(frameMax) \
	for(sceneEnter(), sc_denom = (frameMax), sc_numer = 0; sc_numer <= (frameMax) && (sc_rate = (double)sc_numer / sc_denom, 1); sc_numer++)

/*
ex.
	ifsceneBegin(60, count)
	{
		// count == (0 - 60), sc_numer == (0 - 60), 61 ‰ñˆ—‚³‚ê‚éB
		EachFrame();
	}
	ifscene(90)
	{
		// count == (61 - 151), sc_numer == (0 - 90), 91 ‰ñˆ—‚³‚ê‚éB
		EachFrame();
	}
	ifscene(50)
	{
		// count == (152 - 202), sc_numer == (0 - 50), 51 ‰ñˆ—‚³‚ê‚éB
		EachFrame();
	}
	ifsceneEnd
	{
		// count == (203 - *), sc_numer == (0 - *)
		EachFrame();
	}
	sceneLeave();

seq.
	ifsceneBegin (1) -> ifscene (0*) -> ifsceneEnd (0*1) -> sceneLeave (1)
*/
// frameMax: (1 ` *), count: (0 ` *)
#define ifsceneBegin(frameMax, count) \
	if(sceneEnter(), sc_numer = (count), 0) { } ifscene((frameMax))

// frameMax: (1 ` *)
#define ifscene(frameMax) \
	else if(sc_numer <= (frameMax) ? (sc_denom = (frameMax), sc_rate = (double)sc_numer / sc_denom, 1) : (sc_numer -= (frameMax) + 1, 0))

#define ifsceneEnd \
	else

// ---- Curtain ----

extern double CurrCurtainWhiteLevel;

void CurtainEachFrame(int oncePerFrame = 1);
void SetCurtain(int frameMax = 30, double destWhiteLevel = 0.0, double startWhiteLevel = CurrCurtainWhiteLevel);

// ---- Print ----

typedef struct PrintExtra_st
{
	taskList *TL;
	int Color;

	// wrapped by PE_* -->

	int Border; // ˜gü‚Ì‘¾‚³, 0 == –³Œø
	int BorderColor;
	int FontSizeChanged;
	int FontSize;
	int FontTinck;
}
PrintExtra_t;

extern PrintExtra_t PE;

void PE_Border(int color = GetColor(0, 0, 0));
void PE_SetFontSize(int fontSize, int fontTinck = 6);
void PE_Reset(void);

void SetPrint(int x = 0, int y = 0, int yStep = 16);
void PrintRet(void);
void Print(char *line);
void Print_x(char *line);

// ----

void DrawCurtain(double whiteLevel = -1.0);
