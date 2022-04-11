// TX.h

#define abutton1 A10
#define jB1 50  // Joystick button 1
#define jB2 52  // Joystick button 2
#define L2_PIN 43   // Toggle switch 1 
#define R2_PIN 39   // Toggle switch 2 
#define L3X_PIN A5
#define L3Y_PIN A4
#define R3X_PIN A6
#define R3Y_PIN A7
#define L1_PIN A0
#define R1_PIN A1

#define WASDXY A8
#define W_BTN 51   // Button 1
#define A_BTN 49   // Button 2
#define S_BTN 47   // Button 3
#define D_BTN 45   // Button 4
#define X_BTN 42   // Button 5
#define Y_BTN 44   // Button 6
#define L3_BTN 34   // Button 7
#define R3_BTN 36   // Button 8

#define W_VAL 243   // Button 1
#define A_VAL 321   // Button 2
#define S_VAL 609   // Button 3
#define D_VAL 409   // Button 4
#define X_VAL 853   // Button 5
#define Y_VAL 1023   // Button 6

const int MPU = 0x68; 
float AccX, AccY, AccZ;
float GyroX, GyroY, GyroZ;
float accAngleX, accAngleY, gyroAngleX, gyroAngleY;
float angleX, angleY;
float AccErrorX, AccErrorY, GyroErrorX, GyroErrorY;
float elapsedTime, currentTime, previousTime;
int c = 0;
