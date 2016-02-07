#include "Arduino.h"
#include "Wire.h"

#define MPL115_SLAVE_ADDRESS (0x60)

#define MPL115_PADC_MSB_REG 0x00
#define MPL115_TADC_MSB_REG 0x02
#define MPL115_A0_MSB_REG 0x04

class mpl115
{
public:
  int readPADC(void);
  int readTADC(void);
  void readCoefficient(uint16_t *a0, uint16_t *b1, uint16_t *b2, uint16_t *c12);   
  float readPressure();
private:
  void readI2c(byte register_addr, int num, byte *buf);
  void writeI2c(byte register_addr, byte value);
};

extern mpl115 faboBarometer;
