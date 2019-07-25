/*------------------------------------------------------------
This is the .cpp file for ARDUINO PEDOMETER Library
This is the property of Frugal Labs Tech Solutions Pvt. Ltd.
Please refer lisence.txt for complete details.
-------------------------------------------------------------*/
#include "Arduino.h"
#include "Adafruit_ADXL335.h"
#include "C:\Users\Yumi\Desktop\wcs\config.h"


int Adafruit_ADXL335::getPedo()
{
	float zaccl = readAxis(Z_PIN, PEDO_SAMPLE_SIZE);

	int prev = 0;

	float totalVector =  (zaccl - zavg)*(zaccl - zavg);
	diff[idx] = oldVector - totalVector;

	if (idx == 0) {
		prev = 14;
	}
	else
	{
		prev = idx - 1;
	}

    //cal steps 
    if ((diff[idx] - diff[prev]) >threshold)
    {
       steps=steps+1;
       flag=1;
    }

    oldVector = totalVector;
    if (idx == 14) {
    	idx = 0;
    }
    else {
    	idx++;
    }
    return(steps);

}

int Adafruit_ADXL335::readAxis(int axisPin, int samples)
{
  long reading = 0;
  analogRead(axisPin);
  for (int i = 0; i < samples; i++)
  {
    reading += analogRead(axisPin);
  }
  return reading/samples;
}

void Adafruit_ADXL335::calibrate()
{
	zavg = readAxis(Z_PIN, PEDO_CALIBRATION_SAMPLE_SIZE);

	float totalVector = 0;
	float zaccl = 0;

	for (int i=0;i<PEDO_CALIBRATION_SAMPLE_SIZE;i++)
	{
		zaccl += float(analogRead(Z_PIN));

	}

	zaccl /= PEDO_CALIBRATION_SAMPLE_SIZE;

	totalVector =  (zaccl - zavg)*(zaccl - zavg);
	oldVector = totalVector;
	threshold = ACCL;

	Serial.print("Z Average: "); Serial.println(zavg);
	Serial.println("Average has been taken.");
	Serial.print("Threshold: "); Serial.println(threshold);
	Serial.print("Vector without acceleration: "); Serial.println(oldVector);
}

