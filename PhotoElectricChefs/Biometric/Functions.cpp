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
0x30 0x31 - Enroll Fingerprint
0x30 0x32 - Enroll RFID
0x30 0x33 - Delete Fingerprint
0x30 0x34 - Delete RFID
0x30 0x35 - Open Door
0x30 0x36 - Close Door
0x30 0x37 - Trigger Buzzer
0x30 0x38 - Poll
0x30 0x39
0x30 0x3A

Response Codes:
0x40 0x40 - Success - for enroll, delete
0x40 0x41 - Failed, User FP exits
0x40 0x42 - Failed, User RFID exists
0x40 0x43 - Failed, User Does not exist
0x40 0x44 - Fingerprint match - for Poll
0x40 0x45 - RFID flashed - for Poll
0x40 0x46 - 
0x40 0x47
0x40 0x48
0x40 0x49



*/
#include <Arduino.h>
#include <Functions.h>
#include <Wire.h>
#define DEBUG 1
#define DEBUG1 1

unsigned long serialInputNumber;
boolean  serialInputNumberReceived=false;
boolean enrollComplete=false;



void longTobyteArray(unsigned long longToConvert, byte* byteArray)
{
	byteArray[0] = (int)((longToConvert >> 24) & 0xFF) ;
	byteArray[1] = (int)((longToConvert >> 16) & 0xFF) ;
	byteArray[2] = (int)((longToConvert >> 8) & 0XFF);
	byteArray[3] = (int)((longToConvert & 0XFF));
}

union 
{
	struct 
	{
		//const byte header[2]={100,100}; // Hex: 2710
		const byte header[2]={100,100}; // Hex: 2710
		//byte command[2]={0,0};
		byte command[2]; 	
		//byte data[12]={0,0,0,0,0,0,0,0,0,0,0,0};
		byte data[12];
	};
	//byte commandToSlave[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	byte commandToSlave[16];
} commandToSlaveUnion;			

union 
{
	struct 
	{
		//const byte header[2]={100,100}; // Hex: 2710
		const byte header[2]={100,100}; // Hex: 2710
		byte responseCode[2];
		//byte data[12]={0,0,0,0,0,0,0,0,0,0,0,0};
		byte data[12];
	};
	//byte commandToSlave[16]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
	byte responseFromSlave[16];
} responseFromSlaveUnion;		

void readFromSlave()
{
  if(!DEBUGWOWIRE)
  {
  Wire.requestFrom(SLAVEADDRESS, 10);    // request 6 bytes from slave device #2
  //Serial.println("Received Data:");
  while (Wire.available())   // slave may send less than requested
  {
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
  }
  
  delay(500);
  }
}

void writeToSlave()
{
  //Serial.println("Sending command to Slave:");
  if(!DEBUGWOWIRE)
  {
    Wire.beginTransmission(SLAVEADDRESS); // transmit to device #4

  for(unsigned short i=0;i<16;i++)
  {
    Serial.print(commandToSlaveUnion.commandToSlave[i]);
    Wire.write(commandToSlaveUnion.commandToSlave[i]);        // sends five bytes
  }
  
  delay(500);
  }
}

void deleteUser()
{
  
}

void setTimeFunction()  
{
  
}

void enroll(unsigned short userType, unsigned short authtType)
{
	serialInputNumberReceived=false;
// UserType: USER: 0, ADMIN: 1
// AuthType to Enroll: FingerPrint: 0, Password: 1, Card: 2
	Serial.println("Enter ID");
	while (!serialInputNumberReceived)
	{
		Serial.println("Waiting for serial input number");
		receiveSerialInputNumber();
	}
/*	if(authType=0)
		FPID=serialInputNumber;
	else if(authType=1)
		RFID=serialInputNumber;
*/
	Serial.print("ID received:");
	Serial.println(serialInputNumber,DEC);
	// Set command parameters for enroll in I2C communication union.
	// Call I2C function to instruct the other controller to enroll
	commandToSlaveUnion.command[0]=0x30;
	commandToSlaveUnion.command[1]=0x31;
	longTobyteArray(serialInputNumber,commandToSlaveUnion.data);
	//sendToSlave()
	writeToSlave();
	if(DEBUG1)
		enrollComplete=true;
	while(!enrollComplete)
	{
		// wait for enroll to complete from the other controller 
	}
	Serial.println("Enroll Complete");
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
	if(DEBUG)
		Serial.println("receiveSerialInputNumber function called");
    serialInputNumberReceived=false;
    unsigned short count=0;
    char inchar[10]="";
    while (Serial.available()>0) 
    {
        if(DEBUG)
          Serial.println("Serial Input Number received");
        inchar[count]=char(Serial.read());
        count++;  
		serialInputNumberReceived=true;		
    }
    serialInputNumber=strtoul(inchar,NULL,0);

}

void poll()
{
	commandToSlaveUnion.command[0]=0x30;
	commandToSlaveUnion.command[1]=0x38;
	for(unsigned short i=0;i<12;i++)
	{
		commandToSlaveUnion.data[i]=0;
	}
	writeToSlave();	
}




  
