//LED.ino

void setLedLevel(int i)
{
    for (int thisPin = 0; thisPin < pinCount; thisPin++) 
    {
        digitalWrite(ledPins[thisPin], LOW);

    }
    for (int thisPin = pinCount; thisPin >= i; thisPin--) 
    {
        digitalWrite(ledPins[thisPin], HIGH);
    }
}

void switchLED(int n)
{
    if (ledStates[n]) digitalWrite(ledPins[n], HIGH);
    else digitalWrite(ledPins[n], LOW);
}

void LEDoff()
{
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    digitalWrite(LED6, LOW);
    digitalWrite(LED7, LOW);
    //digitalWrite(LED8, LOW);
    //digitalWrite(LED9, LOW);
}