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

#ifndef DATETIME_H
#define DATETIME_H


class DateTime	{

public:
	DateTime();
	DateTime(uint32_t);
	void FromNazaInt(uint32_t);
	inline void FromDateTime(DateTime &date)	{
		seconds = 	date.seconds;
		minute 	= 	date.minute;
		hour 	= 	date.hour;
		day 	= 	date.day;
		month	= 	date.month;
		year 	= 	date.year;
	}

	uint8_t seconds;
	uint8_t minute;
	uint8_t hour;
	uint8_t day;
	uint8_t month;
	uint8_t year;
};
#endif
