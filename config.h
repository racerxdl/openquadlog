/**
  ___                    ___                  _ _
 / _ \ _ __   ___ _ __  / _ \ _   _  __ _  __| | |    ___   __ _
| | | | '_ \ / _ \ '_ \| | | | | | |/ _` |/ _` | |   / _ \ / _` |
| |_| | |_) |  __/ | | | |_| | |_| | (_| | (_| | |__| (_) | (_| |
 \___/| .__/ \___|_| |_|\__\_\\__,_|\__,_|\__,_|_____\___/ \__, |
      |_|                                                  |___/

      By: Lucas Teske
**/


/**
 * Since OQL uses FastSerial as a replacement of Arduino Hardware Serial,
 * you should edit Arduino.h in {arduino folder}/hardware/cores/arduino/
 * and replace:
 *
 * 	#include "HardwareSerial.h"
 *
 * for
 *
 * #ifndef NOHARDWARESERIAL
 * #include "HardwareSerial.h"
 * #endif
 *
 * This will not change anything to your other sketches, but will enable us to
 * disable HardwareSerial just defining NOHARDWARESERIAL at the start of the sketch.
 *
 */
#define NOHARDWARESERIAL


#include <FastSerial.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#ifndef OQLDEFS
#define OQLDEFS

#define MEGA_MODE			//	Test the sketch on Arduino Mega

#define USE_SD				//	Use data logging to SD
#define LOG_FRSKY			//	Log FrSky Messages to SD

#define DEBUG_MEGA			//	Enables Serial0 Debugging. Only on MEGA640 and up
#define DEBUG_OQL			//	Enables Debug OQL at Serial0
/* FrSky Defines */

//#define FRSKY_ACC				//	Eanbles sending accelerometer values
//#define FRSKY_CELLS			//	Enables sending cells voltage
//#define FRSKY_RPM				//	Enables sending RPM data
//#define FRSKY_FUEL			//	Enables sending fuel
//#define FRSKY_FRAME3			//	Enables FrSky Frame 3 (That sends only datetime)
//#define READ_FRSKY_ALARMS		//	Enables reading FrSky Alarm Data.
//#define READ_FRSKY_DATE		//	Enables reading Date From FrSky
//#define READ_FRSKY_VI			//	Enables reading Voltage and Current from FrSky
//#define READ_FRSKY_COURSE		//	Enables reading FrSky Course

#define FRSKY_INTERVAL			5		//	50ms between checks
#define GPS_WRITE_INTERVAL		500		//	250ms between gps writes
#define FRSKY_WRITE_INTERVAL	1000	//	1s between frsky writes

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.write(arg); return obj; }


#endif
