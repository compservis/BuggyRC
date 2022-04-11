// Servo.ino
#include <AltSoftSerial.h>

AltSoftSerial servoSerial; 
String servoBuffer; 

int initServo()
{
	unsigned long timer;
	servoSerial.begin(9600);
	servoSerial.println("HELLO");
	timer = millis();
	delay(2);
	while(1)
	{
		if (servoSerial.available())
		{
			servoBuffer = servoSerial.readString();
			if (servoBuffer.equals("OK"))
			{
				return 0;
			}
		}
		if (timer - millis() >= 200)
		{
			return -1;
		}
	}
}

void setServoAngles(int x, int y)
{
	servoSerial.println(String(x) + "," + String(y));
}

int readServo()
{
	while (servoSerial.available())
	{
		servoBuffer = servoSerial.readString();
		if (servoBuffer.equals("OK"))
		{
			return 0;
		}
		else 
		{
			return -1;
		}
	}
	return 1;
}