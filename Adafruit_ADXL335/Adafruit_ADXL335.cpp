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
  int acc=0;
  float totvect[15]={0};
  float totave[15]={0};
  float xaccl[15]={0};
  float yaccl[15]={0};
  float zaccl[15]={0};
  for (int i=0;i<15;i++)
  {
    xaccl[i]=float(analogRead(X_PIN));
    yaccl[i]=float(analogRead(Y_PIN));
    zaccl[i]=float(analogRead(Z_PIN));
    totvect[i] = sqrt(((xaccl[i]-xavg)* (xaccl[i]-xavg))+ ((yaccl[i] - yavg)*(yaccl[i] - yavg)) + ((zval[i] - zavg)*(zval[i] - zavg)));
    totave[i] = (totvect[i] + totvect[i-1]) / 2 ;

    //cal steps 
    if (totave[i]>threshhold && flag==0)
    {
       steps=steps+1;
       flag=1;
    }
    else if (totave[i] > threshhold && flag==1)
    {
        //do nothing 
    }
    if (totave[i] <threshhold  && flag==1)
    {
      flag=0;
    }
   // Serial.print("steps=");
   // Serial.println(steps);
   return(steps);
  }
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

void Adafruit_ADXL335::setAverage()
{
	xavg = readAxis(X_PIN, PEDO_CALIBRATION_SAMPLE_SIZE);
	yavg = readAxis(Y_PIN, PEDO_CALIBRATION_SAMPLE_SIZE);
	zavg = readAxis(Z_PIN, PEDO_CALIBRATION_SAMPLE_SIZE);
	Serial.print("X Average: "); Serial.println(xavg);
	Serial.print("Y Average: "); Serial.println(yavg);
	Serial.print("Z Average: "); Serial.println(zavg);
	Serial.println("Averages have been taken.");
}

void Adafruit_ADXL335::calibrate()
{
	int xRaw = readAxis(X_PIN, PEDO_SAMPLE_SIZE);
	int yRaw = readAxis(Y_PIN, PEDO_SAMPLE_SIZE);
	int zRaw = readAxis(Z_PIN, PEDO_SAMPLE_SIZE);

	if (xRaw < xRawMin)
	{
	xRawMin = xRaw;
	}
	if (xRaw > xRawMax)
	{
	xRawMax = xRaw;
	}

	if (yRaw < yRawMin)
	{
	yRawMin = yRaw;
	}
	if (yRaw > yRawMax)
	{
	yRawMax = yRaw;
	}

	if (zRaw < zRawMin)
	{
	zRawMin = zRaw;
	}
	if (zRaw > zRawMax)
	{
	zRawMax = zRaw;
	}

}

