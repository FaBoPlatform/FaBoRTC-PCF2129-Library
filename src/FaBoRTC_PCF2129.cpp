#include "FaBoRTC_PCF2129.h"

FaBoRTC_PCF2129::FaBoRTC_PCF2129(uint8_t addr) {
  _i2caddr = addr;
  Wire.begin();
}

bool FaBoRTC_PCF2129::searchDevice(void) {
  return !(readI2c(_i2caddr) >>7);
}

void FaBoRTC_PCF2129::configure(void) {
  set24mode();
}

uint8_t FaBoRTC_PCF2129::getSeconds(void) {
  return bcdToDec(readI2c(PCF2129_SECONDS));
}

void FaBoRTC_PCF2129::setSeconds(uint8_t seconds) {
  if ( seconds>59 && seconds<0 ) {
    seconds = 0;
  }
  writeI2c(PCF2129_SECONDS, (decToBcd(seconds) + 0x80));
}

uint8_t FaBoRTC_PCF2129::getMinutes(void) {
  return bcdToDec(readI2c(PCF2129_MINUTES));
}

void FaBoRTC_PCF2129::setMinutes(uint8_t minutes) {
  if ( minutes>59 && minutes<0 ) {
    minutes = 0;
  }
  writeI2c(PCF2129_MINUTES, decToBcd(minutes));
}

uint8_t FaBoRTC_PCF2129::getHours(void) {
  return bcdToDec(readI2c(PCF2129_HOURS));
}

void FaBoRTC_PCF2129::setHours(uint8_t hours) {
  set24mode();
  if ( hours>23 && hours<0 ) {
    hours = 0;
  }
  writeI2c(PCF2129_HOURS, decToBcd(hours));
}

uint8_t FaBoRTC_PCF2129::getDays(void) {
  return bcdToDec(readI2c(PCF2129_DAYS));
}

void FaBoRTC_PCF2129::setDays(uint8_t days) {
  if ( days>31 && days<1 ) {
    days = 1;
  }
  writeI2c(PCF2129_DAYS, decToBcd(days));
}

uint8_t FaBoRTC_PCF2129::getWeekdays(void) {
  return bcdToDec(readI2c(PCF2129_WEEKDAYS));
}

void FaBoRTC_PCF2129::setWeekdays(uint8_t weekdays) {
  if ( weekdays>6 && weekdays<0 ) {
    weekdays = 0;
  }
  writeI2c(PCF2129_WEEKDAYS, decToBcd(weekdays));
}

uint8_t FaBoRTC_PCF2129::getMonths(void) {
  return bcdToDec(readI2c(PCF2129_MONTHS));
}

void FaBoRTC_PCF2129::setMonths(uint8_t months) {
  if ( months>12 && months<1 ) {
    months = 1;
  }
  writeI2c(PCF2129_MONTHS, decToBcd(months));
}

uint8_t FaBoRTC_PCF2129::getYears(void) {
  return bcdToDec(readI2c(PCF2129_YEARS));
}

void FaBoRTC_PCF2129::setYears(uint8_t years) {
  if ( years>99 && years<0 ) {
    years = 0;
  }
  writeI2c(PCF2129_YEARS, decToBcd(years));
}

DateTime FaBoRTC_PCF2129::now(void) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(PCF2129_SECONDS);
  Wire.endTransmission();
  Wire.requestFrom(_i2caddr, (uint8_t)7);
  while(!Wire.available());
  uint8_t seconds = bcdToDec(Wire.read());
  uint8_t minutes = bcdToDec(Wire.read());
  uint8_t hours = bcdToDec(Wire.read());
  uint8_t days = bcdToDec(Wire.read());
  Wire.read(); // blank read weekdays
  uint8_t months = bcdToDec(Wire.read());
  uint16_t years = bcdToDec(Wire.read()) +2000;

  return DateTime(years, months, days, hours, minutes, seconds);
}

void FaBoRTC_PCF2129::setDate(uint16_t years, uint8_t months, uint8_t days,
                              uint8_t hours, uint8_t minutes, uint8_t seconds) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(PCF2129_SECONDS);
  Wire.write(decToBcd(seconds) + 0x80);
  Wire.write(decToBcd(minutes));
  Wire.write(decToBcd(hours));
  Wire.write(decToBcd(days));
  Wire.write(0x00);
  Wire.write(decToBcd(months));
  Wire.write(decToBcd(years-2000));
  Wire.endTransmission();
}

void FaBoRTC_PCF2129::set12mode(void) {
  uint8_t ctrl;
  ctrl = readCtrl();
  ctrl |= PCF2129_CONTROL_12_24;
  writeCtrl(ctrl);
}

void FaBoRTC_PCF2129::set24mode(void) {
  uint8_t ctrl;
  ctrl = readCtrl();
  ctrl &= ~(PCF2129_CONTROL_12_24);
  writeCtrl(ctrl);
}

////////////////////////////////////////////////////////////////

uint8_t FaBoRTC_PCF2129::bcdToDec(uint8_t value) {
  return (uint8_t) ( (value/16*10) + (value%16) );
}

uint8_t FaBoRTC_PCF2129::decToBcd(uint8_t value) {
  return (uint8_t) ( (value/10*16) + (value%10) );
}

uint8_t FaBoRTC_PCF2129::readI2c(uint8_t address) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(_i2caddr, (uint8_t)1);
  while(!Wire.available());
  return Wire.read();
}

void FaBoRTC_PCF2129::writeI2c(uint8_t address, uint8_t data) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}

uint8_t FaBoRTC_PCF2129::readCtrl(void) {
  return readI2c(PCF2129_CONTROL_REGISTERS);
}

void FaBoRTC_PCF2129::writeCtrl(uint8_t data) {
  writeI2c(PCF2129_CONTROL_REGISTERS, data);
}

////////////////////////////////////////////////////////////////

DateTime::DateTime(uint16_t years, uint8_t months, uint8_t days,
                   uint8_t hours, uint8_t minutes, uint8_t seconds) {
  if (years >= 2000)
      years -= 2000;
  y = years;
  m = months;
  d = days;
  hh = hours;
  mm = minutes;
  ss = seconds;
}
