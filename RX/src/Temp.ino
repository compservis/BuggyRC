#include <Wire.h>                                       

#include <Adafruit_Sensor.h>                            
#include <Adafruit_BME280.h>                            

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;

int initTemp()
{
  if (!bme.begin(TEMP_ADDR)) {                             
     return -1;
  }
  else {return 0;}
}

float readTemp() 
{
  return bme.readTemperature();
}

float readPres() 
{
  return bme.readPressure() / 100.0F;
}

float readAlt() 
{
  return bme.readAltitude(SEALEVELPRESSURE_HPA);
}

float readHum() 
{
  return bme.readHumidity();
}
 
