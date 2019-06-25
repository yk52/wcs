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



void Values::setUVIFlag(void) {
	warning |= uviMask;
}

void Values::setStepFlag(void) {
	warning |= stepMask;
}

void Values::setTempFlag(void) {
	warning |= tempMask;
}

void Values::setVOCFlag(void) {
	warning |= vocMask;
}

void Values::setCO2Flag(void) {
	warning |= co2Mask;
}


void Values::clearAllWarnings(void) {
	warning = 0;
}

void Values::clearUVIFlag(void) {
	warning &= (~uviMask);
}

void Values::clearStepFlag(void) {
	warning &= (~stepMask);
}

void Values::clearTempFlag(void) {
	warning &= (~tempMask);
}

void Values::clearVOCFlag(void) {
	warning &= (~vocMask);
}

void Values::clearCO2Flag(void) {
	warning &= (~co2Mask);
}

bool Values::getUVIFlag(void) {
	if (warning & uviMask) {
		return 1;
	}
	else {
		return 0;
	}
}

bool Values::getStepFlag(void) {
	if (warning & stepMask) {
		return 1;
	}
	else {
		return 0;
	}
}

bool Values::getTempFlag(void) {
	if (warning & tempMask) {
		return 1;
	}
	else {
		return 0;
	}
}

bool Values::getVOCFlag(void) {
	if (warning & vocMask) {
		return 1;
	}
	else {
		return 0;
	}
}

bool Values::getCO2Flag(void) {
	if (warning & co2Mask) {
		return 1;
	}
	else {
		return 0;
	}
}

// Get Thresholds...
uint16_t Values::getCO2Thresh(void) {
	uint8_t val = EEPROM.read(CO2_THRESH_ADDR);
	uint16_t thresh = val*100;
	return thresh;
}

uint8_t Values::getVOCThresh(void) {
	uint8_t thresh = EEPROM.read(VOC_THRESH_ADDR);
	return thresh;
}

uint8_t Values::getTempThresh(void) {
	uint8_t thresh = EEPROM.read(TEMP_THRESH_ADDR);
	return thresh;
}

uint16_t Values::getStepGoal(void) {
	uint8_t LO = EEPROM.read(STEP_GOAL_ADDR_LO);
	uint16_t HI = EEPROM.read(STEP_GOAL_ADDR_HI);
	uint16_t goal = (HI << 8) | LO;
	return stepGoal;
}

uint8_t Values::getUVIThresh(void) {
	uint8_t thresh = EEPROM.read(UVI_THRESH_ADDR);
	return thresh;
}

uint8_t Values::getUVIDurationThresh(void) {
	uint8_t thresh = EEPROM.read(UVI_DUR_THRESH_ADDR);
	return thresh;
}

uint16_t Values::getCurrentCO2FlashIdx(void) {
	uint8_t LO = EEPROM.read(CO2_FLASH_IDX_ADDR_LO);
	uint16_t HI = EEPROM.read(CO2_FLASH_IDX_ADDR_HI);
	uint16_t idx = (HI << 8) | LO;
	return idx;

}

uint16_t Values::getCurrentVOCFlashIdx(void) {
	uint8_t LO = EEPROM.read(VOC_FLASH_IDX_ADDR_LO);
	uint16_t HI = EEPROM.read(VOC_FLASH_IDX_ADDR_HI);
	uint16_t idx = (HI << 8) | LO;
	return idx;
}

uint16_t Values::getCurrentTempFlashIdx(void) {
	uint8_t LO = EEPROM.read(TEMP_FLASH_IDX_ADDR_LO);
	uint16_t HI = EEPROM.read(TEMP_FLASH_IDX_ADDR_HI);
	uint16_t idx = (HI << 8) | LO;
	return idx;
}

uint16_t Values::getCurrentUVIFlashIdx(void) {
	uint8_t LO = EEPROM.read(UVI_FLASH_IDX_ADDR_LO);
	uint16_t HI = EEPROM.read(UVI_FLASH_IDX_ADDR_HI);
	uint16_t idx = (HI << 8) | LO;
	return idx;
}


// ...and most recent values TODO: necessary?
uint16_t Values::getLastCO2(void) {
	return co2[co2_idx-1];
}

uint16_t Values::getLastVOC(void) {
	return voc[voc_idx-1];
}

float Values::getLastTemp(void) {
	if (temp_idx != 0) {
		return temp[temp_idx-1];
	}
}

uint16_t Values::getLastStep(void) {
	return steps;
}
uint8_t Values::getLastUVI(void) {
	return uvi[uvi_idx-1];
}

uint8_t Values::getLastUVIDuration(void) {
	return uviDuration;
}

void Values::setCO2Thresh(uint16_t val) {
	uint16_t oldVal = getCO2Thresh();
	if (oldVal != val) {
		uint8_t byte = val/100;
		EEPROM.write(CO2_THRESH_ADDR, byte);
	}
	co2Thresh = val;
}

// check that maximum val <= 255 in application!
void Values::setVOCThresh(uint8_t val) {
	uint8_t oldVal = getVOCThresh();
	if (oldVal != val) {
		EEPROM.write(VOC_THRESH_ADDR, val);
	}
	vocThresh = val;
}

void Values::setTempThresh(uint8_t val) {
	uint8_t oldVal = getTempThresh();
	if (oldVal != val) {
		EEPROM.write(TEMP_THRESH_ADDR, val);
	}
	tempThresh = val;
}

void Values::setStepGoal(uint16_t val) {
	uint16_t oldVal = getStepGoal();
	if (oldVal != val) {
		uint8_t newLo = val & 0xFF;
		uint8_t newHi = (val >> 8) & 0xFF;
		EEPROM.write(STEP_GOAL_ADDR_LO, newLo);
		EEPROM.write(STEP_GOAL_ADDR_HI, newHi);
	}
	stepGoal = val;
}

void Values::setUVIThresh(uint8_t val) {
	uint8_t oldVal = getUVIThresh();
	if (oldVal != val) {
		EEPROM.write(UVI_THRESH_ADDR, val);
	}
	uviThresh = val;
}

void Values::setUVIDurationThresh(uint8_t val) {
	uint8_t oldVal = getUVIDurationThresh();
	if (oldVal != val) {
		EEPROM.write(UVI_DUR_THRESH_ADDR, val);
	}
	uviDurationThresh = val;
}

void Values::storeCO2(uint16_t val) {
	co2[co2_idx++] = val;
	if (val >= co2Thresh) {
		setCO2Flag();
	}
}

void Values::storeVOC(uint16_t val) {
	voc[voc_idx++] = val;
	if (val >= vocThresh) {
		setVOCFlag();
	}
}

void Values::storeTemp(float val) {
	temp[temp_idx++] = val;
	if (val >= tempThresh) {
		setTempFlag();
	}
}

bool Values::storeSteps(uint16_t val) {
	steps = val;
	if (steps == stepGoal) {
		setStepFlag();
		return 1;
	}
	else {
		return 0;
	}

}

void Values::resetSteps(void) {
	steps = 0;
}

void Values::storeUVI(uint8_t val) {
	uvi[uvi_idx++] = val;
	if (val >= uviThresh) {
		uviDuration++;
	}
	if (uviDuration >= uviDurationThresh) {
		setUVIFlag();
	}
}

void Values::setCurrentCO2FlashIdx(uint16_t idx) {
	uint8_t LO = idx & 0xFF;
	uint8_t HI = (idx >> 8) & 0xFF;
	EEPROM.write(CO2_FLASH_IDX_ADDR_LO, LO);
	EEPROM.write(CO2_FLASH_IDX_ADDR_HI, HI);
}

void Values::setCurrentVOCFlashIdx(uint16_t idx) {
	uint8_t LO = idx & 0xFF;
	uint8_t HI = (idx >> 8) & 0xFF;
	EEPROM.write(VOC_FLASH_IDX_ADDR_LO, LO);
	EEPROM.write(VOC_FLASH_IDX_ADDR_HI, HI);
}
void Values::setCurrentTempFlashIdx(uint16_t idx) {
	uint8_t LO = idx & 0xFF;
	uint8_t HI = (idx >> 8) & 0xFF;
	EEPROM.write(TEMP_FLASH_IDX_ADDR_LO, LO);
	EEPROM.write(TEMP_FLASH_IDX_ADDR_HI, HI);
}
void Values::setCurrentUVIFlashIdx(uint16_t idx) {
	uint8_t LO = idx & 0xFF;
	uint8_t HI = (idx >> 8) & 0xFF;
	EEPROM.write(UVI_FLASH_IDX_ADDR_LO, LO);
	EEPROM.write(UVI_FLASH_IDX_ADDR_HI, HI);
}

bool Values::storeRAMToFlash(void) {
	bool overflow = 0;

    // Store steps
    uint8_t stepLo = steps & 0xFF;
    uint8_t stepHi = (steps >> 8) & 0xFF;
    EEPROM.write(STEPS_FLASH_ADDR_LO, stepLo);
    EEPROM.write(STEPS_FLASH_ADDR_HI, stepHi);

    // Store CO2, TVOC and Temperature
    uint16_t co2Flash_idx = getCurrentCO2FlashIdx();
    uint16_t vocFlash_idx = getCurrentVOCFlashIdx();
    uint16_t tempFlash_idx = getCurrentTempFlashIdx();

    for (int i = 0; i < co2_idx+1; i++) {
    	if (co2Flash_idx > CO2_FLASH_IDX_STOP) {
    		overflow = 1;
    		break;
    	}
    	else {
    		EEPROM.write(co2Flash_idx++, co2[i]/100);
    		EEPROM.write(vocFlash_idx++, voc[i]);
    		EEPROM.write(tempFlash_idx++, temp[i]);
    		setCurrentCO2FlashIdx(co2Flash_idx);
    		setCurrentVOCFlashIdx(vocFlash_idx);
			setCurrentTempFlashIdx(tempFlash_idx);
    	}
    }
    // Store UV
    uint16_t uviFlash_idx = getCurrentUVIFlashIdx();
    for (int i = 0; i < uvi_idx+1; i++) {
    	if (uviFlash_idx > UVI_FLASH_IDX_STOP) {
    		overflow = 1;
    		break;
    	}
    	else {
    		EEPROM.write(uviFlash_idx++, uvi[i]);
    		setCurrentUVIFlashIdx(uviFlash_idx);
    		uviFlash_idx++;
    	}
    }

    co2_idx = 0;
    voc_idx = 0;
    uvi_idx = 0;
    temp_idx = 0;
    EEPROM.commit();

    if (overflow) {
    	return 1;
    }
    else {
    	return 0;
    }
}



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
