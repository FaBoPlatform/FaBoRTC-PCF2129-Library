#include <Wire.h>
#include <RTC_PCF2129.h>

RTC_PCF2129 RTC;

void setup() {
  Serial.begin(115200);

  Serial.println();
  Serial.println("RESET");

  DateTime compiled = DateTime(__DATE__, __TIME__);

  Serial.println("Checking I2C device...");
  if (RTC.searchDevice())
  {
    Serial.println("configuring RTC I2C");
    RTC.configure();

    if (RTC.IsDateTimeValid())
    {
      if (RTC.LastError() != 0)
      {
        Serial.print("RTC communications error = ");
        Serial.println(RTC.LastError());
      }
      else
      {
        // Common Causes:
        //    1) first time you ran and the device wasn't running yet
        //    2) the battery on the device is low or even missing

        Serial.println("RTC lost confidence in the DateTime!");

        // following line sets the RTC to the date & time this sketch was compiled
        // it will also reset the valid flag internally unless the Rtc device is
        // having an issue

        //        RTC.SetDateTime(compiled);
      }
    }
    else
    {
      RTC.SetDateTime(compiled);
    }
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

  Serial.printf("str Data: %s\n", now.getStrData().c_str());
  Serial.printf("str Hora: %s\n", now.getStrHora().c_str());
  Serial.printf("TS: %u\n", now.getTimeStamp());

  delay(1000);
}