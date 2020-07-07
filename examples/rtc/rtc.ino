#include <Wire.h>
#include <RTC_PCF2129.h>

RTC_PCF2129 RTC;

void setup()
{
  Serial.begin(115200);

  Serial.println();
  Serial.println("RESET");

  DateTime compiled = DateTime(__DATE__, __TIME__);

  Serial.println("Checking I2C device...");
  if (RTC.searchDevice())
  {
    Serial.println("configuring RTC I2C");
    RTC.configure();

    if (!RTC.IsDateTimeValid())
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

        RTC.SetDateTime(compiled);
      }
    }
    else
    {
      Serial.printf("Found an RTC with valid time\n");
    }

    uint32_t nowTS = RTC.now().getTimeStamp();
    uint32_t compiledTS = compiled.getTimeStamp();
    if (nowTS < compiledTS)
    {
      Serial.printf("RTC is older than compile time!  (Updating DateTime)\n");
      RTC.SetDateTime(compiled);
    }
    else if (nowTS > compiledTS)
    {
      Serial.printf("RTC is newer than compile time. (this is expected)\n");
    }
    else if (nowTS == compiledTS)
    {
      Serial.printf("RTC is the same as compile time! (not expected but all is fine)\n");
    }
  }
  else
  {
    Serial.printf("device not found\n");
    while (1)
      ;
  }
}

void loop()
{
  DateTime now = RTC.now();

  if (now.IsValid())
  {
    Serial.printf("Date: %d/%d/%d Time: %d:%d:%d week: %d\n", now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second(), now.week());
    Serial.printf("str Data: %s\n", now.getStrData().c_str());
    Serial.printf("str Hora: %s\n", now.getStrHora().c_str());
    Serial.printf("TS: %u\n", now.getTimeStamp());
    delay(random(1000, 5000));
    Serial.printf("TS Now: %u\n", now.getTimeStampNow());
  }
  else
  {
    Serial.printf("DateTime Invalido\n");
  }

  delay(1000);
}