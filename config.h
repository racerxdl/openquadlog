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

#ifndef OQLDEFS
#define OQLDEFS

#define MEGA_MODE				//	Test the sketch on Arduino Mega

#define USE_SD				//	Use data logging to SD
//#define LOG_NAZA				//	Log Naza Messages to SD
//#define LOG_FRSKY				//	Log FrSky Messages to SD
#define READ_FRSKY				//	Read FrSky Messages
#define WRITE_FRSKY				//	Write data to FrSky
#define READ_NAZA				//	Read NAZA Messages

//#define READ_INPUTS

#define DEBUG_MEGA			//	Enables Serial0 Debugging. Only on MEGA640 and up
//#define DEBUG_NAZA
//#define DEBUG_FRSKY

/* FrSky Defines */

//#define DEBUG_MEGA_FRSKY		//	Enables FrSky Framing Time debug

//#define FRSKY_ACC				//	Eanbles sending accelerometer values
//#define FRSKY_CELLS			//	Enables sending cells voltage
//#define FRSKY_RPM				//	Enables sending RPM data
//#define FRSKY_FUEL			//	Enables sending fuel
//#define FRSKY_FRAME3			//	Enables FrSky Frame 3 (That sends only datetime)


template<class T> inline Print &operator <<(Print &obj, T arg) { obj.write(arg); return obj; }


#endif
