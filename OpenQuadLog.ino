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
#include "SoftwareSerial.h"
#include "FrSky.h"
#include "NazaGPS.h"

SoftwareSerial frskyserial(10, 11, true); // RX, TX
NazaGPS naza;
FrSky frsky;
uint8_t buffer[64];

void setup() {
	digitalWrite(13,HIGH);     		// turn on debugging LED
	Serial.begin(115200); 			//	Naza GPS Port
	frskyserial.begin(9600);		//	FrSky Serial Port
}

void loop()	{
	if(naza.CheckData())	
		frsky.UpdateDataWithNaza(naza);
	frsky.CheckData(frskyserial);
}

