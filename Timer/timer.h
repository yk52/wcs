#ifndef TIMER_H_
#define TIMER_H_

#if (ARDUINO >= 100)
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include <time.h>
#include "C:\Users\Yumi\Desktop\wcs\config.h"


uint32_t getMillis(void);
uint32_t getTicks(void);



#endif /* TIMER_H_ */
