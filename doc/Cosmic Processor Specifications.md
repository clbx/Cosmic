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
| **0x00** | NOP  | HCF  | PUSH | POP  | SWP      |         |          |          |      |      |      |      |          |         |          |          |
| **0x10** | ADD# | ADD  | ADD@ | ADDR | **ADD#** | **ADD** | **ADD@** | **ADDR** | SUB# | SUB  | SUB@ | SUBR | **SUB#** | **SUB** | **SUB@** | **SUBR** |
| **0x20** | MUL# | MUL  | MUL@ | MULR | **MUL#** | **MUL** | **MUL@** | **MULR** | DIV# | DIV  | DIV@ | DIVR | **DIV#** | **DIV** | **DIV@** | **DIVR** |
| **0x30** | MOV  | MOV  | MOV  | MOV  | MOV      | MOV     | MOV      | MOV      | MOV  | MOV  | MOV  | MOV  | MOV      | MOV     | MOV      | MOV      |
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

### 8-bit Addressing Modes

**Immediate:** Data is given directly in the operand.

**Absolute:** A position in memory is given, the data is there.

**Indirect:** A position in memory is given, the data is at the position described.

**Register:** A register is given that contains the data.

**Relative:** The data given is an offset of a certain value.

**Implied:** Opcode has a defined source or destination and no operand is needed.

### 16-bit Addressing Modes

**Immediate Ext:** Data is given directly in the operand.

**Absolute Ext:** A position in memory is given, the data is there. The following byte of data is concatenated with the initial byte to form the 16-bit value.

**Indirect Ext:** A position in memory is given, the data is at the position described. The following byte of data is concatenated with the initial byte to form the 16-bit value.

**Register Ext:** A register pair is provided that contains the data.

## Arithmetic and Logic Operations

### ADD/ADDX 

Add value with carry

Flags Affected: Zero, Overflow. 

| Assembler           | Effect                 | Bytes | Opcode |
| ------------------- | ---------------------- | ----- | ------ |
| ADD #oper           | A = A + #oper          | 2     | 0x10   |
| ADD oper            | A = A + mem[oper]      | 3     | 0x11   |
| ADD @oper           | A = A + mem[mem[oper]] | 3     | 0x12   |
| ADD RX              | A = A + RX             | 2     | 0x13   |
| **16bit** ADD #oper | A = A + #oper          | 3     | 0x14   |
| **16bit** ADD oper  | A = A + mem[oper]      | 3     | 0x15   |
| **16bit** ADD @oper | A = A + mem[mem[oper]] | 3     | 0x16   |
| **16bit** ADD RXX   | A = A + RXX            | 2     | 0x17   |

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

| Assembler        | Effect                          | Bytes | Opcode |
| ---------------- | ------------------------------- | ----- | ------ |
| MOV #oper,A      | A = #oper                       |       | 0x30   |
| MOV #oper,R#     | R#= #oper                       |       | 0x31   |
| MOV #oper,oper   | mem[oper] = #oper               |       | 0x32   |
| MOV #oper,@oper  | mem[mem[oper]] = #oper          |       | 0x33   |
| MOV R#, A        | A = R#                          |       | 0x34   |
| MOV R#, R#       | R# = R#                         |       | 0x35   |
| MOV R#, oper     | mem[oper] = R#                  |       | 0x36   |
| MOV R#, @oper    | mem[mem[oper]] = R#             |       | 0x37   |
| MOV oper,A       | A = mem[oper]                   |       | 0x38   |
| MOV oper, R#     | R# = mem[oper]                  |       | 0x39   |
| MOV oper, oper   | mem[oper] = mem[oper]           |       | 0x3A   |
| MOV oper,@oper   | mem[mem[oper]] = mem[oper]      |       | 0x3B   |
| MOV @oper, A     | A = [mem[mem[oper]]             |       | 0x3C   |
| MOV @oper,R#     | R# = [mem[mem[oper]]]           |       | 0x3D   |
| MOV @oper, oper  | mem[oper] = mem[mem[oper]]      |       | 0x3E   |
| MOV @oper, @oper | mem[mem[oper]] = mem[mem[oper]] |       | 0x3F   |

### 

### SWP

Swap register with accumulator

| Assembler | Effect                              | Bytes | Opcode |
| --------- | ----------------------------------- | ----- | ------ |
| SWP R#    | Swaps register with the accumulator | 2     | 0x04   |

### 

### PUSH

Push to stack

| Assembler | Effect                      | Bytes | Opcode |
| --------- | --------------------------- | ----- | ------ |
| PUSH      | Pushes accumulator to stack | 1     | 0x02   |

### POP

Pop from stack

| Assembler | Effect                           | Bytes | Opcode |
| --------- | -------------------------------- | ----- | ------ |
| POP       | Pops top of stack to accumulator | 1     | 0x03   |

### 

### NOP

No Operation

Flags Affected: None

| Assembler | Effect  | Bytes | Opcode |
| --------- | ------- | ----- | ------ |
| NOP       | Nothing | 1     | 0x00   |

### 

## References

-----

https://en.wikipedia.org/wiki/Instruction_set_architecture





