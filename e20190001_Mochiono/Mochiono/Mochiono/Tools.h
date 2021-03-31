typedef struct ExtraErrorInfo_st
{
	int ScenarioLineCnt;
	int RetVal;
}
ExtraErrorInfo_t;
extern ExtraErrorInfo_t ExtraErrorInfo;

void termination(int errorLevel);
void error2(char *source, int lineno, char *function);

#define error() \
	error2(__FILE__, __LINE__, __FUNCTION__)

#define errorCase(status) \
	do { \
	if((status)) error(); \
	} while(0)

void *memAlloc(int size);
void *memCalloc(int size);
void memFree(void *block);

char *xcout(char *format, ...);
char *strx(char *line);
char *addLine(char *line, char *lineAdd);
char *addChar(char *line, int chrAdd);

void Init_Rnd(int seed);
int Get_Rnd(int maxval);
double krnd(void);
double rnd(void);
double rndpm(void);
int rndbnd(int minval, int maxval);
int rndp1m1(void);

int DFOpen(char *file);
void DFClose(int fp);
int DFRead(int fp);
char *DFReadLine(int fp);

char *GetJStamp(time_t t);
void noop(void);

template <class Element_t>
void Bulldoze(Element_t *list, int listSize, Element_t pVal)
{
	errorCase(list == NULL);
	errorCase(listSize < 1);

	for(int i = 0; i < listSize; i++)
	{
		list[i] = pVal;
	}
}

template <class Element_t>
void Shuffle(Element_t *list, int listSize)
{
	errorCase(list == NULL);
	errorCase(listSize < 1);

	for(int i = 0; i < listSize - 1; i++)
	{
		int j = rndbnd(i, listSize - 1);

		if(i < j)
		{
			Element_t swap = list[i];

			list[i] = list[j];
			list[j] = swap;
		}
	}
}

char *ZenInt(int value);

void line2domain(char *line);
void line2domain_len(char *line, int lenmax);

void LogWrite(char *line, int value);

#define TOT() \
	LogWrite(__FILE__, __LINE__)

char *GetClipLine(void);

void MyCls(void);
void MyPrint(char *line);
void MyPrint_x(char *line);
void ExecMyPrint(void);
