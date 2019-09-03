# Processor Research 

Random thoughts/notes



#### Memory pages: 

memory pages are a fixed length block of virtual memory, described by a single entry in the page table 



**page table** 



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

