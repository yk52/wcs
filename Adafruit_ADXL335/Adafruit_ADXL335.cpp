/*------------------------------------------------------------
This is the .cpp file for ARDUINO PEDOMETER Library
This is the property of Frugal Labs Tech Solutions Pvt. Ltd.
Please refer lisence.txt for complete details.
-------------------------------------------------------------*/
#include "Arduino.h"
#include "Adafruit_ADXL335.h"
#include <Config.h>


int Adafruit_ADXL335::getPedo()
{
	float xaccl = readAxis(X_PIN, PEDO_SAMPLE_SIZE);
	float yaccl = readAxis(Y_PIN, PEDO_SAMPLE_SIZE);
	float zaccl = readAxis(Z_PIN, PEDO_SAMPLE_SIZE);

	int prev = 0;

	float totalVector =  ((xaccl - xavg)* (xaccl - xavg))+ ((yaccl - yavg)*(yaccl - yavg)) + ((zaccl - zavg)*(zaccl - zavg));
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
       // Serial.println((diff[idx] - diff[prev]));
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
	xavg = readAxis(X_PIN, PEDO_CALIBRATION_SAMPLE_SIZE);
	yavg = readAxis(Y_PIN, PEDO_CALIBRATION_SAMPLE_SIZE);
	zavg = readAxis(Z_PIN, PEDO_CALIBRATION_SAMPLE_SIZE);

	float totalVector = 0;
	float xaccl = 0;
	float yaccl = 0;
	float zaccl = 0;

	for (int i=0;i<PEDO_CALIBRATION_SAMPLE_SIZE;i++)
	{
		xaccl += float(analogRead(X_PIN));
		yaccl += float(analogRead(Y_PIN));
		zaccl += float(analogRead(Z_PIN));

	}
	xaccl /= PEDO_CALIBRATION_SAMPLE_SIZE;
	yaccl /= PEDO_CALIBRATION_SAMPLE_SIZE;
	zaccl /= PEDO_CALIBRATION_SAMPLE_SIZE;

	totalVector =  ((xaccl - xavg)* (xaccl - xavg))+ ((yaccl - yavg)*(yaccl - yavg)) + ((zaccl - zavg)*(zaccl - zavg));
	oldVector = totalVector;
	threshold = ACCL;

	Serial.print("X Average: "); Serial.println(xavg);
	Serial.print("Y Average: "); Serial.println(yavg);
	Serial.print("Z Average: "); Serial.println(zavg);
	Serial.println("Averages have been taken.");
	Serial.print("Threshold: "); Serial.println(threshold);
	Serial.print("Vector without acceleration: "); Serial.println(oldVector);
}

