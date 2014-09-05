/**
  ___                    ___                  _ _
 / _ \ _ __   ___ _ __  / _ \ _   _  __ _  __| | |    ___   __ _
| | | | '_ \ / _ \ '_ \| | | | | | |/ _` |/ _` | |   / _ \ / _` |
| |_| | |_) |  __/ | | | |_| | |_| | (_| | (_| | |__| (_) | (_| |
 \___/| .__/ \___|_| |_|\__\_\\__,_|\__,_|\__,_|_____\___/ \__, |
      |_|                                                  |___/

      By: Lucas Teske
      
      This is the version of Local Controller Data Logger. 
      It receives the data from FrSky Telemetry and stores on MicroSD Card
**/

#include <FastSerial.h>	
#include <Wire.h>
#include <SPI.h>
#include <SD.h>


#include "config.h"

FastSerialPort0(Serial);

#ifdef MEGA_MODE
FastSerialPort1(Serial1);
#endif

#include <ctype.h>
#include "SoftwareSerial.h"
#include "FrSky.h"
#include "OQL.h"

FrSky frsky;
SoftwareSerial frskyport(10,11,true); 	// RX, TX
OQL *oql;

unsigned long lastFrSkyTime = 0;		//	Just to loadoff the mainloop. We only start the FrSky checks every 200ms


void setup() {
	Serial.begin(115200);				//	Naza GPS Port on Non-MEGA mode, Debug Port on MEGA Mode
	frskyport.begin(9600);				//	FrSky Serial Port
	lastFrSkyTime = millis();
	oql = new OQL();
}

void loop()	{
	if(millis() > (lastFrSkyTime+FRSKY_INTERVAL)){
		digitalWrite(13,LOW);
		digitalWrite(14, LOW);
		frsky.CheckData(frskyport);
		lastFrSkyTime= millis();
	}
}

