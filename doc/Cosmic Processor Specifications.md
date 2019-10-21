# Cosmic Processor Specifications (Coz502)

-------

## Registers

-----

### General Registers

* 8x 8-bit general registers that can be used for any purpose

  **A**, **B**, **C**, **D**, **E**, **F**, **G**, **H**

  **AB**,**CD**, **EF,** **GH**, can be grouped together to be used as 4 16-bit registers.

  **A** is the accumulator in 8-bit operations, **A/B** becomes Accumulator in 16-bit operations 

* 1x, 16-bit Stack Pointer

  Stack pointer points to a position in memory for the stack. Can be changed? Increments/Decrements with ``PUSH`` and ``POP`` 

* 1x 16-bit Program Counter

  Holds the current position in memory that is being read from. 

* 1x 8-bit Status Register

  ```
  7  bit  0
  xxIP OCNZ
  |||| ||||
  |||| |||+-- (Z) Zero
  |||| ||+--- (N) Negative, high if result is negative
  |||| |+---- (C) Carry/Borrow, result if overflow
  |||| +----- (O) Overflow, result if overflows carry
  |||+------- (P) Parity, checks parity
  ||+-------- (I) Interrupt disable
  |+---------  X
  +----------  X
  ```

  

## Instruction Set

----

| Hi\Lo    | 0x00         | 0x01         | 0x02          | 0x03         | 0x04         | 0x05         | 0x06         | 0x07         | 0x08         | 00x9         | 0x0A         | 0x0B         | 0x0C         | 0x0D         | 0x0E         | 0x0F         |
| -------- | ------------ | ------------ | ------------- | ------------ | ------------ | ------------ | ------------ | ------------ | ------------ | ------------ | ------------ | ------------ | ------------ | ------------ | ------------ | ------------ |
| **0x00** | [NOP](#NOP)  | [HCF](#HCF)  | [PUSH](#PUSH) | [POP](#POP)  | [SWP](#SWP)  |              |              |              |              |              |              |              |              |              |              |              |
| **0x10** | [ADD](#ADD)  | [ADD](#ADD)  | [ADD](#ADD)   | [ADD](#ADD)  | [ADDX](#ADD) | [ADDX](#ADD) | [ADDX](#ADD) | [ADDX](#ADD) | [SUB](#SUB)  | [SUB](#SUB)  | [SUB](#SUB)  | [SUB](#SUB)  | [SUBX](#SUB) | [SUBX](#SUB) | [SUBX](#SUB) | [SUBX](#SUB) |
| **0x20** | [MUL](#MUL)  | [MUL](#MUL)  | [MUL](#MUL)   | [MUL](#MUL)  | [MULX](#MUL) | [MULX](#MUL) | [MULX](#MUL) | [MULX](#MUL) | [DIV](#DIV)  | [DIV](#DIV)  | [DIV](#DIV)  | [DIV](#DIV)  | [DIVX](#DIV) | [DIVX](#DIV) | [DIVX](#DIV) | [DIVX](#DIV) |
| **0x30** | [MOV](#MOV)  | [MOV](#MOV)  | [MOV](#MOV)   | [MOV](#MOV)  | [MOV](#MOV)  | [MOV](#MOV)  | [MOV](#MOV)  | [MOV](#MOV)  | [MOV](#MOV)  | [MOV](#MOV)  | [MOV](#MOV)  | [MOV](#MOV)  |              |              |              |              |
| **0x40** | [MOVX](#MOV) | [MOVX](#MOV) | [MOVX](#MOV)  | [MOVX](#MOV) | [MOVX](#MOV) | [MOVX](#MOV) | [MOVX](#MOV) | [MOVX](#MOV) | [MOVX](#MOV) | [MOVX](#MOV) | [MOVX](#MOV) | [MOVX](#MOV) |              |              |              |              |
| **0x50** |              |              |               |              |              |              |              |              |              |              |              |              |              |              |              |              |
| **0x60** |              |              |               |              |              |              |              |              |              |              |              |              |              |              |              |              |
| **0x70** |              |              |               |              |              |              |              |              |              |              |              |              |              |              |              |              |
| **0x80** |              |              |               |              |              |              |              |              |              |              |              |              |              |              |              |              |
| **0x90** |              |              |               |              |              |              |              |              |              |              |              |              |              |              |              |              |
| **0xA0** |              |              |               |              |              |              |              |              |              |              |              |              |              |              |              |              |
| **0xB0** |              |              |               |              |              |              |              |              |              |              |              |              |              |              |              |              |
| **0xC0** |              |              |               |              |              |              |              |              |              |              |              |              |              |              |              |              |
| **0xD0** |              |              |               |              |              |              |              |              |              |              |              |              |              |              |              |              |
| **0xE0** |              |              |               |              |              |              |              |              |              |              |              |              |              |              |              |              |
| **0xF0** |              |              |               |              |              |              |              |              |              |              |              |              |              |              |              |              |

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

<a name="ADD"></a>

### ADD/ADDX

Add memory to accumulator

```````
A = A + Data										x x I P  O C N Z
																- - - -  + + + +
```````

| Addressing      | Assembler  | Opcode | Bytes                                 |
| --------------- | ---------- | ------ | ------------------------------------- |
| Immediate       | ADD #oper  | 0x10   | 2 `` opcode value ``                  |
| Absolute        | ADD oper   | 0x11   | 3 ``opcode locationHigh locationLow`` |
| Indirect        | ADD @oper  | 0x12   | 3 ``opcode locationHigh locationLow`` |
| Register        | ADD RX     | 0x13   | 2 ``opcode register``                 |
| 16bit Immediate | ADDX #oper | 0x14   | 3 ``opcode valueHigh valueLow``       |
| 16bit Absolute  | ADDX oper  | 0x15   | 3 ``opcode locationHigh locationLow`` |
| 16bit Indirect  | ADDX @oper | 0x16   | 3 ``opcode locationHigh locationLow`` |
| 16bit Register  | ADDX RX    | 0x17   | 2 ``opcode register``                 |

<a name="SUB"></a>

### SUB/SUBX

Subtract memory from accumualtor

```
A = A - Data										x x I P  O C N Z
																- - - -  + + + +
```

| Addressing      | Assembler  | Opcode | Bytes                                 |
| --------------- | ---------- | ------ | ------------------------------------- |
| Immediate       | SUB #oper  | 0x18   | 2 ``opcode value``                    |
| Absolute        | SUB oper   | 0x19   | 3 ``opcode locationHigh locationLow`` |
| Indirect        | SUB @oper  | 0x1A   | 3 ``opcode locationHigh locationLow`` |
| Register        | SUB RX     | 0x1B   | 2 ``opcode register``                 |
| 16bit Immediate | SUBX #oper | 0x1C   | 3 ``opcode valueHigh valueLow``       |
| 16bit Absolute  | SUBX oper  | 0x1D   | 3 ``opcode locationHigh locationLow`` |
| 16bit Indirect  | SUBX @oper | 0x1E   | 3 ``opcode locationHigh locationLow`` |
| 16bit Register  | SUBX RX    | 0x1F   | 2 ``opcode register``                 |

###  

<a name="MUL"></a>

### MUL/MULX

Multiplies the accumulator

```
A = A * Data										x x I P  O C N Z
																- - - -  + + + +
```

| Addressing      | Assembler  | Opcode | Bytes                                 |
| --------------- | ---------- | ------ | ------------------------------------- |
| Immediate       | MUL #oper  | 0x20   | 2 ``opcode value``                    |
| Absolute        | MUL oper   | 0x21   | 3 ``opcode locationHigh locationLow`` |
| Indirect        | MUL @oper  | 0x22   | 3 ``opcode locationHigh locationLow`` |
| Register        | MUL RX     | 0x23   | 2 ``opcode register``                 |
| 16bit Immediate | MULX #oper | 0x24   | 3 ``opcode valueHigh valueLow``       |
| 16bit Absolute  | MULX oper  | 0x25   | 3 ``opcode locationHigh locationLow`` |
| 16bit Indirect  | MULX @oper | 0x26   | 3 ``opcode locationHigh locationLow`` |
| 16bit Register  | MULX RX    | 0x27   | 2 ``opcode register``                 |



<a name="DIV"></a>

### DIV

Divides the Accumulator, puts remainder in B(8bit mode) or C/D (16bit mode)

```
A = A / D,B = R.    					x x I P  O C N Z
															- - - -  + + + +
```

| Addressing      | Assembler  | Opcode | Bytes                                 |
| --------------- | ---------- | ------ | ------------------------------------- |
| Immediate       | DIV #oper  | 0x28   | 2 ``opcode value``                    |
| Absolute        | DIV oper   | 0x29   | 3 ``opcode locationHigh locationLow`` |
| Indirect        | DIV @oper  | 0x2A   | 3 ``opcode locationHigh locationLow`` |
| Register        | DIV RX     | 0x2B   | 2 ``opcode register``                 |
| 16bit Immediate | DIVX #oper | 0x2C   | 3 ``opcode valueHigh valueLow``       |
| 16bit Absolute  | DIVX oper  | 0x2D   | 3 ``opcode locationHigh locationLow`` |
| 16bit Indirect  | DIVX @oper | 0x2E   | 3 ``opcode locationHigh locationLow`` |
| 16bit Register  | DIVX RX    | 0x2F   | 2 ``opcode register``                 |

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

## Data Handling and Memory Operations

<a name="MOV"></a>

### MOV/MOVX

Copies memory from one location to another

```
Loc = Data.          					x x I P  O C N Z
															- - - -  - - + +
```

| Addressing                       | Assembler         | Opcode | Bytes                                      |
| -------------------------------- | ----------------- | ------ | ------------------------------------------ |
| Immediate -> **Absolute**        | MOV #oper, oper   |        | 4 ``opcode value dstHigh dstLow``          |
| Absolute -> **Absolute**         | MOV oper, oper    |        | 5 ``opcode srcHigh srcLow dstHigh dstLow`` |
| Indirect -> **Absolute**         | MOV @oper, oper   |        | 5 ``opcode srcHigh srcLow dstHigh dstLow`` |
| Register -> **Absolute**         | MOV RX, oper      |        | 4 ``opcode reg dstHigh dstLow``            |
| Immediate -> **Indirect**        | MOV #oper, @oper  |        | 5 ``opcode value dstHigh dstLow``          |
| Absolute -> **Indirect**         | MOV oper, @oper   |        | 5 ``opcode srcHigh srcLow dstHigh dstLow`` |
| Indirect -> **Indirect**         | MOV @oper, @oper  |        | 5 ``opcode srcHigh srcLow dstHigh dstLow`` |
| Register -> **Indirect**         | MOV RX, @oper     |        | 4 ``opcode reg dstHigh dstLow``            |
| Immediate -> **Register**        | MOV #oper, RX     |        | 3 ``opcode val reg``                       |
| Absolute -> **Register**         | MOV oper, RX      |        | 4 ``opcode locHigh locLow reg``            |
| Indirect -> **Register**         | MOV @oper, RX     |        | 4 ``opcode locHigh locLow reg``            |
| Register -> **Register**         | MOV RX, RX        |        | 3 ``opcode reg reg``                       |
| 16 bit Immediate -> **Absolute** | MOVX #oper, oper  |        | 5 ``opcode valHigh valLow locHigh locLow`` |
| 16 bit Absolute -> **Absolute**  | MOVX oper, oper   |        | 5 ``opcode srcHigh srcLow dstHigh dstLow`` |
| 16 bit Indirect -> **Absolute**  | MOVX @oper, oper  |        | 5 ``opcode srcHigh srcLow dstHigh dstLow`` |
| 16 bit Register -> **Absolute**  | MOVX RX, oper     |        | 4 ``opcode reg dstHigh dstLow``            |
| 16 bit Immediate -> **Indirect** | MOVX #oper, @oper |        | 5 ``opcode valHigh valLow dstHigh dstLow`` |
| 16 bit Absolute -> **Indirect**  | MOVX oper, @oper  |        | 5 ``opcode srcHigh srcLow dstHigh dstLow`` |
| 16 bit Indirect -> **Indirect**  | MOVX @oper, @oper |        | 5 ``opcode srcHigh srcLow dstHigh dstLow`` |
| 16 bit Register -> **Indirect**  | MOVX RX, @oper    |        | 4 ``opcode reg srcHigh srcLow``            |
| 16 bit Immediate -> **Register** | MOVX #oper, RX    |        | 4 ``opcode valHigh valLow reg``            |
| 16 bit Absolute -> **Register**  | MOVX oper, RX     |        | 4 ``opcode srcHigh srcLow reg``            |
| Indirect -> **Register**         | MOVX @oper, RX    |        | 4 ``opcode srcHigh srcLow reg``            |
| Register -> **Register**         | MOVX RX, RX       |        | 3 ``opcode reg reg``                       |

### 

_Note: This is where the principals of RISC start to break down in favor of a more fleshed out instruction set_. In a true RISC instruction set each target addressing mode would be it's own instruction, but that's annoying. 

<a name="SWP"></a>

### SWP

Swap registers

```
R1 <-> R2   	        				x x I P  O C N Z
															- - - -  - - + +
```

| Addressing | Assembler  | Opcode | Bytes                  |
| ---------- | ---------- | ------ | ---------------------- |
| Register   | SWP R1, R2 |        | 3 ``opcode reg1 reg2`` |

<a name="PUSH"></a>

### PUSH

Pushes value in accumulator to the stack

```
sp[++i] = A 					x x I P  O C N Z
											- - - -  - - + +
```

| Addressing | Assembler | Opcode | Bytes        |
| ---------- | --------- | ------ | ------------ |
| Implied    | PUSH      |        | 1 ``opcode`` |

<a name="POP"></a>

### POP

Pops value from stack into the accumulator

```
sp[i--] = A    					x x I P  O C N Z
												- - - -  - - + +
```

| Addressing | Assembler | Opcode | Bytes        |
| ---------- | --------- | ------ | ------------ |
| Implied    | POP       |        | 1 ``opcode`` |

### 

<a name="NOP"></a>

### NOP

No Operation

```
Nothing Happened!   					x x I P  O C N Z
															- - - -  - - - - 
```

| Addressing | Assembler | Opcode | Bytes        |
| ---------- | --------- | ------ | ------------ |
| Implied    | NOP       | 0x00   | 1 ``opcode`` |

### 

## References

-----

https://en.wikipedia.org/wiki/Instruction_set_architecture





