#include <gLCD_u8glib.h>

#define DEBUG 1
unsigned short currentMenu=0;
unsigned short prevMenu=0,prevMenu1=0;
unsigned short displayStartIndex=0;
unsigned short prevDisplayStartIndex=0, prevDisplayStartIndex1=0;
int currentMenuIndex=0;
unsigned short prevMenuIndex=0, prevMenuIndex1=0;
unsigned short MenuFunctionToCallIndex=0;
state previousState=NONE;
state currentState=HOMESCREEN;
state newState=NONE;
U8GLIB_KS0108_128 u8g( 7, 8, 9, 10, 11, 12, 13, 14, 6, 15, 16, 4, 5); 		// 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs1=14, cs2=15,di=17,rw=16

const char *monthName[12] = {
  "Jan", "Feb", "Mar", "Apr", "May", "Jun",
  "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

const char *WeekDay[7] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"}; 

uint8_t menu_redraw_required = 0;
void escMenuList()
{
	if(currentMenu==0)
		{
			updateState(HOMESCREEN);
			displayStartIndex=0;
			currentMenuIndex=0;
		}
			
	else
	{
		currentMenu=prevMenu;
		prevMenu=prevMenu1;
		prevMenu1=0;
		displayStartIndex=prevDisplayStartIndex;
		prevDisplayStartIndex=prevDisplayStartIndex1;
		prevDisplayStartIndex1=0;
		currentMenuIndex=prevMenuIndex;
		prevMenuIndex=prevMenuIndex1;
		prevMenuIndex1=0;
	}
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
	 prevMenu1=prevMenu;
     prevMenu=currentMenu;
     currentMenu=currentMenuList.nextLevelMenuIndex[currentMenuIndex];
   
	 prevDisplayStartIndex1=prevDisplayStartIndex;
     prevDisplayStartIndex=displayStartIndex;
     displayStartIndex=0;
	 
	 prevMenuIndex1=prevMenuIndex;
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

void displayMessage(char *message)
{
	if(DEBUG)
		Serial.println("Display Message function called");
	uint8_t i,count, h;
	u8g_uint_t w, d;

	//  u8g.setFont(u8g_font_7x13B);
	u8g.setFont(u8g_font_7x14);
	u8g.setFontRefHeightText();
	u8g.setFontPosTop();
	h = u8g.getFontAscent()-u8g.getFontDescent();

	w = u8g.getWidth();
    u8g.setDefaultForegroundColor();
        // Heading for Menu
    //char *message;   
	unsigned short mid=0;
	/*switch(messageIndex)
	{
		case 0://Please wait..checking sensor
				message="Please Wait ...";
				mid = (w-u8g.getStrWidth(message))/2;
				break;
		case 1://Press finger
				message="Press Finger";
				mid = (w-u8g.getStrWidth(message))/2;
				break;
		case 2://No finger detected (after timeout)
				message="No Finger Detected";
				mid = (w-u8g.getStrWidth(message))/2;
				break;
		case 3://Press finger again
				message="Press Finger Again";
				mid = (w-u8g.getStrWidth(message))/2;
				break;
		case 4://Fingerprints do not match.
				message="FP mismatch!";
				mid = (w-u8g.getStrWidth(message))/2;
				break;
		case 5://Enroll successful
				message="Enroll Successful";
				mid = (w-u8g.getStrWidth(message))/2;
				break;
		case 6://Delete Successful
				message="Delete Successful";
				mid = (w-u8g.getStrWidth(message))/2;
				break;
		case 7://Downloading DB
				message="Downloading DB...";
				mid = (w-u8g.getStrWidth(message))/2;
				break;
		case 8://Welcome
				message="WELCOME!!!";
				mid = (w-u8g.getStrWidth(message))/2;
				break;
		case 9://Invalid Fingerprint
				message="INVALID FP!!!";
				mid = (w-u8g.getStrWidth(message))/2;
				break;
		
		default:break;
		}
		*/
	//	;
	mid = (w-u8g.getStrWidth(message))/2;
		u8g.firstPage();  
	do {
         // if(DEBUG)
           //  Serial.println("Sending MenuToDisplay");
		u8g.drawStr(mid, 1*h+1, message);
	} while( u8g.nextPage() );
		
}

void displayMessage2(char *message1, char *message2)
{
	if(DEBUG)
		Serial.println("Display Message function called");
	uint8_t i,count, h;
	u8g_uint_t w, d;

	//  u8g.setFont(u8g_font_7x13B);
	u8g.setFont(u8g_font_7x14);
	u8g.setFontRefHeightText();
	u8g.setFontPosTop();
	h = u8g.getFontAscent()-u8g.getFontDescent();

	w = u8g.getWidth();
    u8g.setDefaultForegroundColor();
        // Heading for Menu
    //char *message;   
	unsigned short mid1=0;
	unsigned short mid2=0;

	mid1 = (w-u8g.getStrWidth(message1))/2;
	mid2 = (w-u8g.getStrWidth(message1))/2;
		u8g.firstPage();  
	do {
         // if(DEBUG)
           //  Serial.println("Sending MenuToDisplay");
		u8g.drawStr(mid1, 1*h+1, message1);
		u8g.drawStr(mid2, 2*h+1, message2);
	} while( u8g.nextPage() );
		
}

void displayHomeScreen(tmElements_t currentTimeReceived)
{
	static boolean skip=true;
	char timeLine1[6]="";
	char timeLine2[16]="";
	char catString[4]="";
	itoa((int)currentTimeReceived.Hour,catString,10);
	if(currentTimeReceived.Hour<10)
	{
		strcat(timeLine1,"0");
	}
	strcat(timeLine1,catString);
	if(skip)
	{
	skip=false;
	strcat(timeLine1," ");
	}
	else
	{
	skip=true;
	strcat(timeLine1,":");
	}
	itoa((int)currentTimeReceived.Minute,catString,10);
	if(currentTimeReceived.Minute<10)
	{
		strcat(timeLine1,"0");
	}
	strcat(timeLine1,catString);
	strcat(timeLine2,WeekDay[(int)currentTimeReceived.Wday-1]);
	strcat(timeLine2," ");
	itoa((int)currentTimeReceived.Day,catString,10);
	strcat(timeLine2,catString);
	strcat(timeLine2," ");
	//itoa((int)currentTimeReceived.Month,catString,10);
	//strcat(timeLine2,catString);
	strcat(timeLine2,monthName[(int)currentTimeReceived.Month-1]);
	strcat(timeLine2," ");
	itoa((int)tmYearToCalendar(currentTimeReceived.Year),catString,10);
	strcat(timeLine2,catString);
	unsigned short mid1=0;
	unsigned short mid2=0;
	uint8_t i,count, h;
	u8g_uint_t w, d;

	//  u8g.setFont(u8g_font_7x13B);
	u8g.setFont(u8g_font_freedoomr25n);
	u8g.setFontRefHeightText();
	u8g.setFontPosTop();
	h = u8g.getFontAscent()-u8g.getFontDescent();

	w = u8g.getWidth();
    u8g.setDefaultForegroundColor();
	mid1 = (w-u8g.getStrWidth(timeLine1))/2;
	//mid2 = (w-u8g.getStrWidth(timeLine2))/2;
	mid2=15;
	/*if(DEBUG)
		{
			Serial.println(timeLine1);
			Serial.println(timeLine2);
		}
		*/
		u8g.firstPage(); 
		do {
         // if(DEBUG)
           //  Serial.println("Sending MenuToDisplay");
		u8g.setFont(u8g_font_freedoomr25n);  
//		u8g.setFont(u8g_font_7x14);  
		u8g.setFontRefHeightText();
		h = u8g.getFontAscent()-u8g.getFontDescent();
		w = u8g.getWidth();
		//u8g.drawStr(mid1, 4*h+1, timeLine1);
		u8g.drawStr(mid1, 1.5*h+1, timeLine1);
		u8g.setFont(u8g_font_7x14);
		u8g.setFontRefHeightText();
		h = u8g.getFontAscent()-u8g.getFontDescent();
		w = u8g.getWidth();
		u8g.drawStr(mid2, 4*h+1, timeLine2);
		
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