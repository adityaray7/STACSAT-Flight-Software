#include <SD.h>
int a = 101;
float b=34.67;
String str="test";
File myFile;
 
void setup()
{
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
   pinMode(10, OUTPUT);
 
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
 
  myFile = SD.open("sample.txt", FILE_WRITE);
 
  if (myFile) {
    Serial.print("Writing to test.txt...");
    myFile.print(a);
    myFile.print(",");
    myFile.print(b);
    myFile.print(",");
    myFile.print(str);
    myFile.print(";");
    myFile.println("");
    myFile.close();
    Serial.println("done.");
  } else {
    Serial.println("error opening sample.txt");
  }
}
 
void loop()
{
}
