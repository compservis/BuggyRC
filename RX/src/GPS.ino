#include <SoftwareSerial.h>
#include <TinyGPS.h> 

TinyGPS gps;
SoftwareSerial gpsSerial(GPS_RX, GPS_TX); 

bool newdata = false;
unsigned long start;
long latitude, longtitude;
unsigned long date, time;


void initGPS()
{
    gpsSerial.begin(9600); 
}

struct location readGPS()
{
    newdata = _readgps();
    if (newdata)
    {
        start = millis();

        gps.get_position(&latitude, &longtitude);
        gps.get_datetime(&date, &time);
        // valid time and date are more then 6-digit values
        if (time < 99999) {time = 0;}
        if (date < 99999) {date = 0;}

        l.lat = latitude;
        l.lon = longtitude;
        l.time = time;
        l.date = date;
    }
    return l;
}

bool _readgps()
{
    while (gpsSerial.available())
    {
        int b = gpsSerial.read();
        if('\r' != b)
        {
            if (gps.encode(b))
                return true;
        }
    }
    return false;
}
