#include "Arduino.h"
#include "SingleLineDTP.h"

SingleLineDTP::SingleLineDTP(byte myAddress, int transferspeed){
    paritytype = even;
    
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


byte SingleLineDTP::setMyAddress(byte myAddress){
    if(myAddress > -1 && myAddress < 16){
            _myAddress = myAddress;
            return 1;
        }
    
    _myAddress = 0;
    return 1;
}
byte SingleLineDTP::getMyAddress(){
   return _myAddress;
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
    for(int i = 7; i >= 0; i--){
        writeout(bitRead(data,i));
    }

      //Parity (Bit: 27)
    writeout(getParity());
    if(getPackageNumber() != (paritytype % 2)){
        senderrors++;
        return;
    }

      //end reset bits (FalseBits: 28-30)
    for(int i = 0; i < 4; i++)
        writeout(0);
}



int SingleLineDTP::getPackage(bool strict){
	_strict = strict;
	int x;
    valid = true;
    state = 0;
    oldstate = 1;
    parity_memory[28] = {0};

    while(valid){
        while(map(analogRead(A2), 0,850,-1, 1) < 0);  //Wait while there is no voltage on the line.
        val = map(analogRead(A2), 0, 850, -1, 1); //Get the bit.
        if(val < 0) continue;                         //Safety check
        while(map(analogRead(A2), 0,850,-1, 1) == val);

        if(oldstate != state){
            oldstate = state;
        } 
        
        if(state == 0)
        	parity_memory[pos] = val;
        else if(state == 1)
        	parity_memory[pos+12] = val;
        else if(state == 2)
        	parity_memory[pos+16] = val;
        else if(state == 3)
        	parity_memory[pos+19] = val;
        else if(state == 4)
        	parity_memory[pos+27] = val;

        if(state == 0) { //PREAMBLE
            if((byte)val == (byte)preamblebitpattern[pos]){ //If the current input matches the preamblebit, continue
                pos++;
                if(pos > 11){
                    state++;
                    pos = 0;
                }
            }
            else{
                pos = 0;
            }
        }
        else if(state == 1){ //ADDRESS
            if(pos == 0) x = 0;
            bitWrite(x,pos,val);
            if(pos == 3){               	
                if(x == _myAddress)
                    state++;
                else{
                    state = 0;
                    pack = 8;
                	x = 0;
                }
                pos = 0;
                continue;
            }
            pos++;
        }
        else if(state == 2){ //PACKAGE COUNT
            if(pos == 0) x = 0;
            bitWrite(x,pos,val);
            if(pos == 2){
                if(_strict){
                	//Serial.print(x);
                	if(x == getPackageNumber()){
                	pack++;
                	if(pack > 7) pack = 0;
                	} 
                	else{
                		pack = 7;
                		return -2; //ERROR: PACKAGE DROPPED
                	} 
                }                
                state++;	
                pos = 0;
                continue;
            }
            pos++;
        }   
        else if(state == 3){ //DATA
            if(pos == 0) data = 0;
            bitWrite(data,7-pos,val);  
            if(pos == 7){
                state = 4;
                pos = 0;
                _data = data;
                continue;
            }
            pos++;
        }
        else if(state == 4){ //PARITY
            if(checkParity()){                
                pos = 0; 
                valid = false;
                state = 0;
                return _data;
            }
            pos = 0;
            state = 0;
        }
    }
    return 0;
}



int SingleLineDTP::getPackageNumber(){
    if(pack < 7) return pack + 1;
    else return 0;
}



void SingleLineDTP::printpos(){
    //Example of print: "Pos: 3 Value: 1"
    Serial.print(pos);
    Serial.print(" : ");
    Serial.println(val);
}



bool SingleLineDTP::checkParity(){
    int cnt = 0;
    for(int i = 0; i < sizeof(parity_memory); i++) 
        if(parity_memory[i] == 1) 
            cnt++;
    parity_memory[28] = {0};

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
    delay(_transferspeed);
    digitalWrite(A0,0);
    digitalWrite(A1,0); 
    delay(_transferspeed);  
}
