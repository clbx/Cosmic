byte fib = 5
;The Fibonacci number we wish to calculate
byte counter = 2
byte first = 0
byte second = 1
;Initialize the first two numbers in the sequence to be 1
byte final = 0
JMP #08
end:
    MOV R0 final
    HCF
    ;End of program
MOV #1 R0
CMP fib
JZS #end
;Check to see if fib is 1
calculate:
    MOV first R0
    ADD second
    MOV second first
    MOV R0 second
    MOV fib R0
    CMP counter
    MOV second R0
    ;Needed since we store R0 in final
    JZS #end
    INC counter
    JMP #11