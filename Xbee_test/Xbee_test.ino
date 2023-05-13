#include<SoftwareSerial.h>

SoftwareSerial XBee(10,11);
void setup()
{
  XBee.begin(9600);
  Serial.begin(9600);
}

void loop()
{
    XBee.write("Transmission Test");
    Serial.print("Serial printing");
    delay(1000);
    
    
}
