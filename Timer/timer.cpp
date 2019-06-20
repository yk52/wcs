#include "C:\Users\Yumi\Desktop\wcs\Timer\Timer.h"
// include rtc.h?

/**************************************************************************/
/*! 
	Scheduling class to make scheduling easier
*/
/**************************************************************************/

bool flag = false;
uint32_t globalBootTime = 0;

void Timer::init(void) {
	rtc_clk_init(slow_freq : 2);
	rtc_clk_32k_enable_external();
	rtc_clk_slow_freq_set(1);
	int hz = rtc_clk_slow_freq_get_hz(); // should return 32768
	if (hz != 32768) {
		// TODO
		while (1) { }
	}
}

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
