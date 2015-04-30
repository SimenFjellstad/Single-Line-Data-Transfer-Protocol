#include <SingleLineDTP.h>

SingleLineDTP sl(0,500);

void setup(){
  pinMode(A2, INPUT);
  Serial.begin(9600);
  sl.setMyAddress(0);
}

void loop(){
  //delay(50);
  //Serial.println(analogRead(A2));

  sl.getPackage();
}

