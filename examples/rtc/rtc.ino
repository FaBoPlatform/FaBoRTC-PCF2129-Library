#include <Wire.h>
#include <RTC_PCF2129.h>

RTC_PCF2129 RTC;

uint8_t dow(uint16_t y, uint8_t m, uint8_t d)
{
  uint8_t dow;
  uint8_t dowArray[] PROGMEM = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};

  y -= m < 3;
  dow = ((y + y / 4 - y / 100 + y / 400 + pgm_read_byte(dowArray + (m - 1)) + d) % 7);

  if (dow == 0)
  {
    return 7;
  }
  return dow;
}

void setup() {
  Serial.begin(115200);

  Serial.println();
  Serial.println("RESET");

  Serial.println("Checking I2C device...");
  if (RTC.searchDevice())
  {
    Serial.println("configuring RTC I2C");
    RTC.configure();
  } else {
    Serial.println("device not found");
    while (1);
  }

  //  Serial.println("set date/time");
  //  RTC.setDate(2020, 4, 2, 14, 14, 43, 00);
}

void loop() {
  DateTime now = RTC.now();

  Serial.print("Time: ");
  Serial.print(now.year());
  Serial.print("/");
  Serial.print(now.month());
  Serial.print("/");
  Serial.print(now.day());
  Serial.print(" ");
  Serial.print(now.hour());
  Serial.print(":");
  Serial.print(now.minute());
  Serial.print(":");
  Serial.print(now.second());
  Serial.print(" - ");
  Serial.println(now.week());

  delay(1000);
}