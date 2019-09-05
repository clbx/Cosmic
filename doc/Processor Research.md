**Cosmic** is a software simulated, from-scratch, 8-bit microcomputer designed to resemble retro computers. Cosmic consists of many parts including a simulated CPU, underlaying system, inputs and outputs, and graphics. In addition to the simulated machine, we plan to write software that targets the device. The goal of Cosmic is to develop a system that is equivalent to an Apple II, Commodore 64, or similar machines of the early 1980s

**memory pages** are a fixed length block of virtual memory, described by a single entry in the page table 



### questions that need answering

* how to handle memory, treat memory and I/O the same? Create memory pages? how to handle "slots"

  8051 has page 0 for stack page 1 for "register space"







### Addressing Modes

------

* **Implied**: Opcode has a defined source/destination and does not need to be specified
* **Immediate**: #, uses data given in operand (constant) ``MOV #35H, A`
* **Register Addressing**: R, Addresses a register 
* **Absolute**: Gets the value at an address 
* **Indirect**: Gets the value from the address that the operand specifies   





### Registers

------

For Cosmic: 8 general purpose registers 8-bit, can be used as 16bit, (H/L registers)

**Z80**

* **General registers**: A,B,C,D,E Flags, and H/L 8bit registers. All except A and Flag can be used in 16bit. A is Accumulator, Flags holds flags. 

  A 16bit stack pointer

  A 16bit program counter

  _these registers are identical to the i8080_

* **New Z80 Registers** (not sure what any of them do)

  * **IX** & **IY** 16-bit index or base register for 8-biut immediate offsets of 2 8bit registers
  * **I** Interrupt vector base, 8bit. Holds some kind of priority
  * **R** Dram refresh counter, 8bit. Increments every time an opcode is fetched. Is used as a hardware counter, or to create "random" numbers
  * **Alternate registers** cannot be accessed directly, only can be accsed by ``EX`` and ``EXX`` instructions which (from what I can tell ) just flip the registers and alternate registers
  * 4 Bits of interrupt status


**6502**

​	very few registers

* 8bit Accumulator
* 2 8-Bit index registers
* 7-Bit processor status flag
* 8-Bit stack pointer
* 16-bit program counter

However, first 256bytes of RAM can be directly addressed using one address byte. 

