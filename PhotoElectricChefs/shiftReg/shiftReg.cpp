#include "shiftReg.h"

void shiftReg::shiftClkPulse()
{
  digitalWrite(shiftRegPins.SRCK, HIGH);
  delayMicroseconds(1);
  digitalWrite(shiftRegPins.SRCK, LOW);
}

void shiftReg::regClkPulse()
{
  digitalWrite(shiftRegPins.RCK, HIGH);
  delayMicroseconds(1);
  digitalWrite(shiftRegPins.RCK, LOW);
}

void shiftReg::writeSER(boolean _bit)
{
  digitalWrite(shiftRegPins.SER,_bit);
  delayMicroseconds(1);
  shiftClkPulse();  
}

void shiftReg::SClearClkPulse()
{
  digitalWrite(shiftRegPins.SRCLR, LOW);
  delayMicroseconds(1);
  digitalWrite(shiftRegPins.SRCLR, HIGH);
}

void shiftReg::srinit(unsigned short int pinSER, unsigned short int pinRCK, unsigned short int pinSRCK, unsigned short int pinSRCLR) {    
  shiftRegPins.SER=pinSER;
  shiftRegPins.RCK=pinRCK;
  shiftRegPins.SRCK=pinSRCK;
  shiftRegPins.SRCLR=pinSRCLR;
  pinMode(shiftRegPins.SER, OUTPUT);     
  pinMode(shiftRegPins.RCK, OUTPUT);     
  pinMode(shiftRegPins.SRCK, OUTPUT);     
  pinMode(shiftRegPins.SRCLR, OUTPUT);
  digitalWrite(shiftRegPins.RCK, LOW);
  digitalWrite(shiftRegPins.SRCK, LOW);
  digitalWrite(shiftRegPins.SRCLR, HIGH);
  delayMicroseconds(1);
  regClkPulse();
  
  // Clear registers
  SClearClkPulse();
}  

void shiftReg::writeData(byte dataToWrite)
{
 // Write 8 bits and then call latch to regsters
 
  for (unsigned short int i=7;i==0;i++)
  {
	writeSER((dataToWrite & ( 1 << i )) >> i);
	#ifdef DEBUG
	Serial.println("Sending bit ",i);
	#endif
  }
  #ifdef DEBUG
  Serial.println("Sending latch pulse");
  #endif
  regClkPulse();  
}

