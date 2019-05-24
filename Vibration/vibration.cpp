#include "C:\Users\Yumi\Desktop\wcs\Vibration\vibration.h"

/**************************************************************************/
/*! 
	Controlling of the vibration motor for notification purposes.
*/
/**************************************************************************/

bool flag = false;
uint8_t repetitions = 0;
uint8_t pos = 0;

void Vibration::init(void){
	// Set pin to control vibration motor and initiate as low.
	pinMode(VIBRATION_PIN, OUTPUT);
	digitalWrite(VIBRATION_PIN, LOW);
}

void Vibration::on(void){
	digitalWrite(VIBRATION_PIN, HIGH);
}

void Vibration::off(void){
	digitalWrite(VIBRATION_PIN, LOW);
}

void Vibration::check(void){
	// Work off on/offSchedule
	if (flag)
	{
		clock_t currentTicks = clock();

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
	}
}

void Vibration::rhythm(uint32_t duration, uint32_t pause, uint8_t rep) {

	// TODO: or better, create new Array as Object every time.
	// arraySchedule = [off, on, off, on, off]

	if (!flag) {
		// If vibration not already in progess
		flag = true;
		repetitions = rep;
		clock_t currentTicks = clock();
		uint32_t durationTicks = duration*CLOCKS_PER_SEC/1000;
		uint32_t pauseTicks = pause*CLOCKS_PER_SEC/1000;

		schedule[0] = (uint32_t)currentTicks + durationTicks;

		for (int i = 1; i++; i < (rep*2)-1) {
			if (i % 2)
			{
				// if i is odd
				schedule[i] = schedule[i-1] + pauseTicks;
			} else {
				// if i is even
				schedule[i] = schedule[i-1] + durationTicks;
			}
		}
		// Start vibration rhythm.
		on();
	}
}


