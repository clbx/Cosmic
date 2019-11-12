# Cosmic Assembler
# Author: Clay Buxton
#
# Assembles assembly code into machine code
#
#
# I have no idea how to (if you even can) write clean python code


import sys
import re 

output = bytearray()

bitnessPattern = re.compile("[A-Z]{3}[X].*$")
#Addressing Modes
impliedPattern = re.compile("[A-Z]{3,4}$")
immmediatePattern = re.compile("[A-Z]{3,4} [#][0-9,A-F]{1,4}$")
absolutePattern = re.compile("[A-Z]{3,4} [0-9,A-F]{1,4}$")
indirectPattern = re.compile("[A-Z]{3,4} [@][0-9,A-F]{1,4}$")
registerPattern = re.compile("[A-Z]{3,4} [R][0-7]$")

labelPattern = re.compile("[A-Z]{3,4} [A-Z,a-z,0-9]{1,}")

symbolTable = {}

InstructionSet = {
    'NOP':[0x00],
    'HCF':[0x01],
    'PUSH':[0x02],
    'POP':[0x03],
    'SWP':[0x04],
    'CALL':[0x05,0x06,0x07],
    'RET':[0x08],
    'ADD':[0x10,0x11,0x12,0x13],
    'ADDX':[0x14,0x15,0x17,0x17],
    'SUB':[0x18,0x19,0x1A,0x1B],
    'SUBX':[0x1C,0x1D,0x1E,0x1F],
    'MUL':[0x20,0x21,0x22,0x23],
    'MULX':[0x24,0x25,0x26,0x27],
    'DIV':[0x28,0x29,0x2A,0x2B],
    'DIVX':[0x2C,0x2D,0x2E,0x2F],
    'SHL':[0x3C,0x3D,0x3E,0x3F],
    'SHLX':[0x4C,0x4D,0x4E,0x4F],
    'AND':[0x50,0x51,0x52,0x53],
    'OR':[0x54,0x55,0x56,0x57],
    'XOR':[0x58,0x59,0x5A,0x5B],
    'CMP':[0x60,0x61,0x62,0x63],
    'JMP':[0x70,0x71,0x72],
    'JZS':[0x73,0x74,0x75],
    'JNZ':[0x76,0x77,0x78],
    'JCS':[0x79,0x7A,0x7B],
    'JNC':[0x7C,0x7D,0x7E],
    'JOS':[0x80,0x81,0x82],
    'JNS':[0x83,0x84,0x85],
    'SID':[]
}
    

def getAddrMode(instruction):
    if(immmediatePattern.match(instruction)):
        return 0
    elif(absolutePattern.match(instruction)):
        return 1
    elif(indirectPattern.match(instruction)):
        return 2
    elif(registerPattern.match(instruction)):
        return 3
    else:
        return -1

def handleMov(tokens):
    return 0


def main():
    if(len(sys.argv) < 2):
        print("Please supply a file")
        return -1

    #Put's all read instructions into a list by line
    inputFile = open(sys.argv[1],'r')
    instructions = list(inputFile)
    inputFile.close()

    print("Read " + str(len(instructions)) + " lines")



    #Go through the instructions
    for i in range(0, len(instructions)):
        #Tokenize
        tokens = instructions[i].split()     

        #If its any opcode except MOV
        if(tokens[0] in InstructionSet):            

            if(labelPattern.match(instructions[i]) and tokens[1] in symbolTable):    
                tokens[1] = str(symbolTable[tokens[1]])
                instructions[i] = " ".join(tokens)
                
            #Get Opcode
            addrMode = getAddrMode(instructions[i])
            output.append(InstructionSet[tokens[0]][addrMode])
            
            #Get Operand(s) if it's not implied
            if(impliedPattern.match(instructions[i])):
                break
            
            

            #If it's Absolute then we get all of the operands
            elif(absolutePattern.match(instructions[i])):
                #If operand is 16 bit split the number
                if(int(tokens[1],16) > 0xFF):
                    output.append(int(tokens[1][1:],16) >> 8)
                    output.append(int(tokens[1][1:],16) & 0xFF)
                else:
                    output.append(int(tokens[1],16))
            
            #Its immediate, indirect, or register
            else:
                #If operand is 16 bit split the number
                if(int(tokens[1][1:],16) > 0xFF):
                    output.append(int(tokens[1][1:],16) >> 8)
                    output.append(int(tokens[1][1:],16) & 0xFF)
                else:
                    output.append(int(tokens[1][1:],16))



        #If its MOV/MOVX
        elif(tokens[0] == "MOV" or tokens[0] == "MOVX"):
            print("hit mov")
            break;

        #If it's something else! Figure out if its good or not.
        else:
            #Make sure it's not supposed to be an opcode
            if(impliedPattern.match(tokens[0]) or immmediatePattern.match(tokens[0]) or absolutePattern.match(tokens[0]) or indirectPattern.match(tokens[0]) or registerPattern.match(tokens[0])):
                print("Syntax Error on line " + str(i+1))
                return -1
            
            elif(tokens[0][-1:] == ':'):
                if(tokens[0][:-1] in symbolTable):
                    print("Error line " + str(i+1) + ". " + str(tokens[0][:-1]) + " label already exists")
                symbolTable[tokens[0][:-1]] = len(output)+1

            else:
                print("You put something in line " + str(i+1) + " that is syntactically correct, but isn't so try again")
        
    

    outputFile = open('output.bin','w+b')
    outputFile.write(output)
    outputFile.close()


if __name__ == "__main__":
    main()
