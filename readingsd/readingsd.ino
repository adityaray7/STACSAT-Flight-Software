#include <SD.h>
 
File myFile;
void setup()
{
  Serial.begin(9600);
  Serial.print("Initializing SD card..");
   pinMode(10, OUTPUT);
 
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
 
  myFile = SD.open("sample.txt");
  if (myFile) {
    Serial.println("sample.txt:");
 
    while (myFile.available()) {
      Serial.write(myFile.read());
    }
    // close the file:
    myFile.close();
  } else {
    Serial.println("error opening test.txt");
  }
}
 
void loop()
{
}
