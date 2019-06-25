#ifndef VALUES_H_
#define VALUES_H_

#include "Arduino.h"

// #include "WProgram.h"

#include "C:\Users\Yumi\Desktop\wcs\config.h"
#include <EEPROM.h>
#include <stdio.h>
using namespace std;


class Values {
	public:
		//constructors
		Values(void) {
			EEPROM.begin(FLASH_SIZE);
			uint8_t thresholdsSet = EEPROM.read(VALUES_SET_ADDR);
			if (thresholdsSet != 1) {
				// Values initiated flag
				EEPROM.write(VALUES_SET_ADDR, 1);
				// Set thresholds
				EEPROM.write(CO2_THRESH_ADDR, 14); // 14 (*100) = 1400
				co2Thresh = 1400;
				EEPROM.write(VOC_THRESH_ADDR, 1);
				vocThresh = 1;
				EEPROM.write(UVI_THRESH_ADDR, 8);
				uviThresh = 8;
				EEPROM.write(UVI_DUR_THRESH_ADDR, 10);
				uviDurationThresh = 10;
				EEPROM.write(TEMP_THRESH_ADDR, 30);
				tempThresh = 30;
				EEPROM.write(STEP_GOAL_ADDR_HI, 0x27); // 0x2710 = 10000
				EEPROM.write(STEP_GOAL_ADDR_LO, 0x10);
				stepGoal = 10000;

				// Set Flash storage indices
				EEPROM.write(CO2_FLASH_IDX_ADDR_LO, CO2_FLASH_IDX_START & 0xFF);
				EEPROM.write(CO2_FLASH_IDX_ADDR_HI, (CO2_FLASH_IDX_START >> 8) & 0xFF);

				EEPROM.write(VOC_FLASH_IDX_ADDR_LO, VOC_FLASH_IDX_START & 0xFF);
				EEPROM.write(VOC_FLASH_IDX_ADDR_HI, (VOC_FLASH_IDX_START >> 8) & 0xFF);

				EEPROM.write(UVI_FLASH_IDX_ADDR_LO, UVI_FLASH_IDX_START & 0xFF);
				EEPROM.write(UVI_FLASH_IDX_ADDR_HI, (UVI_FLASH_IDX_START >> 8) & 0xFF);

				EEPROM.write(TEMP_FLASH_IDX_ADDR_LO, TEMP_FLASH_IDX_START & 0xFF);
				EEPROM.write(TEMP_FLASH_IDX_ADDR_HI, (TEMP_FLASH_IDX_START >> 8) & 0xFF);

				EEPROM.commit();
			}
			else if (thresholdsSet == 1) {
				co2Thresh = getCO2Thresh();
				vocThresh = getVOCThresh();
				tempThresh = getTempThresh();
				uviThresh = getUVIThresh();
				uviDurationThresh = getUVIDurationThresh();
				stepGoal = getStepGoal();
			}


		};
		~Values(void) {};

		// Default thresholds
		uint16_t co2Thresh;
		uint8_t vocThresh;
		uint8_t tempThresh;
		uint8_t uviThresh;
		uint8_t uviDurationThresh;
		uint8_t uviDuration;
		uint16_t stepGoal;

		// Each bit/flag stands for one warning. Flag is set to 1 if value exceeded.
		// x | x | CO2 (4)| VOC (3)| TEMP (2)| UVI (1)| STEPS (0)|
		uint8_t warning = 0;
		uint8_t stepMask = 0x01;
		uint8_t uviMask = 0x02;
		uint8_t tempMask = 0x04;
		uint8_t vocMask = 0x08;
		uint8_t co2Mask = 0x10;

		void setUVIFlag(void);
		void setStepFlag(void);
		void setTempFlag(void);
		void setVOCFlag(void);
		void setCO2Flag(void);
		void clearAllWarnings(void);
		void clearUVIFlag(void);
		void clearStepFlag(void);
		void clearTempFlag(void);
		void clearVOCFlag(void);
		void clearCO2Flag(void);
		bool getUVIFlag(void);
		bool getStepFlag(void);
		bool getTempFlag(void);
		bool getVOCFlag(void);
		bool getCO2Flag(void);

		// Ram Storage arrays
		uint16_t co2[CO2_STORAGE_SIZE] = {0};  // in ppm. in flash memory, its divided by 100 to fit in a byte
		uint8_t co2_idx;
		uint16_t voc[VOC_STORAGE_SIZE] = {0}; // in ppb, bzw. mg/m3
		uint8_t voc_idx;
		uint8_t uvi[UVI_STORAGE_SIZE] = {0};   // UV index
		uint8_t uvi_idx;
		float temp[TEMP_STORAGE_SIZE] = {0}; // in degrees
		uint8_t temp_idx;
		uint16_t steps;

		// Set Threshold values
		void setCO2Thresh(uint16_t val);
		void setVOCThresh(uint8_t val);
		void setTempThresh(uint8_t val);
		void setStepGoal(uint16_t val);
		void setUVIThresh(uint8_t val);
		void setUVIDurationThresh(uint8_t val);

		// Get Thresholds...
		uint16_t getCO2Thresh(void);
		uint8_t getVOCThresh(void);
		uint8_t getTempThresh(void);
		uint16_t getStepGoal(void);
		uint8_t getUVIThresh(void);
		uint8_t getUVIDurationThresh(void);

		//... current flash storage index...
		uint16_t getCurrentCO2FlashIdx(void);
		uint16_t getCurrentVOCFlashIdx(void);
		uint16_t getCurrentTempFlashIdx(void);
		uint16_t getCurrentUVIFlashIdx(void);

		// Set most current flash storage index (commit needs to be done seperately)...
		void setCurrentCO2FlashIdx(uint16_t);
		void setCurrentVOCFlashIdx(uint16_t);
		void setCurrentTempFlashIdx(uint16_t);
		void setCurrentUVIFlashIdx(uint16_t);

		// ...and get most recent values
		uint16_t getLastCO2(void);
		uint16_t getLastVOC(void);
		float getLastTemp(void);
		uint16_t getLastStep(void);
		uint8_t getLastUVI(void);
		uint8_t getLastUVIDuration(void);

		// Store values in RAM and check thresholds at the same time.
		void storeCO2(uint16_t);
		void storeVOC(uint16_t);
		void storeTemp(float);
		bool storeSteps(uint16_t);
		void resetSteps(void);
		void storeUVI(uint8_t);
		bool storeRAMToFlash(void);

		// String must look like: "UVthreshold:10"
		void processMessage(string);



	private:

};

#endif /* VALUES_H_ */
