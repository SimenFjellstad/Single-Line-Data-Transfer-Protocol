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
    SingleLineDTP(int myAddress, int transferspeed);

    //Secondary functions
    int getPackageNumber();
    int paritytype;
    int getParityType();
    int getParity();
    bool DEBUG;
	int _myAddress;
	int getMyAddress();
	int setMyAddress();
    void printpos();
  	int _transferspeed;
  	int getTransferSpeed();
  	int setTransferSpeed();
  	int getTransferLog();
    int preamblebitpattern[];
    int data;
    int address[4];

    void sendPackage(byte address, byte data);
    int getPackage();
    void resetPackage();
  private:
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
    void writeout(int level);
    bool checkParity();
};

#endif
