#include <Arduino.h>
#include "RtcDateTime.h"

uint8_t StringToUint8(const char *pString)
{
  uint8_t value = 0;

  // skip leading 0 and spaces
  while ('0' == *pString || *pString == ' ')
  {
    pString++;
  }

  // calculate number until we hit non-numeral char
  while ('0' <= *pString && *pString <= '9')
  {
    value *= 10;
    value += *pString - '0';
    pString++;
  }
  return value;
}

DateTime::DateTime(const char *date, const char *time)
{
  // sample input: date = "Dec 06 2009", time = "12:34:56"
  _year = StringToUint8(date + 9);
  // Jan Feb Mar Apr May Jun Jul Aug Sep Oct Nov Dec
  switch (date[0])
  {
  case 'J':
    if (date[1] == 'a')
      _month = 1;
    else if (date[2] == 'n')
      _month = 6;
    else
      _month = 7;
    break;
  case 'F':
    _month = 2;
    break;
  case 'A':
    _month = date[1] == 'p' ? 4 : 8;
    break;
  case 'M':
    _month = date[2] == 'r' ? 3 : 5;
    break;
  case 'S':
    _month = 9;
    break;
  case 'O':
    _month = 10;
    break;
  case 'N':
    _month = 11;
    break;
  case 'D':
    _month = 12;
    break;
  }
  _day = StringToUint8(date + 4);
  _week = dow(_year, _month, _day);
  _hour = StringToUint8(time);
  _minute = StringToUint8(time + 3);
  _second = StringToUint8(time + 6);
}

String DateTime::getStrData()
{
  String strData = "";
  if (_year > 00 && _year < 34)
  {
    if (_day < 10)
      strData += "0" + String(_day);
    else
      strData += String(_day);
    strData += "/";
    if (_month < 10)
      strData += "0" + String(_month);
    else
      strData += String(_month);
    strData += "/";
    strData += "20" + String(_year);
  }
  else
    strData = F("RTC Desatualizado");
  return strData;
}

String DateTime::getStrHora()
{
  String strHora = "";
  if (!(_year > 00 && _year < 34))
    return strHora;

  if (_hour < 10)
    strHora += "0" + String(_hour);
  else
    strHora += String(_hour);
  strHora += ":";
  if (_minute < 10)
    strHora += "0" + String(_minute);
  else
    strHora += String(_minute);
  strHora += ":";
  if (_second < 10)
    strHora += "0" + String(_second);
  else
    strHora += String(_second);
  return strHora;
}

uint32_t DateTime::getTimeStamp()
{
  if (_year > 00 && _year < 34)
  {
    uint16_t dc;
    static const uint8_t dim[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    dc = _day;
    for (uint8_t i = 0; i < (_month - 1); i++)
      dc += dim[i];
    if ((_month > 2) && (((_year) % 4) == 0))
      ++dc;
    dc = dc + (365 * (_year)) + (((_year) + 3) / 4) - 1;

    return ((((((dc * 24L) + _hour) * 60) + _minute) * 60) + _second) + 946684800;
  }
  else
  {
    return 0;
  }
}

uint32_t DateTime::getTimeStampNow()
{
  return this->getTimeStamp() +             // Last TimeStamp by RTC
         ((millis() - _lastUpdate) / 1000); // Time since last update
}

//const uint8_t monthDays[] PROGMEM = {31,28,31,30,31,30,31,31,30,31,30,31};
const uint8_t monthDays[] PROGMEM = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
void DateTime::unixTimeToDateTime(uint32_t epoch)
{
  byte year, month, monthLength;
  unsigned long days;
  _second = epoch % 60;
  epoch /= 60; // now it is minutes
  _minute = epoch % 60;
  epoch /= 60; // now it is hours
  _hour = epoch % 24;
  epoch /= 24; // now it is days
  //  _week = (epoch + 4) % 7;
  year = 70;
  days = 0;
  while ((unsigned)(days += (LEAP_YEAR(year) ? 366 : 365)) <= epoch)
  {
    year++;
  }
  //dt.year = year; // is returned as years from 1900
  days -= LEAP_YEAR(year) ? 366 : 365;
  epoch -= days; // now it is days in this year, starting at 0
  for (month = 0; month < 12; month++)
  {
    monthLength = ((month == 1) && LEAP_YEAR(year)) ? 29 : pgm_read_byte(monthDays + month); // month==1 -> february
    if (epoch >= monthLength)
    {
      epoch -= monthLength;
    }
    else
    {
      break;
    }
  }
  _month = month + 1; // jan is month 0
  _day = epoch + 1;   // day of month
  _year = year - 100;
  _week = dow(_year, _month, _day);
}

bool DateTime::IsValid() const
{
  // this just tests the most basic validity of the value ranges
  // and valid leap years
  // It does not check any time zone or daylight savings time
  if ((_month > 0 && _month < 13) &&
      (_day > 0 && _day < 32) &&
      (_hour < 24) &&
      (_minute < 60) &&
      (_second < 60))
  {
    // days in a month tests
    //
    if (_month == 2)
    {
      if (_day > 29)
      {
        return false;
      }
      else if (_day > 28)
      {
        // leap day
        // check year to make sure its a leap year
        uint16_t Year = year();

        if ((Year % 4) != 0)
        {
          return false;
        }

        if ((Year % 100) == 0 &&
            (Year % 400) != 0)
        {
          return false;
        }
      }
    }
    else if (_day == 31)
    {
      if ((((_month - 1) % 7) % 2) == 1)
      {
        return false;
      }
    }

    return true;
  }
  return false;
}
