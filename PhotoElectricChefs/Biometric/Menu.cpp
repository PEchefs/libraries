#include "Menu.h"
#include <gLCD_u8glib.h>
menu_st Menu[7];
unsigned short currentMenu=0;
unsigned short prevMenu=0;
unsigned short displayStartIndex=0;
unsigned short prevDisplayStartIndex=0;
int currentMenuIndex=0;
unsigned short prevMenuIndex=0;
unsigned short MenuFunctionToCallIndex=0;
state previousState=NONE;
state currentState=MENUSCREEN;
state newState=NONE;
void initializeMenus(void)
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
}

void escMenuList()
{
   currentMenu=prevMenu;
   prevMenu=0;
   displayStartIndex=prevDisplayStartIndex;
   prevDisplayStartIndex=0;
   currentMenuIndex=prevMenuIndex;
   prevMenuIndex=0;
}

void updateState(state stateToUpdateTo)
{
  previousState=currentState;
  currentState=stateToUpdateTo;
}



void enterMenuItem(menu_st currentMenuList)
{
   if(currentMenuList.nextLevelMenuIndex[currentMenuIndex] != 255)
   {
     //next level submenu exists
     prevMenu=currentMenu;
     currentMenu=currentMenuList.nextLevelMenuIndex[currentMenuIndex];
   
     prevDisplayStartIndex=displayStartIndex;
     displayStartIndex=0;

     prevMenuIndex=currentMenuIndex;
     currentMenuIndex=0;
   }
   else
   {
    //next level submenu doesn't exist. Call, function corresponding to the menu item selected
    MenuFunctionToCallIndex=currentMenuList.functionPointerIndex[currentMenuIndex];
    newState=MENUFUNCTION;
    updateState(newState);
   }
}

void drawMenu(menu_st menuToDisplay) {
	uint8_t i,count, h;
	u8g_uint_t w, d;

	//  u8g.setFont(u8g_font_7x13B);
	u8g.setFont(u8g_font_7x14);
	u8g.setFontRefHeightText();
	u8g.setFontPosTop();
	h = u8g.getFontAscent()-u8g.getFontDescent();

	w = u8g.getWidth();
        
        // Heading for Menu
        unsigned short mid = (w-u8g.getStrWidth(menuToDisplay.heading))/2;
//        u8g.drawStr(mid, 0*h+1, menuToDisplay.heading);
  /*      if (currentMenuIndex == 2)
          return; */
	if (( menuToDisplay.numOfMenuListItems - displayStartIndex ) <= MAX_NUM_DISPLAY_LINES )
	{
               // if(DEBUG)
                 // Serial.println("Single Page Menu Display");
		if(currentMenuIndex>=menuToDisplay.numOfMenuListItems)
		{
			currentMenuIndex=0;
			displayStartIndex=0;
		}
		else if(currentMenuIndex<0)
		{
			currentMenuIndex=menuToDisplay.numOfMenuListItems-1;
			displayStartIndex=0;
		}
		if(currentMenuIndex < displayStartIndex)
		{
			displayStartIndex--;
		}


        
		//loop(from displayStartIndex to numOfMenuListItems) 
                //if(DEBUG)
                  //Serial.println("Entered Loop to display items");
		for( i = displayStartIndex,count=1; i < menuToDisplay.numOfMenuListItems; i++,count++ ) 
                {
			d=0;
			u8g.setDefaultForegroundColor();
                        // Display Heading
                        u8g.drawStr(mid, 0*h+1, menuToDisplay.heading);
			if ( i == currentMenuIndex ) 
                        {
				u8g.drawBox(0, count*h+1, w, h);
				//    u8g.drawBox(0, i*h+1, w, h+2);
				u8g.setDefaultBackgroundColor();
			}
			//   u8g.setFontPosBaseline();
			u8g.drawStr(d, count*h+1, menuToDisplay.List[i]);
		}
               // if(DEBUG)
                 // Serial.println("Finished Single Page Menu Display");
	}
	else
	{
		/*if(DEBUG)
		{ 
			Serial.print("currentMenuIndex=");Serial.println(currentMenuIndex);
			Serial.print("displayStartIndex=");Serial.println(displayStartIndex);
		}*/
		if(currentMenuIndex>=displayStartIndex+MAX_NUM_DISPLAY_LINES)
		{
			displayStartIndex++;
		}
		else if(currentMenuIndex<0)
		{
			currentMenuIndex=menuToDisplay.numOfMenuListItems-1;
			displayStartIndex=menuToDisplay.numOfMenuListItems-MAX_NUM_DISPLAY_LINES;
		}
		if(currentMenuIndex < displayStartIndex)
		{
			displayStartIndex--;
		}
		//loop(from displayStartIndex to displayStartIndex+maxNumOfDisplayLines-1)

		for( i = displayStartIndex,count=1; i < displayStartIndex+MAX_NUM_DISPLAY_LINES; i++,count++ ) 
                {
			d=0;
			u8g.setDefaultForegroundColor();
                        //Display Heading
                        u8g.drawStr(mid, 0*h+1, menuToDisplay.heading);
			if ( i == currentMenuIndex ) {
				u8g.drawBox(0, count*h+1, w, h);
				//    u8g.drawBox(0, i*h+1, w, h+2);
				u8g.setDefaultBackgroundColor();
			}
			//   u8g.setFontPosBaseline();
			u8g.drawStr(d, count*h+1, menuToDisplay.List[i]);
		}
	}




	/*if(DEBUG)
	{ 
		Serial.print("currentMenuIndex=");Serial.println(currentMenuIndex);
		Serial.print("displayStartIndex=");Serial.println(displayStartIndex);
	}*/
}

void displayMenu(menu_st MenuToDisplay)
//void displayMenu()
{
  
	// put your main code here, to run repeatedly:
	//initializeMenus();
	u8g.firstPage();  
	do {
         // if(DEBUG)
           //  Serial.println("Sending MenuToDisplay");
		drawMenu(MenuToDisplay);
	} while( u8g.nextPage() );

}


void scrolldown()
{
	currentMenuIndex++; 
}

void scrollup()
{
	currentMenuIndex--; 
}
