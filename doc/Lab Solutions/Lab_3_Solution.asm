word inputLoc = C402
word outputLoc = C403
start:
	MOV inputLoc R0
    AND FF
	JNZ #turnON
    JMP #turnOFF
turnON:
	MOV FF outputLoc
    JMP #start
turnOFF:
	MOV 80 outputLoc
	JMP #start