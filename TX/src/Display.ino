// Display.ino

void updateDisplay()
{
    showGeneral();
}

void showGeneral()
{
	display.clearDisplay();
    display.setCursor(0,0);
    display.setTextSize(2);
    display.setTextColor(BLACK, WHITE);
    if (!online & transmissionSuccess) {
        display.println("SIGNALLOST");
    }
    else if (!online) {
        display.println(" NO SIGNAL");
    }
    else if (radioLevel == 0){
        display.println("   NEAR   ");
    }
    else if (radioLevel == 1){
        display.println("  NOT FAR ");
    }
    else if (radioLevel == 2){
        display.println("   FAR    ");
    }
    else if (radioLevel == 3){
        display.println(" VERY FAR ");
    }
    else {
        display.println("          ");
    }
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.println("");

    if (modeGyro) display.println("MODE: GYRO   ");
    else if (!modeGyro) display.println("MODE: JOYSTICK ");

    if (status[SYSTEM_GYRO] != CODE_OK) display.println("GYRO " + String(scstr(status[SYSTEM_GYRO])) + " ");
    if (status[SYSTEM_GPS] != CODE_OK) display.print("GPS " + String(scstr(status[SYSTEM_GPS])) + " ");
    if (status[SYSTEM_TEMP] != CODE_OK) display.println("TEMP " + String(scstr(status[SYSTEM_TEMP])) + " ");
    if (status[SYSTEM_ACC] != CODE_OK) display.print("ACC " + String(scstr(status[SYSTEM_ACC])) + " ");
    if (status[SYSTEM_SERVO] != CODE_OK) display.println("SERVO " + String(scstr(status[SYSTEM_SERVO])) + " ");
    if (status[SYSTEM_RADIO] != CODE_OK) display.print("RADIO " + String(scstr(status[SYSTEM_RADIO])) + " ");

    display.display();
}

String scstr(int c)
{
	String str = "";
	if (c == CODE_UNKNOWN) str = "N/A";
	if (c == CODE_OK) str = "OK";
	if (c == CODE_WARNING) str = "WARN";
	if (c == CODE_FAULT) str = "FAULT";
	return str;
}