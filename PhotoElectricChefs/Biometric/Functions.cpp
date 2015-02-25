/*

Function pointers array index:

0  -  Menu -> Delete
1  -  Menu -> Set Time
2  -  Menu -> Enroll -> Enroll User -> FingerPrint
3  -  Menu -> Enroll -> Enroll User -> Password
4  -  Menu -> Enroll -> Enroll User -> Card
5  -  Menu -> Enroll -> Enroll Admin -> FingerPrint
6  -  Menu -> Enroll -> Enroll Admin -> Password
7  -  Menu -> Enroll -> Enroll Admin -> Card
8  -  Menu -> Settings -> Self Test
9  -  Menu -> Settings -> Factory Reset
10 -  Menu -> Settings -> Access -> Attend Set
11 -  Menu -> Settings -> Access -> Open Time
12 -  Menu -> Settings -> Access -> Open TimeOut
13 -  Menu -> Settings -> Access -> Door Sensor
14 -  Menu -> Settings -> Access -> Bell Time
15 -  Menu -> Settings -> Access -> VerifyMode

16 -  Menu -> Settings -> Information -> User
17 -  Menu -> Settings -> Information -> Manager
18 -  Menu -> Settings -> Information -> Finger
19 -  Menu -> Settings -> Information -> DuressFP
20 -  Menu -> Settings -> Information -> Password
21 -  Menu -> Settings -> Information -> Card
22 -  Menu -> Settings -> Information -> Log

Command To Slave:
0x30 0x31 - Check Fingerprint
0x30 0x32 - Enroll Fingerprint1
0x30 0x33 - Enroll Fingerprint2
0x30 0x34 - STORE Fingerprint

0x31 0x32 - Enroll RFID
0x30 0x35 - Delete Fingerprint
0x31 0x34 - Delete RFID
0x32 0x35 - Open Door
0x32 0x36 - Close Door
0x32 0x37 - Trigger Buzzer
0x32 0x38 - Poll
0x32 0x39 - Check Key Press
0x32 0x3A - 

Response Codes:
0x40 0x40 - Success - for enroll, delete
0x40 0x41 - No Fingerprint Found for Enroll	/ Fingerprint sensor not found							//Failed, User FP exits
0x40 0x42 - Failed, User RFID exists
0x40 0x43 - Failed, User Does not exist
0x40 0x44 - Fingerprint match - for Poll
0x40 0x45 - Fingerprint detected, No match found - Poll
0x40 0x46 - Wait, Under processing
0x40 0x47 - RFID flashed - for Poll
0x40 0x48 - No FP or RFID detected
0x40 0x49 - Key pressed
0x40 0x4A - No Key pressed



*/
#include <Arduino.h>
#include <Functions.h>
#include <Wire.h>
#include <Wifi.h>
#include "Database.h";
#define DEBUG 0
#define DEBUG1 1
#define DEBUGWOWIRE 0

unsigned long serialInputNumber;
boolean  serialInputNumberReceived=false;
boolean enrollComplete=false;

char enrollFail[]="Enroll Failed!";
char retry[]="Please retry!";


void longTobyteArray(unsigned long longToConvert, byte *byteArray)
{
	//Serial.print("Long to convert:");Serial.println(longToConvert,DEC);
	byteArray[3] = (int)((longToConvert >> 24) & 0xFF) ;
	byteArray[2] = (int)((longToConvert >> 16) & 0xFF) ;
	byteArray[1] = (int)((longToConvert >> 8) & 0XFF);
	byteArray[0] = (int)((longToConvert & 0XFF));
	//Serial.print("byteArray[0]=");Serial.println(byteArray[0],DEC);
	//Serial.print("byteArray[1]=");Serial.println(byteArray[1],DEC);
	//Serial.print("byteArray[2]=");Serial.println(byteArray[2],DEC);
	//Serial.print("byteArray[3]=");Serial.println(byteArray[3],DEC);
	//Serial.print("ID converted(in loop): ");Serial.write(byteArray,12);Serial.println("");
}

union 
{
	struct 
	{
		//const byte header[2]={100,100}; // Hex: 2710
		byte header[2]; // Hex: 2710
		byte command[2]; 	
		byte data[12];
	};
	byte commandToSlave[16];
} commandToSlaveUnion;			

union 
{
	struct 
	{
		//const byte header[2]={100,100}; // Hex: 2710
		byte header[2]; // Hex: 2710
		byte responseCode[2];
		byte data[12];
	};
	byte responseFromSlave[16];
} responseFromSlaveUnion;		

unsigned short readFromSlave()
{
  if(!DEBUGWOWIRE)
  {
	if(DEBUG)
		Serial.println("Requesting info from Slave");
    unsigned short j=0;
	boolean validResponseReceived=false;
  do
  {
	j++;
	responseFromSlaveUnion.responseCode[0]=0;
	responseFromSlaveUnion.responseCode[1]=0;
	if(DEBUG)
		Serial.println("Requesting response from Slave");
	Wire.requestFrom(SLAVEADDRESS, 16);    // request 16 bytes from slave device #2
 // if(DEBUG)
//	Serial.println("Request for 16 bytes sent to Slave");
  //Serial.println("Received Data:");
  unsigned short i=0;
  unsigned long time1=millis();
  unsigned long time2=0;
  while(Wire.available()!=16)
  {
  if(DEBUG)
		{
			Serial.print("Response bytes: ");Serial.println(Wire.available());
		}
	if(DEBUG)
		Serial.println("Waiting for response from slave");
		//delay(100);
		delay(10);
	time2=millis();
	if(time2-time1>10000)
		{
			if(DEBUG)
				Serial.println("Time Out on wire receive");
			//	break;
			return 0;
		}
  }
  //Serial.println("Received:");
  delay(10);

	

	//while (Wire.available())   // slave may send less than requested
	for(i=0;i<16;i++)
	{
		responseFromSlaveUnion.responseFromSlave[i]=Wire.read();
//		Serial.print(responseFromSlaveUnion.responseFromSlave[i]);
 	//   char c = Wire.read(); // receive a byte as character
 	//   Serial.print(c);         // print the character
		//i++;
	}
	while (Wire.available())
	{
		Wire.read(); //Flush any unwanted wire bytes received.
	}
	if(responseFromSlaveUnion.responseCode[0]==0x40 && responseFromSlaveUnion.responseCode[1]!=0x46)
			validResponseReceived=true;
	else if(responseFromSlaveUnion.responseCode[0]==0x40 && responseFromSlaveUnion.responseCode[1]==0x46)
			//delay(500);
			delay(50);
	if(DEBUG)
	{
		Serial.print("responseFromSlaveUnion.responseCode[0]: ");Serial.print(responseFromSlaveUnion.responseCode[0],DEC);
		Serial.print("responseFromSlaveUnion.responseCode[1]: ");Serial.print(responseFromSlaveUnion.responseCode[1],DEC);
	}
/*	if(i==0)
		if(DEBUG)
			Serial.println("Failed Enroll! No response received");
		//TODO : Add LCD call to display failure
	else if (i!=16)
		if(DEBUG)
			Serial.println("Failed Enroll! 16 bytes not received");
		//TODO : Add LCD call to display failure
		*/
	//delay(200);
	delay(20);
	}
  while((j<200) && !validResponseReceived);
  if(!validResponseReceived || responseFromSlaveUnion.responseCode[1]==0x41 || responseFromSlaveUnion.responseCode[1]==0x42 || responseFromSlaveUnion.responseCode[1]==0x43)
	return 0;
	
  return 1;
  }
}


void writeToSlave()
{
	if(DEBUG)
		Serial.println("Sending command to Slave");
  if(!DEBUGWOWIRE)
  {
	commandToSlaveUnion.header[0]=100;
	commandToSlaveUnion.header[1]=100;
    Wire.beginTransmission(SLAVEADDRESS); // transmit to device SLAVEADDRESS
//	Serial.write(commandToSlaveUnion.commandToSlave,16);
    Wire.write(commandToSlaveUnion.commandToSlave,16);        // sends 16 bytes
	Wire.endTransmission();    // stop transmitting
  delay(10);
  }
}

void deleteUser()
{
  
}

void setTimeFunction()  
{
  
}

unsigned short enroll(unsigned short userType, unsigned short authType)
{
	serialInputNumberReceived=false;
// UserType: USER: 0, ADMIN: 1
// AuthType to Enroll: FingerPrint: 0, Password: 1, Card: 2
	displayMessage("Enter ID..");
	Serial.println("Enter ID");
	while (!serialInputNumberReceived)
	{
		if(DEBUG)
			{
				Serial.println("Waiting for serial input number");
				delay(1000);
			}
		receiveSerialInputNumber();
	}
/*	if(authType=0)
		FPID=serialInputNumber;
	else if(authType=1)
		RFID=serialInputNumber;
*/
	if(DEBUG)
	{
		Serial.print("ID received:");
		Serial.println(serialInputNumber,DEC);
	}
	
	// Set command parameters for enroll in I2C communication union.
	// Call I2C function to instruct the other controller to enroll
	//0x30 0x31 - Check Fingerprint
	//0x30 0x32 - Enroll Fingerprint1
	//0x30 0x33 - Enroll Fingerprint2
	//0x30 0x34 - STORE Fingerprint

	//0x31 0x32 - Enroll RFID
	/*if(DEBUG)
	{
		Serial.print("ID received (just before authType condition:");
		Serial.println(serialInputNumber,DEC);
	}*/
	if(authType==0)
		{
	/*		if(DEBUG)
	{
		Serial.print("ID received (inside authType condition 1:");
		Serial.println(serialInputNumber,DEC);
	}*/
		displayMessage("Please Wait...");
		unsigned short retCode=0;
		



			if(DEBUG)
				Serial.println("Auth Type = Fingerprint");
		// 0x30 0x31 - Check Fingerprint
		commandToSlaveUnion.command[0]=0x30;
		commandToSlaveUnion.command[1]=0x31;
		for(unsigned short i=0;i<12;i++)
			commandToSlaveUnion.data[i]=0;
		//longTobyteArray(serialInputNumber,commandToSlaveUnion.data);
		writeToSlave();
		delay(100);
		/*if(DEBUG)
	{
		Serial.print("ID received (betw write and read:");
		Serial.println(serialInputNumber,DEC);
	}*/
		retCode=readFromSlave();
		if(retCode==0)
			//No valid response - No fingerprint found
			{
			displayMessage2(enrollFail,retry);
			delay(2500);
			return -1;
			}
	/*	else if(retCode==0)
			// Timeout - No fingerprint found
			{
			////displayMessage(1);
			displayMessage2(enrollFail,retry);
			return -1;
			}*/
	/*		if(DEBUG)
	{
		Serial.print("ID received (inside authType condition 1:");
		Serial.println(serialInputNumber,DEC);
	}*/
		//0x30 0x32 - Enroll Fingerprint1
		displayMessage("Press Finger!");
		commandToSlaveUnion.command[0]=0x30;
		commandToSlaveUnion.command[1]=0x32;
		for(unsigned short i=0;i<12;i++)
			commandToSlaveUnion.data[i]=0;
		writeToSlave();
		retCode=readFromSlave();
		if(retCode==0)
			//No valid response - Finger not pressed
			{
			////displayMessage(2);
			displayMessage2(enrollFail,retry);
			delay(2500);
			return -1;
			}
	/*	else if(retCode==0)
			// Timeout - Finger not pressed
			{
			////displayMessage(3);
			return -1;
			}*/
		/*if(DEBUG)
	{
		Serial.print("ID received (inside authType condition 1:");
		Serial.println(serialInputNumber,DEC);
	}*/
		//0x30 0x33 - Enroll Fingerprint2
		displayMessage2("Press Finger","Again!");
		commandToSlaveUnion.command[0]=0x30;
		commandToSlaveUnion.command[1]=0x33;
		for(unsigned short i=0;i<12;i++)
			commandToSlaveUnion.data[i]=0;
		writeToSlave();
		retCode=readFromSlave();
		if(retCode==0)
			//No valid response - Fingerprints do not match
			{
			////displayMessage(4);
			displayMessage2(enrollFail,retry);
			delay(2500);
			return -1;
			}
		/*else if(retCode==0)
			// Timeout - Finger not pressed
			{
			////displayMessage(5);
			return -1;
			}*/
		/*if(DEBUG)
	{
		Serial.print("ID received (inside authType condition 1:");
		Serial.println(serialInputNumber,DEC);
	}*/
		//0x30 0x34 - STORE Fingerprint
		displayMessage("Enrolling...");
		commandToSlaveUnion.command[0]=0x30;
		commandToSlaveUnion.command[1]=0x34;
	/*		if(DEBUG)
	{
		Serial.print("SerialInputNumber before longToByte:");
		Serial.println(serialInputNumber,DEC);
	}*/
	
		longTobyteArray(serialInputNumber,commandToSlaveUnion.data);
			writeToSlave();
		retCode=readFromSlave();
		if(retCode==0)
			//No valid response - Enroll Failed. 
			{
			//////displayMessage(6);
			displayMessage2(enrollFail,retry);
			delay(2500);
			return -1;
			}
		/*else if(retCode==0)
			// Timeout - Could not enroll!. Unknown error.
			{
			////displayMessage(7);
			return -1;		
			}*/
		}
		
	else if(authType==1)
		{
		// Enroll RFID
		commandToSlaveUnion.command[1]=0x32;
		longTobyteArray(serialInputNumber,commandToSlaveUnion.data);
		writeToSlave();
		readFromSlave();
		}
	
	if(DEBUG1)
		enrollComplete=true;
	while(!enrollComplete)
	{
		// wait for enroll to complete from the other controller 
	}
	displayMessage("Enroll Successful!");
	
	if(DEBUG)
		Serial.println("Enroll Complete");
	delay(3000);
}

boolean selfTest()
{
	return true;
}

void factoryReset()
{

}

void setAttendance()
{

}

void setOpenTime()
{

}

void setOpenTimeOut()
{

}

void setDoorSensor()
{

}

void setBellTime()
{

}

void setVerifyMode()
{

}

void infoUser()
{

}

void infoManager()
{

}

void infoFinger()
{

}

void infoDuressFP()
{

}

void infoPassword()
{

}

void infoCard()
{

}

void infoLog()
{

}

void receiveSerialInputNumber()
{
	
//	if(DEBUG)
//		Serial.println("receiveSerialInputNumber function called");
    serialInputNumber=0;
	serialInputNumberReceived=false;
    unsigned short count=0;
    char inchar[10]="";
    while (Serial.available()>0) 
    {
 //       if(DEBUG)
 //         Serial.println("Serial Input Number received");
        inchar[count]=char(Serial.read());
        count++;  
		serialInputNumberReceived=true;		
    }
/*	while (mySerial.available()>0) 
    {
 //       if(DEBUG)
 //         Serial.println("Serial Input Number received");
        inchar[count]=char(mySerial.read());
        count++;  
		serialInputNumberReceived=true;		
    }
    serialInputNumber=strtoul(inchar,NULL,0);
*/
}

void poll()
{
	char tmpStr[4]="";
	commandToSlaveUnion.command[0]=0x30;
	commandToSlaveUnion.command[1]=0x38;
	for(unsigned short i=0;i<12;i++)
	{
		commandToSlaveUnion.data[i]=0;
	}
	writeToSlave();	
	readFromSlave();
	switch(responseFromSlaveUnion.responseCode[1])
		{
			case 0x44://Fingerprint match found.
					  sprintf(tmpStr,"%d",responseFromSlaveUnion.data[0]);
					  database_getemployee_byfid(responseFromSlaveUnion.data[0],employeeStats.data);
					  for(int i=0;i<USER_DATA_LENGTH;i++)
						Serial.print(employeeStats.data[i]);					
					  displayMessage2("Welcome!",tmpStr);
					  buzzerCommandToSlave();
					  openDoorCommandToSlave(0);
					  openDoorCommandToSlave(1);
					  openDoorCommandToSlave(2);
					  openDoorCommandToSlave(3);
					  delay(2000);
					  closeDoorCommandToSlave(0);
					  closeDoorCommandToSlave(1);
					  closeDoorCommandToSlave(2);
					  closeDoorCommandToSlave(3);
					  break;
			case 0x45://Fingerprint detected. No match found.
					  displayMessage("Invalid FP!!!");
					  delay(2000);
					  break;
			case 0x47://RFID detected. TODO: Validate the RFID in the DB to check if a valid user is associated with this RFID
					  sprintf(tmpStr,"%c%c%c%c%c%c%c%c%c%c%c%c",responseFromSlaveUnion.data[0],responseFromSlaveUnion.data[1],responseFromSlaveUnion.data[2],
					  responseFromSlaveUnion.data[3],responseFromSlaveUnion.data[4],responseFromSlaveUnion.data[5],responseFromSlaveUnion.data[6],responseFromSlaveUnion.data[7],
					  responseFromSlaveUnion.data[8],responseFromSlaveUnion.data[9],responseFromSlaveUnion.data[10],responseFromSlaveUnion.data[11]);
					  displayMessage2("Welcome",tmpStr);
					  buzzerCommandToSlave();
					  openDoorCommandToSlave(0);
					  openDoorCommandToSlave(1);
					  openDoorCommandToSlave(2);
					  openDoorCommandToSlave(3);
					  delay(2000);
					  closeDoorCommandToSlave(0);
					  closeDoorCommandToSlave(1);
					  closeDoorCommandToSlave(2);
					  closeDoorCommandToSlave(3);					  
					  break;
			default: //No Finger or RFID detected
					  break;
		}
					 
}

void updateDB()
{
	//Serial.println("Entered Update DB");
	//wifi_GetEmployee(1);
	byte db_temp[60]={0},temp2=0;
	//wifi_sendLog(305419896,temp);
	for(int j=1;j<50;j++)
	{
		if(!wifi_GetEmployee(j,db_temp))
			return;
		temp2=db_temp[0];
		db_temp[0]=db_temp[1];
		db_temp[1]=temp2;
		
		temp2=db_temp[46];
		db_temp[46]=db_temp[47];
		db_temp[47]=temp2;
		if(database_setemployee(db_temp))
			Serial.println("Data written");
		for(int i=0;i<60;i++)
			Serial.println(db_temp[i],DEC);
		delay(1000);
	}
		
}

char checkKeyPress()
{
	// Check Key Press - 0x32 0x39
		commandToSlaveUnion.command[0]=0x32;
		commandToSlaveUnion.command[1]=0x39;
		for(unsigned short i=0;i<12;i++)
			commandToSlaveUnion.data[i]=0;
		writeToSlave();
		readFromSlave();
		if(responseFromSlaveUnion.responseCode[1]==0x4A)
			// No key pressed
			return 'F'; //Failure
		
		else if(responseFromSlaveUnion.responseCode[1]==0x49)
			// Key pressedreturn
			return (char)responseFromSlaveUnion.data[0];
		else
			return 'F';
}
//0x32 0x35 - Open Door
//0x32 0x36 - Close Door
//0x32 0x37 - Trigger Buzzer
void openDoorCommandToSlave(unsigned short doorIndex)
{
		commandToSlaveUnion.command[0]=0x32;
		commandToSlaveUnion.command[1]=0x35;
		commandToSlaveUnion.data[0]=doorIndex;
		for(unsigned short i=1;i<12;i++)
			commandToSlaveUnion.data[i]=0;
		writeToSlave();
		//readFromSlave();
}

void closeDoorCommandToSlave(unsigned short doorIndex)
{
		commandToSlaveUnion.command[0]=0x32;
		commandToSlaveUnion.command[1]=0x36;
		commandToSlaveUnion.data[0]=doorIndex;
		for(unsigned short i=1;i<12;i++)
			commandToSlaveUnion.data[i]=0;
		writeToSlave();
		//readFromSlave();
}

void buzzerCommandToSlave()
{
		commandToSlaveUnion.command[0]=0x32;
		commandToSlaveUnion.command[1]=0x37;
		for(unsigned short i=0;i<12;i++)
			commandToSlaveUnion.data[i]=0;
		writeToSlave();
		//readFromSlave();
}

