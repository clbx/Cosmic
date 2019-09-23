# Cosmic

Cosmic is a software simulated, from-scratch, 8-bit microcomputer designed to resemble retro computers. Cosmic consists of many parts including a simulated CPU, underlaying system, inputs and outputs, and graphics. In addition to the simulated machine, we plan to write software that targets the device. The goal of Cosmic is to develop a system that is equivalent to an Apple II, TRS-80, and similar machines of the era.

Elizabethtown College Senior Project - Clay Buxton, Kevin Carman


### Directory Structure


```
.
|- /doc - holds all of the documentaion for the project.
|- /bin - holds all ROM binaries    
|- /src - holds all source files
   |- Makefile 
   |- /sys  - holds all the source files for the system
   |- /proc - holds all the source files for the processor
   |- /aux  - holds all of the auxilarry software (simulation environment)
   |- /prog - holds all of the programs written for the system
   |- /asm  - holds the source files for the assembler

```