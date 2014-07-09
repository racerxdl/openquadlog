
#include "NazaGPS.h"
NazaGPS naza;
template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

void setup() {
	digitalWrite(13,HIGH);     		// turn on debugging LED
	Serial.begin(115200); 			//	PC Port
	Serial1.begin(115200);		    //	Naza Serial Port
}

void loop()	{
    if(naza.CheckData())	{
        Serial << "Sats: " << naza.numSat << " Lat: " << naza.latitude << " Lon: " << naza.longitude << " Alt: " << naza.altitude << "\n";
        //Serial << "Hardware Version: " << String((const char *)naza.hardware_version) << " Software Version:  " << String((const char *)naza.software_version) << "\n";
        Serial << naza.time.day << "/" << naza.time.month << "/" << naza.time.year << " - " << naza.time.hour << ":" << naza.time.minute << ":" << naza.time.seconds << "\n";
    }
}
