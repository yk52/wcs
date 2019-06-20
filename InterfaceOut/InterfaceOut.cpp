#include "C:\Users\Yumi\Desktop\wcs\InterfaceOut\InterfaceOut.h"
#include <stdio.h>
using namespace std;

/**************************************************************************/
/*! 
	Controlling of the InterfaceOut motor for notification purposes.
*/
/**************************************************************************/


void InterfaceOut::on(void){
	digitalWrite(this->pin, HIGH);
}

void InterfaceOut::off(void){
	digitalWrite(this->pin, LOW);
}

void InterfaceOut::toggle(void) {
	if (digitalRead(this->pin) == HIGH) {
		off();
	} else {
		on();
	}
}


void InterfaceOut::rhythm(uint32_t duration, uint32_t pause, uint8_t rep) {

	// TODO: or better, create new Array as Object every time.
	// arraySchedule = [off, on, off, on, off]
	// cout << "Duration " << duration;
/*
	if (!flag) {
		// If InterfaceOut not already in progess
		flag = true;
		repetitions = rep;
		clock_t currentTicks = clock();
		uint32_t durationTicks = duration*CLOCKS_PER_SEC/1000;
		uint32_t pauseTicks = pause*CLOCKS_PER_SEC/1000;

		// printf("DurationTick %d \n", durationTicks);
		// printf("pauseTick %d \n", pauseTicks);

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
		// Start InterfaceOut rhythm.
		on();
	}
	*/
}


