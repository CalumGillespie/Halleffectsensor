/*
  SD card read/write
*/
#include <RTClib.h>
RTC_DS3231 rtc;

#include <SPI.h>
#include <SD.h>

File myFile; 

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

const int chipSelect = 10;          // change this to match your SD shield or module; Uno=4, Mega=53

int hallEffect = A1;                // hall effect senser pin
char inputCommand ;                // a string to hold incoming data
boolean inputComplete = false;
const int button = 4;              // data record button

bool button_status = LOW;          // default data record button position
bool SD_present = HIGH;            // default SD card status

String filename = "file_0.txt";

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


}

void loop() {

  // make the sensors's pin an input:
  pinMode(hallEffect, INPUT);
  int sensorState = analogRead(hallEffect);


  // SETUP RTC MODULE
  Serial.print("Initializing RTC...");
  if (!rtc.begin()) {
    Serial.println(F("Couldn't find RTC"));
    delay(500);
  }

  // When time needs to be set on a new device, or after a power loss
  // Set the RTC to the date & time this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  // SETUP SD CARD
  Serial.print("Initializing SD card...");
  if (!SD.begin(chipSelect) or !card.init(SPI_HALF_SPEED, chipSelect) or !volume.init(card)) {
    
    // Identify problem
    if (!card.init(SPI_HALF_SPEED, chipSelect)) {
      Serial.println("card not detected!");
      delay(500);
    }

    if (!volume.init(card)) {
      Serial.println("FAT 16/32 partition not detected!");
      delay(500);
    }
  else{Serial.println("SD card initialization done.");}


  // creating a string of values for hall effect sensor readings
   



    // If record button is released 
    if(digitalRead(button) == HIGH){   // button released (internal pullup)

      Serial.println("stop");
      if(button_status){               // if newly released
        Serial.println("closing file");// stop recording 
        myFile.close();
        button_status = LOW;
      }
     }
      
  void loop () {
    // If button is pressed 
    else{                              
  
      if(!button_status){                               // if newly pressed
        button_status = HIGH;
        for(int i=0; i<100; i++){                       // check for exting files of format "file_x.txt"
          Serial.println("file_" + String(i) + ".txt");
          if(SD.exists("file_" + String(i) + ".txt")){
            Serial.println(" exists");
          }
          else{
            filename = "file_" + String(i) + ".txt";   // when an unused file name is found
            Serial.println("new_file_name"); 
            myFile = SD.open(filename, FILE_WRITE);    // open the file
            myFile.println();
            // write column headings
            myFile.println("Date, Time, ms_from_start, sensorState");
            break;  
          }
          //delay(1000);
        }
       }
      
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

      
      for(int i=0; i<n_sliders; i++){
        myFile.print(sensorState[i]);    // write sensor val to SD card

    
      }
      myFile.println();

 
