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
#include <Arduino.h>
#include "DateTime.h"

#ifndef NAZAGPS_H
#define NAZAGPS_H

extern FastSerial Serial1;

class	NazaGPS	{

private:
	/**
	 * Gets the Unsigned 32 bit value from following pointer
	 */
	static inline uint32_t UInt32Val(uint8_t *data)	{	return *((uint32_t *) (data));	};

	/**
	 * Gets the Unsigned 16 bit value from following pointer
	 */
	static inline uint16_t UInt16Val(uint8_t *data)	{	return *((uint16_t *) (data));	};

	/**
	 * Gets the 32 bit value from following pointer
	 */
	static inline int32_t  Int32Val (uint8_t *data)	{	return *((int32_t *)  (data));	};


	/**
	 * Gets the 16 bit value from following pointer
	 */
	static inline int16_t  Int16Val (uint8_t *data)	{	return *((int16_t *)  (data));	};

	/**
	 * Temporary Buffer for receiving Naza Payloads
	 */
	uint8_t buffer[128];

	/**
	 * Current Temp Buffer Position
	 */
	uint8_t buffpos = 0;

	/**
	 * The current payload size
	 */
	uint8_t payloadsize = 0;

	/**
	 * Calculated Checksum for the payload
	 */
	uint8_t CheckSum[2];

	/**
	 * Inline Calculation of the Checksum
	 */
	inline void CalcChecksum() {
		CheckSum[0] = 0;
		CheckSum[1] = 0;
		for (int i = 2; i < payloadsize+4 ;i++) {
			CheckSum[0] += buffer[i];
			CheckSum[1] += CheckSum[0];
		}
	}

	/**
	 * Does an inline compartion of internal calculated checksum and the checksum on that pointer.
	 */
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
		hardware_version = new uint8_t[13];
		software_version = new uint8_t[13];
		for(int i=0;i<13;i++)	{
			hardware_version[i] = 0x00;
			software_version[i] = 0x00;
		}
	};

	/**
	 * Generates String to Log
	 */
	String GenerateGPSString();

	/**
	 * Decodes the Naza payload
	 */
	void DecodeMessage(uint8_t *, uint8_t, uint8_t);

	/**
	 * Clears the buffer
	 */
	void ClearBuffer();

	/**
	 * Checks if there is any data.
	 */
	uint8_t CheckData(FastSerial &);

	/**
	 * The GPS time
	 */
	DateTime time;

	/**
	 * GPS Number of connected Satellites
	 */
	uint8_t numSat 						= 	0;

	/**
	 * GPS Latitude, multiplied by 1e7
	 */
	int32_t latitude 					= 	0;	//	* 10^7

	/**
	 * GPS Latitude, multiplied by 1e7
	 */
	int32_t longitude					=	0;	//	* 10^7

	/**
	 * GPS Altitude in mm
	 */
	int32_t altitude					=	0;	//	mm

	/**
	 * The GPS Start altitude in mm, for calculating relative altitudes
	 */
	int32_t start_altitude				=	0;	//	mm

	/**
	 * The horizontal accuracy in mm
	 */
	uint32_t horizontal_accuracy 		= 	0;	//	mm

	/**
	 * The vertical accuracy in mm
	 */
	uint32_t vertical_accuracy			=	0;	//	mm

	/**
	 * The North Velocity in cm/s
	 */
	int32_t velN						=	0;	//	North Velocity in cm/s

	/**
	 * The East Velocity in cm/s
	 */
	int32_t velE						=	0;	//	East Velocity in cm/s

	/**
	 * The Down Velocity in cm/s
	 */
	int32_t velD						=	0;	//	Down Velocity in cm/s

	/**
	 * The Ground Speed (a.k.a. 2D Speed) in cm/s
	 */
	uint32_t ground_speed				=	0;	//	cm/s

	/**
	 * The Flight Speed (a.k.a. 3D Speed) in cm/s
	 */
	uint32_t flight_speed				=	0;	//	cm/s

	/**
	 * The Position Dilution of Precision
	 */
	uint16_t position_dop				=	0;

	/**
	 * The Vertical Dilution of Precision
	 */
	uint16_t vertical_dop				=	0;

	/**
	 * The North Dilution of Precision
	 */
	uint16_t north_dop					=	0;

	/**
	 * The East Dilution of Precision
	 */
	uint16_t east_dop					=	0;


	/**
	 * The Current Fix
	 */
	FixType fix							= 	NO_FIX;

	/**
	 * Last Fix. To calculating Relative Altitude
	 */
	FixType lastfix						=	NO_FIX;

	/**
	 * The Fix Status
	 */
	uint8_t FixStatus						= 	0;

	/**
	 * The Magnetometer X value
	 */
	uint16_t MagX						=	0;

	/**
	 * The Magnetometer Y value
	 */
	uint16_t MagY						=	0;


	/**
	 * The Magnetometer Z value
	 */
	uint16_t MagZ						=	0;

	/**
	 * The Magnetometer calculated heading (No tilt compensated) in degrees
	 */
	float MagHead						=	0;

	/**
	 * The GPS calculated heading
	 */
	float GPSHead						=	0;

	/**
	 * The Hardware Version String
	 */
	uint8_t *hardware_version;

	/**
	 * The Software Version String
	 */
	uint8_t *software_version;


	uint8_t recv_version;
};

#endif
