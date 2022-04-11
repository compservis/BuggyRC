//Radio.ino

int counter = 0; 
void packTelemetry()
{
    telemetry.status = encodeVehicleStatus(counter, status[counter]);
    counter++;
    if (counter >= systemsAmount) counter = 0;

    telemetry.lat = 123;
    telemetry.lon = 234;
    telemetry.velocity = 500;
    telemetry.wheelAngle = 17;
    telemetry.motorTemp = 23;
}


void decodeButtons(byte i)
{
    btns = "";
    LEDoff();
    if((i & W) == W) {btns += "w"; digitalWrite(LED1, HIGH);}
    if((i & A) == A) {btns += "a"; digitalWrite(LED2, HIGH);} 
    if((i & S) == S) {btns += "s"; digitalWrite(LED3, HIGH);} 
    if((i & D) == D) {btns += "d"; digitalWrite(LED4, HIGH);} 
    if((i & X) == X) {btns += "x"; digitalWrite(LED6, HIGH);} 
    if((i & Y) == Y) {btns += "y"; digitalWrite(LED7, HIGH);} 
    if((i & L3) == L3) {btns += "l";} 
    if((i & R3) == R3) {btns += "r";} 

}

void recvData()
{
    uint8_t pipe;
    if (radio.available(&pipe)) 
    {             
        radio.read(&data, sizeof(remoteControlPacket));
        lastRecvTime = millis();
        movementAllowed = true;
        newDataAvailable = true;
        online = true;
        syncSignal = true;

        packTelemetry();
        radio.writeAckPayload(1, &telemetry, sizeof(telemetryPacket));
    }
}

void sendData()
{
    unsigned long start_timer = micros();                   
    bool report = radio.write(&telemetry, sizeof(telemetryPacket));
    unsigned long end_timer = micros();
    
    if (report) 
    {
        transmissionSuccess = true;
        syncTimer = millis();
        latency = end_timer - start_timer;
    } 
    else 
    {
        transmissionSuccess = false;
    }
}

void resetData() 
{
    data.L1 = 147; 
    data.L2 = 0;
    data.R3X = 0;
    data.L3Y = 0; 
    data.R1 = 147;
    data.R2 = 0;
    movementAllowed = false;
    fullStop();
}

void encreaseRadioPower()
{
    int pwr = radio.getPALevel();
    if (pwr == RF24_PA_MIN) radio.setPALevel(RF24_PA_LOW);
    else if (pwr == RF24_PA_LOW) radio.setPALevel(RF24_PA_HIGH);
    else if (pwr == RF24_PA_HIGH) radio.setPALevel(RF24_PA_MAX);
}

void decreaseRadioPower()
{
    int pwr = radio.getPALevel();
    if (pwr == RF24_PA_MAX) radio.setPALevel(RF24_PA_HIGH);
    else if (pwr == RF24_PA_HIGH) radio.setPALevel(RF24_PA_LOW);
    else if (pwr == RF24_PA_LOW) radio.setPALevel(RF24_PA_MIN);
}
