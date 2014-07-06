/**
  ___                    ___                  _ _
 / _ \ _ __   ___ _ __  / _ \ _   _  __ _  __| | |    ___   __ _
| | | | '_ \ / _ \ '_ \| | | | | | |/ _` |/ _` | |   / _ \ / _` |
| |_| | |_) |  __/ | | | |_| | |_| | (_| | (_| | |__| (_) | (_| |
 \___/| .__/ \___|_| |_|\__\_\\__,_|\__,_|\__,_|_____\___/ \__, |
      |_|                                                  |___/

      By: Lucas Teske
**/


#include <ctype.h>
#include "SoftwareSerial.h"
SoftwareSerial mySerial(10, 11, true); // RX, TX

void setup() {
  digitalWrite(13,HIGH);     // turn on debugging LED
  Serial.begin(9600); 
  mySerial.begin(9600);
}

void loop()
{
  if (mySerial.available())
    Serial.write(mySerial.read());
}

