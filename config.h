#ifndef CONFIG_H_
#define CONFIG_H_

#define LIGHT_SLEEP			0
#define ONLY_BT				1
#define SENSORS_ACTIVE		2

#define BAUDRATE			9600

#define FLASH_SIZE			4000000 // 4 MB
#define CO2_THRESH_ADDR_LO		0
#define CO2_THRESH_ADDR_HI		1
#define UVI_DUR_THRESH_ADDR		2
#define UVI_THRESH_ADDR			3
#define VOC_THRESH_ADDR			3
#define TEMP_THRESH_ADDR		4
#define STEP_GOAL_ADDR_LO		5
#define STEP_GOAL_ADDR_HI		6



// Currently arbitary numbers.
#define	VIBRATION_PIN		19
#define LEDRED_PIN			15
#define BUTTON_PIN			27
#define BT_PIN				12
#define INTERRUPT_LVL		1

#define SENSORS_EN_PIN		12

#define SDA_PIN				4
#define SCL_PIN				16


// Frequencies
#define PEDO_FREQ			30
#define AQ_FREQ				600000	// 10 min
#define UV_FREQ				60000	// 1 min

#define CO2_STORAGE_SIZE	5000
#define VOC_STORAGE_SIZE	5000
#define TEMP_STORAGE_SIZE	5000
#define UVI_STORAGE_SIZE	5000
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
