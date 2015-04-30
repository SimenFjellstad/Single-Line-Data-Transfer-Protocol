#include <SingleLineDTP.h>

SingleLineDTP sl(0, 5);
int k = 0;
void setup() {
  Serial.begin(9600);
  pinMode(A2, INPUT);

  sl.setMyAddress(0b1001);
}

void loop() {
  Serial.print(k);
  int x = sl.getPackage(true);
  if(x >= 0){
    Serial.print(" | ");
    Serial.println(x);
  }
  else if(x = -2) Serial.println(" E:PD"); //PACKAGE DROP
  
  k++;
}

