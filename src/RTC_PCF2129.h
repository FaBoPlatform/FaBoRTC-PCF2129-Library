#ifndef RTC_PCF2129_H
#define RTC_PCF2129_H

#include <Arduino.h>
#include <Wire.h>

#include "RtcDateTime.h"
#include "RtcUtility.h"

#define PCF2129_SLAVE_ADDRESS 0x51 ///< I2C Slave Address

#define PCF2129_CONTROL_REGISTERS 0x00
#define PCF2129_CONTROL_12_24 0x04
#define PCF2129_SECONDS 0x03
#define PCF2129_MINUTES 0x04
#define PCF2129_HOURS 0x05
#define PCF2129_DAYS 0x06
#define PCF2129_WEEKDAYS 0x07
#define PCF2129_MONTHS 0x08
#define PCF2129_YEARS 0x09

#define PCF2129_OSF 7

class RTC_PCF2129
{
public:
  RTC_PCF2129(uint8_t addr = PCF2129_SLAVE_ADDRESS);
  bool searchDevice(void);
  void configure(void);
  bool IsDateTimeValid(void);
  DateTime now(void);
  void SetDateTime(const DateTime &dt);
  void UpdateRTCbyTS(uint32_t timeTS);
  void set12mode(void);
  void set24mode(void);
  uint8_t LastError(void);

private:
  uint8_t _lastError;
  uint8_t _i2caddr;
  uint8_t readI2c(uint8_t address);
  void writeI2c(uint8_t address, uint8_t data);
  uint8_t readCtrl(void);
  void writeCtrl(uint8_t data);
};

#endif // RTC_PCF2129_H