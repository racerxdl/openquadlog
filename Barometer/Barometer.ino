#include <Wire.h>
#include "Baro.h"


Barometer bmp;
  
void setup() {
  Serial.begin(9600);
  if (!bmp.Start()) {
	Serial.println("Could not find a valid BMP085 sensor, check wiring!");
	while (1) {}
  }
}
  
void loop() {
    long startt = millis();
    long endt = 0;
    long deltaUp1;
    bmp.UpdateTemp();
    deltaUp1 = millis();
    bmp.UpdatePressure();
    endt = millis();
    
    Serial.print("Temperature = ");
    Serial.print(bmp.Temperature);
    Serial.println(" *C");
    Serial.print("Altitude = ");
    Serial.print(bmp.Altitude);
    Serial.println(" meters");
    Serial.print("Pressure = ");
    Serial.print(bmp.Pressure);
    Serial.println(" hPa");
    Serial.print("DeltaUp1: ");
    Serial.print(deltaUp1-startt);
    Serial.println(" ms");
    Serial.print("Delta: ");
    Serial.print(endt-startt);
    Serial.println(" ms");

    delay(500);
}
