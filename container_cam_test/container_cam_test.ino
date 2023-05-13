int trig = 4;

void setup() {                
  pinMode(trig, OUTPUT);
  digitalWrite(trig, HIGH); 
}


void loop() {
  digitalWrite(trig, LOW);
  delay(1000);               
  digitalWrite(trig, HIGH);
  delay(10000);               
  
}
