#ifndef CONFIG_H_
#define CONFIG_H_

#define DEEP_SLEEP			0
#define ONLY_BT				1
#define SENSORS_ACTIVE		2

#define BAUDRATE			9600

#define FLASH_SIZE			4000000 // 4 MB
#define VALUES_SET_ADDR			0
#define CO2_THRESH_ADDR			1
#define UVI_DUR_THRESH_ADDR		2
#define UVI_THRESH_ADDR			3
#define VOC_THRESH_ADDR			3
#define TEMP_THRESH_ADDR		4
#define STEP_GOAL_ADDR_LO		5
#define STEP_GOAL_ADDR_HI		6

#define WAKEUP_HOUR_ADDR
#define WAKEUP_MIN_ADDR

// Store current FLASH storage index before going to sleep and every 150 min
// 2 Bytes necessary because index <= 1000
#define STEPS_FLASH_ADDR_LO 90
#define STEPS_FLASH_ADDR_HI 91
#define CO2_FLASH_IDX_ADDR_LO		92
#define CO2_FLASH_IDX_ADDR_HI		93
#define VOC_FLASH_IDX_ADDR_LO		94
#define VOC_FLASH_IDX_ADDR_HI		95
#define UVI_FLASH_IDX_ADDR_LO		96
#define UVI_FLASH_IDX_ADDR_HI		97
#define TEMP_FLASH_IDX_ADDR_LO		98
#define TEMP_FLASH_IDX_ADDR_HI		99

#define CO2_FLASH_IDX_START		101
#define CO2_FLASH_IDX_STOP		500
#define VOC_FLASH_IDX_START		501
#define VOC_FLASH_IDX_STOP		1000
#define UVI_FLASH_IDX_START		1001
#define UVI_FLASH_IDX_STOP		1500
#define TEMP_FLASH_IDX_START	1501
#define TEMP_FLASH_IDX_STOP		2000

#define STORE_TO_FLASH_AFTER_MS	8400000	// 150 min

// Threshold durations for different skin types (min)
#define SKIN_TYPE_1				10
#define SKIN_TYPE_2				20
#define SKIN_TYPE_3				30
#define SKIN_TYPE_4				50	// actually >30
#define SKIN_TYPE_5				90	// actually >90




//



// Currently arbitary numbers.
#define	VIBRATION_PIN		19
#define LEDRED_PIN			15
#define BUTTON_PIN			27
#define BT_PIN				12

#define SENSORS_EN_PIN		12

#define SDA_PIN				4
#define SCL_PIN				16


// Frequencies
#define PEDO_FREQ			30
#define AQ_FREQ				600000	// 10 min
#define UV_FREQ				60000	// 1 min

// Hold info for 150 minutes
#define CO2_STORAGE_SIZE	15
#define VOC_STORAGE_SIZE	15
#define TEMP_STORAGE_SIZE	15
#define UVI_STORAGE_SIZE	150
#define WARNING_STORAGE_SIZE 50

// Pedometer
#define X_PIN 				32
#define Y_PIN 				34
#define Z_PIN 				35
#define PEDO_CALIBRATION_SAMPLE_SIZE 40
#define PEDO_SAMPLE_SIZE 		15
#define ACCL				85000
#define WAIT_AFTER_STEP		300







#endif /* CONFIG_H_ */
