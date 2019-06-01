#include "C:\Users\Yumi\Desktop\wcs\Timer\Timer.h"

/**************************************************************************/
/*! 
	Scheduling class to make scheduling easier
*/
/**************************************************************************/


uint32_t Timer::getMillis(void) {
	clock_t currentTicks = clock();
	uint32_t millis = currentTicks/CLOCKS_PER_SEC*1000;
	return millis;
}

uint32_t Timer::getTicks(void) {
	clock_t currentTicks = clock();
	return (uint32_t) currentTicks;
}
