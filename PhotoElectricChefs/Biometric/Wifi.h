/*
WIFI 
*/
#ifndef WIFI_H
#define WIFI_H
#include "Arduino.h"
#include <SoftwareSerial.h>

bool wifi_Init();
bool wifi_Poll();
bool wifi_GetEmployee(int,byte*);
bool wifi_sendLog(byte*);
bool wifi_sendtemplate(long ,byte*);
#endif
