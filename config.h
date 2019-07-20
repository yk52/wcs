#ifndef CONFIG_H_
#define CONFIG_H_

#define LIGHT_SLEEP			0
#define SENSORS_ACTIVE		1
#define ONLY_BT				2

#define BAUDRATE			9600

// FLASH_____________________
#define FLASH_SIZE			3000000 // 3 MB
#define VALUES_SET_ADDR			0
#define CO2_THRESH_ADDR			1
#define UVI_DUR_THRESH_ADDR		2
#define UVI_THRESH_ADDR			3
#define VOC_THRESH_ADDR			4
#define TEMP_THRESH_ADDR		5
#define STEP_GOAL_ADDR_LO		6
#define STEP_GOAL_ADDR_HI		7

// Store current FLASH storage index before going to sleep and every 150 min
// 2 Bytes necessary because index <= 1000
#define STEPS_FLASH_ADDR_LO			8
#define STEPS_FLASH_ADDR_HI			9
#define CO2_FLASH_IDX_ADDR_LO		10
#define CO2_FLASH_IDX_ADDR_HI		11
#define VOC_FLASH_IDX_ADDR_LO		12
#define VOC_FLASH_IDX_ADDR_HI		13
#define UVI_FLASH_IDX_ADDR_LO		14
#define UVI_FLASH_IDX_ADDR_HI		15
#define TEMP_FLASH_IDX_ADDR_LO		16
#define TEMP_FLASH_IDX_ADDR_HI		17

#define SLEEP_DUR_ADDR_LO		30
#define SLEEP_DUR_ADDR_HI		31
#define IDX_BEFORE_SLEEP_ADDR_LO	32
#define IDX_BEFORE_SLEEP_ADDR_LO	33

/*
// Store deep sleep duration in ms
#define SLEEP_DUR_IDX_START		3001
#define SLEEP_DUR_IDX_STOP		3400
// Store index of last sensor measurement before deep sleep happened
#define IDX_BEFORE_SLEEP_START	3401
#define IDX_BEFORE_SLEEP_STOP	3800
*/




#define CO2_FLASH_IDX_START			1000
#define CO2_FLASH_IDX_STOP			5999
#define VOC_FLASH_IDX_START			10000
#define VOC_FLASH_IDX_STOP		14999
#define UVI_FLASH_IDX_START		18000
#define UVI_FLASH_IDX_STOP		22999
#define TEMP_FLASH_IDX_START	26000
#define TEMP_FLASH_IDX_STOP		50999

#define STORE_TO_FLASH_AFTER_MS		10000// 7200000	// 120 min

// _____________________

// Threshold durations for different skin types (min)
#define SKIN_TYPE_1				10
#define SKIN_TYPE_2				20
#define SKIN_TYPE_3				30
#define SKIN_TYPE_4				50	// actually >30
#define SKIN_TYPE_5				90	// actually >90


// Version 10 //Pins as in WCS_Version5
#define	VIBRATION_PIN		2
#define LEDBLUE_PIN			23 // Bluetooth
#define	LEDGREEN_PIN		16 // Power
#define LEDRED_PIN			15 // Warning
// Button pins
#define POWER_PIN			36
#define BLUETOOTH_PIN		34
#define PRESSED_BUTTON_LEVEL	0

#define SENSORS_EN_PIN		13
#define GAS_SENSOR_INT_PIN	4

#define SDA_PIN				22
#define SCL_PIN				21


// Frequencies
#define PEDO_FREQ			30
#define AQ_FREQ				2000 // 600000	// 10 min
#define UV_FREQ				2000	// 1 min

// Hold info for 150 minutes
#define CO2_STORAGE_SIZE	1000
#define VOC_STORAGE_SIZE	1000
#define TEMP_STORAGE_SIZE	1000
#define UVI_STORAGE_SIZE	1000
#define WARNING_STORAGE_SIZE 50

// Pedometer
#define X_PIN 				12
#define Y_PIN 				14
#define Z_PIN 				27
#define PEDO_CALIBRATION_SAMPLE_SIZE 15
#define PEDO_SAMPLE_SIZE 		15
#define ACCL				85000
#define WAIT_AFTER_STEP		400







#endif /* CONFIG_H_ */
