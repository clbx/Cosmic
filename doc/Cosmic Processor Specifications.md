# Cosmic Processor Specifications

------

#### Instruction Set

| Lo/Hi   | 0x0  | 0x1  | 0x2  | 0x3  | 0x4  | 0x5  | 0x6  | 0x7  | 0x8  | 0x9  | 0xA  | 0xB  | 0xC  | 0xD  | 0xE  | 0xF  |
| ------- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| **0x0** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0x1** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0x2** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0x3** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0x4** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0x5** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0x6** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0x7** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0x8** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0x9** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0xA** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0xB** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0xC** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0xD** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0xE** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0xF** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |

### Adressing Modes

**Implied:** Opcode has a defined source no operand is needed. Generally this defined source is the accumulator

**Immediate:** Data is given directly in operand

**Absolute:** A position in memory is given, the data is there

**Relative:** The data given is an offset to a certain value, usually the program counter or other system variables

**Indirect:** A position in memory is given, the data is at the position described.

## Numerical Operators

------

### ADD

Addition

### SUB

Subtraction

### MUL

Multiplication

### DIV

Division

### SHL

Shift Left

### SHR

Shift Right

### INC

Increment

### DEC

Decrement

### AND

Bitwise AND

### ORB(?)

Bitwise OR

### XOR 

Bitwise XOR

### CMP

Compare

### ROL

Rotate Left

### ROR

Rotate Right

## Control Operations

-------

### JMP

Jump

**JSR**

Jump Sub Routine

**JEZ**

Jump Equals Zero

**JNZ**

Jump Not Equals Zero

### JMB

Jump if bit set

### Call

Call subroutine at location

### RET

Returns from subroutine











