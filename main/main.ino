/*
	Main class for the Vitameter.
*/

#include <Wire.h>
#include <BluetoothSerial.h>
// #include <BLE_wcs.h>
#include <Adafruit_CCS811.h>
#include <Adafruit_VEML6075.h>
#include <Adafruit_ADXL335.h>
#include <InterfaceOut.h>
#include <timer.h>
#include <Values.h>
#include "C:\Users\Yumi\Desktop\wcs\config.h"


uint8_t state = DEEP_SLEEP;
RTC_DATA_ATTR uint32_t rtcData = 0;

BluetoothSerial SerialBT;
// BLE_wcs ble;

Timer timer;
// By creating values object, the thresholds are automatically set.
Values values;

Adafruit_CCS811 ccs;
Adafruit_VEML6075 uv = Adafruit_VEML6075();
Adafruit_ADXL335 pedo;

InterfaceOut vib(VIBRATION_PIN);
InterfaceOut ledRed(LEDRED_PIN);
InterfaceOut ledGreen(LEDGREEN_PIN);
InterfaceOut ledBlue(LEDBLUE_PIN);
InterfaceOut sensors(SENSORS_EN_PIN); // auf high schalten?

bool error = 0;
uint8_t interruptFlag = 0;
uint32_t debounceTimer = 0;
int warningCounter = 0;
uint32_t minuteCounter = 0;
bool firstBooted = 1;

// Timers
uint32_t ms = 0;
uint32_t lastEmptied = 0;
uint32_t pedoTimeout = 0;
uint32_t uvTimeout = 0;
uint32_t airTimeout = 0;
uint32_t showTimeout = 0;

bool bleOn = 0;


void setup() {
  // initialize the serial communications:

  Serial.begin(BAUDRATE);
  while (!Serial) {
    delay(10);
  }
  Wire.begin(SDA_PIN, SCL_PIN);
  pinMode(BUTTON_PIN, INPUT);
  values.init();
}

void readGasSensor() {
  uint16_t c = ccs.geteCO2();
  uint16_t v = ccs.getTVOC();
  values.storeCO2(c);
  values.storeVOC(v);
  values.storeTemp(ccs.calculateTemperature());
}

void readUVISensor() {
  uint8_t u = uv.readUVI();
  values.storeUVI(u);
}

void loop() {
  // TODO change to RTC clock
  ms = millis();
  
  if (state == SENSORS_ACTIVE) {
    /*
    if (warning) {
      handleWarning();
    }
    */

    if (!values.pedoEnable) {
      readUVISensor();
      if (minuteCounter % 10 == 0) {
        while (!ccs.available());
        readGasSensor();
      }
      goLightSleepTimeout(60000 - millis() + ms);
    }
    else {
      if ((ms > airTimeout) && ccs.available()) {
        airTimeout += AQ_FREQ;
        readGasSensor();
      }
      if (ms > uvTimeout) {
        uvTimeout += UV_FREQ;
        readUVISensor();
      }
      if (ms > pedoTimeout) {
        uint16_t x = pedo.getPedo();
        bool goalAchieved = values.storeSteps(x);
  
        if (goalAchieved) {
          // Do some fancy light and Vibration stuff
        }
        
        // Step registered
        if (pedo.flag) {
          Serial.println(x);
          pedoTimeout += WAIT_AFTER_STEP;
          pedo.flag = 0;
        }
        else {
          pedoTimeout += PEDO_FREQ;
        }
      }
    }

    
    if ((ms - lastEmptied) > STORE_TO_FLASH_AFTER_MS) {
      lastEmptied = ms;
      values.storeRAMToFlash();
    }
  }
  else if (state == DEEP_SLEEP) {
    if (values.co2_idx >= 6) {
      values.storeRAMToFlash();
    }
    goSleep();
  }

  if (interruptFlag > 0) {
    if (debounceTimer < ms) {
      interruptFlag = 0;
    }
  }
}

void goSleep() {
  // delete(&SerialBLUETOOTH);   // TODO
  Serial.println("Enter sleep");
  detachInterrupt(digitalPinToInterrupt(BUTTON_PIN));
  delay(2000);
  // TODO: Take necessary measures (like storing remaining data into flash, reset steps etc etc)

  // TODO Add Bluetooth button later
  // TODO change Interrupt level if necessary
  gpio_wakeup_enable(GPIO_NUM_27, GPIO_INTR_HIGH_LEVEL);
  esp_sleep_enable_gpio_wakeup();
  esp_light_sleep_start();
  wakeUp();
}

void goLightSleepTimeout(int milli) {
  esp_sleep_enable_timer_wakeup(milli*1000);
  esp_light_sleep_start();
  if (milli == 60000) {
    minuteCounter++;
  }
}

void wakeUp() {
  if (digitalRead(BUTTON_PIN) == 1) {
    state = SENSORS_ACTIVE;
    sensorsInit();
    
    // BluetoothSerial SerialBLUETOOTH; // TODO
    // SerialBLUETOOTH.begin("Yumi"); // TODO
  }
  else if (digitalRead(BLUETOOTH_PIN) == 1) {
    // ble.init(); TODO
    bleOn = 1;
  }
  esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_GPIO);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, RISING);
  attachInterrupt(digitalPinToInterrupt(BLUETOOTH_PIN), buttonISR, RISING);
  Serial.println("Good morning!");
}

void buttonISR() {
  if (interruptFlag == 0) {
    interruptFlag++;
    debounceTimer = ms + 100;
    ledRed.toggle();

    if (state == SENSORS_ACTIVE) {
      state = DEEP_SLEEP;
    }
  }
}

void bluetoothButtonISR() {
  // Only for BLUETOOTH off, because BLUETOOTH on is handled in wakeup function
  // If bluetooth on


  if (interruptFlag == 0) {
    interruptFlag++;
    debounceTimer = ms + 100;
    if (bleOn) {
      // ble.deinit(); TODO
      bleOn = 0;
    }
    else {
      // ble.init(); TODO
      bleOn = 1;
    }

  }
}

void sensorsInit() {
  // UV
  if (!uv.begin()) {
    Serial.println("Failed to communicate with VEML6075 UV sensor! Please check your wiring.");
    error = 1;
  }
  Serial.println("Found VEML6075 (UV) sensor");

  // Air Quality init
  if (!ccs.begin()) {
    Serial.println("Failed to start Air Quality sensor! Please check your wiring.");
    while (1);
  }
  Serial.println("Found CCS811 (Air Quality) sensor");
  //calibrate temperature sensor on CCS811
  while (!ccs.available());
  float t = ccs.calculateTemperature();
  ccs.setTempOffset(t - 25.0);
  pedo.calibrate();
}

void handleWarning() {
  if (warningCounter == 0) {
    warningCounter++;
    
  }
}
