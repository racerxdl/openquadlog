/**
  ___                    ___                  _ _
 / _ \ _ __   ___ _ __  / _ \ _   _  __ _  __| | |    ___   __ _
| | | | '_ \ / _ \ '_ \| | | | | | |/ _` |/ _` | |   / _ \ / _` |
| |_| | |_) |  __/ | | | |_| | |_| | (_| | (_| | |__| (_) | (_| |
 \___/| .__/ \___|_| |_|\__\_\\__,_|\__,_|\__,_|_____\___/ \__, |
      |_|                                                  |___/

      By: Lucas Teske
**/

#ifndef DATETIME_H
#define DATETIME_H


class DateTime	{

public:
	DateTime();
	DateTime(uint32_t);
	void FromNazaInt(uint32_t);

	short seconds;
	short minute;
	short hour;
	short day;
	short month;
	short year;
};
#endif
