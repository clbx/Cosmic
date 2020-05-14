byte brightness = FE
;Start at full brightness -1 so that the program terminates accordingly.
word start = 8000
;Starting pixel
loop:
	MOV brightness @start
	;Set the brightness of this pixel
	DEC brightness
    DEC brightness
    ;Decrement the brightness by two because the video out is 64x64
	INCX start
	CMP brightness
	JNZ #loop
	;Repeat until the brightness reaches 0
HCF
;End of program