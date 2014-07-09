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
#include <stdint.h>
#include <math.h>
#include <stdio.h>
#include <Arduino.h>
#include "config.h"


#ifndef OQL_H
#define OQL_H

#define ADC_REF 5.f

#define DIV_CONST_S1 (1/10)							//	1k / 10k
#define DIV_CONST_S2 DIV_CONST_S1					//	Same as S1
#define ADC2VOLT(x)  (x * ADC_REF/1024.f)			//	Macro to convert ADC Values to Volts

#define S1TOVAL(x) 	 (ADC2VOLT(x) * DIV_CONST_S1)	//	Macro to convert S1 ADC Value to real value
#define S2TOVAL(x) 	 (ADC2VOLT(x) * DIV_CONST_S2)	//	Macro to convert S1 ADC Value to real value

class OQL	{
private:
	void PoolAnalog();
	void PoolDigital();

	static const int V1_PIN = A0;
	static const int V2_PIN = A1;
	static const int S1_PIN = A2;
	static const int S2_PIN = A3;
	static const int SD_SEL = 4;	//	SD Chip Select

	String filename;
public:

	OQL()	{
		pinMode(S1_PIN, INPUT);
		pinMode(S2_PIN, INPUT);
#ifdef USE_SD
		logen = SD.begin(SD_SEL);
#endif
	}

	float v1		=	0;	//	Voltage Measure
	float v2		=	0;	//	Current Measure
	uint8_t s1		=	0;	//	Used as Green Led on NAZA
	uint8_t s2		=	0;	//	Used as Red Led on NAZA

#ifdef USE_SD
	uint8_t logen	= 	0;	//	If logging to SD is enabled

	void WriteToLog(String &);
	void WriteToLog(const char *);
	void SetFilename();
#endif

	uint8_t CheckData();
};

#endif
