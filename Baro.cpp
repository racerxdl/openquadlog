/**
  ___                    ___                  _ _
 / _ \ _ __   ___ _ __  / _ \ _   _  __ _  __| | |    ___   __ _
| | | | '_ \ / _ \ '_ \| | | | | | |/ _` |/ _` | |   / _ \ / _` |
| |_| | |_) |  __/ | | | |_| | |_| | (_| | (_| | |__| (_) | (_| |
 \___/| .__/ \___|_| |_|\__\_\\__,_|\__,_|\__,_|_____\___/ \__, |
      |_|                                                  |___/

      By: Lucas Teske
      Based on Adafruit BMP085 Library: https://github.com/adafruit/Adafruit-BMP085-Library
**/

#include "Baro.h"
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <Arduino.h>

bool Barometer::Start() {
	Wire.begin();

	if (ReadByte(0xD0) != 0x55) return false;

	ok  = true;

	ac1 = ReadShort(BARO_AC1);
	ac2 = ReadShort(BARO_AC2);
	ac3 = ReadShort(BARO_AC3);
	ac4 = ReadShort(BARO_AC4);
	ac5 = ReadShort(BARO_AC5);
	ac6 = ReadShort(BARO_AC6);

	b1 = ReadShort(BARO_B1);
	b2 = ReadShort(BARO_B2);

	mb = ReadShort(BARO_MB);
	mc = ReadShort(BARO_MC);
	md = ReadShort(BARO_MD);

	return true;
}

void Barometer::UpdateTemp()	{
	if(ok)	{
		SendByte(BARO_CONTROL, BARO_READTEMPCMD);
		delay(5);
		UT = ReadShort(BARO_TEMPDATA);
		B5 = CalcB5(UT);
		Temperature = (B5+8) >> 4;
		Temperature /= 10;
	}
}

void Barometer::UpdatePressure()	{
	int32_t B3, B6, X1, X2, X3;
	uint32_t B4, B7;
	if(ok)	{

		SendByte(BARO_CONTROL, BARO_READPRESSURECMD);
		delay(5);
		UP = ReadShort(BARO_PRESSUREDATA);

		UP <<= 8;
		UP |= ReadByte(BARO_PRESSUREDATA+2);
		UP >>= (8);

		B6 = B5 - 4000;
		X1 = ((int32_t)b2 * ( (B6 * B6)>>12 )) >> 11;
		X2 = ((int32_t)ac2 * B6) >> 11;
		X3 = X1 + X2;
		B3 = ((((int32_t)ac1*4 + X3)) + 2) / 4;

		X1 = ((int32_t)ac3 * B6) >> 13;
		X2 = ((int32_t)b1 * ((B6 * B6) >> 12)) >> 16;
		X3 = ((X1 + X2) + 2) >> 2;
		B4 = ((uint32_t)ac4 * (uint32_t)(X3 + 32768)) >> 15;
		B7 = ((uint32_t)UP - B3) * (uint32_t)( 50000UL );

		if (B7 < 0x80000000)
			Pressure = (B7 * 2) / B4;
		else
			Pressure = (B7 / B4) * 2;

		X1 = (Pressure >> 8) * (Pressure >> 8);
		X1 = (X1 * 3038) >> 16;
		X2 = (-7357 * Pressure) >> 16;

		Pressure = Pressure + ((X1 + X2 + (int32_t)3791)>>4);
		Altitude = 44330 * (1.0 - pow(Pressure /101325.0,0.1903));
	}
}
