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
#include "DateTime.h"

#ifndef NAZAGPS_H
#define NAZAGPS_H


class	NazaGPS	{

private:
	static inline uint32_t UInt32Val(char *data)	{	return *((uint32_t *) (data));	};
	static inline uint16_t UInt16Val(char *data)	{	return *((uint16_t *) (data));	};
	static inline int32_t  Int32Val (char *data)	{	return *((int32_t *)  (data));	};
	static inline int16_t  Int16Val (char *data)	{	return *((int16_t *)  (data));	};

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
		hardware_version = new char[12];
		software_version = new char[12];
	};
	void DecodeMessage(char *, char, char);
	char GenMagMask(char);

	DateTime time;

	char numSat 						= 	0;
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
	char FixStatus						= 	0;

	uint16_t MagX						=	0;
	uint16_t MagY						=	0;
	uint16_t MagZ						=	0;
	float MagHead						=	0;
	float GPSHead						=	0;
	char *hardware_version;
	char *software_version;

};

#endif
