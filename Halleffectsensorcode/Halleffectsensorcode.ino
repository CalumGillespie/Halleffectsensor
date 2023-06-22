#include <SPI.h>
#include <SD.h>

const int analogPin = A1;
const int buttonPin = 3;
const int chipSelectPin = 4;

File dataFile;
bool isRecording = false;
unsigned int fileCounter = 0;
const int filenameMaxSize = 20;
char filename[filenameMaxSize];

void setup() {
  Serial.begin(9600);

  pinMode(buttonPin, INPUT_PULLUP);

  // Initialize SD card
  if (!SD.begin(chipSelectPin)) {
    Serial.println("SD card initialization failed.");
    return;
  }

  Serial.println("SD card initialized.");
}

void loop() {
  if (digitalRead(buttonPin) == LOW && !isRecording) {
    // Button is pressed, start recording
    isRecording = true;
    createFilename();
    Serial.print("Recording started. File: ");
    Serial.println(filename);
    openFile();
  }

  if (isRecording) {
    // Read analog sensor value
    int sensorValue = analogRead(analogPin);

    // Print value to serial monitor
    Serial.println(sensorValue);

    // Write value to file
    if (dataFile) {
      dataFile.println(sensorValue);
    }
  }

  // Check if the button is released to stop recording
  if (digitalRead(buttonPin) == HIGH && isRecording) {
    isRecording = false;
    Serial.println("Recording stopped.");
    closeFile();
    fileCounter++;
  }

  // Wait for a short duration
  delay(1000);
}

void createFilename() {
  snprintf(filename, filenameMaxSize, "data_%03d.txt", fileCounter);
}

void openFile() {
  dataFile = SD.open(filename, FILE_WRITE);
  if (!dataFile) {
    Serial.println("Error opening file.");
    return;
  }
  Serial.println("File opened successfully.");
}

void closeFile() {
  // Close the file
  dataFile.close();
  Serial.println("File closed.");
}

 
