#include <SingleLineDTP.h>

SingleLineDTP sl(1, 1000);

void setup(){
  Serial.begin(9600);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  
  sl.setMyAddress(0);
  Serial.print("My address is:");
  Serial.print(sl.getMyAddress());

}

void loop(){
  delay(1000);
  sl.sendPackage(1,0b00110011);
}
