/*
Menu/Sun Menu List Index:

0  -  Main Menu
1  -  Main -> Enroll Menu
2  -  Main -> View Log Menu
3  -  Main -> USB Disk Menu
4  -  Main -> Settings Menu
5  -  Main -> Enroll -> Enroll User Menu
6  -  Main -> Enroll -> Enroll Admin Menu
7  -  Main -> Enroll -> Enroll Manager Menu
8  -  Main -> Settings -> General
9  -  Main -> Settings -> Log
10 -  Main -> Settings -> Communication
11 -  Main -> Settings -> Access 
12 -  Main -> Settings -> Information
13 -  Main -> Settings -> Self-Test


*/
#ifndef MENU_H
#define MENU_H
	#define MAX_NUM_OF_MENU_LIST_ITEMS 7



	typedef struct  {    
					   unsigned short numOfMenuListItems;
					   char *heading;
					   char *List[MAX_NUM_OF_MENU_LIST_ITEMS];        
					   unsigned short nextLevelMenuIndex[MAX_NUM_OF_MENU_LIST_ITEMS];
					   unsigned short functionPointerIndex[MAX_NUM_OF_MENU_LIST_ITEMS];
					   // void (*functionPointer[MAX_NUM_OF_MENU_LIST_ITEMS])(void);
					   
					  // struct * nextLevelMenu[MAX_NUM_OF_MENU_LIST_ITEMS];
					//  menu_st *ptr[MAX_NUM_OF_MENU_LIST_ITEMS]=
					  //struct p *ptr;
	} menu_st;
	  
	 
	enum state {HOMESCREEN=0,MENUSCREEN,FPAUTH,PWDAUTH,RFIDAUTH,MENUFUNCTION,NONE};
	enum key {POWER=0,SCROLLDOWN,SCROLLUP,ENTER,ESC,MENU,ZERO,ONE,TWO,THREE,FOUR,FIVE,SIX,SEVEN,EIGHT,NINE,NOKEY};


	void initializeMenus(void);
	void displayMenu(menu_st MenuToDisplay);
	void drawMenu(menu_st menuToDisplay);
	void enterMenuItem(menu_st currentMenuList);
	void updateState(state stateToUpdateTo);
	void escMenuList();
	void scrollup(void);
	void scrolldown(void);

#endif
