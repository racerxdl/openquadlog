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

void FrSky::CheckData(SoftwareSerial &ser)	{
	/** Checks Frame 1 **/
	if(lastframe1+FRAME1_TIME < millis())	{
		SendFrame1(ser);
		lastframe1 = millis();
	}

	/** Checks Frame 2 **/
	if(lastframe2+FRAME2_TIME < millis())	{
		SendFrame2(ser);
		lastframe2 = millis();
	}

	/** Checks Frame 3 **/
	if(lastframe3+FRAME3_TIME < millis())	{
		SendFrame3(ser);
		lastframe3 = millis();
	}

	/** Checks for FrSky sending Telemetry Info **/
	if(ser.available())	{
		inbuffer[inbuffpos] = ser.read();		//	Read the byte
		if(xornext)	{							//	Check if byte is stuffed
			inbuffer[inbuffpos] ^= 0x20;
			xornext = 0;
		}else if(inbuffer[inbuffpos] == 0x7D)	//	Check if the next byte is stuffed, if so we discard current one.
			xornext = 1;
		else									//	If not, just get to next position
			inbuffpos++;

		if(inbuffer[0] == 0x7E)	{				//	We have a valid start
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

void FrSky::WriteBuffer(SoftwareSerial &ser)	{
	int i = 0;
	while(i<buffsize)	{
		if(i%4)	{
			switch(buffer[i])	{
			case HEADER:
				ser.write(0x5D);
				ser.write(0x3E);
				break;
			case ESCAPE:
				ser.write(0x5D);
				ser.write(0x3D);
				break;
			default:
				ser.write(buffer[i]);
				break;
			}
		}else
			ser.write(buffer[i]);
		i++;
	}
	ClearBuffer();
}

void FrSky::UpdateDataWithNaza(NazaGPS &naza)	{
	gps_altitude 	= 	naza.altitude;
	latitude 		= 	naza.latitude;
	longitude 		= 	naza.longitude;

	altitude		=	gps_altitude;
	gps_speed		=	naza.ground_speed;

	course 			=	naza.GPSHead;
	time.FromDateTime(naza.time);
}

void FrSky::SendFrame1(SoftwareSerial &ser)	{
	AddToBuffer(ACCX);
	AddToBuffer(ACCY);
	AddToBuffer(ACCZ);
	AddToBuffer(ALTITUDE);
	AddToBuffer(TEMP1);
	AddToBuffer(TEMP2);
	AddToBuffer(INDVOLT);
	AddToBuffer(CURRENT);
	AddToBuffer(VOLTAGE);
	AddToBuffer(RPM);
	buffer[buffsize++] = TAIL;
	WriteBuffer(ser);
}

void FrSky::SendFrame2(SoftwareSerial &ser)	{
	AddToBuffer(COURSE);
	AddToBuffer(LATITUDE);
	AddToBuffer(LONGITUDE);
	AddToBuffer(GPSSPEED);
	AddToBuffer(GPSALT);
	AddToBuffer(FUEL);
	buffer[buffsize++] = TAIL;
	WriteBuffer(ser);
}

void FrSky::SendFrame3(SoftwareSerial &ser)	{
	AddToBuffer(DATE);
	AddToBuffer(TIME);
	buffer[buffsize++] = TAIL;
	WriteBuffer(ser);
}

void FrSky::AddToBuffer(FrSkyID id)	{
	int k,d;
	switch(id)	{
	case GPSALT:
		k = (gps_altitude/1e3);
		d = (gps_altitude - (k*1e3));

		buffer[buffsize	] = HEADER;
		buffer[buffsize+1] = GPSALT;
		buffer[buffsize+2] = lsb(k);
		buffer[buffsize+3] = msb(k);

		buffer[buffsize+4] = HEADER;
		buffer[buffsize+5] = GPSALT+DECIMAL;
		buffer[buffsize+6] = lsb(d);
		buffer[buffsize+7] = msb(d);

		buffsize += 8;
		break;
	case TEMP1:
		buffer[buffsize	] = HEADER;
		buffer[buffsize+1] = TEMP1;
		buffer[buffsize+2] = lsb(temperature1);
		buffer[buffsize+3] = msb(temperature1);
		buffsize += 4;
		break;
	case RPM:
		buffer[buffsize	] = HEADER;
		buffer[buffsize+1] = RPM;
		buffer[buffsize+2] = lsb(rpm/60);
		buffer[buffsize+3] = msb(rpm/60);
		buffsize += 4;
		break;
	case FUEL:
		buffer[buffsize	] = HEADER;
		buffer[buffsize+1] = FUEL;
		buffer[buffsize+2] = lsb(fuellevel);
		buffer[buffsize+3] = msb(fuellevel);
		buffsize += 4;
		break;
	case TEMP2:
		buffer[buffsize	] = HEADER;
		buffer[buffsize+1] = TEMP2;
		buffer[buffsize+2] = lsb(temperature2);
		buffer[buffsize+3] = msb(temperature2);
		buffsize += 4;
		break;
	case INDVOLT:	/** TODO **/ break;
	case ALTITUDE:
		k = (altitude/1e3);
		d = (altitude - (k*1e3));

		buffer[buffsize	] = HEADER;
		buffer[buffsize+1] = ALTITUDE;
		buffer[buffsize+2] = lsb(k);
		buffer[buffsize+3] = msb(k);

		buffer[buffsize+4] = HEADER;
		buffer[buffsize+5] = ALTIDEC;
		buffer[buffsize+6] = lsb(d);
		buffer[buffsize+7] = msb(d);

		buffsize += 8;
		break;
	case GPSSPEED:
		k = (gps_speed/1e2);
		d = (gps_speed - (k*1e2));

		buffer[buffsize	] = HEADER;
		buffer[buffsize+1] = GPSSPEED;
		buffer[buffsize+2] = lsb(k);
		buffer[buffsize+3] = msb(k);

		buffer[buffsize+4] = HEADER;
		buffer[buffsize+5] = GPSSPEED+DECIMAL;
		buffer[buffsize+6] = lsb(d);
		buffer[buffsize+7] = msb(d);

		buffsize += 8;
		break;
	case LONGITUDE:
		k = (longitude/1e7);
		d = (longitude - (k*1e7));

		buffer[buffsize	] = HEADER;
		buffer[buffsize+1] = LONGITUDE;
		buffer[buffsize+2] = lsb(k);
		buffer[buffsize+3] = msb(k);

		buffer[buffsize+4] = HEADER;
		buffer[buffsize+5] = LONGITUDE+DECIMAL;
		buffer[buffsize+6] = lsb(d);
		buffer[buffsize+7] = msb(d);


		buffer[buffsize+8] = HEADER;
		buffer[buffsize+9] = EASTWEST;
		buffer[buffsize+10] = (longitude < 0 ) ? 'W' : 'E';
		buffer[buffsize+11] = 0;

		buffsize += 12;
		break;
	case LATITUDE:
		k = (latitude/1e7);
		d = (latitude - (k*1e7));

		buffer[buffsize	] = HEADER;
		buffer[buffsize+1] = LATITUDE;
		buffer[buffsize+2] = lsb(k);
		buffer[buffsize+3] = msb(k);

		buffer[buffsize+4] = HEADER;
		buffer[buffsize+5] = LATITUDE+DECIMAL;
		buffer[buffsize+6] = lsb(d);
		buffer[buffsize+7] = msb(d);

		buffer[buffsize+8] = HEADER;
		buffer[buffsize+9] = NORTHSOUTH;
		buffer[buffsize+10] = (latitude < 0 ) ? 'S' : 'N';
		buffer[buffsize+11] = 0;

		buffsize += 12;
		break;
	case COURSE:
		k = (int)(course);
		d = (course - k)*1e3;

		buffer[buffsize	] = HEADER;
		buffer[buffsize+1] = COURSE;
		buffer[buffsize+2] = lsb(k);
		buffer[buffsize+3] = msb(k);

		buffer[buffsize+4] = HEADER;
		buffer[buffsize+5] = COURSE+DECIMAL;
		buffer[buffsize+6] = lsb(d);
		buffer[buffsize+7] = msb(d);

		buffsize += 8;
		break;
	case DATE:
		buffer[buffsize	] = HEADER;
		buffer[buffsize+1] = DATE;
		buffer[buffsize+2] = time.day;
		buffer[buffsize+3] = time.month;
		buffsize += 4;
		break;
	case YEAR:
		buffer[buffsize	] = HEADER;
		buffer[buffsize+1] = YEAR;
		buffer[buffsize+2] = time.year;
		buffer[buffsize+3] = 0;
		buffsize += 4;
		break;
	case TIME:
		buffer[buffsize	] = HEADER;
		buffer[buffsize+1] = TIME;
		buffer[buffsize+2] = time.hour;
		buffer[buffsize+3] = time.minute;
		buffsize += 4;
		break;
	case SECOND:
		buffer[buffsize	] = HEADER;
		buffer[buffsize+1] = SECOND;
		buffer[buffsize+2] = time.seconds;
		buffer[buffsize+3] = 0;
		buffsize += 4;
		break;
	case ACCX:
		buffer[buffsize	] = HEADER;
		buffer[buffsize+1] = ACCX;
		buffer[buffsize+2] = lsb(acc[0]);
		buffer[buffsize+3] = msb(acc[0]);
		buffsize += 4;
		break;
	case ACCY:
		buffer[buffsize	] = HEADER;
		buffer[buffsize+1] = ACCY;
		buffer[buffsize+2] = lsb(acc[1]);
		buffer[buffsize+3] = msb(acc[1]);
		buffsize += 4;
		break;
	case ACCZ:
		buffer[buffsize	] = HEADER;
		buffer[buffsize+1] = ACCZ;
		buffer[buffsize+2] = lsb(acc[2]);
		buffer[buffsize+3] = msb(acc[2]);
		buffsize += 4;
		break;
	case CURRENT:
		buffer[buffsize	] = HEADER;
		buffer[buffsize+1] = CURRENT;
		buffer[buffsize+2] = lsb(current);
		buffer[buffsize+3] = msb(current);
		buffsize += 4;
		break;
	case VOLTAGE:
		float v 	= 	voltage * 0.5238f;
		float vdec	=	(voltage - v)  * 10.f;

		buffer[buffsize	] = HEADER;
		buffer[buffsize+1] = VOLTAGE;
		buffer[buffsize+2] = lsb((int)v);
		buffer[buffsize+3] = msb((int)v);

		buffer[buffsize+4] = HEADER;
		buffer[buffsize+5] = VOLTAGEDEC;
		buffer[buffsize+6] = lsb((int)vdec);
		buffer[buffsize+7] = msb((int)vdec);
		buffsize += 4;

		break;
	}
}
