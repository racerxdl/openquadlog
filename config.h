/**
  ___                    ___                  _ _
 / _ \ _ __   ___ _ __  / _ \ _   _  __ _  __| | |    ___   __ _
| | | | '_ \ / _ \ '_ \| | | | | | |/ _` |/ _` | |   / _ \ / _` |
| |_| | |_) |  __/ | | | |_| | |_| | (_| | (_| | |__| (_) | (_| |
 \___/| .__/ \___|_| |_|\__\_\\__,_|\__,_|\__,_|_____\___/ \__, |
      |_|                                                  |___/

      By: Lucas Teske
**/

#ifndef OQLDEFS
#define OQLDEFS

//#define USE_SD		//	Use data logging to SD
//#define LOG_NAZA		//	Log Naza Messages to SD
//#define LOG_FRSKY		//	Log FrSky Messages to SD
//#define READ_FRSKY		//	Read FrSky Messages
#define WRITE_FRSKY		//	Write data to FrSky
#define READ_NAZA		//	Read NAZA Messages

//#define READ_INPUTS

template<class T> inline Print &operator <<(Print &obj, T arg) { obj.print(arg); return obj; }


#endif
