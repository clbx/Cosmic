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
constantPattern = re.compile("(word|byte) [A-Z,a-z]{1}[A-Z,a-z,0-9]{0,128} [=] (0x)?[0-9,A-F,a-f]{1,4}")
vairablePattern = re.compile("var (word|byte) [A-Z,a-z]{1}[A-Z,a-z,0-9]{0,128} [=] (0x)?[0-9,A-F,a-f]{1,4}")

labelPattern = re.compile("[A-Z,a-z,0-9]{1,}:$")

constantTable = {}
variableTable = {}
labelTable = {}

currentLine = 0

#The instruction Set (Except MOV)
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

MovSet = {
    'MOVA':[0x30,0x31,0x32,0x33], #Moving to an Absolute Location
    'MOVI':[0x34,0x35,0x36,0x37], #Moving to an Indirect Location
    'MOVR':[0x38,0x39,0x3A,0x3B], #Moving to a Register
    'MOVXA':[0x40,0x41,0x42,0x43], #Moving 16 bits to an Absolute Location
    'MOVXI':[0x44,0x45,0x46,0x47], #Moving 16 bits to an Indirect Location
    'MOVXR':[0x48,0x49,0x4A,0x4B]  #Moving 16 bits to a register
}

    
#Returns the addressing mode
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


def getVariables(tokens):
    if(constantPattern.match(" ".join(tokens))):
        handleConstant(tokens)
    elif(vairablePattern.match(" ".join(tokens))):
        handleVariable(tokens)
    else:
        return

def handleConstant(tokens):
    return 0

def handleVariable(tokens):
    return 0


def handleOpcode(tokens):
    return 0

def handleMovOpcode(tokens):
    return 0

#Handle a label being encountered
def handleLabel(tokens):
    label = tokens[0][:-1]
    print("Encountered label {}".format(label))
    #check if the label is already in the label
    if(label in labelTable):
        error("Label {} already exists".format(label))
    #if its not already in the label table then we need to add it
    currentByte = len(output)
    print("Label {} correlates to position {}".format(label,currentByte))
    labelTable[label] = currentByte
    return


def assemble(tokens):
    #Check if it's in the opcode table
    if(tokens[0] in InstructionSet):
        print("Handling Opcode")
        handleOpcode(tokens)
        return
    #Check if it's a MOV

    if(tokens[0] == "MOV"):
        print("Handling Move Opcode")
        handleMovOpcode(tokens)
        return
    #Check if it's a label
    if(labelPattern.match(" ".join(tokens))):
        print("Handling Label")
        handleLabel(tokens)
        return
    #Check if it's a constant or variable
    if(constantPattern.match(" ".join(tokens)) or vairablePattern.match(" ".join(tokens))):
        print("Handling a variable or constant") #It should have already been handled in the first pass
        return
    #Check if it's a comment
    if(tokens[0][0] == ';'):
        print("Handling a Comment")
        return

    error("{} did not match any proper input".format(" ".join(tokens)))
    return 0


#Writes an error to the console. Stops exectuion
def error(msg):
    print("Error on line {} : {} ".format(currentLine,msg))
    sys.exit()

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

    #Gather Variables and put them in
    for i in range(0, len(instructions)):
        tokens = instructions[i].split()
        getVariables(tokens)


    #Go through line by line
    #Everything else, give Assemble one line at a time
    #Set current line
    for i in range(0, len(instructions)):
        curentLine = i+1
        tokens = instructions[i].split()
        assemble(tokens)

       
    

    outputFile = open('output.bin','w+b')
    outputFile.write(output)
    outputFile.close()


if __name__ == "__main__":
    main()
