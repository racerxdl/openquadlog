/**
  ___                    ___                  _ _
 / _ \ _ __   ___ _ __  / _ \ _   _  __ _  __| | |    ___   __ _
| | | | '_ \ / _ \ '_ \| | | | | | |/ _` |/ _` | |   / _ \ / _` |
| |_| | |_) |  __/ | | | |_| | |_| | (_| | (_| | |__| (_) | (_| |
 \___/| .__/ \___|_| |_|\__\_\\__,_|\__,_|\__,_|_____\___/ \__, |
      |_|                                                  |___/

      By: Lucas Teske
**/

#include "config.h"
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <SPI.h>
#include <SD.h>
#include <Arduino.h>
#include "config.h"
#include "NazaGPS.h"
#include "FrSky.h"


#ifndef OQL_H
#define OQL_H

#define ADC_REF 5.f

#define DIV_CONST_S1 (1/10)							//	1k / 10k
#define DIV_CONST_S2 DIV_CONST_S1					//	Same as S1
#define ADC2VOLT(x)  (x * ADC_REF/1024.f)			//	Macro to convert ADC Values to Volts

#define S1TOVAL(x) 	 (ADC2VOLT(x) * DIV_CONST_S1)	//	Macro to convert S1 ADC Value to real value
#define S2TOVAL(x) 	 (ADC2VOLT(x) * DIV_CONST_S2)	//	Macro to convert S1 ADC Value to real value

#define OQL_SOFTVER "SV"
#define OQL_HARDVER "HV"
#define OQL_GPS     "GPS"
#define OQL_FRSKY   "FRSKY"

class OQL	{
private:
	void PoolAnalog();
	void PoolDigital();

	static const int V1_PIN = A0;
	static const int V2_PIN = A1;
	static const int S1_PIN = A2;
	static const int S2_PIN = A3;
	static const int SD_SEL = 22;	//	SD Chip Select

	String filename;
	Sd2Card card;
	SdVolume volume;
	SdFile root;
public:

	OQL()	{
		pinMode(S1_PIN, INPUT);
		pinMode(S2_PIN, INPUT);
#ifdef USE_SD
		logen = SD.begin(SD_SEL);
		card.init(SPI_HALF_SPEED, SD_SEL);
		#ifdef DEBUG_MEGA
		Serial.print("Card type: ");
		switch (card.type()) {
			case SD_CARD_TYPE_SD1:
			  Serial.println("SD1");
			  break;
			case SD_CARD_TYPE_SD2:
			  Serial.println("SD2");
			  break;
			case SD_CARD_TYPE_SDHC:
			  Serial.println("SDHC");
			  break;
			default:
			  Serial.println("Unknown");
			  break;
		}
		#endif
		if (!volume.init(card)) {
			#ifdef DEBUG_MEGA
			Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
			#endif
			logen = 0;
		}
		#ifdef DEBUG_MEGA
		uint32_t volumesize;
		Serial.print("Volume type is FAT");
		Serial.println(volume.fatType(), DEC);
		Serial.println();
		volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
		volumesize *= volume.clusterCount();       // we'll have a lot of clusters
		volumesize *= 512;                         // SD card blocks are always 512 bytes
		volumesize /= 1024;
		Serial.print("Volume size (Mbytes): ");
		volumesize /= 1024;
		Serial.println(volumesize);
		#endif
		SetFilename();
#endif
	}

	float v1		=	0;	//	Voltage Measure
	float v2		=	0;	//	Current Measure
	uint8_t s1		=	0;	//	Used as Green Led on NAZA
	uint8_t s2		=	0;	//	Used as Red Led on NAZA

#ifdef USE_SD
	uint8_t logen	= 	0;	//	If logging to SD is enabled

	void WriteToLog(String &);
	void WriteToLog(const char *);
	void SetFilename();
#endif
#ifdef READ_NAZA
	uint8_t versionwrote = 0;

	int32_t lastlat = 0;
	int32_t lastlon = 0;
	int32_t lastalt = 0;
	uint8_t lastfix = 0;
	uint8_t lastsats= 0;

	inline uint8_t GPSChanged(NazaGPS &naza)	{
		return  (naza.numSat != lastsats) 	|| (naza.fix != lastfix) 		||
				(naza.longitude != lastlon) || (naza.latitude != lastlat) 	||
				(naza.altitude != lastalt);
	}
#endif
#ifdef READ_FRSKY
	uint8_t lastRSSI			=	0;
	uint8_t lastA1				=	0;
	uint8_t lastA2				=	0;

	inline uint8_t FrSkyChanged(FrSky &frsky)	{
		return	(frsky.RSSI != lastRSSI)	||	(frsky.A1 != lastA1)	||	(frsky.A2 != lastA2);
	}
#endif
	uint8_t CheckData(NazaGPS &, FrSky &);
};

#endif
