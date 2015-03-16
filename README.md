# Single-Line-Data-Transfer-Protocol
A data transfer protocol utilizing only a single line for transfer. The data signal type is based upon a Return-to-zero coding, although it is positively flipped, so we have two positive voltages, instead of a positive and a negative voltage. These voltages are as following:  
5V: Defines a logic 1 in signal.  
2.5V: Defines a logic 0 in signal.  
0V: Idle, doesn't affect signal, this is white space.  

<i>The Single line system uses two base packages:</i>  
<b>Singleline Basic:</b>  
The singleline basic protocol is shorter and easier than the PRO Protocol. Basic only features 1 Byte of data, and no Datalength variable. It is created as an easier alternative to PRO. Also, we use this for low amount of data transfers.

<b>Singleline Pro:</b>  
The singleline pro protocol is an extension of the basic. Now we have a couple of bits to tell how many bytes of data this package contains, and we can bring more payload in one package.



##<b>Group description:</b>  
<b>Pre-amble:</b>  
Bits for "wakeup" and validating that we're at the start of a package. In NDD it is an easy pattern going "101010...". This is also considered a safety addition to the code as we won't get corrupted data from starting listening mid-package.

<b>Address:</b>  
The adress is a code telling who this data is for. It is composed of 4 bits, meaning up to 16 listeners can be added to this system. If the address is not matching the receiver's address, the package is thrown away.

<b>Pack Counter:</b>  
This is one of the safety features of the NDD Singleline system. The package counter is taking this "package code" and checking if this is the next one in line. If not, the package is thrown away. For this to accept wrong package, 7 consecutive packages have to be dropped.

<b>Data Length:</b>  
The datalength bits is an indicator of how many bytes of data this package contains. The numbers are 0 indexed, so a code of 0b00 tells us that only 1 byte of data is coming. This system goes up to 4 bytes of data with the code 0b11 given.

<b>Data:</b>  
The data bits are the payload we actually want to carry on. The data length ranges from 1-4 bytes. The format of this data varies as the recievers expect different formats.

<b>Parity:</b>  
The parity bit is another safety feature of the NDD Singleline protocol. It is used for error detection where we count the number of 0b1 bits. And as we want to achieve an odd number one, the parity bit is et accordingly. Ex. of we have a total of 8 0b1 bits before the parity, the parity bit is set to 0b1 to achive an odd number.

<b>Reset bits:</b>  
The reset bits' function is not that much. They are only here to help the program get some space before receiving the next package.


<table>
		<tbody>
			<tr>
				<th colspan="31"><b>Singleline Basic</b></th>
			</tr>
			<tr>
				<td colspan="12">Pre-amble <br> 12 bits</td>
				<td colspan="4">Address<br>4 bits</td>
				<td colspan="3">Pack Counter<br> 3 bit</td>
				<td colspan="8">Data<br>8 bits</td>
				<td colspan="1">Parity<br>1 bit</td>
				<td colspan="3">Reset <br> 3 bits (0)</td>
			</tr>
			<tr>
				<td>1</td>
				<td>0</td>
				<td>1</td>
				<td>0</td>
				<td>1</td>
				<td>0</td>
				<td>1</td>
				<td>0</td>
				<td>1</td>
				<td>0</td>
				<td>1</td>
				<td>0</td>

				<td>0</td>
				<td>0</td>
				<td>0</td>
 				<td>1</td>

				<td>1</td>
				<td>0</td>
 				<td>1</td>
				<td>1</td>
				<td>0</td>
 				<td>1</td>
				<td>0</td>
				<td>1</td>

 				<td>0</td>
 				<td>0</td>
 				<td>1</td>

 				<td>0</td>

 				<td>0</td>
 				<td>0</td>
 				<td>0</td>

			</tr>
			<tr>
				<td colspan="12">NaN</td>
				<td colspan="4">0x1</td>
				<td colspan="3">0x5</td>
				<td colspan="8">0xA9</td>
				<td colspan="1">0</td>
				<td colspan="3">Zero</td>
			</tr>
		</tbody> 
	</table>
	<br>  
<table border="solid"style="text-align:center">
	<tbody>
		<tr>
			<th colspan="49"><b>Singleline Pro<br>Coordinate example</b></th>
		</tr>
		<tr>
			<td colspan="12">Pre-amble <br> 12 bits</td>
			<td colspan="4">Address<br>4 bits</td>
			<td colspan="3">Pack Counter<br> 3 bit</td>
			<td colspan="2">Data length<br> 2 bit</td>
			<td colspan="24">Data<br>3 Byte</td>
			<td colspan="1">Parity<br>1 bit</td>
			<td colspan="3">Reset <br> 3 bits (0)</td>
		</tr>
		<tr>
			<td>1</td>
			<td>0</td>
			<td>1</td>
			<td>0</td>
			<td>1</td>
			<td>0</td>
			<td>1</td>
			<td>0</td>
			<td>1</td>
			<td>0</td>
			<td>1</td>
			<td>0</td>

			<td>0</td>
			<td>0</td>
			<td>0</td>
 			<td>1</td>

			<td>0</td>
			<td>0</td>
 			<td>1</td>

 			<td>1</td>
 			<td>0</td>

			<td>0</td>
			<td>0</td>
 			<td>0</td>
			<td>0</td>
			<td>0</td>
			<td>0</td>
			<td>0</td>
 			<td>1</td>
			<td>0</td>
			<td>0</td>
 			<td>1</td>
			<td>0</td>
			<td>0</td>
			<td>0</td>
			<td>0</td>
 			<td>1</td>
			<td>0</td>
			<td>0</td>
 			<td>1</td>
			<td>0</td>
			<td>1</td>
 			<td>1</td>
 			<td>0</td>
 			<td>0</td>

 			<td>0</td>

 			<td>0</td>
 			<td>0</td>
 			<td>0</td>

		</tr>
		<tr>
			<td colspan="12">NaN</td>
			<td colspan="4">0x1</td>
			<td colspan="3">0x1</td>
			<td colspan="2">0b10</td>
			<td colspan="8">Internal: 0x1</td>
			<td colspan="8">X: 0x21</td>
			<td colspan="8">Y: 0x2C</td>
			<td colspan="1">0</td>
			<td colspan="3">Zero</td>
		</tr>
	</tbody> 
</table>
