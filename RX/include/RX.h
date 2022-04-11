// RX.h

#define LED1 13 
#define LED2 35 
#define LED3 36 
#define LED4 37 
#define LED6 5  
#define LED7 7  
#define LED8 9
#define LED9 11

#define SERVO1_PIN 33
#define SERVO2_PIN 34

#define SERVO3_PIN 33
#define SERVO4_PIN 34

#define ENA 12
#define ENB 2
#define IN1 10
#define IN2 8
#define IN3 6
#define IN4 4

#define CSN 24
#define CE 26

#define GPS_RX 35 
#define GPS_TX 36 

#define SERVO_RX 48 
#define SERVO_TX 46 

#define TEMP_ADDR 0x76



enum BuggyMode
{
	BUGGY_MODE_MANUAL = 1,
	BUGGY_MODE_AUTO = 1 << 2, // Use sonar sensors
	BUGGY_MODE_SINGLE_JOYSTICK = 1 << 3, 	// L3 controls motor, R3 controls servos, Otherwise L3Y controls motor speed, R3X controls rotation
	BUGGY_MODE_LINE = 1 << 4 // Use line sensors
};


int escValue, servo1Value, servo2Value, mRight, mLeft, mForvard, mReverce, aButton1;

void setLedLevel(int i);
void decodeButtons(byte i);
void switchLED(int n);
void LEDoff();
void fullStop();
void recvData();
void setSpeed();


