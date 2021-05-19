#define SCC_EVENT_MAX 1024

extern char *Scc_EventList[SCC_EVENT_MAX];

void Scc_ProcInit(void);
void Scc_ProcFnlz(void);

int Scc_GetEventIndex(char *eventLine);
char *Scc_GetEvent(int index);

void Scc_SetPuzzleParams(char *puzzTitle);
