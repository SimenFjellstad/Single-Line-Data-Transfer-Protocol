# Fjellstad-Single-Line-Data-Transfer-Protocol
A data transfer protocol utilizing only a single line for transfer


Singleline Basic:
The singleline basic protocol is shorter and easier than the PRO Protocol. Basic only features 1 Byte of data, and no Datalength variable. It is created as an easier alternative to PRO. Also, we use this for low amount of data transfers.

Singleline Pro:
The singleline pro protocol is an extension of the basic. Now we have a couple of bits to tell how many bytes of data this package contains, and we can bring more payload in one package.



Group description:
Pre-amble:
Bits for "wakeup" and validating that we're at the start of a package. In NDD it is an easy pattern going "101010...". This is also considered a safety addition to the code as we won't get corrupted data from starting listening mid-package.

Address:
The adress is a code telling who this data is for. It is composed of 4 bits, meaning up to 16 listeners can be added to this system. If the address is not matching the receiver's address, the package is thrown away.

Pack Counter:
This is one of the safety features of the NDD Singleline system. The package counter is taking this "package code" and checking if this is the next one in line. If not, the package is thrown away. For this to accept wrong package, 7 consecutive packages have to be dropped.

Data Length:
The datalength bits is an indicator of how many bytes of data this package contains. The numbers are 0 indexed, so a code of 0b00 tells us that only 1 byte of data is coming. This system goes up to 4 bytes of data with the code 0b11 given.

Data:
The data bits are the payload we actually want to carry on. The data length ranges from 1-4 bytes. The format of this data varies as the recievers expect different formats.

Parity:
The parity bit is another safety feature of the NDD Singleline protocol. It is used for error detection where we count the number of 0b1 bits. And as we want to achieve an odd number one, the parity bit is et accordingly. Ex. of we have a total of 8 0b1 bits before the parity, the parity bit is set to 0b1 to achive an odd number.

Reset bits:
The reset bits' function is not that much. They are only here to help the program get some space before receiving the next package.
