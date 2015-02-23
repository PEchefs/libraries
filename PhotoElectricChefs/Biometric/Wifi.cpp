 #include "Ext_EEPROM.h" //I2C library
 #include "Database.h"
 #include "Wifi.h"
 /***************************USAGE*******************/

 /*****************************************************/
SoftwareSerial mySerial(3, 2); //RX,TX
byte serialData[100];
union {
  int serialDataCMDID;
  byte serialDataCMDIDbytearray[2];
} cmdid;

bool wifi_Init()
{
	  mySerial.begin(9600);
}

bool wifi_Poll()
{
	 unsigned short count=0;
     if(mySerial.available()>=10)
     {
       byte header[2]={0,0};
       header[0]=mySerial.read();
       header[1]=mySerial.read();
       if(header[0]==0x50&&header[1]==0x50)
       {
         //Display on LCD: Receiving instructions from Server
         cmdid.serialDataCMDIDbytearray[0]=mySerial.read();
         cmdid.serialDataCMDIDbytearray[1]=mySerial.read();
         byte temp[100]={0};
  
             switch(cmdid.serialDataCMDID)
             {
               case 0x7170://FP download from server to biometric device
                           //Display on LCD: Downloading FingerPrint Database
                           break;
               case 0x7270://DB download from server to EEPROM
                           //Display on LCD: Downloading Employee Database
                           for(count=0;count<60;count++)
                           {
                             temp[count]=mySerial.read();
                           }
                           database_setemployee(temp);
  
                           break;                       
           //    case 0x7170://FP download from server to biometric device
             //              break;                       
               //case 0x7170://FP download from server to biometric device
                 //          break;                       
               default:break;
             }
		 return true;
       }
     }
	 return false;
}
bool wifi_GetEmployee(int sl_no,byte* resp)
{
		union {
		  int id;
		  byte id_byte[2];
		} emp_id;
		 emp_id.id=sl_no;
         byte temp[100]={0};
		 temp[0]=0x50;
		 temp[1]=0x51;
		 temp[2]=0x32;
		 temp[3]=0x31;
		 temp[4]=emp_id.id_byte[0];
		 temp[5]=emp_id.id_byte[1];
		 for(int i=0;i<100;i++)
			mySerial.write(temp[i]);
		while(!mySerial.available());
		int i=1;
		while(i<60)
			if(mySerial.available())
				resp[i++]=mySerial.read();
		while(mySerial.available())mySerial.read();
		if(resp[1]==0)
			return false;
		else 
			return true;
}
bool wifi_sendLog(long sl_no,byte* resp)
{
		 logStats.log.logSlNo=sl_no;
		 logStats.log.empId[0]='A';
		 logStats.log.empId[1]='B';
		 logStats.log.empId[2]='C';
		 logStats.log.empId[3]='-';
		 logStats.log.empId[4]='1';
		 logStats.log.empId[5]='2';
		 logStats.log.empId[6]='3';
		 logStats.log.empId[7]='4';
		 logStats.log.empId[8]='5';
		 logStats.log.empId[9]='6';
		 logStats.log.empId[10]='D';
		 logStats.log.empId[11]='E';

		 logStats.log.empMode=1;
		 logStats.log.logTime=1424504569;
         byte temp[100]={0};
		 temp[0]=0x50;
		 temp[1]=0x51;
		 temp[2]=0x31;
		 temp[3]=0x33;
		 for(int i=4;i<25;i++)
			temp[i]=logStats.data[i-4];
		 for(int i=0;i<100;i++)
			mySerial.write(temp[i]);
		while(!mySerial.available());
		int i=0;
		while(mySerial.available())
			resp[i++]=mySerial.read();
}
bool wifi_sendtemplate(long sl_no,byte* resp)
{
	logStats.log.logSlNo=sl_no;
	 logStats.log.empId[0]='A';
	 logStats.log.empId[1]='B';
	 logStats.log.empId[2]='C';
	 logStats.log.empId[3]='-';
	 logStats.log.empId[4]='1';
	 logStats.log.empId[5]='2';
	 logStats.log.empId[6]='3';
	 logStats.log.empId[7]='4';
	 logStats.log.empId[8]='5';
	 logStats.log.empId[9]='6';
	 logStats.log.empId[10]='D';
	 logStats.log.empId[11]='E';

	 logStats.log.empMode=1;
	 logStats.log.logTime=1424504569;
	 byte temp[100]={0};
	 temp[0]=0x50;
	 temp[1]=0x51;
	 temp[2]=0x31;
	 temp[3]=0x32;
	 for(int i=4;i<25;i++)
		temp[i]=logStats.data[i-4];
	 for(int i=0;i<100;i++)
		mySerial.write(temp[i]);
	while(!mySerial.available());
	int i=0;
	while(mySerial.available())
		resp[i++]=mySerial.read();
}