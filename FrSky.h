/**
  ___                    ___                  _ _
 / _ \ _ __   ___ _ __  / _ \ _   _  __ _  __| | |    ___   __ _
| | | | '_ \ / _ \ '_ \| | | | | | |/ _` |/ _` | |   / _ \ / _` |
| |_| | |_) |  __/ | | | |_| | |_| | (_| | (_| | |__| (_) | (_| |
 \___/| .__/ \___|_| |_|\__\_\\__,_|\__,_|\__,_|_____\___/ \__, |
      |_|                                                  |___/

      By: Lucas Teske
**/


#ifndef FRSKY_H
#define FRSKY_H

#include <SoftwareSerial.h>
#include "nazagps.h"
#include "DateTime.h"

class FrSky	{
private:
	int32_t gps_altitude	= 	0;
	int32_t latitude		= 	0;
	int32_t longitude		= 	0;

	int8_t temperature1		= 	0;
	int8_t temperature2		= 	0;

	uint16_t rpm			=	0;
	uint16_t fuellevel		=	0;

	uint16_t altitude		=	0;
	uint32_t gps_speed		=	0;

	uint16_t cell[6]		=	{0,0,0,0,0,0};

	float course			=	0;
	DateTime time;

	uint16_t voltage		=	0;	//	0.5V Steps. 0 to 48V
	uint16_t current		=	0;  //  0.1A Steps. 0 to 100V

	uint16_t acc[3]			=	{0,0,0};

	char *buffer			=	new char[64];
	uint8_t buffsize		=	0;

	enum FrSkyID	{
		GPSALT         =	0x1,
		TEMP1          =	0x2,
		RPM            =	0x3,
		FUEL           =	0x4,
		TEMP2          =	0x5,
		INDVOLT        =	0x6,
		ALTITUDE       =	0x10,
		GPSSPEED       =	0x11,
		LONGITUDE      =	0x12,
		LATITUDE       =	0x13,
		COURSE         =	0x14,
		DATE           =	0x15,
		YEAR           =	0x16,
		TIME           =	0x17,
		SECOND         =	0x18,
		ALTIDEC        =	0x21,
		EASTWEST       =	0x22,
		NORTHSOUTH     =	0x23,
		ACCX           =	0x24,
		ACCY           =	0x25,
		ACCZ           =	0x26,
		CURRENT        =	0x28,
		VOLTAGE        =	0x3A,
		VOLTAGEDEC     =	0x3B
	};

	enum FrSkyCtrl	{
		HEADER	=	0x5e,
		TAIL	=	0x5e,
		ESCAPE	=	0x5d,
		DECIMAL	=	0x8
	};

public:
	void WriteBuffer(SoftwareSerial&);
	void UpdateDataWithNaza(NazaGPS &);
	void AddToBuffer(FrSkyID);
	void ClearBuffer();

	/**
	 * Sends Frame1 Data. This should be done with an 200ms interval
	 *
	 * It is composed by:
	 * <p>
	 * 	- Accelerometer Values (x,y,z) 	[ Divided by 1000 ]
	 * 	- Altitude (From variometer)	[ - 10cm ]
	 * 	- Temperature 1
	 * 	- Temperature 2
	 * 	- Voltage
	 * 	- Current						[ Divided by 10 ]
	 * 	- Motor RPM						[ Divided by 60, a.k.a. RPS ]
	 */
	void SendFrame1(SoftwareSerial &);	//	200ms

	/**
	 * Sends Frame2 Data. This should be done with an 1s interval
	 *
	 * It is composed by:
	 * <p>
	 * 	- Course
	 * 	- Latitude
	 * 	- Longitude
	 * 	- Speed
	 * 	- Altitude	( From GPS )
	 * 	- Fuel Level
	 */
	void SendFrame2(SoftwareSerial &);	//	1s

	/**
	 * Sends Frame3 Data. This should be done with an 5s interval
	 *
	 * It is composed by:
	 * <p>
	 * 	- Date
	 * 	- Time
	 */
	void SendFrame3(SoftwareSerial &);	//	5s

	static inline uint8_t lsb(uint16_t value)	{  return ((uint8_t) ((value) & 0xff));	}
	static inline uint8_t msb(uint16_t value)	{  return ((uint8_t) ((value) >> 8));	}
};

#endif
