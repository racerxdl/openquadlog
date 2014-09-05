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

#define OQL_SOFTVER "SV"
#define OQL_HARDVER "HV"
#define OQL_GPS     "GPS"
#define OQL_FRSKY   "FRSKY"
#define OQL_VERSION	"1.00"

class OQL	{
private:
	void PoolAnalog();
	void PoolDigital();

	static const int SD_SEL 	= 22;	//	SD Chip Select
	static const String OQL_VER	= new String(OQL_VERSION);
	unsigned long lastFrSkyTime = 0;	//	Last time FrSky has been written
	unsigned long lastGPSTime 	= 0;	//	Last time GPS has been written

	String filename;
	Sd2Card card;
	SdVolume volume;
	SdFile root;
public:

	OQL()	{
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


#ifdef USE_SD
	uint8_t logen	= 	0;	//	If logging to SD is enabled

	void WriteToLog(const String &);
	void WriteToLog(const char *);
	void SetFilename();
#endif
	uint8_t CheckData(FrSky &);
};

#endif
