#ifndef SingleLineDTP_h
#define SingleLineDTP_h

#include "Arduino.h"


#define even 0
#define odd 1;

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2
#define BYTE 0

class SingleLineDTP
{
  public:
    SingleLineDTP(byte myAddress, int transferspeed);
    byte _myAddress;
    int _transferspeed;
    int paritytype;
    bool _strict;
    bool DEBUG;
    int preamblebitpattern[12] = {1,0,1,0,1,0,1,0,1,0,1,0};
    int parity_memory[28];
    int data;
    int _data;
    int address[4];
    int outgoingpackage;

    //Secondary functions
    int getPackage(bool strict);
    int getPackageNumber();
    int getParityType();
    int getParity();
    byte getMyAddress();
    int getTransferSpeed();
    int getTransferLog();

    byte setMyAddress(byte myAddress);
    int setTransferSpeed();
    void sendPackage(byte address, byte data);
    void resetPackage();
    void printpos();
    void writeout(int level);
  private:
    bool checkParity();
    bool valid = true;
    int state;
    int oldstate;
    int pos;
    int val;
    int pack;
    int parity;
    int packagecount;
    int completePackages;
    int errorPackages;
    int senderrors;
};

#endif
