/**
  ___                    ___                  _ _
 / _ \ _ __   ___ _ __  / _ \ _   _  __ _  __| | |    ___   __ _
| | | | '_ \ / _ \ '_ \| | | | | | |/ _` |/ _` | |   / _ \ / _` |
| |_| | |_) |  __/ | | | |_| | |_| | (_| | (_| | |__| (_) | (_| |
 \___/| .__/ \___|_| |_|\__\_\\__,_|\__,_|\__,_|_____\___/ \__, |
      |_|                                                  |___/

      By: Lucas Teske
**/

#include "DateTime.h"

DateTime::DateTime()	{
	seconds = 0;
	minutes = 0;
	hours = 0;
	day = 0;
	month = 0;
	year = 0;
}

DateTime::DateTime(uint32_t data)	{
	FromNazaString(data);
}

void DateTime::FromNazaInt(uint32_t data)	{
	seconds =   (data & 0x3F        )   >>  0;
	minutes =   (data & 0xFC0       )   >>  6;
	hours   =   (data & 0xF000      )   >>  12;
	day     =   (data & 0x1F0000    )   >>  16;
	month   =   (data & 0x1E00000   )   >>  21;
	year    =   (data & 0xFE000000  )   >>  25;
}
