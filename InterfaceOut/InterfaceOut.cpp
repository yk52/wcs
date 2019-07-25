#include "C:\Users\Yumi\Desktop\wcs\InterfaceOut\InterfaceOut.h"


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


