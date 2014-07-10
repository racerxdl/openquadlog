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

uint8_t OQL::CheckData(NazaGPS &naza, FrSky &frsky)	{
#ifdef READ_INPUTS
	uint16_t v1r = analogRead(V1_PIN);
	uint16_t v2r = analogRead(V2_PIN);
	uint8_t  s1r = digitalRead(S1_PIN);
	uint8_t  s2r = digitalRead(S2_PIN);

	if(S1TOVAL(v1r) != v1 || S1TOVAL(v2r) != v2 || s1r != s1 || s2r != s2)	{
		s1 = s1r;
		s2 = s2r;
		v1 = S1TOVAL(v1r);
		v2 = S1TOVAL(v2r);
		return 1;
	}
#endif
	String tmp;
	if(logen)	{
		File dataFile = SD.open(filename.c_str(), FILE_WRITE);
		if(dataFile)	{
			#ifdef READ_NAZA
			if((naza.recv_version==1) && (versionwrote!=1))	{
				dataFile << OQL_SOFTVER << ":" << (const char *)naza.software_version << "\n";
				dataFile << OQL_HARDVER << ":" << (const char *)naza.hardware_version << "\n";
				versionwrote = 1;
			}
			if(GPSChanged(naza))	{
				lastlat = naza.latitude;
				lastlon = naza.longitude;
				lastalt = naza.altitude;
				lastfix = naza.fix;
				lastsats= naza.numSat;
				String gps = naza.GenerateGPSString();
				#if defined(DEBUG_MEGA) && defined(MEGA_MODE)
					Serial.println(gps);
				#endif
				dataFile << gps.c_str() << "\n";
			}
			#endif
			#ifdef READ_FRSKY
			if(FrSkyChanged(frsky))	{
				lastRSSI = frsky.RSSI;
				lastA1   = frsky.A1;
				lastA2   = frsky.A2;
				String frskys = frsky.GenerateFrSkyString();
				#if defined(DEBUG_MEGA) && defined(MEGA_MODE)
					Serial.println(frskys);
				#endif
				dataFile << frskys.c_str() << "\n";
			}

			#endif
			dataFile.close();
		}
	}

	return 0;
}

#ifdef USE_SD
void OQL::WriteToLog(String &text)	{
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
		WriteToLog("LOG:STARTED\n");
	}
}
#endif
