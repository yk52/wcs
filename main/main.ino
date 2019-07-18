/*
  Main class for the Vitameter.
*/

#include <Wire.h>
#include <BluetoothSerial.h>
#include <BLE_wcs.h>
#include <stdio.h>
#include <string>
#include <Adafruit_CCS811.h>
#include <Adafruit_VEML6075.h>
#include <Adafruit_ADXL335.h>
#include <InterfaceOut.h>
#include <Values.h>
#include "C:\Users\Yumi\Desktop\wcs\config.h"

uint8_t state = LIGHT_SLEEP;

// Bluetooth
std::string sent;
std::string processed = "prodef";
BLE_wcs ble;

Values values;
Adafruit_CCS811 ccs;
Adafruit_VEML6075 uv = Adafruit_VEML6075();
Adafruit_ADXL335 pedo;

InterfaceOut vib(VIBRATION_PIN); //VIBRATION_PIN);
InterfaceOut ledRed(LEDRED_PIN);
InterfaceOut ledGreen(LEDGREEN_PIN);
InterfaceOut ledBlue(LEDBLUE_PIN);
InterfaceOut sensors(SENSORS_EN_PIN); // auf high schalten nach wakeup

bool firstBoot = 1;
bool error = 0;
volatile bool checkBT = 0;
volatile bool checkPower = 0;
volatile uint32_t powerDebounceTimer = 0;
volatile uint32_t btDebounceTimer = 0;
volatile uint32_t btButtonPressed = 0;
volatile uint32_t powerButtonPressed = 0;


// Timers
uint32_t ms = 0;
bool warning = 0;
uint32_t warningTimeout = 0;
uint8_t warningCounter = 0;
uint32_t warningVibTimeout = 0;
bool goalVib = 0;
uint8_t vibCounter = 0;
uint32_t vibTimeout = 0;
uint32_t showFreq = 2000;
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
  values.setStepGoal(10);
  
  Serial.print("Step goal: ");
  Serial.println(values.getStepGoal());
  sensors.on();
  delay(500);
  pedo.calibrate();
  // values.pedoEnable = 1;
  // sensors.off(); // TODO ADD AGAIN LATER
}


//////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  ms = millis();

  if (checkBT || checkPower) {
    checkButtonState();
  }
  //___Bluetooth communication on ____________________________________
  if (bleOn) {
    Serial.print("message sent:   ");
    sent = ble.getMessage();
    Serial.println(sent.c_str());
    processed = values.processMessage(sent);

    Serial.print("message processed:   ");
    Serial.println(processed.c_str());
    
    Serial.print("parameter:   ");
    Serial.println(values.parameter.c_str());
    
    Serial.print("value is:   ");
    Serial.println(values._value);
    
    ble.write(processed);

    Serial.println("");
    Serial.println("");
    Serial.println("");
    Serial.println("");
  }
  //__________________________________________________________________
  if (state == LIGHT_SLEEP) {
    /*
    if (values.uvi_idx >= 1) {
      values.storeRAMToFlash();
    }
    */
    values.resetSteps();
    goLightSleep();
  }
  //__________________________________________________________________
  else if (state == SENSORS_ACTIVE) {
    if ((warningTimeout < ms) && values.warning) {
      handleWarning();
    }
    if (values.pedoEnable && ms > pedoTimeout) {
      uint16_t x = pedo.getPedo();
      
      // Step registered
      if (pedo.flag) {
        Serial.println(x);
        bool goalAchieved = values.storeSteps(x);
        if (goalAchieved) {
          // vibrate in short intervals
          goalVib = 1;
          vibTimeout = ms;
        }
        pedoTimeout += WAIT_AFTER_STEP;
        pedo.flag = 0;
      }
      else {
        pedoTimeout += PEDO_FREQ;
      }
      if (goalVib && ms > vibTimeout) {
        vib.toggle();
        vibTimeout += 500;
        vibCounter++;
      }
      if (vibCounter >= 4) {
        goalVib = 0;
        vibCounter= 0;
        vib.off();
      }
    }
    
    if (ms > uvTimeout) {
      uvTimeout += UV_FREQ;
      uint8_t u = uv.readUVI();
      values.storeUVI(u);
    }
    if (ms > airTimeout) {
      ccs.readData();
      airTimeout += AQ_FREQ;
      uint16_t c = ccs.geteCO2();
      uint16_t v = ccs.getTVOC();
      values.storeCO2(c);
      values.storeVOC(v);
      values.storeTemp(ccs.calculateTemperature());
    }

    if (ms > showTimeout) {
      Serial.print("CO2_idx: ");
      Serial.println(values.co2_idx);
      Serial.print("UV_idx: ");
      Serial.println(values.uvi_idx);
      Serial.print("CO2: ");
      Serial.println(values.getLastCO2());
      Serial.print("TVOC: ");
      Serial.println(values.getLastVOC());
      Serial.print("UV: ");
      Serial.println(values.getLastUVI());
      Serial.print("Temp: ");
      Serial.println(values.getLastTemp());
      Serial.println();

      showTimeout += showFreq;
    }
    if ((ms - lastEmptied) > STORE_TO_FLASH_AFTER_MS) {
      lastEmptied = ms;
      // values.storeRAMToFlash();
    }
    
    //_____ Go sleep until next timeout ________________________________

    if (!(checkBT || checkPower) && !values.pedoEnable && !bleOn && !warning) {
      // 1 second Delay to give ESP32 enough time to finish its tasks. Serial communication seems to stop abruptly without finishing when going into sleep.
      // Sleep a second less instead.
      ms = millis();
      if (uvTimeout > ms) {
        uint32_t timeLeft = uvTimeout - ms;
        if (timeLeft > 2000) {
          delay(500);
          gpio_wakeup_enable(GPIO_NUM_36, GPIO_INTR_LOW_LEVEL);
          gpio_wakeup_enable(GPIO_NUM_34, GPIO_INTR_LOW_LEVEL); 
          esp_sleep_enable_gpio_wakeup();
          goLightSleepTimeout(timeLeft - 500);//timeLeft - 1000);
          if (esp_sleep_get_wakeup_cause() == 7) {
            if (digitalRead(POWER_PIN) == PRESSED_BUTTON_LEVEL) {
              checkPower = 1; 
              powerButtonPressed = ms;
            }
            else if (digitalRead(BLUETOOTH_PIN) == PRESSED_BUTTON_LEVEL) {
              checkBT = 1; 
              btButtonPressed = ms;
            }
          }
        }
      }
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////

void goLightSleepTimeout(uint64_t sleepMillis) {
  esp_sleep_enable_timer_wakeup(sleepMillis * 1000);
  esp_light_sleep_start();
  esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_TIMER);
}

void goLightSleep() {
  Serial.println("Enter sleep");
  detachInterrupt(digitalPinToInterrupt(POWER_PIN));
  detachInterrupt(digitalPinToInterrupt(BLUETOOTH_PIN));
  delay(1000);
  bleOn = 0;
  // sensors.off();
  ledGreen.off();
  ledRed.off();
  ledBlue.off();
  vib.off();
  gpio_wakeup_enable(GPIO_NUM_36, GPIO_INTR_LOW_LEVEL);
  gpio_wakeup_enable(GPIO_NUM_34, GPIO_INTR_LOW_LEVEL);
  esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_TIMER);
  esp_sleep_enable_gpio_wakeup();
  esp_light_sleep_start();
  wakeUp();
}

void setTimeouts() {
  ms = millis();
  pedoTimeout = PEDO_FREQ + ms;
  uvTimeout = UV_FREQ + ms;
  airTimeout = AQ_FREQ + ms;
  showTimeout = ms + showFreq;
}

void wakeUp() {
  // If button still pressed after 1 second
  delay(1000);
  if (digitalRead(POWER_PIN) == PRESSED_BUTTON_LEVEL) {
    Serial.println("WAKE UP, POWER UP!");
    Serial.println(values.getStepGoal());
    state = SENSORS_ACTIVE;
    // ledGreen.on();
    ledRed.on();
    sensorsInit();
    setTimeouts();
  }
  else if (digitalRead(BLUETOOTH_PIN) == PRESSED_BUTTON_LEVEL) {
    Serial.println("ONLY BLUETOOTH");
    ledBlue.on();
    bleOn = 1;
    ble.init("Vitameter Main");
    state = ONLY_BT;
  }
  else {
    // Ignore if button is pressed less than a second
    Serial.println("Nevermind. Button pressed < 1 s");
    state = LIGHT_SLEEP;
    return;
  }
  esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_GPIO);
  attachInterrupt(digitalPinToInterrupt(POWER_PIN), buttonISR, FALLING);
  attachInterrupt(digitalPinToInterrupt(BLUETOOTH_PIN), bluetoothButtonISR, FALLING);
  Serial.println("Good morning!");
}

void checkButtonState() {
  // Check Bluetooth button
  if (checkBT) {
    if (ms > btButtonPressed + 1500) {
      checkBT = 0;
      if (digitalRead(BLUETOOTH_PIN) == PRESSED_BUTTON_LEVEL) {
        if (bleOn) {
            ble.deinit();
            bleOn = 0;
            ledBlue.off();
            Serial.println("BT off");
            if (state == ONLY_BT) {
              Serial.println("Good Night");
              state = LIGHT_SLEEP;
            }
          }
        else {
          ble.init("Vitameter main");
          bleOn = 1;
          ledBlue.on();
          Serial.println("BT on");
        }
      }
      else if (ms > btButtonPressed + 500) {
        if (digitalRead(BLUETOOTH_PIN) == !PRESSED_BUTTON_LEVEL) {
          checkBT = 0;
          Serial.println("BT nevermind");
        }
      }
    }
  }
  // Check Power Button
  if (checkPower) {
    if (ms > powerButtonPressed + 1500) {
      checkPower = 0;
      if (digitalRead(POWER_PIN) == PRESSED_BUTTON_LEVEL) {
        if (state == SENSORS_ACTIVE) {
          state = LIGHT_SLEEP;
        }
        else {
          // Wake up sensors.
          state = SENSORS_ACTIVE;
          // ledGreen.on();
          ledRed.on();
          sensorsInit();
          // values.storeSleepDuration(ms - sleepTime); // Does nothing currently. later store ms - sleepTime
          setTimeouts();
        }
      }
      else if (ms > powerButtonPressed + 500) {
        if (digitalRead(POWER_PIN) == !PRESSED_BUTTON_LEVEL) {
          checkPower = 0;
          if (values.warning) {
            dismissWarning();
          }
          vib.off();
          ledRed.off();
        }
      }
    }
  }
}

void buttonISR() {
  if (powerDebounceTimer < ms) {
    powerDebounceTimer = ms + 100;
    powerButtonPressed = ms;
    checkPower = 1;
  }
}

void bluetoothButtonISR() {
  // If bluetooth on
  if (btDebounceTimer < ms) {
    btDebounceTimer = ms + 100;
    btButtonPressed = ms;
    checkBT = 1;
  }
}

void sensorsInit() {
  sensors.on();
  delay(3000); 

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
  else {
    Serial.println("Found CCS811 (Air Quality) sensor");
  }
  while (error) {
    ledRed.on();
    delay(1000);
    ledRed.off();
    delay(1000);
    if (digitalRead(POWER_PIN) == PRESSED_BUTTON_LEVEL) {
      state = LIGHT_SLEEP;
      return;
    }
  }

  if (firstBoot) {
    while (!ccs.available());
    float t = ccs.calculateTemperature();
    ccs.setTempOffset(t - 23.0);

    firstBoot = 0;    
  }
  
  pedo.calibrate();
  error = 0;
}

void handleWarning() {
  ledRed.on();
  warning = 1;
  if (warningVibTimeout < ms) {
    warningCounter++;
    warningVibTimeout = millis() + 1000;
    vib.toggle();
  }
  if (warningCounter >= 8) {
    warningCounter = 0;
    dismissWarning();
  }
}

void dismissWarning() {
  Serial.println("Warning dismissed");
  warning = 0;
  warningCounter = 0;
  warningTimeout = millis() + 60000;
  ledRed.off();
  vib.off();
}
