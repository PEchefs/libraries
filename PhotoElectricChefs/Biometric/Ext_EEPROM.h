/*
External EEPROM LIBRARY FOR 512KB
*/
#ifndef EXT_EEPROM_H
#define EXT_EEPROM_H
#include "Arduino.h"

#define MAX_EEPROM_SIZE 256000
#define EEPROM_ADDR 0x50
  void i2c_eeprom_write_byte( unsigned int eeaddress, byte data );
  void i2c_eeprom_write_page( unsigned int eeaddresspage, byte* data, byte length );
  byte i2c_eeprom_read_byte(  unsigned int eeaddress );
  void i2c_eeprom_read_buffer( unsigned int eeaddress, byte *buffer, int length );
  void i2c_eeprom_init(void);

#endif
