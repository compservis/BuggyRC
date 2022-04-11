#include <SPI.h>
#include <RF24.h>

#define ROLE 0 // 0 - RX, 1 - TX
#define TIMEOUT 1 * 1000 // timeout time in seconds * 1000

struct remoteControlPacket {
  byte L1;  
  byte L2;
  byte L3X;
  byte L3Y;
  byte R3X;
  byte R3Y;
  byte R1;
  byte R2;
  byte btn;
};

remoteControlPacket data;


struct telemetryPacket {
  int velocity;
  int lat;
  int lon;
  int wheelAngle;
  int motorTemp;
  byte status;
};

telemetryPacket telemetry;


uint8_t addr[][6] = {"1Node", "2Node"};
bool radioNumber = ROLE; // 0 - RX, 1 - TX

bool newDataAvailable; 
bool transmissionSuccess;

const uint8_t W = 0x80;
const uint8_t A = 0x40;
const uint8_t S = 0x20;
const uint8_t D = 0x10;
const uint8_t X = 0x08;
const uint8_t Y = 0x04;
const uint8_t L3 = 0x02;
const uint8_t R3 = 0x01;

void resetData();
void updateHardware();

enum BuggySystems
{
  SYSTEM_ALL,
  SYSTEM_GYRO,
  SYSTEM_GPS,
  SYSTEM_TEMP,
  SYSTEM_ACC,
  SYSTEM_SERVO,
  SYSTEM_RADIO
};

enum StatusCodes
{
  CODE_UNKNOWN = 0,
  CODE_OK,
  CODE_WARNING,
  CODE_FAULT
};

// ------- VEHICLE STATUS -------

byte encodeVehicleStatus(byte system, byte code)
{
    byte res = (system << 4) | (code & 0xF);
    return res;
}

byte decodeVehicleStatusSystem(int code)
{
    return ((byte)code & 0xF0) >> 4;
}

byte decodeVehicleStatusCode(int code)
{
    return (byte)code & 0xF;
}

// -------------------------------
