# VirtualComputer
 
This is a program that simulates a CPU and main memory. As input, it receives a program that is written in machine code (only hexadecimal). It will then run that program. After each instruction execution, the registers of the CPU will be printed to the console.
 
## Example Program
 
The following example program adds two numbers. It has three instructions. First, load the number 4 to the accumulator register that is in the CPU. Second, add the number 3 to the number in the accumulator register that is in the CPU. The third, and final, instruction tells the CPU that the program is finished. 

The program represented in *assembly code*:<br>
LOAD 4; <br>
ADD 3;<br>
STOP;<br>

The program represented in *machine code (hexadecimal)*:<br>
C0 0004<br>
70 0003<br>
00 0000<br>

The program represented in *machine code (binary)*:<br>
11000000 0000000000000100<br>
01110000 0000000000000011<br>
00000000 000000000000000<br>

