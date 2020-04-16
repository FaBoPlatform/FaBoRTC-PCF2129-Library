#include "RTC_PCF2129.h"

/**
 @brief Constructor
*/
RTC_PCF2129::RTC_PCF2129(uint8_t addr) {
  _i2caddr = addr;
  Wire.begin();
}

bool RTC_PCF2129::searchDevice(void) {
  return !(readI2c(_i2caddr) >>7);
}


void RTC_PCF2129::configure(void) {
  set24mode();
}

bool RTC_PCF2129::IsDateTimeValid(void)
{
	uint8_t status = readI2c(PCF2129_SECONDS);
	return !(status & _BV(PCF2129_OSF));
}

uint8_t RTC_PCF2129::LastError(void)
{
	return _lastError;
}

DateTime RTC_PCF2129::now(void) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(PCF2129_SECONDS);
  _lastError = Wire.endTransmission();
  Wire.requestFrom(_i2caddr, (uint8_t)7);
  while(!Wire.available());
  uint8_t seconds = bcdToDec(Wire.read());
  uint8_t minutes = bcdToDec(Wire.read());
  uint8_t hours = bcdToDec(Wire.read());
  uint8_t days = bcdToDec(Wire.read());
  uint8_t weekdays = bcdToDec(Wire.read());
  uint8_t months = bcdToDec(Wire.read());
  uint16_t years = bcdToDec(Wire.read()) +2000;

  return DateTime(years, months, weekdays, days, hours, minutes, seconds);
}

/**
 @brief Set to RTC
 @param [in] DateTime DateTime 
*/
void RTC_PCF2129::SetDateTime(const DateTime& dt) {
  uint8_t status = readI2c(PCF2129_SECONDS);
  status &= ~_BV(PCF2129_OSF); // clear the flag
  writeI2c(PCF2129_SECONDS, status);

  Wire.beginTransmission(_i2caddr);
  Wire.write(PCF2129_SECONDS);
  Wire.write(decToBcd(dt.second()) + 0x80);
  Wire.write(decToBcd(dt.minute()));
  Wire.write(decToBcd(dt.hour()));
  Wire.write(decToBcd(dt.day()));
  Wire.write(decToBcd(dt.week()));
  Wire.write(decToBcd(dt.month()));
  Wire.write(decToBcd(dt.year()-2000));
  _lastError = Wire.endTransmission();
}

void RTC_PCF2129::set12mode(void) {
  uint8_t ctrl;
  ctrl = readCtrl();
  ctrl |= PCF2129_CONTROL_12_24;
  writeCtrl(ctrl);
}

/**
 @brief Set to 24 hour mode
*/
void RTC_PCF2129::set24mode(void) {
  uint8_t ctrl;
  ctrl = readCtrl();
  ctrl &= ~(PCF2129_CONTROL_12_24);
  writeCtrl(ctrl);
}

////////////////////////////////////////////////////////////////

/**
 @brief Read I2C Data
 @param [in] address register address 
 @param [out] data read data 
*/
uint8_t RTC_PCF2129::readI2c(uint8_t address) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(address);
  _lastError = Wire.endTransmission();
  Wire.requestFrom(_i2caddr, (uint8_t)1);
  while(!Wire.available());
  return Wire.read();
}

/**
 @brief Write I2C Data
 @param [in] address register address 
 @param [in] data write data 
*/
void RTC_PCF2129::writeI2c(uint8_t address, uint8_t data) {
  Wire.beginTransmission(_i2caddr);
  Wire.write(address);
  Wire.write(data);
  _lastError = Wire.endTransmission();
}

/**
 @brief Read Control Register
 @param [out] data register data 
*/
uint8_t RTC_PCF2129::readCtrl(void) {
  return readI2c(PCF2129_CONTROL_REGISTERS);
}

/**
 @brief Write Control Register
 @param [in] data register data 
*/
void RTC_PCF2129::writeCtrl(uint8_t data) {
  writeI2c(PCF2129_CONTROL_REGISTERS, data);
}

////////////////////////////////////////////////////////////////
