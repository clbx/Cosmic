MOVX #8000 R0
loop:
MOVX R0 80
SHRX #7
SWP R0 R1
XOR 81
SHL #1
MOV R0 @80
MOVX 80 R0
INCX R0
AND #BF
JMP #loop