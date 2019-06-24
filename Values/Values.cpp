#include "C:\Users\Yumi\Desktop\wcs\Values\Values.h"


/**************************************************************************/
/*! 
	Organize, store and check sensor values.
*/
/**************************************************************************/


// SRAM storage index
uint16_t co2_idx = 0;
uint16_t voc_idx = 0;
uint16_t uvi_idx = 0;
uint16_t temp_idx = 0;

uint8_t uviDuration = 0;

uint16_t warn_idx = 0;
bool warningOverflow = 0;


void Values::setCO2Thresh(uint16_t val) {
	uint8_t oldLo = EEPROM.read(CO2_THRESH_ADDR_LO);
	uint16_t oldHi = EEPROM.read(CO2_THRESH_ADDR_HI);
	uint16_t oldVal = (HI << 8) || LO;
	if (oldVal != val) {
		uint8_t newLo = val && 0xFF;
		uint8_t newHi = (val >> 8) && 0xFF;
		EEPROM.write(CO2_THRESH_ADDR_LO, newLo);
		EEPROM.write(CO2_THRESH_ADDR_HI, newHi);
	}
}

// check that maximum val <= 255 in application!
void Values::setVOCThresh(uint16_t val) {
	uint16_t oldVal = EEPROM.read(VOC_THRESH_ADDR);
	if (oldVal != val) {
		val = (uint8_t) val
		EEPROM.write(VOC_THRESH_ADDR, val);
	}
}

void Values::setTempThresh(uint8_t val) {
	uint8_t oldVal = EEPROM.read(TEMP_THRESH_ADDR);
	if (oldVal != val) {
		EEPROM.write(TEMP_THRESH_ADDR, val);
	}
}

void Values::setStepGoal(uint16_t val) {
	uint8_t oldLo = EEPROM.read(STEP_GOAL_ADDR_LO);
	uint16_t oldHi = EEPROM.read(STEP_GOAL_ADDR_HI);
	uint16_t oldVal = (HI << 8) || LO;
	if (oldVal != val) {
		uint8_t newLo = val && 0xFF;
		uint8_t newHi = (val >> 8) && 0xFF;
		EEPROM.write(STEP_GOAL_ADDR_LO, newLo);
		EEPROM.write(STEP_GOAL_ADDR_HI, newHi);
	}
}

void Values::setUVIThresh(uint8_t val) {
	uint8_t oldVal = EEPROM.read(UVI_THRESH_ADDR);
	if (oldVal != val) {
		EEPROM.write(UVI_THRESH_ADDR, val);
	}
}

void Values::setUVIDurationThresh(uint8_t val) {
	uint8_t oldVal = EEPROM.read(UVI_DUR_THRESH_ADDR);
	if (oldVal != val) {
		EEPROM.write(UVI_DUR_THRESH_ADDR, val);
	}
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

bool Values::storeSteps(uint16_t val) {
	steps = val;
	if (steps == stepGoal) {
		return 1;
	}
	else {
		return 0;
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

// Get Thresholds...
uint16_t getCO2Thresh(void) {
	uint8_t LO = EEPROM.read(CO2_THRESH_ADDR_LO);
	uint16_t HI = EEPROM.read(CO2_THRESH_ADDR_HI);
	uint16_t thresh = (HI << 8) || LO;
	return thresh;
}

uint16_t getVOCThresh(void) {
	uint16_t thresh = EEPROM.read(VOC_THRESH_ADDR);
	return thresh;
}

uint8_t getTempThresh(void) {
	float thresh = (float)EEPROM.read(TEMP_THRESH_ADDR);
	return thresh;
}

uint16_t getStepGoal(void) {
	uint8_t LO = EEPROM.read(STEP_GOAL_ADDR_LO);
	uint16_t HI = EEPROM.read(STEP_GOAL_ADDR_HI);
	uint16_t goal = (HI << 8) || LO;
	return goal;
}

uint8_t getUVIThresh(void) {
	return EEPROM.read(UVI_THRESH_ADDR);
}

uint8_t getUVIDurationThresh(void) {
	return EEPROM.read(UVI_DUR_THRESH_ADDR);
}

// ...and most recent values
uint16_t getLastCO2(void);
uint16_t getLastVOC(void);
float getLastTemp(void);
uint16_t getLastStep(void);
uint8_t getLastUVI(void);
uint8_t getLastUVIDuration(void);

// TODO
void Values::processMessage(string rxValue) {
	/*
	if (rxValue.find(":") != -1) {
		size_t cut = rxValue.find(":");

	    std::string parameter = rxValue.substr(0, cut);
		std::string sValue = rxValue.substr(cut, -1);
		int value = atoi(sValue);

		if (parameter.compare("Skintype")) {
			m_Skintype = value;
		} else if (parameter.compare("UVthreshold")) {
			m_UVthreshold = value;
		} else if (parameter.compare("AQthreshold")) {
			m_AQthreshold = value;

	} else if (rxValue.find("DataRequest") != -1) {
		// init serial bluetooth
	} else if (rxValue.find("get") != -1) {
		char txString[20] = "AQthreshold: " + std::to_string(AQthreshold);
		pCharacteristic->setValue(txString);
		pCharacteristic->notify(); 						// Send the value
	}
	*/
}
