/**
  ___                    ___                  _ _
 / _ \ _ __   ___ _ __  / _ \ _   _  __ _  __| | |    ___   __ _
| | | | '_ \ / _ \ '_ \| | | | | | |/ _` |/ _` | |   / _ \ / _` |
| |_| | |_) |  __/ | | | |_| | |_| | (_| | (_| | |__| (_) | (_| |
 \___/| .__/ \___|_| |_|\__\_\\__,_|\__,_|\__,_|_____\___/ \__, |
      |_|                                                  |___/

      By: Lucas Teske
**/

#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <Arduino.h>

#include "DateTime.h"

#ifndef NAZAGPS_H
#define NAZAGPS_H

#define CHKSUM_ERROR_PIN 13
#define PACKET_OK_PIN 14


class	NazaGPS	{

private:
	static inline uint32_t UInt32Val(uint8_t *data)	{	return *((uint32_t *) (data));	};
	static inline uint16_t UInt16Val(uint8_t *data)	{	return *((uint16_t *) (data));	};
	static inline int32_t  Int32Val (uint8_t *data)	{	return *((int32_t *)  (data));	};
	static inline int16_t  Int16Val (uint8_t *data)	{	return *((int16_t *)  (data));	};

	uint8_t buffer[64];
	uint8_t buffpos = 0;
	uint8_t payloadsize = 0;
	uint8_t CheckSum[2];

	inline void CalcChecksum() {
		uint8_t CK_A = 0, CK_B = 0;
		uint8_t *payload = &buffer[4];	//	The 5th byte is the start of payload
		for (int i = 0; i < payloadsize ;i++) {
			CK_A = CK_A + *payload;
			CK_B = CK_B + CK_A;
			payload++;
		}
		CheckSum[0] = CK_A;
		CheckSum[1] = CK_B;
	}
	inline uint8_t CompareChecksum(uint8_t *paycheck)	{
		uint16_t payloadcheck = *((uint16_t *)paycheck);
		uint16_t checksum = *((uint16_t *) &CheckSum);
		return payloadcheck == checksum;
	}

public:
	enum FixType	{
		NO_GPS  = 0,
		NO_FIX	= 1,
		LOCK_2D = 2,
		LOCK_3D = 3
	};

	enum MessageType	{
		GPS		=	0x10,
		MAG		=	0x20,
		FIRM	=	0x30
	};

	NazaGPS() {
		hardware_version = new uint8_t[12];
		software_version = new uint8_t[12];
	};
	void DecodeMessage(uint8_t *, uint8_t, uint8_t);
	void CheckData();
	uint8_t GenMagMask(uint8_t);

	DateTime time;

	uint8_t numSat 						= 	0;
	int32_t latitude 					= 	0;	//	* 10^7
	int32_t longitude					=	0;	//	* 10^7
	int32_t altitude					=	0;	//	mm

	uint32_t horizontal_accuracy 		= 	0;	//	mm
	uint32_t vertical_accuracy			=	0;	//	mm

	int32_t velN						=	0;	//	North Velocity in cm/s
	int32_t velE						=	0;	//	East Velocity in cm/s
	int32_t velD						=	0;	//	Down Velocity in cm/s

	uint32_t ground_speed				=	0;	//	cm/s
	uint32_t flight_speed				=	0;	//	cm/s

	//	 Dilution of precision
	uint16_t position_dop				=	0;
	uint16_t vertical_dop				=	0;
	uint16_t north_dop					=	0;
	uint16_t east_dop					=	0;


	FixType fix							= 	NO_FIX;
	uint8_t FixStatus						= 	0;

	uint16_t MagX						=	0;
	uint16_t MagY						=	0;
	uint16_t MagZ						=	0;
	float MagHead						=	0;
	float GPSHead						=	0;
	uint8_t *hardware_version;
	uint8_t *software_version;

};

#endif
