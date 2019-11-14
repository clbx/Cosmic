# Overview

The Cosmic Assembler is designed to give an easier way to write programs for the Cosmic System than straight machine code. The assembly language is designed to be as easy to use as possible for the user.

Everything here is subject to change.

Some current limitations of the Assembler:
* There is no form of scope

----
## General Usage

Usage: ``python cosasm <source-file> <output-file>``
If the output file is not included a file called ``output.bin`` will be created

### Statements
A statement consists of one valid instruction and it's operands. Only one instruction is allowed per line

```
ADD #50
```
Only a comment can be on the same line as an instruction
```
ADD #50 //Add 50
```

### Addressing Modes

The Cosmic Processor has 5 Addressing Modes:
* Implied: All information needed is known when the opcode is called.
``RET`` Will return from a subroutine and does not need any additional data
* Immediate: The data is given in the operand
``ADD #50`` will add ``0x50``
* Absolute: The operand describes a place in memory
``ADD 50`` will add whatever value is in ``0x50``
* Indirect: The operand describes a place in memory, which holds another value on where to look in memory
``ADD @50`` will look at ``0x50`` then retreieve the data at the position that is decribed there


### Labels

Labels and variables may be used anywhere throughout your code. Labels can be as long as needed, but can only consist of letters, numbers, or underscores. They cannot be already reserved word.

Labels **must** end with a colon
```
start:
    ADD #50
    SUB #50
    JMP start
```
This will set start whatever memory address ADD is at. On assembly ``start`` is replaced with that memory address and the ``JMP`` instruction will be given the memory address it neeeds to go to. 


### Data Types
There are 2 data types that cosmic supports: Byte and Word. A byte holds 8 bits, a word holds 16 bits

When a identifier is defined it can be defined as a byte or word.

```
word HighestValue = 0xF005
byte Lowest value = 0x00
```

The high and low byte of words can also be referenced

```
HighestValue.high = 0xF0
HighestValue.low = 0x05
```

### Variables and Constants

A constant cannot change, a variable is a specified place in memory that can be changed

To define a constant
```
word myConstant = 0x1010
```
To define a variable
```
var word myVariable = 0x0000
```

On assembly variables all variables will be put at the top of memory and a ``jmp`` instruction will be added to go to the beginning of the program.

If 8 bytes worth of variables are defined the memory will look like the following:

```
   00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F

00 70 0A -=  Variables Here  =-  -= Program Starts =- 
```



### Comments