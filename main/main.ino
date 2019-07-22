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
uint32_t bleTimer = 0;
bool bluetoothOn = 0;
BluetoothSerial btSerial;

Values values;
Adafruit_CCS811 ccs;
Adafruit_VEML6075 uv = Adafruit_VEML6075();
Adafruit_ADXL335 pedo;

InterfaceOut vib(VIBRATION_PIN);
InterfaceOut ledRed(LEDRED_PIN);
InterfaceOut ledGreen(LEDGREEN_PIN);
InterfaceOut ledBlue(LEDBLUE_PIN);
InterfaceOut sensors(SENSORS_EN_PIN); // auf high schalten nach wakeup

bool firstBoot = 1;
bool reactivateWarning = 0;
volatile bool checkBT = 0;
volatile bool checkPower = 0;
volatile uint32_t powerDebounceTimer = 0;
volatile uint32_t btDebounceTimer = 0;
volatile uint32_t btButtonPressed = 0;
volatile uint32_t powerButtonPressed = 0;


// Timers
uint32_t ms = 0;
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

  // ADC init
  pinMode(Y_PIN, INPUT);
  pinMode(Z_PIN, INPUT);
  
  // Thresholds for sensor values init
  values.init();
  values.warnCO2 = 1;
  values.warnVOC = 1;
  values.warnUVI = 1;
  sensors.on();
  delay(500);
  pedo.calibrate();
  values.pedoEnable = 1;
  ble.init("Vitameter BLE");
}


//////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  ms = millis();

  if (checkBT || checkPower) {
    checkButtonState();
  }

  if (reactivateWarning && ms > warningTimeout) {
    Serial.println("CO2 Warnings reactivated");
    Serial.println();
    values.warnCO2 = 1;
    // values.warnVOC = 1;
    // values.warnTemp = 1;
    reactivateWarning = 0;
  }
  
  //___Bluetooth low energy  communication on (is always on) ____________________________________
  if (bluetoothOn) {
    if (ms > bleTimer) {
      if (values.initBtSerial) {
        Serial.println("initBtSerial");
        btSerial.begin("Vitameter serial");
        values.initBtSerial = 0;
      }
      if (values.dataWanted_CO2) {
        Serial.println("CO2 Data wanted");
        btSerial.println(values.prepareCO2Data().c_str());
        values.dataWanted_CO2 = 0;
      } else if (values.dataWanted_UVI) {
        Serial.println("UVI Data wanted");
        btSerial.println(values.prepareUVIData().c_str());
        values.dataWanted_UVI = 0;
      } else if (values.dataWanted_steps) {
        Serial.println("Steps Data wanted");
        btSerial.println(values.prepareStepData().c_str());
        values.dataWanted_steps = 0;
      } else {
        bleTimer = ms + 3000;
        sent = ble.getMessage();
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
      }
    }
  }
  //__________________________________________________________________
  if (state == LIGHT_SLEEP) {
    
    if (values.uvi_idx >= 10) {
      values.storeRAMToFlash();
    }
    
    // values.resetSteps();
    goLightSleep();
  }
  //__________________________________________________________________
  else if (state == SENSORS_ACTIVE) {
    if (values.warning) {
      handleWarning();
    }
    else {
      warningCounter = 0;
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
        pedoTimeout = ms + WAIT_AFTER_STEP;
        pedo.flag = 0;
      }
      else {
        pedoTimeout += PEDO_FREQ;
      }
      if (goalVib && ms > vibTimeout) {
        vib.toggle();
        vibTimeout = ms + 200;
        vibCounter++;
      }
      if (vibCounter > 4) {
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
      Serial.print("Measurement number: ");
      Serial.println(values.co2_idx);

      Serial.print("CO2: ");
      Serial.println(values.getLastCO2());
      Serial.print("TVOC: ");
      Serial.println(values.getLastVOC());
      Serial.print("UV: ");
      Serial.println(values.getLastUVI());
      Serial.print("Temp: ");
      Serial.println(values.getLastTemp());
      Serial.println();
      Serial.println();
      
      showTimeout += showFreq;
    }
    if ((ms - lastEmptied) > STORE_TO_FLASH_AFTER_MS) {
      lastEmptied = ms;
      values.storeRAMToFlash();
    }
    
    //_____ Go sleep until next timeout ________________________________

    if (!(checkBT || checkPower) && !values.pedoEnable && !bluetoothOn && !values.warning) {
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
          goLightSleepTimeout(timeLeft - 500);
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
  bluetoothOn = 0;
  values.clearAllWarnings();
  vib.off();
  ledGreen.off();
  ledRed.off();
  ledBlue.off();
  gpio_wakeup_enable(GPIO_NUM_36, GPIO_INTR_LOW_LEVEL);
  gpio_wakeup_enable(GPIO_NUM_34, GPIO_INTR_LOW_LEVEL);
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
    state = SENSORS_ACTIVE;
    Serial.print("UVI thresh: ");
    Serial.println(EEPROM.read(UVI_THRESH_ADDR));
    Serial.print("CO2 thresh: ");
    Serial.println(values.getCO2Thresh());
    Serial.print("VOC thresh: ");
    Serial.println(EEPROM.read(VOC_THRESH_ADDR));
    Serial.print("Temp thresh: ");
    Serial.println(EEPROM.read(TEMP_THRESH_ADDR));
  
    Serial.print("Step goal: ");
    Serial.println(values.getStepGoal());
    
    ledGreen.on();
    sensorsInit();
    setTimeouts();
  }
  else if (digitalRead(BLUETOOTH_PIN) == PRESSED_BUTTON_LEVEL) {
    Serial.println("ONLY BLUETOOTH");
    ledBlue.on();
    bluetoothOn = 1;
    state = ONLY_BT;
    bleTimer = millis();
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
  ms = millis();
  
  // Check Bluetooth button
  if (checkBT) {
    if (ms > btButtonPressed + 1500) {
      checkBT = 0;
      if (digitalRead(BLUETOOTH_PIN) == PRESSED_BUTTON_LEVEL) {
        if (bluetoothOn) {
            bluetoothOn = 0;
            btSerial.end();
            values.setFlashIndexToStart();
            ledBlue.off();
            Serial.println("BT off");
            if (state == ONLY_BT) {
              Serial.println("Good Night");
              state = LIGHT_SLEEP;
            }
          }
        else {
          bluetoothOn = 1;
          ledBlue.on();
          Serial.println("BT on");
        }
      }
    }
    else if (ms > btButtonPressed + 500) {
      if (digitalRead(BLUETOOTH_PIN) == !PRESSED_BUTTON_LEVEL) {
        checkBT = 0;
        if (values.warning) {
          vib.off();
          dismissWarning();
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
          ledGreen.on();
          sensorsInit();
          setTimeouts();
        }
      }     
    }
  }
}

void buttonISR() {
  ms = millis();
  if (powerDebounceTimer < ms) {
    powerDebounceTimer = ms + 100;
    powerButtonPressed = ms;
    checkPower = 1;
  }
}

void bluetoothButtonISR() {
  ms = millis();
  // If bluetooth on
  if (btDebounceTimer < ms) {
    btDebounceTimer = ms + 100;
    btButtonPressed = ms;
    checkBT = 1;
  }
}

void sensorsInit() {
  bool error = 0;
  sensors.on();
  delay(500); 

  pedo.calibrate(); // TODO try it out here?
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
  
}

void handleWarning() {
  ledRed.on();
  ms = millis();
  if (warningVibTimeout < ms) {
    warningCounter++;
    warningVibTimeout = ms + 500;
    vib.toggle();
  }
  if (warningCounter >= 10) {
    dismissWarning();
  }
}

void dismissWarning() {
  String s;
  if (values.getUVIFlag()) {
    values.warnUVI = 0;
    values.clearUVIFlag();
    s = "UVI";
  }
  else if (values.getTempFlag()) {
    values.warnTemp = 0;
    values.clearTempFlag();
    s = "Temp";
  }
  else if (values.getVOCFlag()) {
    values.warnVOC = 0;
    values.clearVOCFlag();
    s = "VOC";
  }
  else if (values.getCO2Flag()) {
    values.warnCO2 = 0;
    values.clearCO2Flag();
    s = "CO2";
  }
  Serial.print("Warning dismissed: ");
  Serial.println(s);
  warningCounter = 0;
  warningTimeout = millis() + 30000;
  reactivateWarning = 1;
  ledRed.off();
  vib.off();
}
