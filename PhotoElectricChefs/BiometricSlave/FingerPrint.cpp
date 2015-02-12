#include "Arduino.h"
#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
#include <FingerPrint.h>
SoftwareSerial mySerial(4, 5);
Adafruit_Fingerprint finger=Adafruit_Fingerprint(&mySerial); 
uint8_t fingerprintdata[79];

uint8_t fingerprint_init()
{
	finger.begin(57600);
	 
 }
uint8_t fingerprint_check()
{
	  if (finger.verifyPassword())
	  {
		Serial.println("Found fingerprint sensor!");
		return 1;
	  } 
	  else 
	  {
		Serial.println("Did not find fingerprint sensor :(");
		return 0;
	  }
}
uint8_t uploadtemp(uint8_t id)
{
  Serial.println("Type in the ID # you want to upload the template...");

  uint8_t p = finger.uploadtemp();
  for (int i=6;i<79;i++)
  {
      Serial.print("0x");
      Serial.print(fingerprintdata[i], HEX);
      Serial.print(", ");
		if ((i%16) == 0 )
		Serial.println();
		mySerial.write((uint8_t)(fingerprintdata[i]));
  }
  Serial.println(p, HEX);
  
   p= finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }      
}

uint8_t getFingerprintID() 
{
  Serial.println("Put your finger to search");
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
 // if (p != FINGERPRINT_OK)  return -1;
  if (p == FINGERPRINT_NOTFOUND)  return 2;
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}


void tempnumber()
{
int count=0;
count = finger.getTemplateCount();
Serial.println(count, HEX);
}

void image()
{
uint8_t p = -1;
p=finger.SendImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.print("template "); Serial.println(" transferring");
      break;
   default:
      Serial.print("Unknown error "); Serial.println(p);
      return;
  }

    for (int i = 0; i < 550; i++)
    {
      Serial.print("0x");
      Serial.print(mySerial.read(), HEX);
      Serial.print(", ");
    }
    Serial.println();
}

void showtemplate(uint8_t id)
{
 Serial.println("Type in the ID # you want to show finger template...");  
  Serial.print("Enrolling ID #");
  Serial.println(id);
  
  while (!  uploadFingerpintTemplate(id) );
}


uint8_t uploadFingerpintTemplate(uint8_t id)
{
 uint8_t p = finger.loadModel(id);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.print("template "); Serial.print(id); Serial.println(" loaded");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    default:
      Serial.print("Unknown error "); Serial.println(p);
      return p;
  }

  // OK success!

  p = finger.getModel();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.print("template "); Serial.print(id); Serial.println(" transferring");
      break;
   default:
      Serial.print("Unknown error "); Serial.println(p);
      return p;
  }
  delay(100);
  Serial.print("Count of data rev: ");
  Serial.println(mySerial.available(),DEC);
 
  //Template data seems to be 78 bytes long?  This prints out 5 lines of 16 bytes (so there's 2 extra FF bytes at the end)
    for (int i = 0; i < 79; i++)
    {
      Serial.print("0x");
      
	  fingerprintdata[i]= mySerial.read();
 
      Serial.print(fingerprintdata[i], HEX);
      Serial.print(", ");
     if ((i%16) == 0 )
     Serial.println();
	}
  }
 uint8_t deleteFingerprint(uint8_t id) {
  uint8_t p = -1;
  
  p = finger.deleteModel(id);

  if (p == FINGERPRINT_OK) {
    Serial.println("Deleted!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not delete in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.print("Unknown error: 0x"); Serial.println(p, HEX);
    return p;
  }   
}

uint8_t getFingerprintEnroll1() {
  uint8_t p = -1;
  Serial.println("Waiting for valid finger to enroll");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  Serial.println("Remove finger");
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  delay(1000);
  return 1;
}
uint8_t getFingerprintEnroll2() {
  uint8_t p = -1;

  Serial.println("Place same finger again");
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.print(".");
      break;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      break;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      break;
    default:
      Serial.println("Unknown error");
      break;
    }
  }

  // OK success!

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
}
uint8_t storeFingerprint(uint8_t id) { 
  uint8_t p = -1;
  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
}