#include <AT24C256.h>
#include <Wire.h>

void setup()
{
  char somedata[] = "this is data from the eeprom"; // data to write
  static eeprom at1(0x50);
  Serial.begin(9600);
  at1.i2c_eeprom_write_page(0, (byte *)somedata, sizeof(somedata)); // write to EEPROM

  at1.i2c_eeprom_write_byte(100, (byte)0xab); // write to EEPROM
  at1.i2c_eeprom_write_byte(101, (byte)0xab); // write to EEPROM

  Serial.println("Memory written");
}

void loop()
{
  static eeprom at1(0x50);
  int addr = 0; //first address
  byte b = at1.i2c_eeprom_read_byte(0); // access the first address from the memory

  while (b != 0)
  {
    Serial.print((char)b); //print content to serial port
    addr++; //increase address
    b = at1.i2c_eeprom_read_byte(addr); //access an address from the memory
  }
  Serial.println(" ");
  
  int c = at1.i2c_eeprom_read_int16(100); // access the first address from the memory
  Serial.print((int)c,HEX); //print content to serial port

  //b = at1.i2c_eeprom_read_byte(101); // access the first address from the memory
  //Serial.print((int)b,HEX); //print content to serial port
  
  Serial.println(" ");
  delay(2000);
}
