
/*
 *  Use the I2C bus with EEPROM 24LC64
 *  Sketch:    eeprom.pde
 *  AT24C256.h
 *
 *  Author: hkhijhe
 *  Date: 01/10/2010
 *
 *
 */
#include <Wire.h> //I2C library
#include <Arduino.h>

#define WRITE_DELAY 1
class eeprom
{
private:
  int deviceaddress;
public:
  eeprom(int defined_deviceaddress)
  {
    deviceaddress = defined_deviceaddress;
    Wire.begin(); // initialise the connection
  }
  void i2c_eeprom_write_byte(unsigned int eeaddress, byte data);
  void i2c_eeprom_write_page(unsigned int eeaddresspage, byte* data, byte length);
  byte i2c_eeprom_read_byte(unsigned int eeaddress);
  int i2c_eeprom_read_int16(unsigned int eeaddress);
  void i2c_eeprom_read_buffer(unsigned int eeaddress, byte *buffer, int length);
};

void eeprom::i2c_eeprom_write_byte(unsigned int eeaddress, byte data)
{
  int rdata = data;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.write(rdata);
  Wire.endTransmission();
  delay(WRITE_DELAY);
}

// WARNING: address is a page address, 6-bit end will wrap around
// also, data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
void eeprom::i2c_eeprom_write_page(unsigned int eeaddresspage, byte* data, byte length)
{
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddresspage >> 8)); // MSB
  Wire.write((int)(eeaddresspage & 0xFF)); // LSB
  byte c;
  for ( c = 0; c < length; c++)
    Wire.write(data[c]);
  Wire.endTransmission();
  delay(WRITE_DELAY);
}

byte eeprom::i2c_eeprom_read_byte(unsigned int eeaddress)
{
  byte rdata = 0xFF;
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress, 1);
  if (Wire.available()) rdata = Wire.read();
  return rdata;
}
int eeprom::i2c_eeprom_read_int16(unsigned int eeaddress)
{
  byte byteMSB = i2c_eeprom_read_byte(eeaddress);
  byte byteLSB = i2c_eeprom_read_byte(eeaddress+1);
  int rdata;
  rdata = ( (int)byteMSB << 8 ) + (int)byteLSB;
  return rdata;
}

// maybe let's not read more than 30 or 32 bytes at a time!
void eeprom::i2c_eeprom_read_buffer(unsigned int eeaddress, byte *buffer, int length)
{
  Wire.beginTransmission(deviceaddress);
  Wire.write((int)(eeaddress >> 8)); // MSB
  Wire.write((int)(eeaddress & 0xFF)); // LSB
  Wire.endTransmission();
  Wire.requestFrom(deviceaddress, length);
  int c = 0;
  for ( c = 0; c < length; c++ )
  {
    if (Wire.available()) buffer[c] = Wire.read();
  }
}
