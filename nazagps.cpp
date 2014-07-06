/**
  ___                    ___                  _ _
 / _ \ _ __   ___ _ __  / _ \ _   _  __ _  __| | |    ___   __ _
| | | | '_ \ / _ \ '_ \| | | | | | |/ _` |/ _` | |   / _ \ / _` |
| |_| | |_) |  __/ | | | |_| | |_| | (_| | (_| | |__| (_) | (_| |
 \___/| .__/ \___|_| |_|\__\_\\__,_|\__,_|\__,_|_____\___/ \__, |
      |_|                                                  |___/

      By: Lucas Teske
**/

#include "nazagps.h"

void NazaGPS::DecodeMessage(char *data, char id, char size)	{
	switch(id)	{
		case MessageType::GPS:
			char xormask = data[55];							//	This byte isnt xored, so we can use as mask. Its always 0
			uint16_t sequence = *((uint16_t *) (&data[56])); 	//	Sequence Number is a short at position 56. Not xored
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

			position_dop		=	UInt16Val(&data[40]);
			vertical_dop		=	UInt16Val(&data[42]);
			north_dop			=	UInt16Val(&data[44]);
			east_dop			=	UInt16Val(&data[46]);

			fixtype				=	data[50];
			fixstatus			=	data[52];

		break;
		case MessageType::MAG:
			char xormask = data[5];
			for(int i=0;i<size;i++)
				if(i!=5)	data[i] ^= xormask;

			MagX	=	Int16Val(&data[0]);
			MagY	=	Int16Val(&data[2]);
			MagZ	=	Int16Val(&data[4]);

			MagHead = -atan2(MagY, MagX) * 180.0 / M_PI;
			if (MagHead < 0.0) MagHead += 360.0;

		break;
		case MessageType::FIRM:
			sprintf(hardware_version, "%x.%x.%x.%x", data[11], data[10], data[9], data[8]);
			sprintf(software_version, "%x.%x.%x.%x", data[7], data[6], data[5], data[4]);
		break;
		default:
			// Invalid Message: TODO
	}
}
