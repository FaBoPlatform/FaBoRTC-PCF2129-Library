#ifndef __RTCDATETIME_H__
#define __RTCDATETIME_H__

// ESP32 complains if not included
#if defined(ARDUINO_ARCH_ESP32)
#include <inttypes.h>
#endif

#include "RtcUtility.h"
#define LEAP_YEAR(_year) ((_year % 4) == 0)

enum DayOfWeek
{
    DayOfWeek_Sunday = 0,
    DayOfWeek_Monday,
    DayOfWeek_Tuesday,
    DayOfWeek_Wednesday,
    DayOfWeek_Thursday,
    DayOfWeek_Friday,
    DayOfWeek_Saturday,
};

const uint16_t c_OriginYear = 2000;
const uint32_t c_Epoch32OfOriginYear = 946684800;
extern const uint8_t monthDays[] PROGMEM;

class DateTime
{
public:
    DateTime(uint16_t year,
             uint8_t month,
             uint8_t week,
             uint8_t day,
             uint8_t hour,
             uint8_t minute,
             uint8_t second) : _year((year >= c_OriginYear) ? year - c_OriginYear : year),
                               _month(month),
                               _week((week > 7) ? dow(year, month, day) : week),
                               _day(day),
                               _hour(hour),
                               _minute(minute),
                               _second(second)
    {
        _lastUpdate = millis();
    }

    // RtcDateTime compileDateTime(__DATE__, __TIME__);
    DateTime(const char *date, const char *time);

    uint16_t year(void) const { return _year + 2000; }
    uint16_t yearFrom2k(void) const { return _year; }
    uint8_t month(void) const { return _month; }
    uint8_t week(void) const { return _week; }
    uint8_t day(void) const { return _day; }
    uint8_t hour(void) const { return _hour; }
    uint8_t minute(void) const { return _minute; }
    uint8_t second(void) const { return _second; }

    void unixTimeToDateTime(uint32_t epoch);
    String getStrData(void);
    String getStrHora(void);
    uint32_t getTimeStamp(void);
    uint32_t getTimeStampNow(void);
    bool IsValid(void) const;
    bool checkWeek(void);

protected:
    uint8_t _year;
    uint8_t _month;
    uint8_t _week;
    uint8_t _day;
    uint8_t _hour;
    uint8_t _minute;
    uint8_t _second;
    uint32_t _lastUpdate;
};
#endif // __RTCDATETIME_H__
