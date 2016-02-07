#include "fabo-mpl115.h"
#include "Wire.h"

void setup() {
  Serial.begin(9600);
}

void loop() {
  
  float pressure = faboBarometer.readPressure();

  Serial.print("pressure: ");
  Serial.println(pressure);

  delay(1000);
}
