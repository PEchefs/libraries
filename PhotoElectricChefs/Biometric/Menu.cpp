#include "Menu.h"
#include "Ext_EEPROM.h" //I2C library
#include <gLCD_u8glib.h>

menu_un Menu;
int getMenu(int id)
{
	int menu_start_addr=MENU_DATA_START_ADDR+(id*MENU_DATA_LENGTH);
	for(int i=0;i<MENU_DATA_LENGTH;i++)
	{
		Menu.data[i]=i2c_eeprom_read_byte(menu_start_addr+i);
		//delay(2);
	}
}
int setMenu(int id)
{
	int i=0;
	menu_un Menu_temp;
	int menu_start_addr=0;
	for(i=0;i<MENU_DATA_LENGTH;i++)
	{
		Menu_temp.data[i]=Menu.data[i];
	}
	menu_start_addr=MENU_DATA_START_ADDR+(id*MENU_DATA_LENGTH);
	for(i=0;i<MENU_DATA_LENGTH;i++)
	{
		i2c_eeprom_write_byte(menu_start_addr+i,Menu_temp.data[i]);
		delay(50);
	}
	getMenu(id);
	for(i=0;i<MENU_DATA_LENGTH;i++)
	{
		if(Menu_temp.data[i]!=Menu.data[i])
			return 0;
	}
	return 1;
}
/*void initializeMenus(void)
{
  Menu[0].numOfMenuListItems=4;
  Menu[0].heading="MENU";
  Menu[0].List[0]="1.  Enroll";
  Menu[0].List[1]="2.  Delete";
  Menu[0].List[2]="3.  Set Time";
  Menu[0].List[3]="4.  Settings";
  Menu[0].nextLevelMenuIndex[0]=1;
  Menu[0].nextLevelMenuIndex[1]=255;
  Menu[0].nextLevelMenuIndex[2]=255;
  Menu[0].nextLevelMenuIndex[3]=2;
  Menu[0].functionPointerIndex[0]=255;
  Menu[0].functionPointerIndex[1]=0;
  Menu[0].functionPointerIndex[2]=1;
  Menu[0].functionPointerIndex[3]=255;                               
                            
  Menu[1].numOfMenuListItems=2;
  Menu[1].heading="ENROLL";
  Menu[1].List[0]="1.   Enroll User";
  Menu[1].List[1]="2.   Enroll Admin";
  Menu[1].nextLevelMenuIndex[0]=3;
  Menu[1].nextLevelMenuIndex[1]=4;

  Menu[1].functionPointerIndex[0]=255;
  Menu[1].functionPointerIndex[1]=255;

  Menu[2].numOfMenuListItems=4;
  Menu[2].heading="SETTINGS";
  Menu[2].List[0]="1.  Access";
  Menu[2].List[1]="2.  Information";
  Menu[2].List[2]="3.  Self Test";
  Menu[2].List[3]="4.  Factory Reset";
  Menu[2].nextLevelMenuIndex[0]=5;
  Menu[2].nextLevelMenuIndex[1]=6;
  Menu[2].nextLevelMenuIndex[2]=255;
  Menu[2].nextLevelMenuIndex[3]=255;
  Menu[2].functionPointerIndex[0]=255;                               
  Menu[2].functionPointerIndex[1]=255;                               
  Menu[2].functionPointerIndex[2]=8;  
  Menu[2].functionPointerIndex[3]=9;  

  Menu[3].numOfMenuListItems=3;
  Menu[3].heading="ENROLL USER";
  Menu[3].List[0]="1.  Fingerprint";
  Menu[3].List[1]="2.  Password";
  Menu[3].List[2]="3.  Card";
  Menu[3].nextLevelMenuIndex[0]=255;
  Menu[3].nextLevelMenuIndex[1]=255;
  Menu[3].nextLevelMenuIndex[2]=255;
  Menu[3].functionPointerIndex[0]=2;
  Menu[3].functionPointerIndex[1]=3;
  Menu[3].functionPointerIndex[2]=4; 
  
  Menu[4].numOfMenuListItems=3;
  Menu[4].heading="ENROLL ADMIN";
  Menu[4].List[0]="1.  Fingerprint";
  Menu[4].List[1]="2.  Password";
  Menu[4].List[2]="3.  Card";
  Menu[4].nextLevelMenuIndex[0]=255;
  Menu[4].nextLevelMenuIndex[1]=255;
  Menu[4].nextLevelMenuIndex[2]=255;
  Menu[4].functionPointerIndex[0]=5;
  Menu[4].functionPointerIndex[1]=6;
  Menu[4].functionPointerIndex[2]=7; 

  Menu[5].numOfMenuListItems=6;
  Menu[5].heading="ACCESS";
  Menu[5].List[0]="1.  Attend. Set";
  Menu[5].List[1]="2.  Open Time";
  Menu[5].List[2]="3.  Open TimeOut";
  Menu[5].List[3]="4.  Door Sensor";
  Menu[5].List[4]="5.  Bell Time";
  Menu[5].List[5]="6.  Verify Mode";
//////  Menu[11].List[11]="6.  Set DuressFP";
  Menu[5].nextLevelMenuIndex[0]=255;
  Menu[5].nextLevelMenuIndex[1]=255;
  Menu[5].nextLevelMenuIndex[2]=255;
  Menu[5].nextLevelMenuIndex[3]=255;
  Menu[5].nextLevelMenuIndex[4]=255;
  Menu[5].nextLevelMenuIndex[5]=255;
  Menu[5].functionPointerIndex[0]=10;
  Menu[5].functionPointerIndex[1]=11;
  Menu[5].functionPointerIndex[2]=12;                               
  Menu[5].functionPointerIndex[3]=13;                               
  Menu[5].functionPointerIndex[4]=14;  
  Menu[5].functionPointerIndex[5]=15;  

  Menu[6].numOfMenuListItems=7;
  Menu[6].heading="INFORMATION";
  Menu[6].List[0]="1.  User";
  Menu[6].List[1]="2.  Manager";
  Menu[6].List[2]="3.  Finger";
  Menu[6].List[3]="4.  DuressFP";
  Menu[6].List[4]="5.  Password";
  Menu[6].List[5]="6.  Card";
  Menu[6].List[6]="7.  Log";
  Menu[6].nextLevelMenuIndex[0]=255;
  Menu[6].nextLevelMenuIndex[1]=255;
  Menu[6].nextLevelMenuIndex[2]=255;
  Menu[6].nextLevelMenuIndex[3]=255;
  Menu[6].nextLevelMenuIndex[4]=255;
  Menu[6].nextLevelMenuIndex[5]=255;
  Menu[6].nextLevelMenuIndex[6]=255;
  Menu[6].nextLevelMenuIndex[7]=255;
  Menu[6].functionPointerIndex[0]=16;
  Menu[6].functionPointerIndex[1]=17;
  Menu[6].functionPointerIndex[2]=18;
  Menu[6].functionPointerIndex[3]=19;                               
  Menu[6].functionPointerIndex[4]=20;                               
  Menu[6].functionPointerIndex[5]=21;  
  Menu[6].functionPointerIndex[6]=22; 
}*/


