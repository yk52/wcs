#ifndef CONFIG_H_
#define CONFIG_H_

#define LIGHT_SLEEP			0
#define ACTIVE				1

#define BAUDRATE			9600

// Currently arbitary numbers.
#define	VIBRATION_PIN		19
#define LEDRED_PIN			15
#define BUTTON_PIN			27

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

// Pedometer
#define X_PIN 				32
#define Y_PIN 				34
#define Z_PIN 				35
#define PEDO_CALIBRATION_SAMPLE_SIZE 40
#define PEDO_SAMPLE_SIZE 		15
#define ACCL				85000
#define WAIT_AFTER_STEP		300







#endif /* CONFIG_H_ */
