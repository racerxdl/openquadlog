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

String FrSky::GenerateFrSkyString()	{
	static long ltime = 0;
	char buff[40];
	long delta = millis() - ltime;
	ltime += delta;
	String tim = time.ToString();
	sprintf(buff,"FRSKY:%s:%ld:%03u:%03u:%03u\x00",tim.c_str(),delta,RSSI,A1,A2);
	return String(buff);
}

String FrSky::GenerateFrSkyGPSString()	{
	static long ltime = 0;
	char buff[40];
	long delta = millis() - ltime;
	ltime += delta;
	String tim = time.ToString();
	uint8_t fix = (temperature1 / 10) > 3 ? 3 : (temperature1/10);
	uint8_t sats = (temperature1) - (fix>0)?fix*10:0;
	sprintf(buff,"GPS:%s:%ld:%ld:%ld:%ld:%ld:%u:%u\x00",tim.c_str(),delta,latitude,longitude,altitude,0,fix,sats);
	return String(buff);
}

void FrSky::CheckData(SoftwareSerial &ser)	{
	uint16_t value;
	uint8_t value2;
	/** Checks for FrSky sending Telemetry Info **/
	while(ser.available())	{
		inbuffer[inbuffpos] = ser.read();		//	Read the byte
		if(xornext)	{							//	Check if byte is stuffed
			inbuffer[inbuffpos] ^= 0x20;
			xornext = 0;
		}else if(inbuffer[inbuffpos] == 0x7D)	//	Check if the next byte is stuffed, if so we discard current one.
			xornext = 1;
		else									//	If not, just get to next position
			inbuffpos++;
		if(inbuffer[0] == HEADER)	{			//	We have a valid data header
			if(inbuffpos == 4)	{
				switch(inbuffer[1])	{
					case 	GPSALT				:
						value = To16(inbuffer[3], inbuffer[2]);
						gps_altitude 	=	(gps_altitude - ((gps_altitude / 1000)*1000)) + value * 1000;
						break;
					case	GPSALT+DECIMAL:
						value = To16(inbuffer[3], inbuffer[2]);
						gps_altitude 	=	((gps_altitude / 1000)*1000) + value;
						inbuffpos 	= 	0;
						break;
					case 	TEMP1				:
						temperature1	=	 To16(inbuffer[3], inbuffer[2]);
						inbuffpos 	= 	0;
						break;
					case 	TEMP2				:
						temperature2	=	 To16(inbuffer[3], inbuffer[2]);
						inbuffpos 	= 	0;
						break;
#ifdef FRSKY_RPM
					case 	RPM					:
						rpm 			=  	To16(inbuffer[3], inbuffer[2]) * 60;
						inbuffpos 	= 	0;
						break;
#endif
#ifdef FRSKY_FUEL
					case 	FUEL				:
						fuellevel		=  	To16(inbuffer[3], inbuffer[2]);
						inbuffpos 	= 	0;
						break;
#endif
#ifdef FRSKY_CELL
					case 	INDVOLT				:
						//	TODO: TEST
						value 			= 	To16(inbuffer[3], inbuffer[2]);
						value2			=	(value >> 4) & 0x07;								//	Cell Number
						value			=	(value & ~(0x07<<4));								//	Remove the cell number
						value			=	((value >> 12) & 0xF) + ((value >> 4) & 0xF) << 4;	//	Get value
						cell[value2]	=	CELL_V2C(value);
						inbuffpos 		= 	0;
						break;
#endif
					case 	ALTITUDE			:
						value 		= 	To16(inbuffer[3], inbuffer[2]);
						altitude 	=	(altitude - ((altitude / 1000)*1000)) + value * 1000;
						inbuffpos 	= 	0;
						break;
					case 	ALTIDEC				:
						value 		= 	To16(inbuffer[3], inbuffer[2]);
						altitude 	=	((altitude / 1000)*1000) + value;
						inbuffpos 	= 	0;
						break;
					case 	GPSSPEED			:
						value 		= 	To16(inbuffer[3], inbuffer[2]);
						altitude 	=	(gps_speed - ((gps_speed / 100)*100)) + value * 100;
						inbuffpos 	= 	0;
						break;
					case 	GPSSPEED+DECIMAL	:
						value 		= 	To16(inbuffer[3], inbuffer[2]);
						gps_speed 	=	((gps_speed / 100)*100) + value;
						inbuffpos 	= 	0;
						break;
					case 	LONGITUDE 			:
						value 		= 	To16(inbuffer[3], inbuffer[2]);
						longitude 	=	(longitude - ((longitude / 10000000)*10000000)) + value * 10000000;
						inbuffpos 	= 	0;
						break;
					case	LONGITUDE+DECIMAL	:
						value 		= 	To16(inbuffer[3], inbuffer[2]);
						longitude 	=	((longitude / 10000000)*10000000) + value;
						inbuffpos 	= 	0;
						break;
					case 	LATITUDE			:
						value 		= 	To16(inbuffer[3], inbuffer[2]);
						latitude 	=	(latitude - ((latitude / 10000000)*10000000)) + value * 10000000;
						inbuffpos 	= 	0;
						break;
					case	LATITUDE+DECIMAL	:
						value 		= 	To16(inbuffer[3], inbuffer[2]);
						latitude 	=	((latitude / 10000000)*10000000) + value;
						inbuffpos 	= 	0;
						break;
					case 	COURSE				:
						value 		= 	To16(inbuffer[3], inbuffer[2]);
						course		=	(course - ((int)course)) + value;
						inbuffpos 	= 	0;
						break;
					case	COURSE+DECIMAL		:
						value 		= 	To16(inbuffer[3], inbuffer[2]);
						course	 	=	(int)course + value/1000.0;
						inbuffpos 	= 	0;
					break;
					case 	DATE				:
						time.day 	=	inbuffer[2];
						time.month	=	inbuffer[3];
						inbuffpos 	= 	0;
						break;
					case 	YEAR				:
						time.year	=	inbuffer[2];
						inbuffpos 	= 	0;
						break;
					case 	TIME				:
						time.hour	=	inbuffer[2];
						time.minute	=	inbuffer[3];
						inbuffpos 	= 	0;
						break;
					case 	SECOND				:
						time.seconds=	inbuffer[2];
						inbuffpos 	= 	0;
						break;
					case 	EASTWEST			:
						if((inbuffer[2] == 'W' && longitude > 0) || (inbuffer[2] == 'E' && longitude < 0))
							longitude = ~longitude + 1;			//	Invert signal
						eastwest = inbuffer[2];
						inbuffpos 	= 	0;
						break;
					case 	NORTHSOUTH			:
						if((inbuffer[2] == 'S' && latitude > 0) || (inbuffer[2] == 'N' && latitude < 0))
							latitude = ~latitude + 1;			//	Invert signal
						eastwest = inbuffer[2];
						inbuffpos 	= 	0;
						break;
#ifdef FRSKY_ACC
					case 	ACCX				:
						acc[0] 	=	To16(inbuffer[3], inbuffer[2]);
						inbuffpos 	= 	0;
						break;
					case 	ACCY				:
						acc[1] 	=	To16(inbuffer[3], inbuffer[2]);
						inbuffpos 	= 	0;
						break;
					case 	ACCZ				:
						acc[2] 	=	To16(inbuffer[3], inbuffer[2]);
						inbuffpos 	= 	0;
						break;
#endif
					case 	CURRENT				:
						current =	To16(inbuffer[3], inbuffer[2]);
						break;
					case 	VOLTAGE				:
						//TODO
						inbuffpos 	= 	0;
						break;
					case 	VOLTAGEDEC			:
						//TODO
						inbuffpos 	= 	0;
						break;
					default:	//	Invalid Data
						inbuffpos 	= 	0;
						break;
				}
			}
		}else if(inbuffer[0] == ALARM_HEAD)	{	//	We have a valid alarm start
			if(inbuffpos == 11)	{				//	Lets see if we have all bytes
				switch(inbuffer[1])	{
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
				case 0xFE:						//	(Data)		VAnalog 1  - VAnalog 2             - Link Quality
					A1		=	inbuffer[2];
					A2		=	inbuffer[3];
					RSSI	=	inbuffer[4];
					break;
				case 0xFD:						//	User bytes on 4 to 9, size on byte 3
												//	TODO: Dunno if we need something here o.O
					break;
				}
				inbuffpos 	= 	0;
			}
		}else									//	We dont have a valid start
			inbuffpos 		= 	0;
	}
}

void FrSky::ClearBuffer()	{
	for(int i=0;i<64;i++)
		buffer[i] = 0;
	buffsize = 0;
}
