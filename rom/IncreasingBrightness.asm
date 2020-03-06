byte bright = 0
word loc = 8000
loop:
	MOV #bright loc
	INC bright
	INC loc
	MOV #bright R0
	CMP #FF
	JNZ loop