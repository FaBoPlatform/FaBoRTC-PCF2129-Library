/**
 * @file  FaBoRTC_PCF2129.h
 * @brief fabo libtary of PCF2129
 * @author Akira Sasaki
 * @date 2,10, 2016
 */
#ifndef FABORTC_PCF2129_H
  #define FABORTC_PCF2129_H

  #include <Arduino.h>
  #include <Wire.h>

  /** SLAVE_ADDRESS */
  #define PCF2129_SLAVE_ADDRESS 0x51

  /** Cntrol Register */
  #define PCF2129_CONTROL_REGISTERS 0x00
  /** Mode Cntrol Register : 12-hour mode or 24-hour mode */
  #define PCF2129_CONTROL_12_24 0x04

  /** Seconds Register */
  #define PCF2129_SECONDS 0x03
  /** Minutes Register */
  #define PCF2129_MINUTES 0x04
  /** Hours Register */
  #define PCF2129_HOURS 0x05
  /** Days Register */
  #define PCF2129_DAYS 0x06
  /** Weekdays Register */
  #define PCF2129_WEEKDAYS 0x07
  /** Months Register */
  #define PCF2129_MONTHS 0x08
  /** Years Register */
  #define PCF2129_YEARS 0x09

  /**
   * @class DateTime
   * @brief Get Date Time
   */
  class DateTime {
    public:
      /** Date Time */
      DateTime (uint16_t years, uint8_t months, uint8_t days,
                uint8_t hours, uint8_t minutes, uint8_t seconds);
      /**
       * @brief Return Year
       * @return uint16_t : year + 2000
       */
      uint16_t year()  const { return y+2000; }
      /**
       * @brief Return Month
       * @return uint8_t : month
       */
      uint8_t month()  const { return m; }
      /**
       * @brief Return day
       * @return uint8_t : day
       */
      uint8_t day()    const { return d; }
      /**
       * @brief Return hour
       * @return uint8_t : hour
       */
      uint8_t hour()   const { return hh; }
      /**
       * @brief Return minute
       * @return uint8_t : minute
       */
      uint8_t minute() const { return mm; }
      /**
       * @brief Return Second
       * @return uint8_t : second
       */
      uint8_t second() const { return ss; }
    protected:
      uint8_t y,m,d,hh,mm,ss;
  };

  /**
   * @class FaBoRTC_PCF2129
   * @brief PCF2129 Control
   */
  class FaBoRTC_PCF2129 {
    public:
      FaBoRTC_PCF2129(uint8_t addr = PCF2129_SLAVE_ADDRESS);
      bool searchDevice(void);
      void configure(void);
      uint8_t getSeconds(void);
      void setSeconds(uint8_t seconds);
      uint8_t getMinutes(void);
      void setMinutes(uint8_t minutes);
      uint8_t getHours(void);
      void setHours(uint8_t hours);
      uint8_t getDays(void);
      void setDays(uint8_t days);
      uint8_t getWeekdays(void);
      void setWeekdays(uint8_t weekdays);
      uint8_t getMonths(void);
      void setMonths(uint8_t months);
      uint8_t getYears(void);
      void setYears(uint8_t years);
      DateTime now(void);
      void setDate(uint16_t years, uint8_t months, uint8_t days,
                   uint8_t hours, uint8_t minutes, uint8_t seconds);
      void set12mode(void);
      void set24mode(void);
    private:
      uint8_t _i2caddr;

      uint8_t bcdToDec(uint8_t value);
      uint8_t decToBcd(uint8_t value);
      uint8_t readI2c(uint8_t address);
      void writeI2c(uint8_t address, uint8_t data);

      uint8_t readCtrl(void);
      void writeCtrl(uint8_t data);
  };

#endif // FABORTC_PCF2129_H
