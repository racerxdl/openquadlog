#include <ctype.h>
#include "SoftwareSerial.h"
#include "FrSky.h"

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }

SoftwareSerial frskyserial(10, 11, true); // RX, TX
FrSky frsky;

void setup() {
	digitalWrite(13,HIGH);     		// turn on debugging LED
	Serial.begin(115200); 			//	Naza GPS Port
	frskyserial.begin(9600);		//	FrSky Serial Port
}

void loop()	{
	frsky.CheckData(frskyserial);
	frsky.latitude = millis();
	//Serial << "RSSI: " << frsky.RSSI << "\n";
	//Serial << "A1: " << frsky.A1 << "\n";
	//Serial << "A2: " << frsky.A2 << "\n";
}

