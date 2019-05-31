#include "C:\Users\Yumi\Desktop\wcs\Vibration\timer.h"

/**************************************************************************/
/*! 
	Scheduling class to make scheduling easier
*/
/**************************************************************************/


uint32_t Timer::getMillis(void){
	clock_t ticks;
	ticks = clock();
	uint32_t millis = ((uint32_t)ticks / CLOCKS_PER_SEC)*1000;
}

#define SCHEDULER_MAX_ENTRIES 30

typedef struct {
	uint32_t id;
	uint32_t nextCall;
	uint32_t interval;
	void (*callback) (void);
	_Bool deleted;
} SchedulerData_t;

SchedulerData_t schedulerData[SCHEDULER_MAX_ENTRIES];
uint32_t schedulerNextFreeEntry = 0;
uint32_t schedulerNextEvent = 0xFFFFFFFF;
uint32_t schedulerNextUId = 1;

void schedulerInit(RunMode_t mode)
{
	if (mode == PREPARE_SLEEP || mode == RESUME)
	{
		schedulerUnregisterAll();
	}
}

void schedulerCheck(void)
{
	uint32_t currentTime = time();
	if (currentTime < schedulerNextEvent)
	{
		return;
	}
	for (uint32_t i = 0; i < schedulerNextFreeEntry; i++) {
		if (currentTime > schedulerData[i].nextCall && !schedulerData[i].deleted)
		{
			schedulerData[i].callback();
			if (schedulerData[i].interval == 0) {
				schedulerData[i].nextCall = 0;
				schedulerData[i].deleted = 1;
			}
			else {
				schedulerData[i].nextCall = currentTime + schedulerData[i].interval;
			}
		}
		if (!schedulerData[i].deleted) {
			schedulerNextEvent = schedulerNextEvent > schedulerData[i].nextCall ? schedulerData[i].nextCall : schedulerNextEvent;
		}
	}

	uint32_t ptrDeleted = 0;
	uint32_t ptrValid = (schedulerNextFreeEntry > 0) ? (schedulerNextFreeEntry - 1) : 0;
	while (ptrDeleted < ptrValid) {
		if (!schedulerData[ptrDeleted].deleted) {
			ptrDeleted++;
		}
		else if (schedulerData[ptrValid].deleted) {
			ptrValid--;
		}
		else {
			schedulerData[ptrDeleted] = schedulerData[ptrValid];
			schedulerNextFreeEntry = ptrValid;
			ptrValid--;
		}
	}
	if (ptrDeleted == ptrValid && schedulerData[ptrDeleted].deleted)
	{
		schedulerNextFreeEntry = ptrDeleted;
	}
}

uint32_t schedulerRegisterInterval(void * callback, uint32_t intervalMs)
{
	if (schedulerNextFreeEntry >= SCHEDULER_MAX_ENTRIES)
	{
		return false;
	}
	uint32_t now = time();
	schedulerData[schedulerNextFreeEntry] = (SchedulerData_t){schedulerNextUId, now + intervalMs, intervalMs, callback, 0};
	schedulerNextFreeEntry++;
	schedulerNextEvent = schedulerNextEvent > (now + intervalMs) ? (now + intervalMs) : schedulerNextEvent;
	return schedulerNextUId++;
}

uint32_t schedulerRegisterTimeout(void * callback, uint32_t timeoutMs)
{
	if (schedulerNextFreeEntry >= SCHEDULER_MAX_ENTRIES)
	{
		return false;
	}
	uint32_t now = time();
	schedulerData[schedulerNextFreeEntry] = (SchedulerData_t){schedulerNextUId, now + timeoutMs, 0, callback, 0};
	schedulerNextFreeEntry++;
	schedulerNextEvent = schedulerNextEvent > (now + timeoutMs) ? (now + timeoutMs) : schedulerNextEvent;
	return schedulerNextUId++;
}


_Bool schedulerUnregister(uint32_t uid) {
	if (uid == 0)
	{
		return false;
	}
	for (uint32_t i = 0; i < schedulerNextFreeEntry; i++)
	{
		if (schedulerData[i].id == uid)
		{
			schedulerData[i].deleted = true;
			return true;
			// No need to clean up; that will happen the next time we trigger.
		}
	}
	return false;
}

void schedulerUnregisterAll(void) {
	for (uint32_t i = 0; i < schedulerNextFreeEntry; i++)
	{
		schedulerData[i].deleted = true;
		// No need to clean up; that will happen the next time we trigger.
	}
	schedulerNextFreeEntry = 0;
}


