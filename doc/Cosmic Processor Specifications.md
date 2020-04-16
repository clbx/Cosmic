# Cosmic Processor Specifications

-------

## Registers

-----

### General Registers

* 8x 8-bit general registers that can be used for any purpose

  **A**, **B**, **C**, **D**, **E**, **F**, **G**, **H**

  **AB**, **CD**, **EF**, and **GH**, can be grouped together to be used as 4x 16-bit registers.

  **A** is the accumulator in 8-bit operations, **A/B** becomes Accumulator in 16-bit operations 

* 1x 16-bit Stack Pointer

  Stack pointer points to a position in memory for the stack. Can be changed? Increments/Decrements with ``PUSH`` and ``POP`` 

* 1x 16-bit Program Counter

  Holds the current position in memory that is being read from. 

* 1x 8-bit Status Register

  ```
  7  bit  0
  xEIP OCNZ
  |||| ||||
  |||| |||+-- (Z) Zero
  |||| ||+--- (N) Negative, high if result is negative
  |||| |+---- (C) Carry/Borrow, result if overflow
  |||| +----- (O) Overflow, result if overflows carry
  |||+------- (L) Less, used with the compare instruction
  ||+-------- (I) Interrupt disable
  |+--------- (E) Error, high if divide by 0
  +----------  X
  ```

## Memory Mapping

It is always assumed that 64K of memory is being used. No reason to not have all addressable memory availble.

Current Memory Map (highly subject to change)
```
        ┌───────────────────────┐
0x0000  │                       │
        │                       │
        │                       │
        │    32k of General     │
        │      Usage Space      │
        │                       │
        │                       │
        |                       |
        |                       |
        |                       |
        │                       │
        │                       │		
0x8000  ├───────────────────────┤
        │                       │		
        │   16k Video Memory    │
        |                       |
        │                       │		 
0xC000	├───────────────────────┤
        │    1k Stack Space     │
0xC400  ├───────────────────────┤
        │     1k I/O Space    	│
0xC800	├───────────────────────┤
        │   5k Variable Space   │
        │                       │
0xCD00  |-----------------------|
        │    7k Empty Space     │
        |                       |
0xFFF0  ├───────────────────────┤
        │  8 Byte Vector Space  │
0xFFFF	└───────────────────────┘
```

### Keyboard input

0xC400: 0xFF if new a new key has been pressed.

0xC401: The specific key press.

### Raspberry Pi Pins
On the Raspberry Pi the 28 GPIO pins correlate to 28 bytes in memory. Cosmic uses the WiringPi method of numbering pins [You can check it here](pinout.xyz)

0xC402: GPIO 0


0xC403: GPIO 1

0xC404: GPIO 2

...

0xC41C: GPIO 26

0xC41D: GPIO 27

0xC41E: GPIO 28

The most signifigant bit in the GPIO Bytes signifies if the pin is an input or output. The remaining 7 bits set it on or off. If the 7 bits equal zero the pin is off, if it is greater than 0 it is on.
```
---------------------------------
| 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
---------------------------------
  |   |   |   |   |   |   |   |
  |   +---+---+---+---+---+---+----> Data
  +--------------------------------> Write = 1, Read = 0
```
On all other platforms this memory is currently unused.

### Interupts

The row of 0xFFF0 is used for handling specific cases
```
0xFFF0 ┬─ Reset Vector
0xFFF1 ┘
0xFFF2 ┬─ Low Priority Interrupt Vector
0xFFF3 ┘
0xFFF4 ┬─ High Priority Interrupt Vector
0xFFF5 ┘
0xFFF6┬─ Currently Unused
...   │
0xFFFF┘
```

## Interrupt Handling

When an interrupt is found, the program counter jumps to the location described in the relevant interrupt vector. The current program counter will be pushed to the stack and ``RET`` is required at the end of the interrupt subroutine

Low priority interrupts will be ignored if the Interrupt flag is enabled. High priority interrupts cannot be ignored.


## Instruction Set

----

| Hi\Lo    | 0x00         | 0x01         | 0x02          | 0x03         | 0x04         | 0x05          | 0x06          | 0x07          | 0x08         | 0x09         | 0x0A         | 0x0B         | 0x0C         | 0x0D         | 0x0E         | 0x0F         |
| -------- | ------------ | ------------ | ------------- | ------------ | ------------ | ------------- | ------------- | ------------- | ------------ | ------------ | ------------ | ------------ | ------------ | ------------ | ------------ | ------------ |
| **0x00** | [NOP](#NOP)  | [HCF](#HCF)  | [PUSH](#PUSH) | [POP](#POP)  | [SWP](#SWP)  | [CALL](#CALL) | [CALL](#CALL) | [CALL](#CALL) | [RET](#RET)  |              |              |              |              |              |              |              |
| **0x10** | [ADD](#ADD)  | [ADD](#ADD)  | [ADD](#ADD)   | [ADD](#ADD)  | [ADDX](#ADD) | [ADDX](#ADD)  | [ADDX](#ADD)  | [ADDX](#ADD)  | [SUB](#SUB)  | [SUB](#SUB)  | [SUB](#SUB)  | [SUB](#SUB)  | [SUBX](#SUB) | [SUBX](#SUB) | [SUBX](#SUB) | [SUBX](#SUB) |
| **0x20** | [MUL](#MUL)  | [MUL](#MUL)  | [MUL](#MUL)   | [MUL](#MUL)  | [MULX](#MUL) | [MULX](#MUL)  | [MULX](#MUL)  | [MULX](#MUL)  | [DIV](#DIV)  | [DIV](#DIV)  | [DIV](#DIV)  | [DIV](#DIV)  | [DIVX](#DIV) | [DIVX](#DIV) | [DIVX](#DIV) | [DIVX](#DIV) |
| **0x30** | [MOV](#MOV)  | [MOV](#MOV)  | [MOV](#MOV)   | [MOV](#MOV)  | [MOV](#MOV)  | [MOV](#MOV)   | [MOV](#MOV)   | [MOV](#MOV)   | [MOV](#MOV)  | [MOV](#MOV)  | [MOV](#MOV)  | [MOV](#MOV)  | [SHL](#SHL)  | [SHL](#SHL)  | [SHL](#SHL)  | [SHL](#SHL)  |
| **0x40** | [MOVX](#MOV) | [MOVX](#MOV) | [MOVX](#MOV)  | [MOVX](#MOV) | [MOVX](#MOV) | [MOVX](#MOV)  | [MOVX](#MOV)  | [MOVX](#MOV)  | [MOVX](#MOV) | [MOVX](#MOV) | [MOVX](#MOV) | [MOVX](#MOV) | [SHLX](#SHL) | [SHLX](#SHL) | [SHLX](#SHL) | [SHLX](#SHL) |
| **0x50** | [AND](#AND)  | [AND](#AND)  | [AND](#AND)   | [AND](#AND)  | [OR](#OR)    | [OR](#OR)     | [OR](#OR)     | [OR](#OR)     | [XOR](#XOR)  | [XOR](#XOR)  | [XOR](#XOR)  | [XOR](#XOR)  | [SHR](#SHR)  | [SHR](#SHR)  | [SHR](#SHR)  | [SHR](#SHR)  |
| **0x60** | [CMP](#CMP)  | [CMP](#CMP)  | [CMP](#CMP)   | [CMP](#CMP)  | [CMPX](#CMP) | [CMPX](#CMP)  | [CMPX](#CMP)  | [CMPX](#CMP)  |              |              |              |              | [SHRX](#SHR) | [SHRX](#SHR) | [SHRX](#SHR) | [SHRX](#SHR) |
| **0x70** | [JMP](#JMP)  | [JMP](#JMP)  | [JMP](#JMP)   | [JMP](#JMP)  | [JZS](#JZS)  | [JZS](#JZS)   | [JZS](#JZS)   | [JZS](#JZS)   | [JNZ](#JNZ)  | [JNZ](#JNZ)  | [JNZ](#JNZ)  | [JNZ](#JNZ)  | [JCS](#JCS)  | [JCS](#JCS)  | [JCS](#JCS)  | [JCS](#JCS)  |
| **0x80** | [JNC](#JNC)  | [JNC](#JNC)  | [JNC](#JNC)   | [JNC](#JNC)  | [JOS](#JOS)  | [JOS](#JOS)   | [JOS](#JOS)   | [JOS](#JOS)   | [JNO](#JNO)  | [JNO](#JNO)  | [JNO](#JNO)  | [JNO](#JNO)  | [JNS](#JNS)  | [JNS](#JNS)  | [JNS](#JNS)  | [JNS](#JNS)  |
| **0x90** | [JNN](#JNN)  | [JNN](#JNN)  | [JNN](#JNN)   | [JNN](#JNN)  | [JLS](#JLS)  | [JLS](#JLS)   | [JLS](#JLS)   | [JLS](#JLS)   | [JNL](#JLS)  | [JNL](#JLS)  | [JNL](#JLS)  | [JNL](#JLS)  | [JES](#JES)  | [JES](#JES)  | [JES](#JES)  | [JES](#JES)  |
| **0xA0** | [CSF](#CSF)  | [CZF](#CZF)  | [SZF](#SZF)   | [CNF](#CNF)  | [SNF](#SNF)  | [CCF](#CCF)   | [SCF](#SCF)   | [COF](#COF)   | [SOF](#SOF)  | [CLF](#CLF)  | [SLF](#SLF)  | [CIF](#CIF)  | [SIF](#SIF)  | [CEF](#CEF)  |              |              |
| **0xB0** | [INC](#INC)  | [INC](#INC)  | [INC](#INC)   | [INCX](#INC) | [INCX](#INC) | [INCX](#INC)  | [DEC](#DEC)   | [DEC](#DEC)   | [DEC](#DEC)  | [DECX](#DEC) | [DECX](#DEC) | [DECX](#DEC) |
| **0xC0** |              |              |               |              |              |               |               |               |              |              |              |              |              |              |              |              |
| **0xD0** |              |              |               |              |              |               |               |               |              |              |              |              |              |              |              |              |
| **0xE0** |              |              |               |              |              |               |               |               |              |              |              |              |              |              |              |              |
| **0xF0** |              |              |               |              |              |               |               |               |              |              |              |              |              |              |              |              |

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

## Data Handling and Memory Operations



<a name="NOP"></a>

### NOP

No Operation

```
Nothing Happened!   					x E I L  O C N Z
							- - - -  - - - - 
```

| Addressing | Assembler | Opcode | Bytes        |
| ---------- | --------- | ------ | ------------ |
| Implied    | NOP       | 0x00   | 1 ``opcode`` |



<a name="HCF"></a>

### HCF

Halt and Catch Fire, Stops execution of the machine entirely 

```
rip.              		x E I L  O C N Z
				- - - -  - - - -
```

| Addressing | Assembler | Opcode | Bytes        |
| ---------- | --------- | ------ | ------------ |
| Implied    | HCF       | 0x01   | 3 ``opcode`` |



<a name="PUSH"></a>

### PUSH

Pushes value in accumulator to the stack

```
sp[++i] = A 					x E I L  O C N Z
						- - - -  - - + +
```

| Addressing | Assembler | Opcode | Bytes        |
| ---------- | --------- | ------ | ------------ |
| Implied    | PUSH      | 0x02   | 1 ``opcode`` |



<a name="POP"></a>

### POP

Pops value from stack into the accumulator

```
sp[i--] = A    					x E I L  O C N Z
						- - - -  - - + +
```

| Addressing | Assembler | Opcode | Bytes        |
| ---------- | --------- | ------ | ------------ |
| Implied    | POP       | 0x03   | 1 ``opcode`` |



<a name="SWP"></a>

### SWP

Swap registers

```
R1 <-> R2   	        				x E I P  O C N Z
							- - - -  - - - -
```

| Addressing | Assembler  | Opcode | Bytes                  |
| ---------- | ---------- | ------ | ---------------------- |
| Register   | SWP R1, R2 | 0x04   | 3 ``opcode reg1 reg2`` |



<a name="CALL"></a>

### CALL

Call subroutine at location. Pushes current location onto stack, jumps to new location

```
push pc; pc = oper		x E I L  O C N Z
				- - - -  - - - -
```

| Addressing | Assembler  | Opcode | Bytes                                         |
| ---------- | ---------- | ------ | --------------------------------------------- |
| Immediate  | CALL #oper | 0x05   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Absolute   | CALL oper  | 0x06   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Indirect   | CALL @oper | 0x07   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |



<a name="RET"></a>

### RET

Returns from subroutine, pops old location off of stack, jumps back

```
pc = pop.          		x E I L  O C N Z
				- - - -  - - - -
```

| Addressing | Assembler | Opcode | Bytes        |
| ---------- | --------- | ------ | ------------ |
| Implied    | RET       | 0x08   | 3 ``opcode`` |



## Arithmetic and Logic Operations



<a name="ADD"></a>

### ADD

Add memory to accumulator

```````
A = A + Data										x E I L  O C N Z
											- - - -  + + + +
```````

| Addressing      | Assembler  | Opcode | Bytes                                 |
| --------------- | ---------- | ------ | ------------------------------------- |
| Immediate       | ADD #oper  | 0x10   | 2 ``opcode value``                  |
| Absolute        | ADD oper   | 0x11   | 3 ``opcode locationHigh locationLow`` |
| Indirect        | ADD @oper  | 0x12   | 3 ``opcode locationHigh locationLow`` |
| Register        | ADD RX     | 0x13   | 2 ``opcode register``                 |
| 16-bit Immediate| ADDX #oper | 0x14   | 3 ``opcode valueHigh valueLow``       |
| 16-bit Absolute | ADDX oper  | 0x15   | 3 ``opcode locationHigh locationLow`` |
| 16-bit Indirect | ADDX @oper | 0x16   | 3 ``opcode locationHigh locationLow`` |
| 16-bit Register | ADDX RX    | 0x17   | 2 ``opcode register``                 |



<a name="SUB"></a>

### SUB

Subtract memory from accumualtor

```
A = A - Data										x E I L  O C N Z
											- - - -  + + + +
```

| Addressing      | Assembler  | Opcode | Bytes                                 |
| --------------- | ---------- | ------ | ------------------------------------- |
| Immediate       | SUB #oper  | 0x18   | 2 ``opcode value``                    |
| Absolute        | SUB oper   | 0x19   | 3 ``opcode locationHigh locationLow`` |
| Indirect        | SUB @oper  | 0x1A   | 3 ``opcode locationHigh locationLow`` |
| Register        | SUB RX     | 0x1B   | 2 ``opcode register``                 |
| 16-bit Immediate| SUBX #oper | 0x1C   | 3 ``opcode valueHigh valueLow``       |
| 16-bit Absolute | SUBX oper  | 0x1D   | 3 ``opcode locationHigh locationLow`` |
| 16-bit Indirect | SUBX @oper | 0x1E   | 3 ``opcode locationHigh locationLow`` |
| 16-bit Register | SUBX RX    | 0x1F   | 2 ``opcode register``                 |



<a name="MUL"></a>

### MUL

Multiplies the accumulator

```
A = A * Data										x E I L  O C N Z
											- - - -  + + + +
```

| Addressing      | Assembler  | Opcode | Bytes                                 |
| --------------- | ---------- | ------ | ------------------------------------- |
| Immediate       | MUL #oper  | 0x20   | 2 ``opcode value``                    |
| Absolute        | MUL oper   | 0x21   | 3 ``opcode locationHigh locationLow`` |
| Indirect        | MUL @oper  | 0x22   | 3 ``opcode locationHigh locationLow`` |
| Register        | MUL RX     | 0x23   | 2 ``opcode register``                 |
| 16-bit Immediate| MULX #oper | 0x24   | 3 ``opcode valueHigh valueLow``       |
| 16-bit Absolute | MULX oper  | 0x25   | 3 ``opcode locationHigh locationLow`` |
| 16-bit Indirect | MULX @oper | 0x26   | 3 ``opcode locationHigh locationLow`` |
| 16-bit Register | MULX RX    | 0x27   | 2 ``opcode register``                 |



<a name="DIV"></a>

### DIV

Divides the Accumulator, puts remainder in B(8bit mode) or C/D (16bit mode)

```
A = A / D,B = R.    					x E I L  O C N Z
							- + - -  + + + +
```

| Addressing       | Assembler  | Opcode | Bytes                                 |
| ---------------- | ---------- | ------ | ------------------------------------- |
| Immediate        | DIV #oper  | 0x28   | 2 ``opcode value``                    |
| Absolute         | DIV oper   | 0x29   | 3 ``opcode locationHigh locationLow`` |
| Indirect         | DIV @oper  | 0x2A   | 3 ``opcode locationHigh locationLow`` |
| Register         | DIV RX     | 0x2B   | 2 ``opcode register``                 |
| 16-bit Immediate | DIVX #oper | 0x2C   | 3 ``opcode valueHigh valueLow``       |
| 16-bit Absolute  | DIVX oper  | 0x2D   | 3 ``opcode locationHigh locationLow`` |
| 16-bit Indirect  | DIVX @oper | 0x2E   | 3 ``opcode locationHigh locationLow`` |
| 16-bit Register  | DIVX RX    | 0x2F   | 2 ``opcode register``                 |



<a name="MOV"></a>

### MOV

Copies memory from one location to another

```
Loc = Data.          					x E I L  O C N Z
							- - - -  - - - -
```

| Addressing                       | Assembler         | Opcode | Bytes                                      |
| -------------------------------- | ----------------- | ------ | ------------------------------------------ |
| Immediate -> **Absolute**        | MOV #oper oper   | 0x30   | 4 ``opcode value dstHigh dstLow``          |
| Absolute -> **Absolute**         | MOV oper oper    | 0x31   | 5 ``opcode srcHigh srcLow dstHigh dstLow`` |
| Indirect -> **Absolute**         | MOV @oper oper   | 0x32   | 5 ``opcode srcHigh srcLow dstHigh dstLow`` |
| Register -> **Absolute**         | MOV RX oper      | 0x33   | 4 ``opcode reg dstHigh dstLow``            |
| Immediate -> **Indirect**        | MOV #oper @oper  | 0x34   | 4 ``opcode value dstHigh dstLow``          |
| Absolute -> **Indirect**         | MOV oper @oper   | 0x35   | 5 ``opcode srcHigh srcLow dstHigh dstLow`` |
| Indirect -> **Indirect**         | MOV @oper @oper  | 0x36   | 5 ``opcode srcHigh srcLow dstHigh dstLow`` |
| Register -> **Indirect**         | MOV RX @oper     | 0x37   | 4 ``opcode reg dstHigh dstLow``            |
| Immediate -> **Register**        | MOV #oper RX     | 0x38   | 3 ``opcode val reg``                       |
| Absolute -> **Register**         | MOV oper RX      | 0x39   | 4 ``opcode locHigh locLow reg``            |
| Indirect -> **Register**         | MOV @oper RX     | 0x3A   | 4 ``opcode locHigh locLow reg``            |
| Register -> **Register**         | MOV RX RX        | 0x3B   | 3 ``opcode reg reg``                       |
| 16-bit Immediate -> **Absolute** | MOVX #oper oper  | 0x40   | 5 ``opcode valHigh valLow locHigh locLow`` |
| 16-bit Absolute -> **Absolute**  | MOVX oper oper   | 0x41   | 5 ``opcode srcHigh srcLow dstHigh dstLow`` |
| 16-bit Indirect -> **Absolute**  | MOVX @oper oper  | 0x42   | 5 ``opcode srcHigh srcLow dstHigh dstLow`` |
| 16-bit Register -> **Absolute**  | MOVX RX oper     | 0x43   | 4 ``opcode reg dstHigh dstLow``            |
| 16-bit Immediate -> **Indirect** | MOVX #oper @oper | 0x44   | 5 ``opcode valHigh valLow dstHigh dstLow`` |
| 16-bit Absolute -> **Indirect**  | MOVX oper @oper  | 0x45   | 5 ``opcode srcHigh srcLow dstHigh dstLow`` |
| 16-bit Indirect -> **Indirect**  | MOVX @oper @oper | 0x46   | 5 ``opcode srcHigh srcLow dstHigh dstLow`` |
| 16-bit Register -> **Indirect**  | MOVX RX @oper    | 0x47   | 4 ``opcode reg srcHigh srcLow``            |
| 16-bit Immediate -> **Register** | MOVX #oper RX    | 0x48   | 4 ``opcode valHigh valLow reg``            |
| 16-bit Absolute -> **Register**  | MOVX oper RX     | 0x49   | 4 ``opcode srcHigh srcLow reg``            |
| 16-bit Indirect -> **Register**  | MOVX @oper RX    | 0x4A   | 4 ``opcode srcHigh srcLow reg``            |
| 16-bit Register -> **Register**  | MOVX RX RX       | 0x4B   | 3 ``opcode reg reg``                       |

_Note: This is where the principals of RISC start to break down in favor of a more fleshed out instruction set_. In a true RISC instruction set each target addressing mode would be it's own instruction, but that's annoying. 



<a name="SHL"></a>

### SHL

Shift the accumulator left,  sets carry if a high bit gets pushed off.

```
A = A << Data        					  x E I L  O C N Z
						          - - - -  - + - +
```

| Addressing       | Assembler  | Opcode | Bytes                                 |
| ---------------- | ---------- | ------ | ------------------------------------- |
| Immediate        | SHL #oper  | 0x3C   | 2 ``opcode value``                    |
| Absolute         | SHL oper   | 0x3D   | 3 ``opcode locationHigh locationLow`` |
| Indirect         | SHL @oper  | 0x3E   | 3 ``opcode locationHigh locationLow`` |
| Register         | SHL RX     | 0x3F   | 2 ``opcode register``                 |
| 16-bit Immediate | SHLX #oper | 0x4C   | 3 ``opcode valueHigh valueLow``       |
| 16-bit Absolute  | SHLX oper  | 0x4D   | 3 ``opcode locationHigh locationLow`` |
| 16-bit Indirect  | SHLX @oper | 0x4E   | 3 ``opcode locationHigh locationLow`` |
| 16-bit Register  | SHLX RX    | 0x4F   | 2 ``opcode register``                 |



<a name="AND"></a>

### AND

AND Memory with the Accumulator

```
data & A = A 					x E I L  O C N Z
						- - - -  - - - +
```

| Addressing | Assembler | Opcode | Bytes                                         |
| ---------- | --------- | ------ | --------------------------------------------- |
| Immediate  | AND #oper | 0x50   | 2 ``opcode`` ``value``                        |
| Absolute   | AND oper  | 0x51   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Indirect   | AND @oper | 0x52   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Register   | AND RX    | 0x53   | 2 ``opcode`` ``register``                     |



<a name="OR"></a>

### OR

OR Memory with the Accumulator

```
data | A = A 					x E I L  O C N Z
						- - - -  - - - +
```

| Addressing | Assembler | Opcode | Bytes                                         |
| ---------- | --------- | ------ | --------------------------------------------- |
| Immediate  | OR #oper  | 0x54   | 2 ``opcode`` ``value``                        |
| Absolute   | OR oper   | 0x55   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Indirect   | OR @oper  | 0x56   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Register   | OR RX     | 0x57   | 2 ``opcode`` ``register``                     |



<a name="XOR"></a>

### XOR 

XOR Memory with the Accumulator

```
data ^ A = A 					x E I L  O C N Z
						- - - -  - - - +
```

| Addressing | Assembler | Opcode | Bytes                                         |
| ---------- | --------- | ------ | --------------------------------------------- |
| Immediate  | XOR #oper | 0x58   | 2 ``opcode`` ``value``                        |
| Absolute   | XOR oper  | 0x59   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Indirect   | XOR @oper | 0x5A   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Register   | XOR RX    | 0x5B   | 2 ``opcode`` ``register``                     |



<a name="SHR"></a>

### SHR

Shift the accumulator right.

```
A = A >> Data        					  x E I L  O C N Z
						          - - - -  - - - +
```

| Addressing       | Assembler  | Opcode | Bytes                                 |
| ---------------- | ---------- | ------ | ------------------------------------- |
| Immediate        | SHR #oper  | 0x5C   | 2 ``opcode value``                    |
| Absolute         | SHR oper   | 0x5D   | 3 ``opcode locationHigh locationLow`` |
| Indirect         | SHR @oper  | 0x5E   | 3 ``opcode locationHigh locationLow`` |
| Register         | SHR RX     | 0x5F   | 2 ``opcode register``                 |
| 16-bit Immediate | SHRX #oper | 0x6C   | 3 ``opcode valueHigh valueLow``       |
| 16-bit Absolute  | SHRX oper  | 0x6D   | 3 ``opcode locationHigh locationLow`` |
| 16-bit Indirect  | SHRX @oper | 0x6E   | 3 ``opcode locationHigh locationLow`` |
| 16-bit Register  | SHRX RX    | 0x6F   | 2 ``opcode register``                 |



<a name="CMP"></a>

### CMP

Compare memory with the Accumulator

```
             					x E I L  O C N Z
						- - - +  + + + +
(unsigned) if accumulator > value: carry is set
(unsigned) if accumulator < value: negative is set
(signed)   if accumulator > value: overflow is set
(signed)   if accumulator < value: less is set
           if accumulator = value: zero is set
```

| Addressing | Assembler  | Opcode | Bytes                                         |
| ---------- | ---------- | ------ | --------------------------------------------- |
| Immediate  | CMP #oper  | 0x60   | 2 ``opcode`` ``value``                        |
| Absolute   | CMP oper   | 0x61   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Indirect   | CMP @oper  | 0x62   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Register   | CMP RX     | 0x63   | 2 ``opcode`` ``register``                     |
| Immediate  | CMPX #oper | 0x64   | 3 ``opcode`` ``valHigh`` ``valLow``           |
| Absolute   | CMPX oper  | 0x65   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Indirect   | CMPX @oper | 0x66   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Register   | CMPX RX    | 0x67   | 2 ``opcode`` ``register``                     |



## Control Flow Operations



<a name="JMP"></a>

### JMP

Jump to a specified memory location

```
pc = data    					x E I L  O C N Z
						- - - -  - - - -
```

| Addressing | Assembler | Opcode | Bytes                                         |
| ---------- | --------- | ------ | --------------------------------------------- |
| Immediate  | JMP #oper | 0x70   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Absolute   | JMP oper  | 0x71   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Indirect   | JMP @oper | 0x72   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Register   | JMP RX    | 0x73   | 2 ``opcode register``                         |



<a name="JZS"></a>

### JZS

Jump equals zero, jump to the given location if the zero flag is set

```
if zero; pc = data		x E I L  O C N Z
				- - - -  - - - -
```

| Addressing | Assembler | Opcode | Bytes                                         |
| ---------- | --------- | ------ | --------------------------------------------- |
| Immediate  | JZS #oper | 0x74   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Absolute   | JZS oper  | 0x75   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Indirect   | JZS @oper | 0x76   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Register   | JZS RX    | 0x77   | 2 ``opcode register``                         |



<a name="JNZ"></a>

### JNZ

Jump not zero, jump to the given location if the zero flag is not set

```
if !zero; pc = data		    x E I L  O C N Z
				    - - - -  - - - -
```

| Addressing | Assembler | Opcode | Bytes                                         |
| ---------- | --------- | ------ | --------------------------------------------- |
| Immediate  | JNZ #oper | 0x78   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Absolute   | JNZ oper  | 0x79   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Indirect   | JNZ @oper | 0x7A   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Register   | JNZ RX    | 0x7B   | 2 ``opcode register``                         |



<a name="JCS"></a>

### JCS

Jump on carry, jump to the given location if the carry flag is set

```
if carry; pc = data		x E I L  O C N Z
				- - - -  - - - -
```

| Addressing | Assembler | Opcode | Bytes                                         |
| ---------- | --------- | ------ | --------------------------------------------- |
| Immediate  | JCS #oper | 0x7C   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Absolute   | JCS oper  | 0x7D   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Indirect   | JCS @oper | 0x7E   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Register   | JCS RX    | 0x7F   | 2 ``opcode register``                         |



<a name="JNC"></a>

### JNC

Jump not carry, jump to the given location if the carry flag is not set

```
if !carry; pc = data	x E I L  O C N Z
			- - - -  - - - -
```

| Addressing | Assembler | Opcode | Bytes                                         |
| ---------- | --------- | ------ | --------------------------------------------- |
| Immediate  | JNC #oper | 0x80   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Absolute   | JNC oper  | 0x81   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Indirect   | JNC @oper | 0x82   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Register   | JNC RX    | 0x83   | 2 ``opcode register``                         |



<a name="JOS"></a>

### JOS

Jump on overflow, jump to the given location if the overflow flag is set

```
if over; pc = data		x E I L  O C N Z
                        	- - - -  - - - -
```

| Addressing | Assembler | Opcode | Bytes                                         |
| ---------- | --------- | ------ | --------------------------------------------- |
| Immediate  | JOS #oper | 0x84   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Absolute   | JOS oper  | 0x85   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Indirect   | JOS @oper | 0x86   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Register   | JOS RX    | 0x87   | 2 ``opcode register``                         |



<a name="JNO"></a>

### JNO

Jump not overflow, jump to the given location if the overflow flag not is set

```
if !over; pc = data		x E I L  O C N Z
                        	- - - -  - - - -
```

| Addressing | Assembler | Opcode | Bytes                                         |
| ---------- | --------- | ------ | --------------------------------------------- |
| Immediate  | JNO #oper | 0x88   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Absolute   | JNO oper  | 0x89   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Indirect   | JNO @oper | 0x8A   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Register   | JNO RX    | 0x8B   | 2 ``opcode register``                         |



<a name="JNS"></a>

### JNS

Jump negative result, jump to the given location if the negative flag is set

```
if negative; pc=data  x E I L  O C N Z
		      - - - -  - - - -
```

| Addressing | Assembler | Opcode | Bytes                                         |
| ---------- | --------- | ------ | --------------------------------------------- |
| Immediate  | JNS #oper | 0x8C   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Absolute   | JNS oper  | 0x8D   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Indirect   | JNS @oper | 0x8E   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Register   | JNS RX    | 0x8F   | 2 ``opcode register``                         |



<a name="JNN"></a>

### JNN

Jump if not  negative result, jump to the given location if the negative flag is not set

```
if !negative; pc=data  x E I L  O C N Z
		       - - - -  - - - -
```

| Addressing | Assembler | Opcode | Bytes                                         |
| ---------- | --------- | ------ | --------------------------------------------- |
| Immediate  | JNN #oper | 0x90   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Absolute   | JNN oper  | 0x91   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Indirect   | JNN @oper | 0x92   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Register   | JNN RX    | 0x93   | 2 ``opcode register``                         |



<a name="JLS"></a>

### JLS

Jump if less result, jump to the given location if the less flag is set

```
if less; pc=data      x E I L  O C N Z
		      - - - -  - - - -
```

| Addressing | Assembler | Opcode | Bytes                                         |
| ---------- | --------- | ------ | --------------------------------------------- |
| Immediate  | JLS #oper | 0x94   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Absolute   | JLS oper  | 0x95   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Indirect   | JLS @oper | 0x96   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Register   | JLS RX    | 0x97   | 2 ``opcode register``                         |



<a name="JNL"></a>

### JNL

Jump if not less result, jump to the given location if the less flag is not set

```
if !less; pc=data      x E I L  O C N Z
		       - - - -  - - - -
```

| Addressing | Assembler | Opcode | Bytes                                         |
| ---------- | --------- | ------ | --------------------------------------------- |
| Immediate  | JNL #oper | 0x98   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Absolute   | JNL oper  | 0x99   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Indirect   | JNL @oper | 0x9A   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Register   | JNL RX    | 0x9B   | 2 ``opcode register``                         |



<a name="JES"></a>

### JES

Jump if error is high, jump to the given location if the error flag is set

```
if error; pc=data      x E I L  O C N Z
		       - - - -  - - - -
```

| Addressing | Assembler | Opcode | Bytes                                         |
| ---------- | --------- | ------ | --------------------------------------------- |
| Immediate  | JES #oper | 0x9C   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Absolute   | JES oper  | 0x9D   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Indirect   | JES @oper | 0x9E   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Register   | JES RX    | 0x9F   | 2 ``opcode register``



<a name="CSF"></a>

### CSF

Clears all status flags

```
                  		x E I L  O C N Z
				+ + + +  + + + +
```

| Addressing | Assembler | Opcode | Bytes        |
| ---------- | --------- | ------ | ------------ |
| Implied    | CSF       | 0xA0   | 1 ``opcode`` |



<a name="CZF"></a>

### CZF

Clears zero flag

```
                  		x E I L  O C N Z
				- - - -  - - - +
```

| Addressing | Assembler | Opcode | Bytes        |
| ---------- | --------- | ------ | ------------ |
| Implied    | CZF       | 0xA1   | 1 ``opcode`` |



<a name="SZF"></a>

### SZF

Sets zero flag

```
                  		x E I L  O C N Z
				- - - -  - - - +
```

| Addressing | Assembler | Opcode | Bytes        |
| ---------- | --------- | ------ | ------------ |
| Implied    | SZF       | 0xA2   | 1 ``opcode`` |



<a name="CNF"></a>

### CNF

Clears negative flag

```
                  		x E I L  O C N Z
				- - - -  - - + -
```

| Addressing | Assembler | Opcode | Bytes        |
| ---------- | --------- | ------ | ------------ |
| Implied    | CNF       | 0xA3   | 1 ``opcode`` |



<a name="SNF"></a>

### SNF

Sets negative flag

```
                  		x E I L  O C N Z
				- - - -  - - + -
```

| Addressing | Assembler | Opcode | Bytes        |
| ---------- | --------- | ------ | ------------ |
| Implied    | SNF       | 0xA4   | 1 ``opcode`` |



<a name="CCF"></a>

### CCF

Clears carry flag

```
                  		x E I L  O C N Z
				- - - -  - + - -
```

| Addressing | Assembler | Opcode | Bytes        |
| ---------- | --------- | ------ | ------------ |
| Implied    | CCF       | 0xA5   | 1 ``opcode`` |



<a name="SCF"></a>

### SCF

Sets carry flag

```
                  		x E I L  O C N Z
				- - - -  - + - -
```

| Addressing | Assembler | Opcode | Bytes        |
| ---------- | --------- | ------ | ------------ |
| Implied    | SCF       | 0xA6   | 1 ``opcode`` |



<a name="COF"></a>

### COF

Clears Overflow flag

```
                  		x E I L  O C N Z
				- - - -  + - - -
```

| Addressing | Assembler | Opcode | Bytes        |
| ---------- | --------- | ------ | ------------ |
| Implied    | COF       | 0xA7   | 1 ``opcode`` |



<a name="SOF"></a>

### SOF

Sets Overflow flag

```
                  		x E I L  O C N Z
				- - - -  + - - -
```

| Addressing | Assembler | Opcode | Bytes        |
| ---------- | --------- | ------ | ------------ |
| Implied    | SOF       | 0xA8   | 1 ``opcode`` |



<a name="CLF"></a>

### CLF

Clears Less flag

```
                  		x E I L  O C N Z
				- - - +  - - - -
```

| Addressing | Assembler | Opcode | Bytes        |
| ---------- | --------- | ------ | ------------ |
| Implied    | CLF       | 0xA9   | 1 ``opcode`` |



<a name="SLF"></a>

### SLF

Sets Less flag

```
                  		x E I L  O C N Z
				- - - +  - - - -
```

| Addressing | Assembler | Opcode | Bytes        |
| ---------- | --------- | ------ | ------------ |
| Implied    | SLF       | 0xAA   | 1 ``opcode`` |



<a name="CIF"></a>

### CIF

Clear interrupt flag

```
interrupt = false 		x E I L  O C N Z
				- - + -  - - - -
```

| Addressing | Assembler | Opcode | Bytes        |
| ---------- | --------- | ------ | ------------ |
| Implied    | CIF       | 0xAB   | 1 ``opcode`` |



<a name="SIF"></a>

### SIF

Set interrupt disable.

```
interrupt = true 		 x E I L  O C N Z
				- - + -  - - - -
```

| Addressing | Assembler | Opcode | Bytes        |
| ---------- | --------- | ------ | ------------ |
| Implied    | SIF       | 0xAC   | 1 ``opcode`` |



<a name="CEF"></a>

### CEF

Clear error flag

```
error = false     		x E I L  O C N Z
				- + - -  - - - -
```

| Addressing | Assembler | Opcode | Bytes        |
| ---------- | --------- | ------ | ------------ |
| Implied    | CEF       | 0xAD   | 1 ``opcode`` |



<a name="INC"></a>

### INC

Increment the data at the specified location

```
data++               x E I P  O C N Z
                     - - - -  + + + +
```

| Addressing        | Assembler  | Opcode | Bytes                                         |
| ----------------- | ---------- | ------ | --------------------------------------------- |
| Absolute          | INC oper   | 0xB0   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Indirect          | INC @oper  | 0xB1   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Register          | INC RX     | 0xB2   | 2 ``opcode register``                         |
| 16-bit Absolute   | INCX oper  | 0xB3   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| 16-bit Indirect   | INCX @oper | 0xB4   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| 16-bit Register   | INCX RX    | 0xB5   | 2 ``opcode register``                         |


<a name="DEC"></a>

### DEC

Decrement the data at the specified location

```
data--               x E I P  O C N Z
                     - - - -  + + + +
```

| Addressing        | Assembler  | Opcode | Bytes                                         |
| ----------------- | ---------- | ------ | --------------------------------------------- |
| Absolute          | DEC oper   | 0xB6   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Indirect          | DEC @oper  | 0xB7   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| Register          | DEC RX     | 0xB8   | 2 ``opcode register``                         |
| 16-bit Absolute   | DECX oper  | 0xB9   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| 16-bit Indirect   | DECX @oper | 0xBA   | 3 ``opcode`` ``locationHigh`` ``locationLow`` |
| 16-bit Register   | DECX RX    | 0xBB   | 2 ``opcode register``                         |



## References

-----

https://en.wikipedia.org/wiki/Instruction_set_architecture
