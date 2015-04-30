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
    bool DEBUG;
    int preamblebitpattern[];
    int data;
    int address[4];

    //Secondary functions
    int getPackage();
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
    bool valid;
    int state;
    int pos;
    int val;
    int pack;
    int parity;
    int packagecount;
    int completePackages;
    int errorPackages;
    int senderrors;
    int outgoingpackage;
};

#endif
