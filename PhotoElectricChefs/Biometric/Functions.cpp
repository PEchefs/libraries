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
0x32 0x39
0x32 0x3A

Response Codes:
0x40 0x40 - Success - for enroll, delete
0x40 0x41 - Failed, User FP exits
0x40 0x42 - Failed, User RFID exists
0x40 0x43 - Failed, User Does not exist
0x40 0x44 - Fingerprint match - for Poll
0x40 0x45 - RFID flashed - for Poll
0x40 0x46 - Wait, Under processing
0x40 0x47
0x40 0x48
0x40 0x49



*/
#include <Arduino.h>
#include <Functions.h>
#include <Wire.h>
#define DEBUG 1
#define DEBUG1 1
#define DEBUGWOWIRE 0

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
		delay(1000);
	time2=millis();
	if(time2-time1>10000)
		{
			if(DEBUG)
				Serial.println("Time Out on wire receive");
			//	break;
			return 0;
		}
  }
  Serial.println("Received:");
  delay(10);

	

	while (Wire.available())   // slave may send less than requested
	{
		
		responseFromSlaveUnion.responseFromSlave[i]=Wire.read();
		Serial.print(responseFromSlaveUnion.responseFromSlave[i]);
 	//   char c = Wire.read(); // receive a byte as character
 	//   Serial.print(c);         // print the character
		i++;
	}
	if(responseFromSlaveUnion.responseCode[0]==0x40 && responseFromSlaveUnion.responseCode[1]!=0x46)
			validResponseReceived=true;
/*	if(i==0)
		if(DEBUG)
			Serial.println("Failed Enroll! No response received");
		//TODO : Add LCD call to display failure
	else if (i!=16)
		if(DEBUG)
			Serial.println("Failed Enroll! 16 bytes not received");
		//TODO : Add LCD call to display failure
		*/
	delay(500);
	}
  while((j<20) && !validResponseReceived);
  if(!validResponseReceived)
	return -1;
	
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
	Serial.write(commandToSlaveUnion.commandToSlave,16);
    Wire.write(commandToSlaveUnion.commandToSlave,16);        // sends 16 bytes
	Wire.endTransmission();    // stop transmitting
  delay(500);
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
	
	if(authType==0)
		{
		unsigned short retCode=0;
		



			if(DEBUG)
				Serial.println("Auth Type = Fingerprint");
		// 0x30 0x31 - Check Fingerprint
		commandToSlaveUnion.command[0]=0x30;
		commandToSlaveUnion.command[1]=0x31;
		for(unsigned short i=0;i<16;i++)
			commandToSlaveUnion.data[i]=0;
		longTobyteArray(serialInputNumber,commandToSlaveUnion.data);
		writeToSlave();
		delay(100);
		retCode=readFromSlave();
		if(retCode==-1)
			//No valid response - No fingerprint found
			{
			displayMessage(0);
			return -1;
			}
		else if(retCode==0)
			// Timeout - No fingerprint found
			{
			displayMessage(1);
			return -1;
			}
		//0x30 0x32 - Enroll Fingerprint1
		commandToSlaveUnion.command[0]=0x30;
		commandToSlaveUnion.command[1]=0x32;
		for(unsigned short i=0;i<16;i++)
			commandToSlaveUnion.data[i]=0;
		writeToSlave();
		readFromSlave();
		if(retCode==-1)
			//No valid response - Finger not pressed
			{
			displayMessage(2);
			return -1;
			}
		else if(retCode==0)
			// Timeout - Finger not pressed
			{
			displayMessage(3);
			return -1;
			}
		//0x30 0x33 - Enroll Fingerprint2
		commandToSlaveUnion.command[0]=0x30;
		commandToSlaveUnion.command[1]=0x33;
		for(unsigned short i=0;i<16;i++)
			commandToSlaveUnion.data[i]=0;
		writeToSlave();
		readFromSlave();
		if(retCode==-1)
			//No valid response - Fingerprints do not match
			{
			displayMessage(4);
			return -1;
			}
		else if(retCode==0)
			// Timeout - Finger not pressed
			{
			displayMessage(5);
			return -1;
			}
		//0x30 0x34 - STORE Fingerprint
		commandToSlaveUnion.command[0]=0x30;
		commandToSlaveUnion.command[1]=0x34;
		for(unsigned short i=0;i<16;i++)
			commandToSlaveUnion.data[i]=0;
		writeToSlave();
		readFromSlave();
		if(retCode==-1)
			//No valid response - Enroll Failed. 
			{
			displayMessage(6);
			return -1;
			}
		else if(retCode==0)
			// Timeout - Could not enroll!. Unknown error.
			{
			displayMessage(7);
			return -1;		
			}
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
	if(DEBUG)
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
//	if(DEBUG)
//		Serial.println("receiveSerialInputNumber function called");
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
	readFromSlave();
}




  
