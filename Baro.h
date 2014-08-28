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

#include "config.h"

#ifndef OQL_BARO
#define OQL_BARO


#define BARO_ADDR 0x77

#define BARO_AC1				0xAA
#define BARO_AC2				0xAC
#define BARO_AC3				0xAE
#define BARO_AC4				0xB0
#define BARO_AC5				0xB2
#define BARO_AC6				0xB4
#define BARO_B1 				0xB6
#define BARO_B2 				0xB8
#define BARO_MB 				0xBA
#define BARO_MC 				0xBC
#define BARO_MD 				0xBE

#define BARO_CONTROL			0xF4
#define BARO_TEMPDATA			0xF6
#define BARO_PRESSUREDATA		0xF6
#define BARO_READTEMPCMD		0x2E
#define BARO_READPRESSURECMD	0x34



class	Barometer	{
private:
		int16_t ac1, ac2, ac3, b1, b2, mb, mc, md;
		uint16_t ac4, ac5, ac6;
		int32_t UT, UP, B5;

		uint8_t ReadByte(uint8_t addr) {
			uint8_t ret;

			Wire.beginTransmission(BARO_ADDR);
			Wire.write(addr);
			Wire.endTransmission();

			Wire.beginTransmission(BARO_ADDR);
			Wire.requestFrom(BARO_ADDR, 1);
			ret = Wire.read();
			Wire.endTransmission();
			return ret;
		}

		uint16_t ReadShort(uint8_t addr) {
			uint16_t ret;

			Wire.beginTransmission(BARO_ADDR);
			Wire.write(addr);
			Wire.endTransmission();

			Wire.beginTransmission(BARO_ADDR);
			Wire.requestFrom(BARO_ADDR, 2);
			ret = Wire.read();
			ret <<= 8;
			ret |= Wire.read();
			Wire.endTransmission();

			return ret;
		}

		void SendByte(uint8_t addr, uint8_t data) {
		  Wire.beginTransmission(BARO_ADDR);
		  Wire.write(addr);
		  Wire.write(data);
		  Wire.endTransmission();
		}


		inline int32_t CalcB5(int32_t UT) {
		  int32_t X1 = (UT - (int32_t)ac6) * ((int32_t)ac5) >> 15;
		  int32_t X2 = ((int32_t)mc << 11) / (X1+(int32_t)md);
		  return X1 + X2;
		}

public:

	float Temperature	=	0;
	float Altitude		=	0;
	float BaseAltitude 	= 	0;

	int32_t Pressure	=	0;
	bool ok			=	false;
	bool Start();
	void UpdateTemp();
	void UpdatePressure();
	inline void SetBaseAltitude()	{
		BaseAltitude = Altitude;
	}

};

#endif
