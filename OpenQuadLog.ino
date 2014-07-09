/**
  ___                    ___                  _ _
 / _ \ _ __   ___ _ __  / _ \ _   _  __ _  __| | |    ___   __ _
| | | | '_ \ / _ \ '_ \| | | | | | |/ _` |/ _` | |   / _ \ / _` |
| |_| | |_) |  __/ | | | |_| | |_| | (_| | (_| | |__| (_) | (_| |
 \___/| .__/ \___|_| |_|\__\_\\__,_|\__,_|\__,_|_____\___/ \__, |
      |_|                                                  |___/

      By: Lucas Teske
**/

#define NOHARDWARESERIAL

#include <FastSerial.h>
FastSerialPort0(Serial);
FastSerialPort1(Serial1);
//FastSerialPort2(Serial2);

#include <ctype.h>
//#include <SD.h>
#include "SoftwareSerial.h"
#include "FrSky.h"
#include "NazaGPS.h"

NazaGPS naza;
FrSky frsky;
SoftwareSerial frskyport(10,11,true); // RX, TX

unsigned long lastFrSkyTime = 0;
void setup() {
	digitalWrite(13,HIGH);     		// turn on debugging LED
	Serial.begin(115200);
	Serial1.begin(115200); 			//	Naza GPS Port
	Serial.println("HUE");
	frskyport.begin(9600);			//	FrSky Serial Port
	lastFrSkyTime = millis();
}

void loop()	{
	if(naza.CheckData())	{
		frsky.UpdateDataWithNaza(naza);
		digitalWrite(13,HIGH);
		//Serial << "Num Sat: " << naza.numSat << "\n";
		//Serial << naza.time.day << "/" << naza.time.month << "/" << naza.time.year << " - " << naza.time.hour << ":" << naza.time.minute << ":" << naza.time.seconds << "\n";
	}
	if(millis() > (lastFrSkyTime+200)){
		digitalWrite(13,LOW);
		digitalWrite(14, LOW);
		frsky.CheckData(frskyport);
		lastFrSkyTime= millis();
	}
}

