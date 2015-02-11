#ifndef GLCD_U8GLIB_H
#define GLCD_U8GLIB_H

#include "Arduino.h"
#include <U8glib.h>
#include <Menu.h>


#define MAX_NUM_DISPLAY_LINES 4


	enum state {HOMESCREEN=0,MENUSCREEN,FPAUTH,PWDAUTH,RFIDAUTH,MENUFUNCTION,NONE};
	enum key {POWER=0,SCROLLDOWN,SCROLLUP,ENTER,ESC,MENU,ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,NOKEY};
	void displayMenu(menu_st);
	void drawMenu(menu_st);
	void enterMenuItem(menu_st);
	void updateState(state);
	void escMenuList();
	void scrollup(void);
	void scrolldown(void);
	void displayMessage(unsigned short);
	
#endif



