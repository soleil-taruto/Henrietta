#include "StdInc.h"

uint mutexOpen(char *mutexName)
{
	HANDLE hdl = CreateMutexA(NULL, FALSE, mutexName);

	if(hdl == NULL)
	{
		error();
	}
	return (uint)hdl;
}
void mutexClose(uint hdl)
{
	if(CloseHandle((HANDLE)hdl) == 0)
	{
		error();
	}
}

uint mutexWaitForMillis(uint hdl, uint millis)
{
	uint retval = WaitForSingleObject((HANDLE)hdl, millis);

	if(retval == WAIT_OBJECT_0)
	{
		return 1; // シグナル状態 (ロック成功)
	}
	if(retval == WAIT_TIMEOUT)
	{
		return 0;
	}
	error();
	return 0; // Dummy
}
void mutexWaitForever(uint hdl)
{
	if(!mutexWaitForMillis(hdl, INFINITE))
	{
		error();
	}
}
void mutexRelease(uint hdl)
{
	if(ReleaseMutex((HANDLE)hdl) == 0)
	{
		error();
	}
}

#define PROC_MUTEX_NAME "{3c6307e7-fc51-4b92-a31c-65fd039d2fe2}"

static uint ProcMutex;

int ProcMutexLock(void)
{
	ProcMutex = mutexOpen(PROC_MUTEX_NAME);

	if(!mutexWaitForMillis(ProcMutex, 0))
	{
		mutexClose(ProcMutex);
		ProcMutex = 0;
		return 0;
	}
	return 1;
}
void ProcMutexUnlock(void)
{
	if(ProcMutex)
	{
		mutexRelease(ProcMutex);
		mutexClose(ProcMutex);
		ProcMutex = 0;
	}
}
