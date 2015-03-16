//Author: Simen Fjellstad
//License: All rights reserved

//Data Transfer Settings:
boolean DEBUG = true;
int paritytype = 1;   //This is tested like this "if(int x % 2 == paritytype % 2);" this means that if putting an odd number,
                      //parity checking will check for odd numbers, same with even numbers.
int preamblebits[] = {1,0,1,0,1,0,1,0,1,0,1,0};
int myAddress = 0b0001;


//Default values & initialization:
int* address = new int[4];
int* data = new int[8];

boolean valid = true;
int state = 0;
int pos = 0;
int val = -1;
int pack = 0;

//System Counters:
int packagecount = 0;
int completePackages = 0;
int errorPackages = 0;


void setup()
{
  //PinModes
  Serial.begin(9600); //Open serial port for communication using serialport
  pinMode(A1, INPUT); //Set port A2 to INPUT mode.
}

void loop()
{ 
  //Test program getting and printing packages to serialport:
  Serial.println("Next package:");
  resetpackage();
  getPackage();
}

void getPackage()
{
  while(valid){
    while(map(analogRead(A1), 0,800,0, 2) == 0);  //Wait while there is no voltage on the line.
    val = map(analogRead(A1), 0, 800, -1, 1); //Get the bit.
    if(val < 0) continue;                         //Safety check
    while(map(analogRead(A1), 0,800,-1, 1) == val);
    
    printpos(); //Prints the current position to the serial port  
    
    if(state == 0) { //PREAMBLE
      if(val == preamblebits[pos]){ //If the current input matches the preamblebit, continue
        pos++;
        if(pos > sizeof(preamblebits)){
          state++;
          pos = 0;
        }
      }
      else{
        pos = 0;
      }
    }
    else if(state == 1){ //ADDRESS
      if(pos < 4) address[pos] = val;
      else{
        int x = address[0];
        for(int i = 1; i<4; i++){
          x<<1;
          x+=address[i];
        }
        if(x == myAddress){
          state++;
          pos = 0;
        }
        else{
          state = 0;
          pos = 0;
          pack = 8;
        }
      }
      pos++;
    }
    else if(state == 2){ //PACKAGE COUNT
      data[pos] = val;
      if(pos == 2){
        int x = data[0];
        for(int i = 1; i<3; i++){
          x<<1;
          x+=data[i];
        }
        if(x == getCurrentPack()){
          pack++;
          state++;
          pos = 0;
        }
        else{
          state = 0;
          pos = 0;
          pack = 8;
        }
      }
      pos++;
    }
    else if(state == 3){ //DATA
      if(pos == 0) memset(data,0,sizeof(data));
      data[pos] = val;  
      if(pos == 8) {
        state = 4;
      }
      pos++;
    }
    else if(state == 4){ //PARITY
     if(pos > 12){
        checkParity();
        pos = 0; 
        valid = false;
        state = 0;
      }
      pos++;
    }
  }
}



int getCurrentPack(){
  if(pack < 7)return pack+1;
  else return 0;
}




void printpos(){
  //Example of print: "Pos: 3 Value: 1"
  Serial.print("Pos: ");
  Serial.print(pos);
  Serial.print(" Value: ");
  Serial.println(val);
}

void checkParity()
{
  int cnt = 0;
  for(int i = 0; i < 25; i++) 
    if(data[i] == 2) 
      cnt++;
    
  
  if(cnt % 2 == paritytype % 2) //Determine if the package is good by comparing cnt with odd or even
    completePackages++;
  else
    errorPackages++;
    
    if(DEBUG){
      Serial.print("Good: ");
      Serial.println(completePackages);
      Serial.print("Bad: ");
      Serial.println(errorPackages);
    }
}



void resetpackage(){
  for(int i = 0; i < 25; i++)
    data[i] = 0;
  
  state = 0;
  valid = true;
}
