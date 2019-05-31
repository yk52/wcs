#ifndef TIMER_H
#define TIMER_H

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <Wire.h>
#include <time.h>
#include "C:\Users\Yumi\Desktop\wcs\config.h"


uint32_t getMillis(void);

void schedulerInit(RunMode_t);
void schedulerCheck(void);
uint32_t schedulerRegisterInterval(void * callback, uint32_t interval);
uint32_t schedulerRegisterTimeout(void * callback, uint32_t timeout);
_Bool schedulerUnregister(uint32_t uid);
void schedulerUnregisterAll(void);



#endif /* TIMER_H_ */
