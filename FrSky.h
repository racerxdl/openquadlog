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
#include "NazaGPS.h"
#include "Baro.h"
#include "DateTime.h"

#ifndef FRSKY_H
#define FRSKY_H

#define FRAME1_TIME 200			//	The FRAME1 Period in ms defaults to 200ms
#define FRAME2_TIME 1000		//	The FRAME2 Period in ms defaults to 1000ms
#define FRAME3_TIME 5000		//	The FRAME3 Period in ms defaults to 5000ms

#ifdef FRSKY_CELL
	#define CELL_VMAX_C 159.f	//	The max value at variable
	#define CELL_VMAX_V 508.f	//	The max voltage that this max variable value is

	#define CELL_V2C(x) ((uint8_t)(( CELL_VMAX_C * x ) / CELL_VMAX_V))
#endif

class FrSky	{
private:

	/**
	 * Buffer for sending FrSky messages
	 */
	uint8_t buffer[128];

	/**
	 * Current Buffer Size (filled bytes)
	 */
	uint8_t buffsize		=	0;

	/**
	 * Last Frame 1 Time (millis)
	 */
	uint32_t lastframe1		=	0;
	/**
	 * Last Frame 2 Time (millis)
	 */
	uint32_t lastframe2		=	0;
	/**
	 * Last Frame 3 Time (millis)
	 */
	uint32_t lastframe3		=	0;

	/**
	 * Buffer for receiving FrSky Messages
	 */
	uint8_t inbuffer[12];

	/**
	 * Input Buffer Position
	 */
	uint8_t inbuffpos		=	0;

	/**
	 * If we need to do byte stuffing on next byte
	 */
	uint8_t xornext			=	0;

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
		HEADER	=	0x5e,
		TAIL	=	0x5e,
		ESCAPE	=	0x5d,
		DECIMAL	=	0x8
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

	/**
	 * Fuel Level
	 *
	 * Scale in percent (0% to 100%)
	 */
	uint16_t fuellevel		=	0;		//	Fuel Level (in percent)

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

    /**
     * 6 cells voltages. Multiplied by 100
     */
	uint16_t cell[6]	    =	{0,0,0,0,0,0};

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

	#ifdef FRSKY_ACC
	/**
	 * Acelerometer
	 * An 16 bit 3 component array corresponding X,Y,Z raw values from accelerometer.
	 */
	uint16_t acc[3]			=	{0,0,0};//	Acelerometer Values
	#endif

	/** Alarms **/
	#ifdef READ_FRSKY
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
		// Lets set the current time to frames
		lastframe1 = millis();
		lastframe2 = millis()+50;
		lastframe3 = millis()+100;
		for(int i=0;i<128;i++)
			buffer[i] = 0x00;
	}

	/**
	 * Generates the FrSky String to logger
	 */

	String GenerateFrSkyString();

	/**
	 * Checks if any frame needs to be sent
	 */
	void CheckData(SoftwareSerial &);

	/**
	 * Writes the internal buffer to the following serial port.
	 */
	void WriteBuffer(SoftwareSerial&);

	/**
	 * Updates the internal data with Naza GPS object
	 */
	void UpdateDataWithNaza(NazaGPS &);

#ifdef READ_BARO
	/**
	 * Updates the internal data with Barometer
	 */

	void UpdateWithBarometer(Barometer &);
#endif

	/**
	 * Adds an Data ID to the internal buffer
	 */
	void AddToBuffer(FrSkyID);

	/**
	 * Clears internal buffer
	 */
	inline void ClearBuffer();

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

	/**
	 * Returns the LSB from the current short
	 */
	static inline uint8_t lsb(uint16_t value)	{  return ((uint8_t) ((value) & 0xff));	}

	/**
	 * Returns the MSB from current short
	 */
	static inline uint8_t msb(uint16_t value)	{  return ((uint8_t) ((value) >> 8));	}
};

#endif
