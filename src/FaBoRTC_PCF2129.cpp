/**
 * @file  FaBoRTC_PCF2129.cpp
 * @brief fabo libtary of PCF2129
 * @author Akira Sasaki
 * @date 2,10, 2016
 */
 #include "FaBoRTC_PCF2129.h"

 /**
  * @brief PCF2129 processing Start
  * @param addr : PCV2129 Slave Address
  */
FaBoRTC_PCF2129::FaBoRTC_PCF2129(uint8_t addr) {
  _i2caddr = addr;
  Wire.begin();
}

/**
 * @brief Search Device PCF2129
 * @retval true  : Found
 * @retval false : Not found
 */
bool FaBoRTC_PCF2129::searchDevice(void) {
  return !(readI2c(_i2caddr) >>7);
}

/**
 * @brief Set Config
 */
void FaBoRTC_PCF2129::configure(void) {
  set24mode();
}

/**
 * @brief Get seconds from PCF2129
 * @return uint8_t : get seconds
 */
uint8_t FaBoRTC_PCF2129::getSeconds(void) {
  return bcdToDec(readI2c(PCF2129_SECONDS));
}

/**
 * @brief Set seconds to PCF2129
 * @param seconds : Set to seconds
 */
void FaBoRTC_PCF2129::setSeconds(uint8_t seconds) {
  if ( seconds>59 && seconds<0 ) {
    seconds = 0;
  }
  writeI2c(PCF2129_SECONDS, (decToBcd(seconds) + 0x80));
}

/**
 * @brief Get minutes from PCF2129
 * @return uint8_t : Get minutes
 */
uint8_t FaBoRTC_PCF2129::getMinutes(void) {
  return bcdToDec(readI2c(PCF2129_MINUTES));
}

/**
 * @brief Set minutes to PCF2129
 * @param [in] minutes : Set to minutes
 */
void FaBoRTC_PCF2129::setMinutes(uint8_t minutes) {
  if ( minutes>59 && minutes<0 ) {
    minutes = 0;
  }
  writeI2c(PCF2129_MINUTES, decToBcd(minutes));
}

/**
 * @brief Get hours from PCF2129
 * @return uint8_t : Get hours
 */
uint8_t FaBoRTC_PCF2129::getHours(void) {
  return bcdToDec(readI2c(PCF2129_HOURS));
}

/**
 * @brief Set hours to PCF2129
 * @param [in] hours : Set to hours
 */
void FaBoRTC_PCF2129::setHours(uint8_t hours) {
  set24mode();
  if ( hours>23 && hours<0 ) {
    hours = 0;
  }
  writeI2c(PCF2129_HOURS, decToBcd(hours));
}

/**
 * @brief Get days from PCF2129
 * @return uint8_t : Get days
 */
uint8_t FaBoRTC_PCF2129::getDays(void) {
  return bcdToDec(readI2c(PCF2129_DAYS));
}

/**
 * @brief Set days to PCF2129
 * @param [in] days : Set to days
 */
void FaBoRTC_PCF2129::setDays(uint8_t days) {
  if ( days>31 && days<1 ) {
    days = 1;
  }
  writeI2c(PCF2129_DAYS, decToBcd(days));
}

/**
 * @brief Get weekdays from PCF2129
 * @return uint8_t : Get weekdays
 */
uint8_t FaBoRTC_PCF2129::getWeekdays(void) {
  return bcdToDec(readI2c(PCF2129_WEEKDAYS));
}

/**
 * @brief Set weekdays to PCF2129
 * @param [in] weekdays : Set to weekdays
 */
void FaBoRTC_PCF2129::setWeekdays(uint8_t weekdays) {
  if ( weekdays>6 && weekdays<0 ) {
    weekdays = 0;
  }
  writeI2c(PCF2129_WEEKDAYS, decToBcd(weekdays));
}

/**
 * @brief Get months from PCF2129
 * @return uint8_t : Get months
 */
uint8_t FaBoRTC_PCF2129::getMonths(void) {
  return bcdToDec(readI2c(PCF2129_MONTHS));
}

/**
 * @brief Set months to PCF2129
 * @param [in] months : Set to months
 */
void FaBoRTC_PCF2129::setMonths(uint8_t months) {
  if ( months>12 && months<1 ) {
    months = 1;
  }
  writeI2c(PCF2129_MONTHS, decToBcd(months));
}

/**
 * @brief Get years from PCF2129
 * @return uint8_t : Get years
 */
uint8_t FaBoRTC_PCF2129::getYears(void) {
  return bcdToDec(readI2c(PCF2129_YEARS));
}

/**
 * @brief Set years to PCF2129
 * @param [in] days : Set to years
 */
void FaBoRTC_PCF2129::setYears(uint8_t years) {
  if ( years>99 && years<0 ) {
    years = 0;
  }
  writeI2c(PCF2129_YEARS, decToBcd(years));
}

/**
 * @brief Get set to variable now from PCF2129
 */
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

/**
 * @brief Set Date from PCF2129
 * @param [in] years   : Set Years Data
 * @param [in] months  : Set months Data
 * @param [in] days    : Set days Data
 * @param [in] hours   : Set hours Data
 * @param [in] minutes : Set minutes Data
 * @param [in] seconds : Set seconds Data
 */
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

/**
 * @brief Set 12-Hour Mode
 */
void FaBoRTC_PCF2129::set12mode(void) {
  uint8_t ctrl;
  ctrl = readCtrl();
  ctrl |= PCF2129_CONTROL_12_24;
  writeCtrl(ctrl);
}

/**
 * @brief Set 24-Hour Mode
 */
void FaBoRTC_PCF2129::set24mode(void) {
  uint8_t ctrl;
  ctrl = readCtrl();
  ctrl &= ~(PCF2129_CONTROL_12_24);
  writeCtrl(ctrl);
}

////////////////////////////////////////////////////////////////

/**
 * @brief Conversion from BCD to DEC
 * @param  [in] value   : BCD Data
 * @return uint8_t : DEC Data
 */
uint8_t FaBoRTC_PCF2129::bcdToDec(uint8_t value) {
  return (uint8_t) ( (value/16*10) + (value%16) );
}

/**
 * @brief Conversion from BCD to DEC
 * @param  [in] value   : DEC Data
 * @return uint8_t : BCD Data
 */
uint8_t FaBoRTC_PCF2129::decToBcd(uint8_t value) {
  return (uint8_t) ( (value/10*16) + (value%10) );
}

/**
 * @brief Read I2C Data
 * @param [in] register_addr : register address
 * @return uint8_t : Read Data
 */
uint8_t FaBoRTC_PCF2129::readI2c(uint8_t address) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(address);
  Wire.endTransmission();
  Wire.requestFrom(_i2caddr, (uint8_t)1);
  while(!Wire.available());
  return Wire.read();
}

/**
 * @brief Write I2C Data
 * @param [in] address : Write Register Address
 * @param [in] data    : Write Data
 */
void FaBoRTC_PCF2129::writeI2c(uint8_t address, uint8_t data) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(address);
  Wire.write(data);
  Wire.endTransmission();
}

/**
 * @brief Read Control and Status Register1
 * @return data : Read Data
 */
uint8_t FaBoRTC_PCF2129::readCtrl(void) {
  return readI2c(PCF2129_CONTROL_REGISTERS);
}

/**
 * @brief Write Control and Status Register1
 * @param [in] data : Write Data
 */
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
