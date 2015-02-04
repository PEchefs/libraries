/*
External EEPROM DATABASE FOR USER AND LOG
*/
#ifndef DATABASE_H
#define DATABASE_H
#include "Arduino.h"

#define MAX_USERS 1000
#define MAX_LOGS  10000
#define USER_DATA_LENGTH 60
#define LOG_DATA_LENGTH 21
#define USER_DATA_START_ADDR 100
#define LOG_DATA_START_ADDR  70000

char database_isFormatted(void);
char database_Format(void);
void database_getstats();
int database_setemployee(byte[]);
int database_getemployee(int,byte[]);
int database_getemployeecount();
#endif
