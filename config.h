#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

struct MyData {
  byte L1;  
  byte L2;
  //byte L3X; // Unused
  byte L3Y;
  byte R3X;
  //byte R3Y; // Unused
  byte R1;
  byte R2;
  //byte dpad; // encoded in btn
  byte btn;
};

MyData data;

/*Create a unique pipe out. The receiver has to 
  wear the same unique code*/ 
const uint64_t pipe = 0xE8E8F0F0A2LL; //IMPORTANT: The same as in the receiver!!!

// Битовые маски для кнопок:
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