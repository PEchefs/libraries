 #include <Wire.h> //I2C library
 #include <Ext_EEPROM.h> //I2C library

  void i2c_eeprom_init()
  {
	Wire.begin(); // initialise the connection
  }
  void i2c_eeprom_write_byte( unsigned int eeaddress, byte data ) {
    byte rdata = data;
    Wire.beginTransmission(EEPROM_ADDR);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.write(rdata);
    Wire.endTransmission();
  }

  // WARNING: address is a page address, 6-bit end will wrap around
  // also, data can be maximum of about 30 bytes, because the Wire library has a buffer of 32 bytes
  void i2c_eeprom_write_page( unsigned int eeaddresspage, byte* data, byte length ) {
    Wire.beginTransmission(EEPROM_ADDR);
    Wire.write((int)(eeaddresspage >> 8)); // MSB
    Wire.write((int)(eeaddresspage & 0xFF)); // LSB
    byte c;
    for ( c = 0; c < length; c++)
      Wire.write(data[c]);
    Wire.endTransmission();
  }

  byte i2c_eeprom_read_byte( unsigned int eeaddress ) {
    byte rdata = 0xFF;
    Wire.beginTransmission(EEPROM_ADDR);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(EEPROM_ADDR,1);
    if (Wire.available()) rdata = Wire.read();
    return rdata;
  }

  // maybe let's not read more than 30 or 32 bytes at a time!
  void i2c_eeprom_read_buffer(unsigned int eeaddress, byte *buffer, int length ) {
    Wire.beginTransmission(EEPROM_ADDR);
    Wire.write((int)(eeaddress >> 8)); // MSB
    Wire.write((int)(eeaddress & 0xFF)); // LSB
    Wire.endTransmission();
    Wire.requestFrom(EEPROM_ADDR,length);
    int c = 0;
    for ( c = 0; c < length; c++ )
      if (Wire.available()) buffer[c] = Wire.read();
  }
