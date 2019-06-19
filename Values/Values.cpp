#include "C:\Users\Yumi\Desktop\wcs\Values\Values.h"

/**************************************************************************/
/*! 
	Organize, store and check sensor values.
*/
/**************************************************************************/

uint16_t co2Thresh = 1400;
uint16_t vocThresh = 100;
float tempThresh = 30.0;
uint8_t uviThresh = 8;
uint8_t uviDurationThresh = 10;
uint8_t uviDuration = 0;
uint16_t co2_idx = 0;
uint16_t voc_idx = 0;
uint16_t uvi_idx = 0;
uint16_t temp_idx = 0;
uint16_t stepGoal = 10000;

uint16_t warn_idx = 0;
bool warningOverflow = 0;
bool dismissSteps = 0;


void Values::setCO2Thresh(uint16_t val) {
	co2Thresh = val;
}

void Values::setVOCThresh(uint16_t val) {
	vocThresh = val;
}

void Values::setTempThresh(float val) {
	tempThresh = val;
}

void Values::setStepGoal(uint16_t val) {
	stepGoal = val;
}

void Values::setUVIThresh(uint8_t val) {
	uviThresh = val;
}

void Values::setUVIDurationThresh(uint8_t val) {
	uviDurationThresh = val;
}

void Values::storeCO2(uint16_t val) {
	co2[co2_idx++] = val;
	if (val >= co2Thresh) {
		// Do something, Light LED or what else
	}
}

void Values::storeVOC(uint16_t val) {
	voc[voc_idx++] = val;
	if (val >= vocThresh) {
		// Do something, Light LED or what else
	}
}

void Values::storeTemp(float val) {
	temp[temp_idx++] = val;
}

void Values::storeSteps(uint16_t val) {
	steps = val;
	if (steps >= stepGoal && !dismissSteps) {
		// Do something
	}

}

void Values::storeUVI(uint8_t val) {
	uvi[uvi_idx++] = val;
	if (val >= uviThresh) {
		uviDuration++;
	}
	if (uviDuration >= uviDurationThresh) {
		// Warning
	}
}
