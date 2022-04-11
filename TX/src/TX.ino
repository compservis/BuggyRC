#include "config.h"
#include "TX.h"
#include <SPI.h>
#include <RF24.h>
#include <I2Cdev.h>
#include <MPU6050.h>
#include <Adafruit_I2CDevice.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define CE_PIN 7
#define CSN_PIN 53
   
#define LATENCY_MAX 1000
#define LATENCY_MIN 800

RF24 radio(CE_PIN, CSN_PIN);
uint8_t address[][6] = {"1Node", "2Node"};

MPU6050 mpu; 
Adafruit_SSD1306 display(128, 64, &Wire, -1);

uint8_t encodeButtons();
uint8_t btns;

int16_t xval, yval, zval, roll, pitch, yaw;
bool mpuState, modeGyro; 

bool online, syncSignal; 

const int systemsAmount = 7;
byte status[systemsAmount];

unsigned long latency; 
int radioLevel; 

unsigned long radioLevelTime = 0;

void setup()
{
    pinMode(L3_BTN, INPUT_PULLUP);
    pinMode(R3_BTN, INPUT_PULLUP);
    Serial.begin(9600);
    mpu.initialize();
    mpuState = mpu.testConnection();

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

    while (!Serial) { }
    
    if (!radio.begin()) {
        Serial.println(F("Radio is not responding"));
        display.clearDisplay();
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0,0);
        display.println("Radio is not responding");
        display.display();
        while (1) { } 
    }

    radio.setPALevel(RF24_PA_MAX);
    radio.setPayloadSize(sizeof(remoteControlPacket));
    
    radio.enableAckPayload();
    radio.setRetries(5,5);

    radio.openWritingPipe(address[ROLE]);
    radio.openReadingPipe(1, address[!ROLE]);

    radio.stopListening();
}

int mapPotentiometers(int val, int lower, int middle, int upper, bool reverse)
{
    val = constrain(val, lower, upper);
    if ( val < middle )
        val = map(val, lower, middle, 0, 128);
    else
        val = map(val, middle, upper, 128, 255);
    return ( reverse ? 255 - val : val );
}

unsigned long syncTimer = 0;
void loop()
{
    updateHardware();
    updateDisplay();
    sendData();
    if (millis() - syncTimer >= TIMEOUT)
    {
        online = false;
    }

    if (millis() - radioLevelTime > 500)
    {
        if (latency > LATENCY_MAX) encreaseRadioPower();
        if (latency < LATENCY_MIN) decreaseRadioPower();
    }
}


void updateHardware() 
{
    bool sw = digitalRead(L2_PIN);
    if (sw & mpuState)
    {
        mpu.getMotion6(&xval, &yval, &zval, &roll, &pitch, &yaw);
        data.L3Y = map(xval, -32768, 32767, 0, 255);
        data.R3X = map(yval, -32768, 32767, 255, 0);
        modeGyro = true;
    }
    if (!sw)
    {
        modeGyro = false;
        data.R3X = mapPotentiometers(analogRead(R3X_PIN), 0, 512, 1024, true);   
        data.L3Y = mapPotentiometers(analogRead(L3Y_PIN), 0, 512, 1024, true);
        data.R3Y = mapPotentiometers(analogRead(R3Y_PIN), 0, 512, 1024, true);   
        data.L3X = mapPotentiometers(analogRead(L3X_PIN), 0, 512, 1024, true);
    }

    data.L1 = mapPotentiometers(analogRead(L1_PIN), 0, 512, 1024, true);
    data.R1 = mapPotentiometers(analogRead(R1_PIN), 0, 512, 1024, true);

    data.btn = encodeButtons();

     if(digitalRead(L2_PIN)) data.L2 = 0xFF; else data.L2 = 0x00;
     if(digitalRead(R2_PIN)) data.R2 = 0xFF; else data.R2 = 0x00;
}

uint8_t encodeButtons()
{
    btns = 0x00;
    int val = analogRead(WASDXY);
    if(val <= W_VAL + 5 && val >= W_VAL - 5) btns = btns | W;
    if(val <= A_VAL + 5 && val >= A_VAL - 5) btns = btns | A;
    if(val <= S_VAL + 5 && val >= S_VAL - 5) btns = btns | S;
    if(val <= D_VAL + 5 && val >= D_VAL - 5) btns = btns | D;
    if(val <= X_VAL + 5 && val >= X_VAL - 5) btns = btns | X;
    if(val <= Y_VAL + 5 && val >= Y_VAL - 5) btns = btns | Y;
    if(!digitalRead(L3_BTN)) btns = btns | L3;
    if(!digitalRead(R3_BTN)) btns = btns | R3;
    return btns;
}
