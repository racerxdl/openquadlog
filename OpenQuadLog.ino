/**
  ___                    ___                  _ _
 / _ \ _ __   ___ _ __  / _ \ _   _  __ _  __| | |    ___   __ _
| | | | '_ \ / _ \ '_ \| | | | | | |/ _` |/ _` | |   / _ \ / _` |
| |_| | |_) |  __/ | | | |_| | |_| | (_| | (_| | |__| (_) | (_| |
 \___/| .__/ \___|_| |_|\__\_\\__,_|\__,_|\__,_|_____\___/ \__, |
      |_|                                                  |___/

      By: Lucas Teske
**/


#include <ctype.h>
#include "SD.h"
#include "SoftwareSerial.h"
#include "FrSky.h"
#include "NazaGPS.h"

NazaGPS naza;
FrSky frsky;

void setup() {
	digitalWrite(13,HIGH);     		// turn on debugging LED
	Serial.begin(115200);
	Serial1.begin(115200); 			//	Naza GPS Port
	Serial2.begin(9600);			//	FrSky Serial Port
}

void loop()	{
	if(naza.CheckData())	{
		frsky.UpdateDataWithNaza(naza);
		digitalWrite(13,HIGH);
		Serial << "Num Sat: " << naza.numSat << "\n";
		Serial << naza.time.day << "/" << naza.time.month << "/" << naza.time.year << " - " << naza.time.hour << ":" << naza.time.minute << ":" << naza.time.seconds << "\n";
	}
	digitalWrite(13,LOW);
	frsky.CheckData(Serial2);
}

