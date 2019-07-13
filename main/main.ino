/*
  Main class for the Vitameter.
*/

#include <Wire.h>
#include <BluetoothSerial.h>
#include <BLE_wcs.h>
#include <Adafruit_CCS811.h>
#include <Adafruit_VEML6075.h>
#include <Adafruit_ADXL335.h>
#include <InterfaceOut.h>
// #include <Timer.h>
#include <Values.h>
#include "C:\Users\Yumi\Desktop\wcs\config.h"


uint8_t state = DEEP_SLEEP;

BluetoothSerial SerialBT;
// BLE_wcs ble;

Values values;

Adafruit_CCS811 ccs;
Adafruit_VEML6075 uv = Adafruit_VEML6075();
Adafruit_ADXL335 pedo;

InterfaceOut vib(VIBRATION_PIN);
InterfaceOut ledRed(LEDRED_PIN);
InterfaceOut ledGreen(LEDGREEN_PIN);
InterfaceOut ledBlue(LEDBLUE_PIN);
InterfaceOut sensors(SENSORS_EN_PIN); // auf high schalten nach wakeup

bool error = 0;
bool checkBT = 0;
bool checkPower = 0;
uint8_t interruptFlag = 0;
uint32_t debounceTimer = 0;
int warningCounter = 0;
bool firstBooted = 1;

// Timers
uint32_t ms = 0;
uint32_t sleepTime = 0;
uint32_t lastEmptied = 0;
uint32_t pedoTimeout = 0;
uint32_t uvTimeout = 0;
uint32_t airTimeout = 0;
uint32_t showTimeout = 0;

bool bleOn = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  // Serial communication init
  Serial.begin(BAUDRATE);
  while (!Serial) {
    delay(10);
  }

  // I2C communication to sensors init
  Wire.begin(SDA_PIN, SCL_PIN);

  // Buttons init
  pinMode(POWER_PIN, INPUT);
  pinMode(BLUETOOTH_PIN, INPUT);

  // Thresholds for sensor values init
  values.init();

  // TEST PURPOSES
  /* 
  // CHECK BUTTON INPUT VALUES
  for (int i = 0; i < 50; i++) {
    Serial.print("button: ");
    Serial.println(digitalRead(POWER_PIN));
    Serial.print("ble: ");
    Serial.println(digitalRead(BLUETOOTH_PIN));
    Serial.println();
    delay(500);
  }

  // CHECK VIBRATION MODULE
  vibrate(300, 5);
  */
  
  // TODO to burn in AQ
  sensors.on();
  delay(500);
}


//////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  ms = millis();

  if (checkBT || checkPower) {
    checkButtonState();
  }
  //___Bluetooth communication on ____________________________________
  if (bleOn) {
    // TODO
  }
  //__________________________________________________________________
  if (state == DEEP_SLEEP) {
    if (values.co2_idx >= 1) {
      values.storeRAMToFlash();
    }
    values.resetSteps();
    goDeepSleep();
  }
  //__________________________________________________________________
  else if (state == SENSORS_ACTIVE) {
    if (error) {
      handleWarning();
    }
    
    if (values.pedoEnable && ms > pedoTimeout) {
      uint16_t x = pedo.getPedo();
      bool goalAchieved = values.storeSteps(x);

      if (goalAchieved) {
        // vibrate in short intervals
        // vibrate(200, 5);
        ledBlue.on();
        delay(1000);
        ledBlue.off();
        delay(1000);
        ledBlue.on();
        delay(1000);
        ledBlue.off();
        delay(1000);
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
    
    if (ms > uvTimeout) {
      uvTimeout += UV_FREQ;
      uint8_t u = uv.readUVI();
      values.storeUVI(u);
    }
    if (ms > airTimeout) {
      while (!ccs.available());
      airTimeout += AQ_FREQ;
      if (!ccs.readData()) {
        uint16_t c = ccs.geteCO2();
        uint16_t v = ccs.getTVOC();
        values.storeCO2(c);
        values.storeVOC(v);
        values.storeTemp(ccs.calculateTemperature());

      } else {
        error = 1;
      }
    }


    if (ms > showTimeout) {

      Serial.println(ms);
      Serial.print("AQ: ");
      Serial.println(values.getLastCO2());
      Serial.print("UV: ");
      Serial.println(values.getLastUVI());
      Serial.print("Temp: ");
      Serial.println(values.getLastTemp());
      Serial.println();
      
      ledRed.toggle();
      ledBlue.toggle();
      ledGreen.toggle();
      // SerialBT.println(ms);
      showTimeout += 1000;
    }
    if ((ms - lastEmptied) > STORE_TO_FLASH_AFTER_MS) {
      lastEmptied = ms;
      values.storeRAMToFlash();
    }
  }
  
//__________________________________________________________________

  if (interruptFlag > 0) {
    if (debounceTimer < ms) {
      interruptFlag = 0;
    }
  }

//_____ Go sleep until next timeout ________________________________
  if (!checkButton && !values.pedoEnable && !bleOn) {
    // 1 second Delay to give ESP32 enough time to finish its tasks. Serial communication seems to stop abruptly without finishing when going into sleep.
    // Sleep a second less instead.
    delay(1000); 
    goLightSleepTimeout(UV_FREQ - 1000);
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void goLightSleepTimeout(uint64_t sleepMillis) {
  esp_sleep_enable_timer_wakeup(sleepMillis * 1000);
  esp_light_sleep_start();
}

void goDeepSleep() {
  sleepTime = ms;
  sensors.off();
  ledGreen.off();
  ledRed.off();
  ledBlue.off();
  Serial.println("Enter sleep");
  detachInterrupt(digitalPinToInterrupt(POWER_PIN));
  detachInterrupt(digitalPinToInterrupt(BLUETOOTH_PIN));
  delay(1000);

  // TODO change Interrupt level if necessary
  gpio_wakeup_enable(GPIO_NUM_4, GPIO_INTR_LOW_LEVEL);
  gpio_wakeup_enable(GPIO_NUM_2, GPIO_INTR_LOW_LEVEL);
  esp_sleep_enable_gpio_wakeup();
  esp_light_sleep_start();
  wakeUp();
}

void wakeUp() {
  delay(2500);
  // If button still pressed after 2.5 seconds
  if (digitalRead(POWER_PIN) == PRESSED_BUTTON_LEVEL) {
    state = SENSORS_ACTIVE;
    ledGreen.on();
    sensorsInit();
    values.storeSleepDuration(ms - sleepTime); // Does nothing currently. later store ms - sleepTime
  }
  else if (digitalRead(BLUETOOTH_PIN) == PRESSED_BUTTON_LEVEL) {
    bleOn = 1;
    state = ONLY_BT;
  }
  else {
    // Ignore if button is pressed less than 2.5 seconds
    goDeepSleep();
    return;
  }
  esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_GPIO);
  attachInterrupt(digitalPinToInterrupt(POWER_PIN), buttonISR, RISING);
  attachInterrupt(digitalPinToInterrupt(BLUETOOTH_PIN), bluetoothButtonISR, RISING);
  Serial.println("Good morning!");
  delay(1000);
}

void checkButtonState() {
  // Check Bluetooth button
  if (checkBT && (ms > btButtonPressed + 2500)) {
    checkBT = 0;
    if (digitalRead(BLUETOOTH_PIN) == PRESSED_BUTTON_LEVEL) {
      if (bleOn) {
          // ble.deinit(); TODO
          bleOn = 0;
          ledBlue.off();
        }
        else {
          // ble.init(); TODO
          bleOn = 1;
          ledBlue.on();
        }
    }
  }
  // Check Power Button
  if (checkPower && (ms > powerButtonPressed + 700) {
    if (digitalRead(POWER_PIN) == !PRESSED_BUTTON_LEVEL) {
      error = 0;
      checkPower = 0;
    }
  }
  if (checkPower && (ms > powerButtonPressed + 2500) {
    checkPower = 0;
    if (digitalRead(POWER_PIN) == PRESSED_BUTTON_LEVEL) {
      if (state == SENSORS_ACTIVE) {
        state = DEEP_SLEEP;
      }
      else {
        // Wake up sensors.
        state = SENSORS_ACTIVE;
        ledGreen.on();
        sensorsInit();
        values.storeSleepDuration(ms - sleepTime); // Does nothing currently. later store ms - sleepTime
      }
    }

  }
}

void buttonISR() {
  if (interruptFlag == 0) {
    interruptFlag++;
    debounceTimer = ms + 100;
    powerButtonPressed = ms;
    checkPower = 1;

  }
}

void bluetoothButtonISR() {
  // If bluetooth on

  if (interruptFlag == 0) {
    interruptFlag++;
    debounceTimer = ms + 100;
    btButtonPressed = ms;
    checkBT = 1;
  }
}

void sensorsInit() {
  sensors.on();
  delay(500);
  // UV
  if (!uv.begin()) {
    Serial.println("Failed to communicate with VEML6075 UV sensor! Please check your wiring.");
    error = 1;
  }
  else {
    Serial.println("Found VEML6075 (UV) sensor");
  }
  
  // Air Quality init
  if (!ccs.begin()) {
    Serial.println("Failed to start Air Quality sensor! Please check your wiring.");
    error = 1;
  }
  while (error) {
    ledRed.on();
    delay(1000);
    ledRed.off();
    delay(1000);
  }
  Serial.println("Found CCS811 (Air Quality) sensor");
  //calibrate temperature sensor on CCS811
  while (!ccs.available());
  float t = ccs.calculateTemperature();
  ccs.setTempOffset(t - 31.0);
  pedo.calibrate();
}

void handleWarning() {
  if (warningCounter == 0) {
    warningCounter++;
  }
  ledRed.on();
}

void vibrate(int delayMs, int times) {
  for (int i = 0; i < times; i++) {
    vib.on();
    delay(delayMs);
    vib.off();
    delay(delayMs);
  }
}
