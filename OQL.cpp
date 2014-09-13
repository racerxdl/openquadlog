/**
  ___                    ___                  _ _
 / _ \ _ __   ___ _ __  / _ \ _   _  __ _  __| | |    ___   __ _
| | | | '_ \ / _ \ '_ \| | | | | | |/ _` |/ _` | |   / _ \ / _` |
| |_| | |_) |  __/ | | | |_| | |_| | (_| | (_| | |__| (_) | (_| |
 \___/| .__/ \___|_| |_|\__\_\\__,_|\__,_|\__,_|_____\___/ \__, |
      |_|                                                  |___/

      By: Lucas Teske
**/

#include "OQL.h"

const String OQL::OQL_VER	= OQL_VERSION;

uint8_t OQL::CheckData(FrSky &frsky)	{
#ifdef USE_SD
#if defined(DEBUG_OQL) && defined (DEBUG_MEGA)
	if(millis() > (lastFrSkyTime+FRSKY_WRITE_INTERVAL))	{
		#if defined(DEBUG_MEGA) && defined(MEGA_MODE)
		frsky.WriteFrskyString(Serial);
		#endif
			lastFrSkyTime = millis();
	}
	if(millis() > (lastGPSTime+GPS_WRITE_INTERVAL))	{
		#if defined(DEBUG_MEGA) && defined(MEGA_MODE)
		frsky.WriteFrskyGPSString(Serial);
		#endif
		lastGPSTime = millis();
	}
#endif
	if(logen)	{
		if(millis() > (lastFrSkyTime+FRSKY_WRITE_INTERVAL))	{
			frsky.WriteFrskyString(filename);
			lastFrSkyTime = millis();
		}
		if(millis() > (lastGPSTime+GPS_WRITE_INTERVAL))	{
			frsky.WriteFrskyGPSString(filename);
			lastGPSTime = millis();
		}
	}
#endif
	return 0;
}

#ifdef USE_SD
void OQL::WriteToLog(const String &text)	{
	if(logen)	{
		File dataFile = SD.open(filename.c_str(), FILE_WRITE);
		if(dataFile)	{
			dataFile.print(text);
			dataFile.close();
		}
	}
}

void OQL::WriteToLog(const char *text)	{
	if(logen)	{
		File dataFile = SD.open(filename.c_str(), FILE_WRITE);
		if(dataFile)	{
			dataFile.print(text);
			dataFile.close();
		}
	}
}
void OQL::SetFilename()	{
	int i;
	char buff[16];
	i=1;
	if(logen)	{
		while(true)	{
			sprintf(buff, "OQL%04d.OQD",i);
			if(!SD.exists(buff))
				break;
			i++;
		}
		filename =  String(buff);
#if defined(DEBUG_MEGA) && defined(MEGA_MODE)
		Serial.print("Current Filename: ");
		Serial.println(filename);
#endif
		WriteToLog("LOG:STARTED:");
		WriteToLog(OQL_VER);
		WriteToLog("\n");
	}
}
#endif
