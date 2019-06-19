/*
	Main class for the Vitameter.
*/

#include <Wire.h>
#include <BluetoothSerial.h>
#include <Adafruit_CCS811.h>
#include <Adafruit_VEML6075.h>
#include <Adafruit_ADXL335.h>
#include <InterfaceOut.h>
#include <Timer.h>
#include <Values.h>
#include "C:\Users\Yumi\Desktop\wcs\config.h"


uint8_t state = LIGHT_SLEEP;

Adafruit_CCS811 ccs;
Adafruit_VEML6075 uv = Adafruit_VEML6075();
Adafruit_ADXL335 pedo;
BluetoothSerial SerialBT;
InterfaceOut vib(VIBRATION_PIN);
InterfaceOut led(LEDRED_PIN);
Timer timer;
Values values;


bool error = 0;
uint8_t interruptFlag = 0;
uint32_t debounceTimer = 0;
bool valueWarning = 0;
bool firstBooted = 1;

// Timers
uint32_t ms = 0;
uint32_t pedoTimeout = 0;
uint32_t uvTimeout = 0;
uint32_t airTimeout = 0;
uint32_t showTimeout = 0;




void setup() {
  // initialize the serial communications:
  Serial.begin(BAUDRATE);
  while (!Serial) {
    delay(10);
  }
  Wire.begin(SDA_PIN, SCL_PIN);
  pinMode(BUTTON_PIN, INPUT);
}

void loop() {
  ms = timer.getMillis();
  
  if (state == SENSORS_ACTIVE) {
    /*  Values.check();
    if (valueWarning) {
      valueWarning = 0;
    }
    */
    if (ms > pedoTimeout) {
      uint16_t x = pedo.getPedo();
      values.storeSteps(x);
      
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

    if ((ms > airTimeout) && ccs.available()) {
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
    if (ms > uvTimeout) {
      uvTimeout += UV_FREQ;
      uint8_t u = uv.readUVI();
      values.storeUVI(u);
    }

    if (ms > showTimeout) {
      /*String c = "CO20 ";

        for (int i = 0; i < co2_idx; i++) {
        c = c + co2[i] + " ";
        }
        c += "CO21";
        SerialBT.println(c);
        Serial.println(c);
        // SerialBT.println(pedo.steps);


        Serial.print("AQ: ");
        Serial.println(co2[co2_idx-1]);
        Serial.print("UV: ");
        Serial.println(uvi[uvi_idx-1]);
        Serial.print("Steps: ");
        Serial.println(pedo.steps);
        Serial.println();
      */
      
      Serial.println(ms);
      showTimeout += 1000;
    }
  }
  else if (state == ONLY_BT) {
    
  }
  else if (state == LIGHT_SLEEP) {
    goSleep();
  }

  if (interruptFlag > 0) {
    if (debounceTimer < ms) {
      interruptFlag = 0;
    }
  }
}

void goSleep() {
  Serial.println("Enter sleep");
  detachInterrupt(digitalPinToInterrupt(BUTTON_PIN));
  delay(2000);
  // esp_sleep_enable_ext0_wakeup(GPIO_NUM_27, 1); // for deep sleep
  // TODO Add Bluetooth button later
  gpio_wakeup_enable(GPIO_NUM_27, GPIO_INTR_HIGH_LEVEL);
  // gpio_wakeup_enable(GPIO_NUM_blabla, GPIO_INTR_HIGH_LEVEL);
  esp_sleep_enable_gpio_wakeup();
  esp_light_sleep_start();
  wakeUp();
}

void wakeUp() {
  if (digitalRead(BUTTON_PIN) == 1) {
    state = SENSORS_ACTIVE;
    sensorsInit();
  }
  else if (digitalRead(BT_PIN) == 1) {
    state = ONLY_BT;
    SerialBT.begin("Vitameter Yumi");
  }
  esp_sleep_disable_wakeup_source(ESP_SLEEP_WAKEUP_GPIO);
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), buttonISR, RISING);
  attachInterrupt(digitalPinToInterrupt(BT_PIN), buttonISR, RISING);
  Serial.println("Good morning!");
}

void buttonISR() {
  if (interruptFlag == 0) {
    interruptFlag++;
    debounceTimer = ms + 100;
    led.toggle();

    if (state == SENSORS_ACTIVE) {
      // esp_bt_controller_disable() ??
      state = LIGHT_SLEEP;
    }
  }
}

void bluetoothButtonISR() {
  // Only for BT off, because BT on is handled in wakeup function
  // If bluetooth on
  // SerialBT.stop();
  if (interruptFlag == 0) {
    interruptFlag++;
    debounceTimer = ms + 100;
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
