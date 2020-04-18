#include <Arduino.h>
#include "RtcUtility.h"

uint8_t BcdToUint8(uint8_t val)
{
    return val - 6 * (val >> 4);
}

uint8_t Uint8ToBcd(uint8_t val)
{
    return val + 6 * (val / 10);
}

uint8_t BcdToBin24Hour(uint8_t bcdHour)
{
    uint8_t hour;
    if (bcdHour & 0x40)
    {
        // 12 hour mode, convert to 24
        bool isPm = ((bcdHour & 0x20) != 0);

        hour = BcdToUint8(bcdHour & 0x1f);
        if (isPm)
        {
            hour += 12;
        }
    }
    else
    {
        hour = BcdToUint8(bcdHour);
    }
    return hour;
}

uint8_t bcdToDec(uint8_t value)
{
    return (uint8_t)((value / 16 * 10) + (value % 16));
}

uint8_t decToBcd(uint8_t value)
{
    return (uint8_t)((value / 10 * 16) + (value % 10));
}

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