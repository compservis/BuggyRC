// Radio.ino

void resetData() 
{
    data.L1 = 0; 
    data.L2 = 0;
    data.R3X = 0;
    data.L3Y = 0; 
}

void sendData()
{
    unsigned long start_timer = micros(); 
    bool report = radio.write(&data, sizeof(remoteControlPacket)); 
    unsigned long end_timer = micros(); 
    
    if (report) 
    {
        transmissionSuccess = true; 
        online = true;
        syncTimer = millis();
        Serial.print(F("Latency:"));
        Serial.print(end_timer - start_timer);

        if (radio.isAckPayloadAvailable())
        {
            radio.read(&telemetry, sizeof(telemetryPacket));
            status[decodeVehicleStatusSystem(telemetry.status)] = decodeVehicleStatusCode(telemetry.status);
            newDataAvailable = true;
            online = true;
            Serial.print("GOT DATA: ");
            Serial.print(telemetry.velocity);
            Serial.print("\t");
            Serial.print(telemetry.lat);
            Serial.print("\t");
            Serial.print(telemetry.lon);
            Serial.print("\t");
            Serial.print(telemetry.wheelAngle);
            Serial.print("\t");
            Serial.print(telemetry.motorTemp);
            Serial.print("\t");
            Serial.print(telemetry.status);
            Serial.print("\n");
        }
        else
        {
            Serial.print("ACK, BUT NO DATA \n");
        }
    } 
    else 
    {
        transmissionSuccess = false;
    }
}

unsigned long lastRecvTime = 0;
void recvData()
{
    uint8_t pipe;
    if (radio.available(&pipe)) 
    {             
        radio.read(&telemetry, sizeof(telemetryPacket));
        newDataAvailable = true;
        online = true;
        syncSignal = true;
        Serial.print("GOT DATA: ");
        Serial.print(telemetry.velocity);
        Serial.print("\t");
        Serial.print(telemetry.lat);
        Serial.print("\t");
        Serial.print(telemetry.lon);
        Serial.print("\t");
        Serial.print(telemetry.wheelAngle);
        Serial.print("\t");
        Serial.print(telemetry.motorTemp);
        Serial.print("\t");
        Serial.print(telemetry.status);
        Serial.print("\n");
    }
}

void encreaseRadioPower()
{
    int pwr = radio.getPALevel();
    if (pwr == RF24_PA_MIN) {radio.setPALevel(RF24_PA_LOW); radioLevel = 1;}
    else if (pwr == RF24_PA_LOW) {radio.setPALevel(RF24_PA_HIGH); radioLevel = 2;}
    else if (pwr == RF24_PA_HIGH) {radio.setPALevel(RF24_PA_MAX); radioLevel = 3;}
}

void decreaseRadioPower()
{
    int pwr = radio.getPALevel();
    if (pwr == RF24_PA_MAX) {radio.setPALevel(RF24_PA_HIGH); radioLevel = 2;}
    else if (pwr == RF24_PA_HIGH) {radio.setPALevel(RF24_PA_LOW); radioLevel = 1;}
    else if (pwr == RF24_PA_LOW) {radio.setPALevel(RF24_PA_MIN); radioLevel = 0;}
}
