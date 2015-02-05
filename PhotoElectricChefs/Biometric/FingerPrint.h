/*
External EEPROM LIBRARY FOR 512KB
*/
#ifndef FINGERPRINT_H
#define FINGERPRINT_H
#include "Arduino.h"

uint8_t fingerprint_init();
uint8_t fingerprint_check();
int getFingerprintIDez();
uint8_t deleteFingerprint(uint8_t id);
uint8_t uploadFingerpintTemplate(uint8_t id);
uint8_t uploadtemp(uint8_t id);
void showtemplate(uint8_t id);
void image();
void tempnumber();
uint8_t getFingerprintID() ;
uint8_t getFingerprintEnroll1();
uint8_t getFingerprintEnroll2();
uint8_t storeFingerprint(uint8_t id);
#endif
