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

int sendPackage(){  
  //Preamble (Bits: 0-11)
  for(int i = 0; i < 6; i++){
    writeout(1);
    writeout(0);
  }
  //Address (Bits: 12-15)
  for(int i = 0; i < 4; i++)
    writeout(address[i]);


  //Package counter: (3 bits auto increment) (Bits: 16-18)
  for(int i = 0; i < 3; i++){  
    writeout(bitRead(k,i));
  }
  k++;
  if(k == 8) k = 0;  
  
  
  //Data (Bits: 19-26)
  for(int i = 0; i < 8; i++)
    writeout(data[i]);


  //Parity (Bit: 27)
  writeout(getParity());
  if(getParity() != (paritytype % 2)) return 0x1;
  
  
  //end reset bits (FalseBits: 28-30)
  for(int i = 0; i < 2; i++)
    writeout(0);
    
  return 0;
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
