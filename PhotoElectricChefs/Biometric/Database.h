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
#define USER_DATA_START_ADDR 5000
#define LOG_DATA_START_ADDR  70000
 union
	{    
	   struct eepromStruct
	   {
		byte isFormatted;
		unsigned int  userCount;
		unsigned int  logCount;
	   }eeprom;
	   byte  data[5];
	} eepromStats;

union
	{    
	   struct employeeStruct
	   {
		int empSlNo;
		char empId[12];
		char empRfid[12];
		char empName[20];
		int  empFid;
		char empEntryType;
		long empInTime;
		long empOutTime;
		char empMode;
		char reserved[2];
	   }employee;
	    byte data[USER_DATA_LENGTH];
	} employeeStats;
union
	{    
	   struct logStruct
	   {
		long logSlNo;
		char empId[12];
		char empMode;
		long logTime;
	   }log;
	    byte data[LOG_DATA_LENGTH];
	} logStats;

char database_isFormatted(void);
char database_Format(void);
void database_getstats();
int database_setemployee(byte[]);  //send byte array as described under employee struct
int database_getemployee(int,byte[]); // send an empty byte array of size USER_DATA_LENGTH and the user id
int database_getemployee_byfid(int,byte[]);//send an empty byte array which gets filled with the result of the search. Parameter for search is the 1 byte fingerprint id
int database_getemployee_byrfid(char[],byte[]);//send an empty byte array which gets filled with the result of the search. Parameter for search is the 12byte RFID char array
int database_getemployeecount(); 
int database_setlog(byte); //send byte array as described under log struct
int database_getlog(int,byte[]); // send an empty byte array of size LOG_DATA_LENGTH and the log id
int database_getlogcount();
#endif
