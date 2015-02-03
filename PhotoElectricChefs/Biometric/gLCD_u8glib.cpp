#include <gLCD_u8glib.h>


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
U8GLIB_KS0108_128 u8g( 7, 8, 9, 10, 11, 12, 13, 14, 6, 15, 16, 4, 5); 		// 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs1=14, cs2=15,di=17,rw=16


uint8_t menu_redraw_required = 0;
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