uint mutexOpen(char *mutexName);
void mutexClose(uint hdl);

uint mutexWaitForMillis(uint hdl, uint millis);
void mutexWaitForever(uint hdl);
void mutexRelease(uint hdl);

int ProcMutexLock(void);
void ProcMutexUnlock(void);
