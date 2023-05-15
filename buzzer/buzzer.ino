int freq = 2000;
int channel = 0;
int resolution = 8;



void buzz() {
  ledcWriteTone(channel, freq);
  ledcWrite(channel, 255);
  delay(1000);
}

void setup() {
  
  Serial.begin(115200);
  ledcSetup(channel, freq, resolution);
  ledcAttachPin(12, channel);
  ledcWriteTone(channel, 0);
  ledcWrite(channel, 0);
  
}
  
void loop() {

  int Choice = Serial.parseInt();
  if (Choice>5.5){
    Serial.print("Buzzing");
    buzz();
  }
}
