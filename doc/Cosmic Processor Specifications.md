# Cosmic Processor Specifications

------

#### Instruction Set

| HiLoi    | 0x00 | 0x01 | 0x02 | 0x03 | 0x04 | 0x05 | 0x06 | 0x07 | 0x08 | 00x9 | 0x0A | 0x0B | 0x0C | 0x0D | 0x0E | 0x0F |
| -------- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- | ---- |
| **0x00** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0x10** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0x20** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0x30** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0x40** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0x50** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0x60** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0x70** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0x80** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0x90** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0xA0** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0xB0** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0xC0** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0xD0** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0xE0** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |
| **0xF0** |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |

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

### OR

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

**JEZ**

Jump Equals Zero

**JNZ**

Jump Not Equals Zero

### JMB

Jump if bit set

### CALL

Call subroutine at location

### RET

Returns from subroutine

## Memory Management

----

### MOV

Move memory

### PUSH

Push to stack

### POP

Pop from stack















