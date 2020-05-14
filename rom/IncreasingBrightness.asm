byte bright = 0
word loc = 8000
loop:
	MOV bright @loc
	INC bright
	INC loc
	MOV bright R0
	CMP #FF
	JNZ #loop
; nothing in variable space
; 31 1200 2480
; b0 1200 
; b3 2480
; 39 1200 00
; 60 ff
; 79 0000