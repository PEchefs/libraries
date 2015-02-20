 #include "Ext_EEPROM.h" //I2C library
 #include "Database.h";
 /***************************USAGE*******************/
 /*
	union eepromStats - Used to get and store user count and log count and also if the eeprom is formatted.
					  - Call database_isFormatted() to get the format status
					  //TODO set usercount and logcount into the memory!
	union employeeStats - Used to get the employee data into a structure within.
	struct employeeStruct - SlNo - 2 bytes
						  - employeeId - 12 bytes;
						  - employeeRfid -12 bytes;
						  - employeeName - 20 bytes;
						  - employeeFingerprintID - 2 bytes
						  - employeeEntryType - 1 byte - denotes if only Fingerprint or with card and if admin or not
						  - employeeInTime - 4 bytes unixtime referring to allowance for entry (start)
						  - employeeOutTime - 4 bytes unixtime referring to allowance for entry (end)
						  - employeeMode - 1 byte referring to whether the above time is taken for consideration
						  - reserved - 2 bytes reserved for future use
	union logStats - Used to get log data into a structure within
	struct logStats - SlNo -2 bytes
					- employeeId - 12 bytes
					- employeeMode - 1 bytes denotes if only Fingerprint or with card and if admin or not
					- LogTime - 4 bytes unixtime referring to log entry time
 */
 /*****************************************************/
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
int database_getemployee_byfid(int fid,byte *emp_data)
{
	for(int j=0;j<1000;j++)
	{
		int employee_start_addr=USER_DATA_START_ADDR+(j*USER_DATA_LENGTH);
		int temp_fid=i2c_eeprom_read_byte(employee_start_addr+45);
		delay(1);
		if(temp_fid==fid)
		{
			for(int i=0;i<USER_DATA_LENGTH;i++)
			{
				emp_data[i]=i2c_eeprom_read_byte(employee_start_addr+i);
				delay(2);
			}
			return 1;
		}
	}
	return 0;
}
int database_getemployee_byrfid(char rfid[12],byte *emp_data)
{
	char found=0;
	for(int j=0;j<1000;j++)
	{
		int employee_start_addr=USER_DATA_START_ADDR+(j*USER_DATA_LENGTH);
		char temp_rfid[12];
		for(int i=13;i<25;i++)
		{
			temp_rfid[i-13]=i2c_eeprom_read_byte(employee_start_addr+i);
			delay(1);
		}
		for(int i=0;i<12;i++)
		{
			if(temp_rfid[i]!=rfid[i])
			{
				found=0;
				break;
			}
			else
				found=1;
		}
		if(found==1)
		{
			for(int i=0;i<USER_DATA_LENGTH;i++)
			{
				emp_data[i]=i2c_eeprom_read_byte(employee_start_addr+i);
				delay(2);
			}
			return 1;
		}
	}
	return 0;
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