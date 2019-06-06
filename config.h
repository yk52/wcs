#ifndef CONFIG_H_
#define CONFIG_H_

#define BAUDRATE			9600

// Currently arbitary numbers.
#define	VIBRATION_PIN		19
#define LEDRED_PIN			15

#define CO2_LIMIT			1400

// Poll Measurements every X ms
#define PEDO_FREQ			1000		// 50 Hz
#define PEDO_STORAGE_SIZE	5000
#define AQ_FREQ				1000	// 1s test
#define CO2_STORAGE_SIZE	5000
#define VOC_STORAGE_SIZE	5000
#define TEMP_STORAGE_SIZE	5000
#define UV_FREQ				1000	// 1s test
#define UVI_STORAGE_SIZE	5000


// Pedometer
#define X_PIN 32
#define Y_PIN 34
#define Z_PIN 35
#define BUTTON_PIN 27
#define PEDO_CALIBRATION_SAMPLE_SIZE 100
#define PEDO_SAMPLE_SIZE 10


#endif /* CONFIG_H_ */
