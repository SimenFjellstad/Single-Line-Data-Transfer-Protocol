#include <SingleLineDTP.h>

SingleLineDTP sl(0, 5);

void setup(){
  Serial.begin(9600);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  
  sl.setMyAddress(0);

}

void loop(){
  //delay(5);
  uint8_t data = random(512);
  Serial.print(sl.outgoingpackage);
  Serial.print(" | ");
  Serial.println(data, DEC);
  sl.sendPackage(0b1001,data);
}
