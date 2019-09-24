# Cosmic Processor Specifications (Coz502)

-------

## Registers

-----

### General Registers

* 8x 8-bit general registers that can be used for any purpose

  **A**, **B**, **C**, **D**, **E**, **F**, **G**, **H**

  **AB**,**CD**, **EF,** **GH**, can be grouped together to be used as 4 16-bit registers. [**maybe**]

  **A** is the accumulator in 8-bit operations, A/B becomes Accumulator in 16-bit operations 

* 1x, 16-bit Stack Pointer

  Stack pointer points to a position in memory for the stack. Can be changed? Increments/Decrements with ``PUSH`` and ``POP`` 

* 1x 16-bit Program Counter

  Holds the current position in memory that is being read from. 

* 1x 8-bit Status Register

  ```
  7  bit  0
  xxxx xxxx
  |||| ||||
  |||| |||+-- (Z) Zero
  |||| ||+--- (N) Negative, high if result is negative
  |||| |+---- (C) Carry/Borrow, result if overflow
  |||| +----- (O) Overflow, result if overflows carry
  |||+------- (P) Parity, checks parity
  ||+--------  X
  |+---------  X
  +----------  X
  ```

  

## Instruction Set

----

Bolded instructions are 16-bit

| Hi\Lo    | 0x00 | 0x01 | 0x02 | 0x03 | 0x04     | 0x05    | 0x06     | 0x07     | 0x08 | 00x9 | 0x0A | 0x0B | 0x0C     | 0x0D    | 0x0E     | 0x0F     |
| -------- | ---- | ---- | ---- | ---- | -------- | ------- | -------- | -------- | ---- | ---- | ---- | ---- | -------- | ------- | -------- | -------- |
| **0x00** | NOP  | HCF  |      |      |          |         |          |          |      |      |      |      |          |         |          |          |
| **0x10** | ADD# | ADD  | ADD@ | ADDR | **ADD#** | **ADD** | **ADD@** | **ADDR** | SUB# | SUB  | SUB@ | SUBR | **SUB#** | **SUB** | **SUB@** | **SUBR** |
| **0x20** | MUL# | MUL  | MUL@ | MULR | **MUL#** | **MUL** | **MUL@** | **MULR** | DIV# | DIV  | DIV@ | DIVR | **DIV#** | **DIV** | **DIV@** | **DIVR** |
| **0x30** |      |      |      |      |          |         |          |          |      |      |      |      |          |         |          |          |
| **0x40** |      |      |      |      |          |         |          |          |      |      |      |      |          |         |          |          |
| **0x50** |      |      |      |      |          |         |          |          |      |      |      |      |          |         |          |          |
| **0x60** |      |      |      |      |          |         |          |          |      |      |      |      |          |         |          |          |
| **0x70** |      |      |      |      |          |         |          |          |      |      |      |      |          |         |          |          |
| **0x80** |      |      |      |      |          |         |          |          |      |      |      |      |          |         |          |          |
| **0x90** |      |      |      |      |          |         |          |          |      |      |      |      |          |         |          |          |
| **0xA0** |      |      |      |      |          |         |          |          |      |      |      |      |          |         |          |          |
| **0xB0** |      |      |      |      |          |         |          |          |      |      |      |      |          |         |          |          |
| **0xC0** |      |      |      |      |          |         |          |          |      |      |      |      |          |         |          |          |
| **0xD0** |      |      |      |      |          |         |          |          |      |      |      |      |          |         |          |          |
| **0xE0** |      |      |      |      |          |         |          |          |      |      |      |      |          |         |          |          |
| **0xF0** |      |      |      |      |          |         |          |          |      |      |      |      |          |         |          |          |

### Adressing Modes

**Implied:** Opcode has a defined source or destination no operand is needed

**Immediate:** Data is given directly in operand

**Absolute:** A position in memory is given, the data is there

**Relative:** The data given is an offset to a certain value.

**Indirect:** A position in memory is given, the data is at the position described.

**Extended Adressing Modes?:** For use with paired 8-bit registers for 16-bit functionality.

## Arithmetic and Logic Operations

### ADD/ADDX 

Add value with carry

Flags Affected: 

| Assembler           | Effect                 | Bytes | Opcode |
| ------------------- | ---------------------- | ----- | ------ |
| ADD #oper           | A = A + #oper          | 2     | 0x10   |
| ADD oper            | A = A + mem[oper]      | 2     | 0x11   |
| ADD @oper           | A = A + mem[mem[oper]] | 2     | 0x12   |
| ADD RX              | A = A + RX             | 2     | 0x13   |
| **16bit** ADD #oper | A = A + #oper          | 3     | 0x14   |
| **16bit** ADD oper  | A = A + mem[oper]      | 3     | 0x15   |
| **16bit** ADD @oper | A = A + mem[mem[oper]] | 3     | 0x16   |
| **16bit** ADD RXX   | A = A + RXX            | 2     | 0x27   |

### SUB

Subtraction

Flags Affected: 

| Assembler           | Effect                 | Bytes | Opcode |
| ------------------- | ---------------------- | ----- | ------ |
| SUB #oper           | A = A - #oper          | 2     | 0x18   |
| SUB oper            | A = A - mem[oper]      | 2     | 0x19   |
| SUB @oper           | A = A - mem[mem[oper]] | 2     | 0x1A   |
| SUB RX              | A = A - RX             | 2     | 0x1B   |
| **16bit** SUB #oper | A = A - #oper          | 3     | 0x1C   |
| **16bit** SUB oper  | A = A - mem[oper]      | 3     | 0x1D   |
| **16bit** SUB @oper | A = A - mem[mem[oper]] | 3     | 0x1E   |
| **16bit** SUB RXX   | A = A - RXX            | 2     | 0x1F   |

###  

### MUL

Multiplication

Flags Affected: 

| Assembler           | Effect                 | Bytes | Opcode |
| ------------------- | ---------------------- | ----- | ------ |
| MUL #oper           | A = A * #oper          | 2     | 0x20   |
| MUL oper            | A = A * mem[oper]      | 2     | 0x21   |
| MUL @oper           | A = A * mem[mem[oper]] | 2     | 0x22   |
| MUL RX              | A = A * RX             | 2     | 0x23   |
| **16bit** MUL #oper | A = A * #oper          | 3     | 0x24   |
| **16bit** MUL oper  | A = A * mem[oper]      | 3     | 0x25   |
| **16bit** MUL @oper | A = A * mem[mem[oper]] | 3     | 0x26   |
| **16bit** MUL RXX   | A = A * RXX            | 2     | 0x27   |

### 

### 

### DIV

Division, puts remainder in B **What about 16-bit?**

Flags Affected: 

| Assembler           | Effect                 | Bytes | Opcode |
| ------------------- | ---------------------- | ----- | ------ |
| DIV #oper           | A = A / #oper          | 2     | 0x28   |
| DIV oper            | A = A / mem[oper]      | 2     | 0x29   |
| DIV @oper           | A = A / mem[mem[oper]] | 2     | 0x2A   |
| DIV RX              | A = A / RX             | 2     | 0x2B   |
| **16bit** DIV #oper | A = A / #oper          | 3     | 0x2C   |
| **16bit** DIV oper  | A = A / mem[oper]      | 3     | 0x2D   |
| **16bit** DIV @oper | A = A / mem[mem[oper]] | 3     | 0x2E   |
| **16bit** DIV RXX   | A = A / RXX            | 2     | 0x2F   |

### 

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

Stops execution of the machine entirely 

| Assembler | Effect        | Bytes | Opcode |
| --------- | ------------- | ----- | ------ |
| HCF       | Stops Machine | 1     | 0x01   |

###  

## Data Handling and Memory Operations

### MOV

Move memory

### SWP

Swap register with accumulator

### PUSH

Push to stack

### POP

Pop from stack

### NOP

No Operation

Flags Affected: None

| Assembler | Effect  | Bytes | Opcode |
| --------- | ------- | ----- | ------ |
| NOP       | Nothing | 1     | 0x00   |

### 



## Pin-out

-----

**40 Pin Package** (At most)

8 Data Pins							[8]

16 Address Pins					[24]

1 Ready 								[25]

1 Interrupt line	 				  [26]

1 Halt									 [27]

1 Reset								  [28]

## References

-----

https://en.wikipedia.org/wiki/Instruction_set_architecture





