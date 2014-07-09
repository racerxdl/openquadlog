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

#ifdef MEGA_MODE
FastSerialPort1(Serial1);
#endif

#include <ctype.h>
//#include <SD.h>
#include "SoftwareSerial.h"
#include "FrSky.h"
#include "NazaGPS.h"

NazaGPS naza;
FrSky frsky;
SoftwareSerial frskyport(10,11,true); 	// RX, TX

unsigned long lastFrSkyTime = 0;		//	Just to loadoff the mainloop. We only start the FrSky checks every 200ms

void setup() {
	Serial.begin(115200);				//	Naza GPS Port on Non-MEGA mode, Debug Port on MEGA Mode
	#ifdef MEGA_MODE
		Serial1.begin(115200); 			//	Naza GPS Port
		#ifdef DEBUG_MEGA
		Serial.println("OQL Started");	
		#endif
	#endif
	frskyport.begin(9600);				//	FrSky Serial Port
	lastFrSkyTime = millis();
}

void loop()	{
#ifdef MEGA_MODE
	if(naza.CheckData(Serial1))	{	//	On MEGA Mode, Naza is hooked up on Serial 1
#else
	if(naza.CheckData(Serial))	{
#endif
		frsky.UpdateDataWithNaza(naza);
		digitalWrite(13,HIGH);
	}
	if(millis() > (lastFrSkyTime+200)){
		digitalWrite(13,LOW);
		digitalWrite(14, LOW);
		frsky.CheckData(frskyport);
		lastFrSkyTime= millis();
	}
}

