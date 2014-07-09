/**
  ___                    ___                  _ _
 / _ \ _ __   ___ _ __  / _ \ _   _  __ _  __| | |    ___   __ _
| | | | '_ \ / _ \ '_ \| | | | | | |/ _` |/ _` | |   / _ \ / _` |
| |_| | |_) |  __/ | | | |_| | |_| | (_| | (_| | |__| (_) | (_| |
 \___/| .__/ \___|_| |_|\__\_\\__,_|\__,_|\__,_|_____\___/ \__, |
      |_|                                                  |___/

      By: Lucas Teske
**/

#include <SoftwareSerial.h>
#include "config.h"
#include "NazaGPS.h"
#include "DateTime.h"

#ifndef FRSKY_H
#define FRSKY_H

#define FRAME1_TIME 200
#define FRAME2_TIME 1000
#define FRAME3_TIME 5000

#define CELL_VMAX_C 159.f
#define CELL_VMAX_V 508.f

#define CELL_V2C(x) ((uint8_t)(( CELL_VMAX_C * x ) / CELL_VMAX_V))

class FrSky	{
private:

	uint8_t *buffer			=	new uint8_t[128];

	uint8_t buffsize		=	0;

	uint32_t lastframe1		=	0;
	uint32_t lastframe2		=	0;
	uint32_t lastframe3		=	0;

	uint8_t inbuffer[12];
	uint8_t inbuffpos		=	0;
	uint8_t xornext			=	0;

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

	uint8_t RSSI	=	0;		//	Receiver Signal
	uint8_t A1		=	0;		//	Analog Input 1 Value
	uint8_t A2		=	0;		//	Analog Input 2 Value

	int32_t gps_altitude	= 	123456;
	int32_t latitude		= 	1000;
	int32_t longitude		=   2000;

	int8_t temperature1		= 	0;
	int8_t temperature2		= 	0;

	uint16_t rpm			=	3200;
	uint16_t fuellevel		=	50;

	uint16_t altitude		=	654321;
	uint32_t gps_speed		=	2500;

    /**
      * Cell Voltage * 100
      */
	uint16_t cell[6]	    =	{422,320,0,210,440,500};     

	float course			=	35;
	DateTime time;

	uint16_t voltage		=	126;	// * 10
	uint16_t current		=	0; 	    //	* 10

	uint16_t acc[3]			=	{0,0,0};

	/** Alarms **/
	uint8_t A1T_1		=	0;	//	Alarm 1 - Analog Input 1 Threshold
	uint8_t A1G_1		=	0;	//	Alarm 1 - Analog Input 1 Greater Than(1) or Lesser Than (2)
	uint8_t A1L_1		=	0;	//	Alarm 1 - Analog Input 1 Alarm Level. (0) Disable, (1) Yellow, (2) Orange, (3) Red
	uint8_t A1T_2		=	0;	//	Alarm 2 - Analog Input 1 Threshold
	uint8_t A1G_2		=	0;	//	Alarm 2 - Analog Input 1 Greater Than(1) or Lesser Than (2)
	uint8_t A1L_2		=	0;	//	Alarm 2 - Analog Input 1 Alarm Level. (0) Disable, (1) Yellow, (2) Orange, (3) Red

	uint8_t A2T_1		=	0;	//	Alarm 1 - Analog Input 2 Threshold
	uint8_t A2G_1		=	0;	//	Alarm 1 - Analog Input 2 Greater Than(1) or Lesser Than (2)
	uint8_t A2L_1		=	0;	//	Alarm 1 - Analog Input 2 Alarm Level. (0) Disable, (1) Yellow, (2) Orange, (3) Red
	uint8_t A2T_2		=	0;	//	Alarm 2 - Analog Input 2 Threshold
	uint8_t A2G_2		=	0;	//	Alarm 2 - Analog Input 2 Greater Than(1) or Lesser Than (2)
	uint8_t A2L_2		=	0;	//	Alarm 2 - Analog Input 2 Alarm Level. (0) Disable, (1) Yellow, (2) Orange, (3) Red



	FrSky()	{
		// Lets set the current time to frames
		lastframe1 = millis();
		lastframe2 = millis();
		lastframe3 = millis();
	}
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

	/**
	 * Adds an Data ID to the internal buffer
	 */
	void AddToBuffer(FrSkyID);

	/**
	 * Clears internal buffer
	 */
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

	/**
	 * Returns the LSB from the current short
	 */
	static uint8_t lsb(uint16_t value)	{  return ((uint8_t) ((value) & 0xff));	}

	/**
	 * Returns the MSB from current short
	 */
	static uint8_t msb(uint16_t value)	{  return ((uint8_t) ((value) >> 8));	}
};

#endif
