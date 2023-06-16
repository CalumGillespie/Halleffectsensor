/*
  SD card read/write
*/
#include <RTClib.h>
RTC_DS3231 rtc;

#include <SPI.h>
#include <SD.h>

File myFile; 

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


}

// the loop routine runs over and over again forever:
void loop() {

  // digital pin 2 has a hall effect sensor attached to it. Give it a name:
int hallEffect = 2;

  // make the sensors's pin an input:
  pinMode(hallEffect, INPUT);
  // read the input pin:
  int sensorState = digitalRead(hallEffect);
  // print out the state of the button:
  Serial.println(sensorState);
  delay(1);  // delay in between reads for stability




  // SETUP RTC MODULE
  Serial.print("Initializing RTC...");
  if (!rtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
    delay(500);
  }

  // When time needs to be set on a new device, or after a power loss
  // Set the RTC to the date & time this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  

  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    while (1);
  }
  Serial.println("initialization done.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  myFile = SD.open("test.txt", FILE_WRITE);
  
  DateTime now = rtc.now();

      myFile.print(now.year(), DEC);     // write date to SD card
      myFile.print("-");
      myFile.print(now.month(), DEC);
      myFile.print("-");
      myFile.print(now.day(), DEC);
      myFile.print(" , ");


      myFile.print(now.hour(), DEC);     // write time to SD card
      myFile.print("-");
      myFile.print(now.minute(), DEC);
      myFile.print("-");
      myFile.print(now.second(), DEC);
      myFile.print(" , ");
      
                         
      unsigned long Time = millis();     // write ms since start of recording to SD card
      myFile.print(Time);                
      myFile.print(" , ");
  // if the file opened okay, write to it:
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.println(sensorState);
    // close the file:
    myFile.close();
    Serial.println("done.");
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }

  // re-open the file for reading:
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("test.txt:");

    // read from the file until there's nothing else in it:
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

