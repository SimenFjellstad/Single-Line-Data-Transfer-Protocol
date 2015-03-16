//Author: Simen Fjellstad
//Description:
//  Transmitter sending serial data.

//Data Transfer Settings:
int paritytype = 1;   //This is tested like this "if(int x % 2 == paritytype % 2);" this means that if putting an odd number,
                      //parity checking will check for odd numbers, same with even numbers.
int preamblebits[] = {1,0,1,0,1,0,1,0,1,0,1,0};
int bitdelay = 10000;

//Test package
int address[] = {0,0,0,1};
int data[] = {0,0,1,0,0,1,0,0};
int parity = 0;
int senderrors;

int k = 0;

int incomingByte;

void setup() {
  pinMode(A0, OUTPUT); //Data 0
  pinMode(A1, OUTPUT); //Data 1
  Serial.begin(9600);
}

void loop() {
  if(createPackage() == 0) sendPackage();
}

// This is the main function to be edited by the user.
// Just edit what address and data is, then call sendPackage, and you're good!
int createPackage(){
  incomingByte = -1; 
  
  // send data only when you receive data:
  if (Serial.available() > 0) {
    incomingByte = Serial.read();

    // say what you got:
    Serial.print("I received: ");
    Serial.println(incomingByte, HEX);
  }
  if(incomingByte >= 0) return 0;
  else return 1;
}

int sendPackage(){  
  //Preamble (Bits: 0-11)
  for(int i = 0; i < 6; i++){
    writeout(1);
    writeout(0);
  }
/*
  //Address (Bits: 12-15)
  for(int i = 0; i < 4; i++)
    writeout(address[i]);


  //Package counter: (3 bits auto increment)
  for(int i = 0; i < 3; i++){  
    writeout(bitRead(k,i));
  }
  k++;
  if(k == 8) k = 0;  
  
  
  //Data (Bits: 16-23)
  for(int i = 0; i < 8; i++)
    writeout(data[i]);


  //Parity (Bit: 24)
  writeout(getParity());
  if(getParity() != (paritytype % 2)) return 0x1;
  
  
  //end reset bits (FalseBits: 25-27)
  for(int i = 0; i < 2; i++)
    writeout(0);
 */
  return 0;
}



int getParity(){
  if(parity % 2 == 1) return 1;
  return 0;
}

void resetPackage(){
  for(int i = 0; i <= 4; i++)
    address[i] = 0;
    
  for(int i = 0; i <= 8; i++)
    data[i] = 0;
    
  parity = 0;
}

void writeout(int level){
  if(level == 0){ //Gives out 2.5 volts
    digitalWrite(A0,1);
    digitalWrite(A1,0);
  }
  else if(level == 1){ //Gives out 5 Volts
    digitalWrite(A0,1);
    digitalWrite(A1,1);
    parity++;
  }
  delayMicroseconds(bitdelay);
  digitalWrite(A0,0);
  digitalWrite(A1,0); 
  delayMicroseconds(bitdelay);  
}
