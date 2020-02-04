# Overview

The Cosmic Assembler is designed to give an easier way to write programs for the Cosmic System than straight machine code. The assembly language is designed to be as easy to use as possible for the user.

Everything here is subject to change.

Some current limitations of the Assembler:
* There is no form of scope

----
## General Usage
**Python 3 must be used** ~~you should be using it anyway~~

Usage: ``python cosasm <source-file> <output-file>``
If the output file is not specified a file called ``output.bin`` will be created

### Statements
A statement consists of one valid instruction and it's operands. Only one instruction is allowed per line

```
label:
    ADD #50
    ;comment
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
 

### Pre Defined Lables:
Some labels translate to specific machine functions
the ``HPI:`` and ``LPI:`` labels will set the High and Low Priority Interrupt Vectors respectivley and the code that will run on their calls

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


To define a variable of size word
```
word myVariable = FFFF
```

To define a variable of size byte

```
byte myByte = FF
```

Variables are put into the 5k variable space between ``0xC800`` and ``0xCD00``


### Comments

Comments can be added anywhere in the code and are signified by a preceeding ``;``

```
; this does nothing
ADD #50
SUB #50
MUL #50

```


### Debug Table (WIP)

During assembly a debug table is also generated for the Cosmic GUI to read to help with debugging. 

This table has:
* Line numbers that correlate with memory location to see which instruction is being executed
* Variable values
* Detection for if the program counter does not point to a proper opcode
