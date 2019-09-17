# Cosmic Processor Specifications (Coz502)

-------

## Registers

-----

### General Registers

* 8, 8-bit general registers that can be used for any purpose

  **A**, **B**, **C**, **D**, **E**, **F**, **G**, **H**

  **C/D**, **E/F,** **G/H**, can be grouped together to be used as 3 16-bit registers. [**maybe**]

  **A** is the accumulator, **B** is specifically not grouped to be used by instructions as a return or a check (many instructions use B to check conditions or values)

* 1, 16-bit Stack Pointer

  Stack pointer points to a position in memory for the stack. Can be changed? Increments/Decrements with ``PUSH`` and ``POP`` 

* 1, 16-bit Program Counter

  Holds the current position in memory that is being read from. 

* 1, 8-bit status register

  ```
  7  bit  0
  xxxx xxxx
  |||| ||||
  |||| |||+-- 
  |||| ||+---
  |||| |+----
  |||| +-----
  |||+-------
  ||+--------
  |+---------
  +----------
  ```

  

## Instruction Set

----

| Hi\Lo    | 0x00 | 0x01 | 0x02 | 0x03 | 0x04 | 0x05 | 0x06 | 0x07 | 0x08 | 00x9 | 0x0A | 0x0B | 0x0C | 0x0D | 0x0E | 0x0F |
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

## Arithmetic and Logic Operations

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

### ASL

Arithmetic Shift Left

### ASR

Arithmetic Shift Right



## Control Flow Operations

### JMP

Jump

**JEZ**

Jump Equals Zero

**JNZ**

Jump Not Equals Zero

### JCC

Jump on Carry Clear

### JCS

Jump on Carry Set

### JNR

Jump on negative result

### INT

Interrupt handling

### SID

Set interrupt disable

### CALL

Call subroutine at location

### RET

Returns from subroutine

### HCF

Halt and Catch Fire

## Data Handling and Memory Operations

### MOV

Move memory

### PUSH

Push to stack

### POP

Pop from stack

### NOP

No Operation



## Pin-out

-----





## References

-----

https://en.wikipedia.org/wiki/Instruction_set_architecture





