#include "config.h" 
#include "RX.h"   

#define DEF_SPEED 127    
#define LATENCY_MAX 1000
#define LATENCY_MIN 800

int ledPins[] = {LED1, LED2, LED3, LED4, LED6, LED7};
boolean ledStates[] = {false, false, false, false, false, false, false};
int pinCount = 6;
String btns; 
int speedA, speedB;
int sA, sB;
bool movementAllowed; 
bool online; 
bool syncSignal; 

int maxSpeed, minSpeed;

RF24 radio(CSN, CE); // select  CSN and CE  pins

BuggyMode mode = BUGGY_MODE_MANUAL;

const int systemsAmount = 7;
byte status[systemsAmount];

unsigned long latency; 

int LED = 3;

struct location
{
    long lat, lon;
    unsigned long time, date;
}location;

struct location l; 

/**************************************************/

void setup()
{  
    pinMode(LED1,OUTPUT);
    pinMode(LED2,OUTPUT);
    pinMode(LED3,OUTPUT);
    pinMode(LED4,OUTPUT);
    //pinMode(LED5,OUTPUT);
    pinMode(LED6,OUTPUT);
    pinMode(LED7,OUTPUT);
    pinMode(LED8,OUTPUT);
    pinMode(LED9,OUTPUT);
    fullStop();

    // For now systems' statuses are unknown
    for(int i=0; i<systemsAmount; i++){
        status[i] = CODE_UNKNOWN;
    }

    Serial.begin(9600);
    resetData();
    while (!Serial) {  }
    if (!radio.begin()) {
        Serial.println(F("radio hardware is not responding!!"));
        status[SYSTEM_RADIO] = CODE_FAULT;
        digitalWrite(LED_BUILTIN, HIGH);
        while (1) {
            digitalWrite(LED3, HIGH);
            delay(1000);
            digitalWrite(LED3, LOW);
            delay(1000);
        }
    }
    radio.setPALevel(RF24_PA_LOW);
    radio.setPayloadSize(sizeof(remoteControlPacket));
    radio.openWritingPipe(addr[radioNumber]); 
    radio.openReadingPipe(1, addr[!radioNumber]);
    radio.enableAckPayload();
    radio.startListening();
    radio.writeAckPayload(1, &telemetry, sizeof(telemetryPacket));
    status[SYSTEM_RADIO] = CODE_OK;

    initGPS();

    if (initTemp() < 0) status[SYSTEM_TEMP] = CODE_FAULT;
    else status[SYSTEM_TEMP] = CODE_OK;

    if (initServo() < 0) status[SYSTEM_TEMP] = CODE_FAULT;
    else status[SYSTEM_TEMP] = CODE_OK;
}

unsigned long lastRecvTime = 0;
unsigned long radioLevelTime = 0;

void loop()
{
    recvData();
    unsigned long now = millis();
    //Here we check if we've lost signal, if we did we reset the values 
    if ( now - lastRecvTime > TIMEOUT ) {
        // Signal lost?
        resetData();
        online = false;
        status[SYSTEM_RADIO] = CODE_WARNING;
    }
    else {
        status[SYSTEM_RADIO] = CODE_OK;
    }

    if (now - radioLevelTime > 500)
    {
        if (latency > LATENCY_MAX) encreaseRadioPower();
        if (latency < LATENCY_MIN) decreaseRadioPower();
    }

    updateHardware();

    readGPS();
    if (l.lat == 0 && l.lon == 0)
    {
        status[SYSTEM_GPS] = CODE_UNKNOWN;
    }
    else if (l.time == 0 && l.date == 0)
    {
        status[SYSTEM_GPS] = CODE_WARNING;
    }
}

unsigned long syncTimer;

void updateHardware()
{
    for (int i=0; i<systemsAmount; i++)
    {
        Serial.print(status[i]);
        Serial.print(" ");
    }
    Serial.print("\t");
    for (int i=0; i<systemsAmount; i++)
    {
        byte b = encodeVehicleStatus(i, status[i]);
        Serial.print(b, HEX);
        Serial.print(" ");
    }
    Serial.println("");

    // Speed constrains
    minSpeed = data.L1;
    maxSpeed = data.R1;

    if (data.L2) digitalWrite(LED8, HIGH); else digitalWrite(LED8, LOW);
    if (data.R2) digitalWrite(LED9, HIGH); else digitalWrite(LED9, LOW);
    
    setLedLevel(map(maxSpeed, 0, 255, 0, 6)); // Indicate max speed

    int speed = constrain(data.L3Y, minSpeed, maxSpeed); // Constrain speed
    int rotation, angleX, angleY;

    // Changing modes
    if (data.R2) 
    {
        mode = (BuggyMode)(mode & BUGGY_MODE_SINGLE_JOYSTICK);
    }
    else 
    {
        mode = (BuggyMode)(mode & ~BUGGY_MODE_SINGLE_JOYSTICK);
    }

    // Set speed depending on mode
    if (mode & BUGGY_MODE_SINGLE_JOYSTICK)
    {
        unsigned long timer;

        rotation = data.L3X;
        servo1Value = map(data.R3X, 0, 255, -90, 90); 
        servo2Value = map(data.R3Y, 0, 255, -90, 90); 

        if (millis() - timer >= 200)
        {
            angleX += servo1Value;
            angleY += servo2Value;
            timer = millis();
        }
        setServoAngles(servo1Value, servo2Value);

        if (readServo() < 0) status[SYSTEM_GPS] = CODE_FAULT;
        if (readServo() == 0) status[SYSTEM_GPS] = CODE_OK;
    }
    else
    {
        rotation = data.R3X;
    }
    

    if (speed - 126 >= 25) moveForward(speed); else if (speed - 126 <= -25) moveBackward(speed); 
    if (rotation - 134 >= 25) turnLeft(rotation); else if (rotation - 134 <= -25) turnRight(rotation); 
    if (rotation - 134 < 25 && rotation - 134 > -25 && data.L3Y - 126 > -25 && data.L3Y - 126 < 25) fullStop();
    if (!movementAllowed) fullStop();
    
    decodeButtons(data.btn);
}
