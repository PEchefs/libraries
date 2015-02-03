#ifndef GLCD_U8GLIB_H
#define GLCD_U8GLIB_H

#include "Arduino.h"
#include <U8glib.h>

#define MAX_NUM_DISPLAY_LINES 4

U8GLIB_KS0108_128 u8g( 7, 8, 9, 10, 11, 12, 13, 14, 6, 15, 16, 4, 5); 		// 8Bit Com: D0..D7: 8,9,10,11,4,5,6,7 en=18, cs1=14, cs2=15,di=17,rw=16


uint8_t menu_redraw_required = 0;
#endif



