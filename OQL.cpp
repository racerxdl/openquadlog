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

uint8_t OQL::CheckData(FrSky &frsky)	{
#ifdef USE_SD
	String tmp;
	if(logen)	{
		File dataFile = SD.open(filename.c_str(), FILE_WRITE);
		if(dataFile)	{
			if(millis() > (lastFrSkyTime+FRSKY_WRITE_INTERVAL))	{
				String frskys = frsky.GenerateFrSkyString();
				#if defined(DEBUG_MEGA) && defined(MEGA_MODE)
					Serial.println(frskys);
				#endif
				dataFile << frskys.c_str() << "\n";
			}
			if(millis() > (lastGPSTime+GPS_WRITE_INTERVAL))	{
				String gps = frsky.GenerateFrSkyGPSString();
				#if defined(DEBUG_MEGA) && defined(MEGA_MODE)
					Serial.println(gps);
				#endif
				dataFile << gps.c_str() << "\n";
			}
			dataFile.close();
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
