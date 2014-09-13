/**
  ___                    ___                  _ _
 / _ \ _ __   ___ _ __  / _ \ _   _  __ _  __| | |    ___   __ _
| | | | '_ \ / _ \ '_ \| | | | | | |/ _` |/ _` | |   / _ \ / _` |
| |_| | |_) |  __/ | | | |_| | |_| | (_| | (_| | |__| (_) | (_| |
 \___/| .__/ \___|_| |_|\__\_\\__,_|\__,_|\__,_|_____\___/ \__, |
      |_|                                                  |___/

      By: Lucas Teske
**/

#include "FrSky.h"

void FrSky::WriteFrskyString(SoftwareSerial &ser)	{
	static long ltime = 0;
	char buff[80];
	long delta = millis() - ltime;
	ltime += delta;
	String tim = time.ToShortString();
	ser.print("FRSKY:");
	ser.print(tim);
	ser.print(":");
	ser.print(delta);
	ser.print(":");
	ser.print(RSSI);
	ser.print(":");
	ser.print(A1);
	ser.print(":");
	ser.println(A2);
}

void FrSky::WriteFrskyString(FastSerial &ser)	{
	static long ltime = 0;
	char buff[80];
	long delta = millis() - ltime;
	ltime += delta;
	String tim = time.ToShortString();
	ser.print("FRSKY:");
	ser.print(tim);
	ser.print(":");
	ser.print(delta);
	ser.print(":");
	ser.print(RSSI);
	ser.print(":");
	ser.print(A1);
	ser.print(":");
	ser.println(A2);
}

void FrSky::WriteFrskyString(const String &filename)	{
	static long ltime = 0;
	char buff[80];
	long delta = millis() - ltime;
	ltime += delta;
	String tim = time.ToShortString();
	File dataFile = SD.open(filename.c_str(), FILE_WRITE);
	if(dataFile)	{
		dataFile.print("FRSKY:");
		dataFile.print(tim);
		dataFile.print(":");
		dataFile.print(delta);
		dataFile.print(":");
		dataFile.print(RSSI);
		dataFile.print(":");
		dataFile.print(A1);
		dataFile.print(":");
		dataFile.println(A2);
	}
}

void FrSky::WriteFrskyGPSString(SoftwareSerial &ser)	{
	static long ltime = 0;
	char buff[128];
	long delta = millis() - ltime;
	ltime += delta;
	String tim = time.ToShortString();
	uint8_t fix = (temperature2 / 10) > 3 ? 3 : (temperature2/10);
	uint8_t sats = (uint8_t)(temperature2) - ((fix>0)?fix*10:0);
	ser.print("GPS:");
	ser.print(tim);
	ser.print(":");
	ser.print(latitude);
	ser.print(":");
	ser.print(longitude);
	ser.print(":");
	ser.print(altitude);
	ser.print(":");
	ser.print(gps_altitude);
	ser.print(":");

	ser.print(fix);
	ser.print(":");
	ser.println(sats);
}
void FrSky::WriteFrskyGPSString(FastSerial &ser)	{
	static long ltime = 0;
	char buff[128];
	long delta = millis() - ltime;
	ltime += delta;
	String tim = time.ToShortString();
	uint8_t fix = (temperature2 / 10) > 3 ? 3 : (temperature2/10);
	uint8_t sats = (uint8_t)(temperature2) - ((fix>0)?fix*10:0);
	ser.print("GPS:");
	ser.print(tim);
	ser.print(":");
	ser.print(latitude);
	ser.print(":");
	ser.print(longitude);
	ser.print(":");
	ser.print(altitude);
	ser.print(":");
	ser.print(gps_altitude);
	ser.print(":");
	ser.print(fix);
	ser.print(":");
	ser.println(sats);
}

void FrSky::WriteFrskyGPSString(const String &filename){
	static long ltime = 0;
	char buff[128];
	long delta = millis() - ltime;
	ltime += delta;
	String tim = time.ToShortString();
	uint8_t fix = (temperature2 / 10) > 3 ? 3 : (temperature2/10);
	uint8_t sats = (uint8_t)(temperature2) - ((fix>0)?fix*10:0);
	File dataFile = SD.open(filename.c_str(), FILE_WRITE);
	if(dataFile)	{
		dataFile.print("GPS:");
		dataFile.print(latitude);
		dataFile.print(",");
		dataFile.print(longitude);
		dataFile.print(",");
		dataFile.print(altitude);
		dataFile.print(",");
		dataFile.print(gps_altitude);
		dataFile.print(",");
		dataFile.print(fix);
		dataFile.print(",");
		dataFile.println(sats);
		dataFile.close();
	}
}

void FrSky::CheckData(SoftwareSerial &ser)	{
	uint8_t i;
	/** Checks for FrSky sending Telemetry Info **/
	while(ser.available())	{
		inbuffer[inbuffpos] = ser.read();		//	Read the byte
		if(inbuffer[inbuffpos] == 0x7E)	{
			inbuffer[0] = 0x7E;					//	Reset Start
			inbuffpos = 1;
		}else{
			if(xornext == 1)	{					//	Check if byte is stuffed
				inbuffer[inbuffpos] ^= 0x20;
				xornext = 0;
				inbuffpos++;
			}else if(inbuffer[inbuffpos] == 0x7D)	//	Check if the next byte is stuffed, if so we discard current one.
				xornext = 1;
			else									//	If not, just get to next position
				inbuffpos++;
		}
		if(inbuffer[0] == PACKET_HEAD)	{		//	We have a valid alarm start
			if(inbuffpos == 10)	{				//	Lets see if we have all bytes
				switch(inbuffer[1])	{
#ifdef READ_FRSKY_ALARMS
				case 0xFC:						//	(Alarm1)	TAnalog 1  - GreaterThan /LessThan - AlarmLevel
					A1T_1 	= 	inbuffer[2];
					A1G_1 	= 	inbuffer[3];
					A1L_1 	= 	inbuffer[4];
					break;
				case 0xFB:						//	(Alarm2)	TAnalog 1  - GreaterThan /LessThan - AlarmLevel
					A1T_2 	= 	inbuffer[2];
					A1G_2 	= 	inbuffer[3];
					A1L_2 	= 	inbuffer[4];
					break;
				case 0xFA:						//	(Alarm1)	TAnalog 2  - GreaterThan /LessThan - AlarmLevel
					A2T_1 	= 	inbuffer[2];
					A2G_1 	= 	inbuffer[3];
					A2L_1 	= 	inbuffer[4];
					break;
				case 0xF9:						//	(Alarm2)	TAnalog 2  - GreaterThan /LessThan - AlarmLevel
					A2T_2 	= 	inbuffer[2];
					A2G_2 	= 	inbuffer[3];
					A2L_2 	= 	inbuffer[4];
					break;
#endif
				case 0xFE:						//	(Data)		VAnalog 1  - VAnalog 2             - Link Quality
					A1		=	inbuffer[2];
					A2		=	inbuffer[3];
					RSSI	=	inbuffer[4];
					break;
				case 0xFD:						//	User bytes on 4 to 9, size on byte 3
					for(i=4;i<(inbuffer[2]&0x7)+4;i++)
						ProcessHUB(inbuffer[i]);
					break;
				}
				inbuffpos 	= 	0;
			}
		}else									//	We dont have a valid start
			inbuffpos 		= 	0;
	}
}

void FrSky::ProcessHUB(uint8_t data)	{
	static uint8_t uxornext = 0;
	if(data == 0x5E)	{
		if(hubsize == 4)
			ProcessHUBData();
		hubbuffer[0] = 0x5E;
		hubsize = 1;
	}else if(data == 0x5D)
		uxornext = 1;
	else{
		if(uxornext)
			hubbuffer[hubsize] = data ^ 0x60;
		else
			hubbuffer[hubsize] = data;
		hubsize++;
	}
}

void FrSky::ProcessHUBData()	{
	uint16_t value;
	uint8_t value2;
	if(hubbuffer[0] == 0x5E)	{
		switch(hubbuffer[1])	{
			case 	GPSALT				:
				value = To16(hubbuffer[3], hubbuffer[2]);
				gps_altitude 	=	(gps_altitude - ((gps_altitude / 1000)*1000)) + (int32_t)value * 1000;
				break;
			case	GPSALT+DECIMAL:
				value = To16(hubbuffer[3], hubbuffer[2]);
				gps_altitude 	=	((gps_altitude / 1000)*1000) + value;
				break;
			case 	TEMP1				:
				temperature1	=	 (int8_t)To16(hubbuffer[3], hubbuffer[2]);
				break;
			case 	TEMP2				:
				temperature2	=	 (int8_t)To16(hubbuffer[3], hubbuffer[2]);
				break;
#ifdef FRSKY_RPM
			case 	RPM					:
				rpm 			=  	To16(inbuffer[3], inbuffer[2]) * 60;
				break;
#endif
#ifdef FRSKY_FUEL
			case 	FUEL				:
				fuellevel		=  	To16(hubbuffer[3], hubbuffer[2]);
				break;
#endif
#ifdef FRSKY_CELL
			case 	INDVOLT				:
				//	TODO: TEST
				value 			= 	To16(hubbuffer[3], hubbuffer[2]);
				value2			=	(value >> 4) & 0x07;								//	Cell Number
				value			=	(value & ~(0x07<<4));								//	Remove the cell number
				value			=	((value >> 12) & 0xF) + ((value >> 4) & 0xF) << 4;	//	Get value
				cell[value2]	=	CELL_V2C(value);
				break;
#endif
			case 	ALTITUDE			:
				value 		= 	To16(hubbuffer[3], hubbuffer[2]);
				altitude 	=	(altitude - ((altitude / 1000)*1000)) + (int32_t)value * 1000;
				break;
			case 	ALTIDEC				:
				value 		= 	To16(hubbuffer[3], hubbuffer[2]);
				altitude 	=	((altitude / 1000)*1000) + (int32_t)value;
				break;
			case 	GPSSPEED			:
				value 		= 	To16(hubbuffer[3], hubbuffer[2]);
				altitude 	=	(gps_speed - ((gps_speed / 100)*100)) + (int32_t)value * 100;
				break;
			case 	GPSSPEED+DECIMAL	:
				value 		= 	To16(hubbuffer[3], hubbuffer[2]);
				gps_speed 	=	((gps_speed / 100)*100) + value;
				break;
			case 	LONGITUDE 			:
				value 		= 	To16(hubbuffer[3], hubbuffer[2]);
				longitude 	=	(longitude - ((longitude / 10000)*10000)) + (int32_t)value * 10000;
				break;
			case	LONGITUDE+DECIMAL	:
				value 		= 	To16(hubbuffer[3], hubbuffer[2]);
				longitude 	=	((longitude / 10000)*10000) + (value%10000);
				break;
			case 	LATITUDE			:
				value 		= 	To16(hubbuffer[3], hubbuffer[2]);
				latitude 	=	(latitude - ((latitude / 10000)*10000)) + (int32_t)value * 10000;
				break;
			case	LATITUDE+DECIMAL	:
				value 		= 	To16(hubbuffer[3], hubbuffer[2]);
				latitude 	=	((latitude / 10000)*10000) + (value%10000);
				break;
#ifdef READ_FRSKY_COURSE
			case 	COURSE				:
				value 		= 	To16(hubbuffer[3], hubbuffer[2]);
				course		=	(course - ((int)course)) + value;
				break;
			case	COURSE+DECIMAL		:
				value 		= 	To16(hubbuffer[3], hubbuffer[2]);
				course	 	=	(int)course + value/1000.0;
			break;
#endif
#ifdef READ_FRSKY_DATE
			case 	DATE				:
				time.day 	=	inbuffer[2];
				time.month	=	inbuffer[3];
				break;
			case 	YEAR				:
				time.year	=	inbuffer[2];
				break;
			case 	TIME				:
				time.hour	=	inbuffer[2];
				time.minute	=	inbuffer[3];
				break;
			case 	SECOND				:
				time.seconds=	inbuffer[2];
				break;
#endif
			case 	EASTWEST			:
				if((inbuffer[2] == 'W' && longitude > 0) || (inbuffer[2] == 'E' && longitude < 0))
					longitude = ~longitude + 1;			//	Invert signal
				eastwest = inbuffer[2];
				break;
			case 	NORTHSOUTH			:
				if((inbuffer[2] == 'S' && latitude > 0) || (inbuffer[2] == 'N' && latitude < 0))
					latitude = ~latitude + 1;			//	Invert signal
				eastwest = inbuffer[2];
				break;
#ifdef FRSKY_ACC
			case 	ACCX				:
				acc[0] 	=	To16(hubbuffer[3], hubbuffer[2]);
				break;
			case 	ACCY				:
				acc[1] 	=	To16(hubbuffer[3], hubbuffer[2]);
				break;
			case 	ACCZ				:
				acc[2] 	=	To16(hubbuffer[3], hubbuffer[2]);
				break;
#endif
#ifdef READ_FRSKY_VI
			case 	CURRENT				:
				current =	To16(hubbuffer[3], hubbuffer[2]);
				break;
			case 	VOLTAGE				:
				//TODO
				break;
			case 	VOLTAGEDEC			:
				//TODO
				break;
#endif
			default:	//	Invalid Data
				break;
		}
	}
	hubsize = 0;
}

void FrSky::ClearBuffer()	{
	for(int i=0;i<12;i++)
		inbuffer[i] = 0;
	inbuffpos = 0;
}
