/**
  ___                    ___                  _ _
 / _ \ _ __   ___ _ __  / _ \ _   _  __ _  __| | |    ___   __ _
| | | | '_ \ / _ \ '_ \| | | | | | |/ _` |/ _` | |   / _ \ / _` |
| |_| | |_) |  __/ | | | |_| | |_| | (_| | (_| | |__| (_) | (_| |
 \___/| .__/ \___|_| |_|\__\_\\__,_|\__,_|\__,_|_____\___/ \__, |
      |_|                                                  |___/

      By: Lucas Teske
**/

#include "NazaGPS.h"
uint8_t NazaGPS::CheckData()	{
#ifdef READ_NAZA
	if(Serial1.available())	{
		/*
		 *	Resets the current status pin states.
		 */
		//digitalWrite(CHKSUM_ERROR_PIN, LOW);
		//digitalWrite(PACKET_OK_PIN, LOW);


		if(buffpos < 2)		{												//	If we dont have the header bytes, we will just read.
			buffer[buffpos] = Serial1.read();
			buffpos ++;
		}else{
			if(buffer[0] == 0x55 && buffer[1] == 0xAA)	{					//	Checks if we have the correct heading
				if(buffpos < 4)	{											//	We need 4 bytes. Head + ID + Size
					buffer[buffpos] = Serial1.read();
					buffpos++;
				}else{
					payloadsize = buffer[3];								//	Size is the 4th byte
					buffer[buffpos] = Serial1.read();
					buffpos ++;
					if(buffpos == payloadsize+6)	{						//	Ok, so we have all data
						CalcChecksum();										//	Calculate the checksum
						if(CompareChecksum(&buffer[buffpos-2]))		{		// 	Checksum OK
							DecodeMessage(&buffer[4], buffer[2], buffer[3]);//	Decode the message and save the data
							//digitalWrite(PACKET_OK_PIN, HIGH);
							buffpos = 0;
							return 1;
						}else{												//	Invalid Checksum. Discard all data.
							buffpos = 0;
							//digitalWrite(CHKSUM_ERROR_PIN, HIGH);
						}
					}
				}
			}else{															// Wrong head, lets clean and restart
				buffpos = 0;
				buffer[buffpos] = Serial1.read();
				buffpos++;
			}
		}
	}
#endif
	return 0;
}

void NazaGPS::DecodeMessage(uint8_t *data, uint8_t id, uint8_t size)	{
#ifdef READ_NAZA
	uint8_t xormask;
	uint16_t sequence;
	switch(id)	{
		case GPS:
			xormask = data[55];									//	This byte isnt xored, so we can use as mask. Its always 0
			sequence = *((uint16_t *) (&data[56])); 			//	Sequence Number is a short at position 56. Not xored
			numSat = data[48];									//	Number of satelites is also not xored

			for(int i=0;i<size;i++)
				data[i] ^= xormask;

			time.FromNazaInt(UInt32Val(&data[0]));

			latitude 			= 	Int32Val(&data[4]);
			longitude			= 	Int32Val(&data[8]);
			altitude 			= 	Int32Val(&data[12]);

			horizontal_accuracy	=	UInt32Val(&data[16]);
			vertical_accuracy	=	UInt32Val(&data[20]);

			velN				=	Int32Val(&data[28]);
			velE				=	Int32Val(&data[32]);
			velD				=	Int32Val(&data[36]);

			ground_speed		= 	sqrt(velN * velN + velE * velE);
			flight_speed		= 	sqrt(velN * velN + velE * velE + velD * velD);

			GPSHead = -atan2(velN, velE) * 180.0 / M_PI;

			position_dop		=	UInt16Val(&data[40]);
			vertical_dop		=	UInt16Val(&data[42]);
			north_dop			=	UInt16Val(&data[44]);
			east_dop			=	UInt16Val(&data[46]);

			fix					=	(FixType)data[50];
			FixStatus			=	data[52];

		break;
		case MAG:
			xormask = data[5];
			for(int i=0;i<size;i++)
				if(i!=5)	data[i] ^= xormask;

			MagX	=	Int16Val(&data[0]);
			MagY	=	Int16Val(&data[2]);
			MagZ	=	Int16Val(&data[4]);

			MagHead = -atan2(MagY, MagX) * 180.0 / M_PI;
			if (MagHead < 0.0) MagHead += 360.0;

		break;
		case FIRM:
			//sprintf((char *)hardware_version, "%x.%x.%x.%x\x00", data[11], data[10], data[9], data[8]);
			//sprintf((char *)software_version, "%x.%x.%x.%x\x00", data[7], data[6], data[5], data[4]);
		break;
		default:
			// Invalid Message: TODO
			break;
	}
#endif
}
