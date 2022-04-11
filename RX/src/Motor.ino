//Motor.ino

void setSpeed(int motor, int speed)
{
    if (speed <= 0) {setDirection(motor, 0); }
    else {setDirection(motor, 1);}

    if(speed < -126) speed = -126;
    if(speed > 126) speed = 127;

    if (motor == 0) 
    {
        if (abs(speed) <= 5) 
        {
            analogWrite(ENA, 0); 
        }
        else 
        {
            analogWrite(ENA, abs(speed)); 
        }
    }
    if (motor == 1) 
    {
        if (abs(speed) <= 5) 
        {
            analogWrite(ENB, 0); 
        }
        else 
        {
            analogWrite(ENB, abs(speed)); 
        }
    }
}

void fullStop()
{
    setSpeed(0, 0);
    setSpeed(1, 0);
}

void setDirection(int motor, boolean forward)
{
    if (motor == 0) 
    {
        if (forward) {digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);}
        else {digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);}
    }
    else if (motor == 1) 
    {
        if (forward) {digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);}
        else {digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);}
    }
}

void turnLeft(int speed)
{
    setDirection(0, 0);
    analogWrite(ENA, abs(255));
}

void turnRight(int speed)
{
    setDirection(0, 1);
    analogWrite(ENA, abs(255));
}

void moveForward(int speed)
{
    setDirection(1, 0);
    analogWrite(ENB, abs(210));
}

void moveBackward(int speed)
{
    setDirection(1, 1);
    analogWrite(ENB, abs(255));
}
