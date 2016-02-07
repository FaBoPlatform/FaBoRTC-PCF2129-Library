#include "fabo-mpl115.h"

float mpl115::readPressure(){
  int padc = readPADC();
  int tadc = readTADC();

  uint16_t a0, b1, b2, c12;
  readCoefficient(&a0, &b1, &b2, &c12);
  
  float pcomp = a0 + (b1 + c12 * tadc) * padc + b2 * tadc;
  float pressure = pcomp*((float)(115-50)/1023) + 50;

  return pressure;
}

int mpl115::readPADC()
{
  byte buffer[2];
  readI2c(MPL115_PADC_MSB_REG, 2, buffer); 
  int pressure_value = buffer[0]<<2;
  pressure_value |= buffer[1];

  return pressure_value;
}

int mpl115::readTADC()
{
  byte buffer[2];
  readI2c(MPL115_TADC_MSB_REG, 2, buffer); 
  int temperature_value = buffer[0]<<2;
  temperature_value |= buffer[1];

  return temperature_value;
}

void mpl115::readCoefficient(uint16_t *a0, uint16_t *b1, uint16_t *b2, uint16_t *c12)
{
  byte buffer[8];
  readI2c(MPL115_A0_MSB_REG, 8, buffer); 
  *a0 = (((int)buffer[0]) << 8) | buffer[1];
  *b1 = (((int)buffer[2]) << 8) | buffer[3];
  *b2 = (((int)buffer[4]) << 8) | buffer[5];
  *c12 = (((int)buffer[6]) << 8) | buffer[7];
}

// I2Cへの書き込み
void mpl115::writeI2c(byte register_addr, byte value) {
  Wire.begin();       // I2Cの開始
  Wire.beginTransmission(MPL115_SLAVE_ADDRESS);  
  Wire.write(register_addr);         
  Wire.write(value);                 
  Wire.endTransmission();        
}

// I2Cへの読み込み
void mpl115::readI2c(byte register_addr, int num, byte buffer[]) {
  Wire.begin();       // I2Cの開始
  Wire.beginTransmission(MPL115_SLAVE_ADDRESS); 
  Wire.write(register_addr);           
  Wire.endTransmission();         

  Wire.beginTransmission(MPL115_SLAVE_ADDRESS); 
  Wire.requestFrom(MPL115_SLAVE_ADDRESS, num);   

  int i = 0;
  while(Wire.available())        
  { 
    buffer[i] = Wire.read();   
    i++;
  }
  Wire.endTransmission();         
}

mpl115 faboBarometer;
