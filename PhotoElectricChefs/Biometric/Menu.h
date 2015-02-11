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
#include <Arduino.h>

#define MAX_NUM_OF_MENU_LIST_ITEMS 7
#define MAX_LENGTH_OF_MENU_LIST_ITEM 20
#define MENU_DATA_START_ADDR 100
//#define MENU_DATA_LENGTH (1+MAX_LENGTH_OF_MENU_LIST_ITEM+(MAX_NUM_OF_MENU_LIST_ITEMS*MAX_LENGTH_OF_MENU_LIST_ITEM)+MAX_NUM_OF_MENU_LIST_ITEMS+MAX_NUM_OF_MENU_LIST_ITEMS)
#define MENU_DATA_LENGTH 201
typedef struct  {    
	   byte numOfMenuListItems;
	   char heading[MAX_LENGTH_OF_MENU_LIST_ITEM];
	   char List[MAX_NUM_OF_MENU_LIST_ITEMS][MAX_LENGTH_OF_MENU_LIST_ITEM];        
	   byte nextLevelMenuIndex[MAX_NUM_OF_MENU_LIST_ITEMS];
	   byte functionPointerIndex[MAX_NUM_OF_MENU_LIST_ITEMS];
					   
} menu_st;
typedef union 
{
	menu_st menu_struct;
	byte data[MENU_DATA_LENGTH];
}menu_un;
	 /*typedef struct  {    
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
		*/



	//void initializeMenus(void);
	int getMenu(int);
	int setMenu(int);

#endif
