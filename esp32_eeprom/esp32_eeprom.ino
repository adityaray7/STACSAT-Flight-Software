#include <EEPROM.h>
#define EEPROM_SIZE 12

void setup() {
  Serial.begin(115200);
  Serial.println(F("Initialize System"));
  EEPROM.begin(EEPROM_SIZE);

  int address = 0;
  int boardId = 18;
  EEPROM.write(address, boardId);
  address += sizeof(boardId); 

  float param = 26.5;
  EEPROM.writeFloat(address, param);
  EEPROM.commit();

  address = 0;
  int readId;
  readId = EEPROM.read(address); 
  Serial.print("Read Id = ");
  Serial.println(readId);
  address += sizeof(readId);

  float readParam;
  EEPROM.get(address, readParam); 
  Serial.print("Read param = ");
  Serial.println(readParam);

  EEPROM.end();
}

void loop() {}
