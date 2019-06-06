#include "C:\Users\Yumi\Desktop\wcs\Timer\Timer.h"

/**************************************************************************/
/*! 
	Scheduling class to make scheduling easier
*/
/**************************************************************************/

bool flag = false;

uint32_t Timer::getMillis(void) {
	float currentTicks = (float) clock();
	float millis = currentTicks/CLOCKS_PER_SEC*1000;
	return (uint32_t) millis;
}

uint32_t Timer::getTicks(void) {
	clock_t currentTicks = clock();
	return (uint32_t) currentTicks;
}

void Timer::add(void) {

}

void Timer::check(void){
	// Work off on/offSchedule
	/*if (flag)
	{
		uint32_t currentTicks = getTicks();

		// If repetitions to make and planned Time arrived
		if (repetitions > 0)
		{
			if (schedule[pos] < (uint32_t)currentTicks) {
				if (pos % 2)
				{
					off();
					repetitions--;
				} else {
					on();
				}
				pos++;
			}
		} else {
			flag = false;
			pos = 0;
		}
	}*/
}
