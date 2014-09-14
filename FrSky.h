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
#include <SPI.h>
#include <SD.h>
#include "DateTime.h"

#ifndef FRSKY_H
#define FRSKY_H


#ifdef FRSKY_CELL
	#define CELL_VMAX_C 159.f	//	The max value at variable
	#define CELL_VMAX_V 508.f	//	The max voltage that this max variable value is

	#define CELL_V2C(x) ((uint8_t)(( CELL_VMAX_C * x ) / CELL_VMAX_V))
	#define CELL_C2V(x) ((uint16_t)(( CELL_VMAX_V * x ) / CELL_VMAX_C))

#endif

class FrSky	{
private:

	/**
	 * Current Buffer Size (filled bytes)
	 */
	uint8_t buffsize		=	0;

	/**
	 * Current HUB Buffer Size (filled bytes)
	 */
	uint8_t hubsize			=	0;

	/**
	 * Buffer for receiving FrSky Messages
	 */
	uint8_t inbuffer[12];

	/**
	 * Buffer for receiving FrSky HUB Messages
	 */
	uint8_t hubbuffer[4];

	/**
	 * Input Buffer Position
	 */
	uint8_t inbuffpos		=	0;

	/**
	 * If we need to do byte stuffing on next byte
	 */
	uint8_t xornext			=	0;

#ifdef LOG_BINARY
	/**
	 * The OQL Binary Protocol IDs
	 */
	enum BID	{
		ERROR	=	0x00,
		GPSDATA	=	0x01,
		FRSKY	=	0x02,
		PSTART	=	0xAF
	};
#endif

	/**
	 * The Message IDs from FrSky
	 */
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

	/**
	 * FrSky Message Control Values
	 */
	enum FrSkyCtrl	{
		HEADER		=	0x5E,
		TAIL		=	0x5E,
		ESCAPE		=	0x5D,
		PACKET_HEAD	=	0x7E,
		DECIMAL		=	0x08
	};

public:

	/**
	 * FrSky Receiver Signal
	 */
	uint8_t RSSI			=	0;		//	Receiver Signal

	/**
	 * FrSky A1 Analog Port Value
	 */
	uint8_t A1				=	0;		//	Analog Input 1 Value

	/**
	 * FrSky A2 Analog Port Value
	 */
	uint8_t A2				=	0;		//	Analog Input 2 Value

	/**
	 * GPS Altitude in mm
	 */
	int32_t gps_altitude	= 	0;		//	The GPS Altitude

	/**
	 * GPS Latitude
	 *
	 * Multiplied by 1e7 (10.000.000)
	 */
	int32_t latitude		= 	0;		//	The GPS Latitude

	/**
	 * GPS Longitude
	 *
	 * Multiplied by 1e7 (10.000.000)
	 */
	int32_t longitude		=   0;		//	The GPS Longitude

	/**
	 * EAST/WEST
	 *
	 * Direction of longitude
	 */

	uint8_t eastwest		=	'E';

	/**
	 * NORTH/SOUTH
	 *
	 * Direction of latitude
	 */

	uint8_t northsouth		=	'N';

	/**
	 * Temperature 1
	 */
	int8_t temperature1		= 	0;		//	Temperature from barometer

	/**
	 * Temperature 2
	 * We actually use this for GPS Fix / Num Sats
	 * We have two digits for T2: XY
	 * X => Fix Type (0 = NoFix, 2 = 2D Fix, 3 = 3D Fix)
	 * Y => Num Sats
	 *
	 * It MAY show 4 in Fix type, this happens because overflow of Y (like, 41 means 3D Fix with 11 Sats)
	 */
	int8_t temperature2		= 	0;		//	Temperature 2

	/**
	 * Motor RPM
	 *
	 * This is multiplied by number of blades
	 */
	uint16_t rpm			=	0;		//	Motor RPM * Number of blades

#ifdef FRSKY_FUEL
	/**
	 * Fuel Level
	 *
	 * Scale in percent (0% to 100%)
	 */
	uint16_t fuellevel		=	0;		//	Fuel Level (in percent)
#endif

	/**
	 * Variometer Altitude
	 *
	 * Scale in cm multiplied by 10
	 */
	uint16_t altitude		=	0;		//	Variometer Altitude (or Barometer)

	/**
	 * GPS Ground Speed in knots
	 */
	uint32_t gps_speed		=	0;		//	GPS Ground Speed (knots)

#ifdef FRSKY_CELLS
    /**
     * 6 cells voltages. Multiplied by 100
     */
	uint16_t cell[6]	    =	{0,0,0,0,0,0};
#endif
	/**
	 * GPS Course
	 *
	 * Scale in Degrees
	 */
	float course			=	0;		//	GPS Course in Degrees

	/**
	 * GPS Time
	 */
	DateTime time;						//	GPS Time

#ifdef READ_FRSKY_VI
	/**
	 * Battery Voltage
	 *
	 * Multiplied by 10
	 */
	uint16_t voltage		=	0;		// 	Battery Voltage * 10

	/**
	 * Current Draw
	 *
	 * Multiplied by 10
	 */
	uint16_t current		=	0;
#endif
#ifdef FRSKY_ACC
	/**
	 * Acelerometer
	 * An 16 bit 3 component array corresponding X,Y,Z raw values from accelerometer.
	 */
	uint16_t acc[3]			=	{0,0,0};//	Acelerometer Values
#endif

	/** Alarms **/
#ifdef READ_FRSKY_ALARMS
	uint8_t A1T_1		=	0;			//	Alarm 1 - Analog Input 1 Threshold
	uint8_t A1G_1		=	0;			//	Alarm 1 - Analog Input 1 Greater Than(1) or Lesser Than (2)
	uint8_t A1L_1		=	0;			//	Alarm 1 - Analog Input 1 Alarm Level. (0) Disable, (1) Yellow, (2) Orange, (3) Red
	uint8_t A1T_2		=	0;			//	Alarm 2 - Analog Input 1 Threshold
	uint8_t A1G_2		=	0;			//	Alarm 2 - Analog Input 1 Greater Than(1) or Lesser Than (2)
	uint8_t A1L_2		=	0;			//	Alarm 2 - Analog Input 1 Alarm Level. (0) Disable, (1) Yellow, (2) Orange, (3) Red

	uint8_t A2T_1		=	0;			//	Alarm 1 - Analog Input 2 Threshold
	uint8_t A2G_1		=	0;			//	Alarm 1 - Analog Input 2 Greater Than(1) or Lesser Than (2)
	uint8_t A2L_1		=	0;			//	Alarm 1 - Analog Input 2 Alarm Level. (0) Disable, (1) Yellow, (2) Orange, (3) Red
	uint8_t A2T_2		=	0;			//	Alarm 2 - Analog Input 2 Threshold
	uint8_t A2G_2		=	0;			//	Alarm 2 - Analog Input 2 Greater Than(1) or Lesser Than (2)
	uint8_t A2L_2		=	0;			//	Alarm 2 - Analog Input 2 Alarm Level. (0) Disable, (1) Yellow, (2) Orange, (3) Red
#endif
	FrSky()	{
		for(int i=0;i<12;i++)
			inbuffer[i] = 0x00;
	}

	/**
	 * Writes FrSky GPS String
	 */
	void WriteFrskyGPSString(FastSerial &);
	void WriteFrskyGPSString(SoftwareSerial &);
	void WriteFrskyGPSString(const String &filename);


	/**
	 * Writes FrSky String
	 */
	void WriteFrskyString(FastSerial &);
	void WriteFrskyString(SoftwareSerial &);
	void WriteFrskyString(const String &filename);

	/**
	 * Checks if any frame needs to be sent
	 */
	void CheckData(SoftwareSerial &);

	/**
	 * Process an HUB Message
	 */

	void ProcessHUB(uint8_t);

	/**
	 * Process Hub Data
	 */

	void ProcessHUBData();

	/**
	 * Clears internal buffer
	 */
	void ClearBuffer();

	/**
	 * Returns the LSB from the current short
	 */
	static uint8_t lsb(uint16_t value)	{  return ((uint8_t) ((value) & 0xff));	}

	/**
	 * Returns the MSB from current short
	 */
	static uint8_t msb(uint16_t value)	{  return ((uint8_t) ((value) >> 8));	}

	/**
	 * Returns an uint16_t given MSB and LSB values
	 */
	static uint16_t To16(uint8_t msb, uint8_t lsb)	{
		return (((uint16_t)msb) << 8) + lsb;
	}
};

#endif
