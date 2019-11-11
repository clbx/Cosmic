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

    #Go through the instructions
    for i in range(0, len(instructions)):
        #Tokenize
        tokens = instructions[i].split()
        print(tokens)
        print(type(InstructionSet))

        
        #If its any opcode except MOV
        if(tokens[0] in InstructionSet):
            #Get Opcode
            addrMode = getAddrMode(instructions[i])
            output.append(InstructionSet[tokens[0]][addrMode])
            #Get Operand(s)

        #If its MOV/MOVX
        elif(tokens[0] == "MOV" or tokens[0] == "MOVX"):
            return 0

        #If it's something else! Figure out if its good or not.
        else:
            return 0


        
        
        



        print(output)

    
    outputFile = open('output.bin','w+b')
    outputFile.write(output)
    outputFile.close()


if __name__ == "__main__":
    main()
