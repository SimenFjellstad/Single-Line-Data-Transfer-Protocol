#include "Arduino.h"
#include "SingleLineDTP.h"

int preamblebitpattern[12] = {1,0,1,0,1,0,1,0,1,0,1,0};



SingleLineDTP::SingleLineDTP(int myAddress, int transferspeed){
    
    paritytype = odd;
    
    val = -1;
    valid = true;

    // µ seconds
    if(myAddress > -1)
        _myAddress = myAddress;
    else 
        _myAddress = 0;

    if(transferspeed > 0)
        _transferspeed = transferspeed;
    else 
        _transferspeed = 1000;
}



void SingleLineDTP::sendPackage(byte address,byte data){  
    //Preamble (Bits: 0-11)
    for(int i = 0; i < 6; i++){
        writeout(1);
        writeout(0);
    }

      //Address (Bits: 12-15)
    for(int i = 0; i < 4; i++)
        writeout(bitRead(address,i)); //MIGHT BE FLIPPED CHECK IT NEXT RUN

      //Package counter: (3 bits auto increment) (Bits: 16-18)
    for(int i = 0; i < 3; i++)
        writeout(bitRead(outgoingpackage,i)); 
    

    outgoingpackage++;
    if(outgoingpackage == 8) 
        outgoingpackage = 0;  

      //Data (Bits: 19-26)
    for(int i = 0; i < 8; i++)
        writeout(bitRead(data,i)); //MIGHT BE FLIPPED CHECK IT NEXT RUN

      //Parity (Bit: 27)
    writeout(getParity());
    if(getPackageNumber() != (paritytype % 2)){
        senderrors++;
        return;
    }

      //end reset bits (FalseBits: 28-30)
    for(int i = 0; i < 2; i++)
        writeout(0);
}



int SingleLineDTP::getPackage(){

    while(valid){
        while(map(analogRead(A1), 0,800,0, 2) == 0);  //Wait while there is no voltage on the line.
        val = map(analogRead(A1), 0, 800, -1, 1); //Get the bit.
        if(val < 0) continue;                         //Safety check
        while(map(analogRead(A1), 0,800,-1, 1) == val);
        
        if(state == 0) { //PREAMBLE
            if(val == preamblebitpattern[pos]){ //If the current input matches the preamblebit, continue
                pos++;
                if(pos > sizeof(preamblebitpattern)){
                    state++;
                    pos = 0;
                }
            }
            else{
                pos = 0;
            }
        }
        else if(state == 1){ //ADDRESS
            if(pos < 4) 
                address[pos] = val;
            else{
                int x = address[0];
                for(int i = 1; i<4; i++){
                    x<<1;
                    x+=address[i];
                }
                if(x == _myAddress){
                    state++;
                }
                else{
                    state = 0;
                    pack = 8;
                }
                pos = 0;
            }
            pos++;
        }
        else if(state == 2){ //PACKAGE COUNT
            bitWrite(data,pos,val);
            if(pos == 2){
                int x = bitRead(data,0);
                for(int i = 1; i<3; i++){
                    x<<1;
                    x += bitRead(data,0);
                }
                if(x == getPackageNumber()){
                    pack++;
                    state++;
                }
                else{
                    state = 0;
                    pack = 8;
                }
                pos = 0;
            }
            pos++;
        }   
        else if(state == 3){ //DATA
            if(pos == 0) data = 0;
                bitWrite(data,pos,val);  
            if(pos == 8)
                state = 4;
            pos++;
        }
        else if(state == 4){ //PARITY
            if(pos > 12){
                if(checkParity())
                    return data;
                
                pos = 0; 
                valid = false;
                state = 0;
            }
            pos++;
        }
    }
}



int SingleLineDTP::getPackageNumber(){
    if(pack < 7)
        return pack + 1;
    else
        return 0;
}



void SingleLineDTP::printpos(){
    //Example of print: "Pos: 3 Value: 1"
    Serial.print("Pos: ");
    Serial.print(pos);
    Serial.print(" Value: ");
    Serial.println(val);
}



bool SingleLineDTP::checkParity()
{
    int cnt = 0;
    for(int i = 0; i < 25; i++) 
        if(bitRead(data,i) == 2) 
            cnt++;

    if(cnt % 2 == paritytype % 2){ //Determine if the package is good by comparing cnt with odd or even
        completePackages++;
        return true;
    }
    else{
        errorPackages++;
        return false;
    }
}



int SingleLineDTP::getParity(){
  if(parity % 2 == 1) return odd;
  return even;
}



void SingleLineDTP::resetPackage(){
    for(int i = 0; i <= 4; i++)
        address[i] = 0;

    for(int i = 0; i <= 8; i++)
        bitWrite(data,i,0);

    state = 0;
    valid = true;
}



void SingleLineDTP::writeout(int level){
    if(level == 0){ //Gives out 2.5 volts
        digitalWrite(A0,1);
        digitalWrite(A1,0);
    }
    else if(level == 1){ //Gives out 5 Volts
        digitalWrite(A0,1);
        digitalWrite(A1,1);
        parity++;
    }
    delayMicroseconds(_transferspeed);
    digitalWrite(A0,0);
    digitalWrite(A1,0); 
    delayMicroseconds(_transferspeed);  
}
