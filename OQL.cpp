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

uint8_t OQL::CheckData()	{
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
	return 0;
}

#ifdef USE_SD
void OQL::WriteToLog(String &text)	{
	if(logen)	{
		File dataFile = SD.open(filename.c_str(), FILE_WRITE);
		if(dataFile)	{
			dataFile.println(text);
			dataFile.close();
		}
	}
}

void OQL::WriteToLog(const char *text)	{
	if(logen)	{
		File dataFile = SD.open(filename.c_str(), FILE_WRITE);
		if(dataFile)	{
			dataFile.println(text);
			dataFile.close();
		}
	}
}
void OQL::SetFilename()	{
	int i=0;
	String file;

	if(logen)	{
		while(true)	{
			file = String("OQL" + String(i, DEC) + ".oqd");
			if(!SD.exists((char *)file.c_str()))
				break;
			i++;
		}
		filename =  file;
		WriteToLog("LOG:STARTED");
	}
}
#endif
