#ifndef CONFIG_H_
#define CONFIG_H_

#define BAUDRATE			9600

// Currently arbitary numbers.
#define	VIBRATION_PIN		19
#define LEDRED_PIN			15

#define SDA_PIN				4
#define SCL_PIN				16

#define CO2_LIMIT			1400

// Pedometer
#define PEDO_FREQ			50		// 20 Hz
#define X_PIN 				32
#define Y_PIN 				34
#define Z_PIN 				35
#define BUTTON_PIN			27
#define PEDO_CALIBRATION_SAMPLE_SIZE 50
#define PEDO_SAMPLE_SIZE 		15
#define ACCL				90000

// Airquality
#define AQ_FREQ				1000	// 1s test
#define CO2_STORAGE_SIZE	5000
#define VOC_STORAGE_SIZE	5000
#define TEMP_STORAGE_SIZE	5000

// UV
#define UV_FREQ				1000	// 1s test
#define UVI_STORAGE_SIZE	5000




#endif /* CONFIG_H_ */
