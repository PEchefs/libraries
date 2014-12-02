/*
(c) PhotoElectricChefs 2014

*/

#ifndef shiftReg_h
#define shiftReg_h

#include <inttypes.h>
#include <Arduino.h>


class shiftReg
{

  public:
	struct shiftRegPins_st
	{
		byte SER;
		byte RCK;
		byte SRCK;
		byte SRCLR;
	} shiftRegPins;
    void shiftClkPulse(void);
    void regClkPulse(void);
	void writeSER(boolean);
	void SClearClkPulse(void);
	void srinit(unsigned short int,unsigned short int,unsigned short int,unsigned short int);
	void writeData(byte);
};

#endif