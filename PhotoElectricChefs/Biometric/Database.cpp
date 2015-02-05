 #include "Ext_EEPROM.h" //I2C library
 #include "Database.h";
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
		char reserver[2];
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
char database_isFormatted()
{
	byte status = i2c_eeprom_read_byte(0);
	if(status==0xAA)
		return 1;
	else
		return 0;
	
} 
char database_Format()
{
	int i=0;
	char zeros[30]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	byte temp=0xAA;
	for(i=0;i<(MAX_EEPROM_SIZE/30);i++)
	{
		i2c_eeprom_write_page(i,(byte *)zeros,sizeof(30));
		Serial.println(i);
		delay(10);
	}
	i2c_eeprom_write_byte(0,temp);
	delay(50);
	return database_isFormatted();
}
void database_getstats()
{
	int i=0;
	for(i=0;i<5;i++)
	{
		eepromStats.data[i]=i2c_eeprom_read_byte(i);
		delay(50);
	}
	Serial.print("Formatted : ");
	if(eepromStats.eeprom.isFormatted==0xAA)
		Serial.println("Yes");
	else
		Serial.println("No");
	Serial.print("User Count : ");Serial.println(eepromStats.eeprom.userCount);
	Serial.print("Log Count : ");Serial.println(eepromStats.eeprom.logCount);
}
int database_setemployee(byte *temp)
{
	int i=0;
	byte temp2[USER_DATA_LENGTH];
	int employee_start_addr=USER_DATA_START_ADDR;
	for(i=0;i<USER_DATA_LENGTH;i++)
	{
		employeeStats.data[i]=temp[i];
	}
	employee_start_addr=USER_DATA_START_ADDR+((employeeStats.employee.empSlNo-1)*USER_DATA_LENGTH);
	for(i=0;i<USER_DATA_LENGTH;i++)
	{
		i2c_eeprom_write_byte(employee_start_addr+i,temp[i]);
		delay(50);
	}
	database_getemployee(employeeStats.employee.empSlNo,temp2);
	for(i=0;i<USER_DATA_LENGTH;i++)
	{
		if(temp[i]!=temp2[i])
			return 0;
	}
	return 1;
	
}
int database_getemployee(int slNo,byte *emp_data)
{
	int employee_start_addr=USER_DATA_START_ADDR+((slNo-1)*USER_DATA_LENGTH);
	for(int i=0;i<USER_DATA_LENGTH;i++)
	{
		emp_data[i]=i2c_eeprom_read_byte(employee_start_addr+i);
		delay(50);
	}
}
int database_getemployeecount()
{
	int employee_start_addr=USER_DATA_START_ADDR;
	int user_count=0;
	for(int i=0;i<1000;i++)
	{
		if(i2c_eeprom_read_byte(employee_start_addr+(i*USER_DATA_LENGTH))>0)
			user_count++;
		else
			break;
		delay(50);
	}
	return user_count;
}
int database_setlog(byte *temp)
{
	int i=0;
	byte temp2[LOG_DATA_LENGTH];
	int log_start_addr=LOG_DATA_START_ADDR;
	for(i=0;i<LOG_DATA_LENGTH;i++)
	{
		logStats.data[i]=temp[i];
	}
	log_start_addr=LOG_DATA_START_ADDR+((logStats.log.logSlNo-1)*LOG_DATA_LENGTH);
	for(i=0;i<LOG_DATA_LENGTH;i++)
	{
		i2c_eeprom_write_byte(log_start_addr+i,temp[i]);
		delay(50);
	}
	database_getlog(logStats.log.logSlNo,temp2);
	for(i=0;i<LOG_DATA_LENGTH;i++)
	{
		if(temp[i]!=temp2[i])
			return 0;
	}
	return 1;
	
}
int database_getlog(int slNo,byte *log_data)
{
	int log_start_addr=LOG_DATA_START_ADDR+((slNo-1)*LOG_DATA_LENGTH);
	for(int i=0;i<LOG_DATA_LENGTH;i++)
	{
		log_data[i]=i2c_eeprom_read_byte(log_start_addr+i);
		delay(50);
	}
}
int database_getlogcount()
{
	int log_start_addr=LOG_DATA_START_ADDR;
	int log_count=0;
	for(int i=0;i<1000;i++)
	{
		if(i2c_eeprom_read_byte(log_start_addr+(i*LOG_DATA_LENGTH))>0)
			log_count++;
		else
			break;
		delay(50);
	}
	return log_count;
}