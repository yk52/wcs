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
		Values(void) {};
		~Values(void) {};

		// Default thresholds
		uint16_t co2Thresh;
		uint8_t vocThresh;
		uint8_t tempThresh;
		uint8_t uviThresh;
		uint8_t uviDurationThresh;
		uint8_t uviDuration;
		uint16_t stepGoal;
		
		// data requested flag
		bool dataRequested = false;

		// Each bit/flag stands for one warning. Flag is set to 1 if value exceeded.
		// x | x | CO2 (4)| VOC (3)| TEMP (2)| UVI (1)| STEPS (0)|
		uint8_t warning = 0;
		uint8_t stepMask = 0x01;
		uint8_t uviMask = 0x02;
		uint8_t tempMask = 0x04;
		uint8_t vocMask = 0x08;
		uint8_t co2Mask = 0x10;

		void init(void);
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

		// take the message sent from the phone and triggers respective functions, sets
		// tresholds, gets values, etc. 
		// Format must be: "setCo2Threshold:4"
		void processMessage(std::string rxValue);

		


	private:

};

#endif /* VALUES_H_ */