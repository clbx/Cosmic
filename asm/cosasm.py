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
variablePattern = re.compile("var (word|byte) [A-Z,a-z]{1}[A-Z,a-z,0-9]{0,128} [=] (0x)?[0-9,A-F,a-f]{1,4}")
hasVarConstPattern = re.compile("[A-Z]{3,4} (#|@|R)?[0-9,A-Z,a-z]{1,}$")

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
    'SID':[0x09],
    'ADD':[0x10,0x11,0x12,0x13],
    'ADDX':[0x14,0x15,0x16,0x17],
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
    'JMP':[0x70,0x71,0x72,0x73],
    'JZS':[0x74,0x75,0x76,0x77],
    'JNZ':[0x78,0x79,0x7A,0x7B],
    'JCS':[0x7C,0x7D,0x7E,0x7F],
    'JNC':[0x80,0x81,0x82,0x83],
    'JOS':[0x84,0x85,0x86,0x87],
    'JNS':[0x88,0x89,0x8A,0x8B]
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
    if(immmediatePattern.match(instruction) or impliedPattern.match(instruction)):
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
    elif(variablePattern.match(" ".join(tokens))):
        handleVariable(tokens)
    else:
        return

# Handles a constant, loads it into memory and assigns it a location
def handleConstant(tokens):
    if(tokens[0] == "word"):
        constantTable[tokens[1]] = len(output)
        output.append(int(tokens[3][2:4],16))
        output.append(int(tokens[3][4:6],16))
        print("Created constant of size word pointing to {}".format(len(output)-2))
    elif(tokens[0] == "byte"):
        constantTable[tokens[1]] = len(output)
        output.append(int(tokens[3][2:4],16))
        print("Created variable of size byte pointing to {}".format(len(output)-1))   
    else:
        error("Invalid size given: {}".format(tokens[0]))    
    return 0

def handleVariable(tokens):
    if(tokens[1] == "word"):
        output.append(int(tokens[4][2:4],16))
        output.append(int(tokens[4][4:6],16))

        if(tokens[2] not in variableTable):
            variableTable[tokens[2]] = len(output)-2

        print("Created variable of size word pointing to {}".format(len(output)-2))

    elif(tokens[1] == "byte"):
        output.append(int(tokens[4][2:4],16))   

        if(tokens[2] not in variableTable):
            variableTable[tokens[2]] = len(output)-1

        print("Created variable of size byte pointing to {}".format(len(output)-1))

    else:
        error("Invalid size given: {}".format(tokens[0]))
    return 0

#im garbage fix me
def handleOpcode(tokens):
    #print("OPCODE HANDLING {}".format(tokens))
    if(hasVar(tokens)):
        if(hasVarConstPattern.match(" ".join(tokens))):
            if(tokens[1][0] == "#" or tokens[1][0] == "@" or tokens[1][0] == "R"):
                front = tokens[1][0]
                if(tokens[1][1:] in constantTable):
                    tokens[1][1] = front + constantTable[tokens[1][1:]]
                elif(tokens[1][1:] in variableTable):
                    tokens[1][1] = front + variableTable[tokens[1][1:]]
                elif(tokens[1][1:] in labelTable):
                    tokens[1][1] = front + labelTable[tokens[1][1:]]
                else:
                    error("Invalid variable or constant {}".format(tokens[1]))
            else:
                if(tokens[1] in constantTable):
                    tokens[1] = str(constantTable[tokens[1]])
                elif(tokens[1] in variableTable):
                    tokens[1] = str(variableTable[tokens[1]])
                elif(tokens[1] in labelTable):
                    tokens[1] = str(labelTable[tokens[1]])
                else:
                    error("Invalid variable or constant {}".format(tokens[1]))
        else:
            error("Something went wrong, idk what right now, maybe i'll not write shit code in the future and find out")

    #at this point all var/const _should_ be resolved to a value
    print(tokens)
    addrMode = getAddrMode(" ".join(tokens))
    opcode = InstructionSet[tokens[0]][addrMode]
    output.append(opcode)
    if(tokens[1][0] == "#" or tokens[1][0] == "@" or tokens[1][0] == "R"):
        output.append(int(tokens[1][1:],16))
    else:
        output.append(int(tokens[1],16))
      


    return 0

# im garbage fix me
def hasVar(tokens):
    if(impliedPattern.match(" ".join(tokens)) or immmediatePattern.match(" ".join(tokens)) or absolutePattern.match(" ".join(tokens)) or indirectPattern.match(" ".join(tokens)) or registerPattern.match(" ".join(tokens))):
        return False
    else:
        return True

def handleMovOpcode(tokens):
    return 0

#Handle a label being encountered
def handleLabel(tokens):
    label = tokens[0][:-1]
    #check if the label is already in the label
    if(label in labelTable):
        error("Label \"{}\" already exists".format(label))
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
        handleLabel(tokens)
        return
    #Check if it's a constant or variable
    if(constantPattern.match(" ".join(tokens)) or variablePattern.match(" ".join(tokens))):
        print("Skipping: Variable or constant") #It should have already been handled in the first pass
        return
    #Check if it's a comment
    if(tokens[0][0] == ';'):
        print("Skipping: Comment")
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

    # The original jump instruction to go to start of program
    output.append(0x70)
    output.append(0xFF)
    output.append(0xFF)


    #Put's all read instructions into a list by line
    inputFile = open(sys.argv[1],'r')
    instructions = list(inputFile)
    inputFile.close()

    print("Read " + str(len(instructions)) + " lines\n")

    #Go through the instructions    

    print("-= First Pass: Finding Variables and Constants =-")
    #Gather Variables and Constants and put them in
    for i in range(0, len(instructions)):
        tokens = instructions[i].split()
        getVariables(tokens)

    output[1] = (len(output) >> 8) & 0xff
    output[2] = len(output) & 0xff
    print("Starting program at position {}".format(len(output)))


    print("\n\n-= Second Pass: Assembly =-")    
    #Go through line by line
    #Everything else, give Assemble one line at a time
    #Set current line
    for i in range(0, len(instructions)):
        tokens = instructions[i].split()
        assemble(tokens)

       
    
    print("\n\n-= Output: =-")
    for i in range(0, len(output)):
        print(hex(output[i]))
    outputFile = open('output.bin','w+b')
    outputFile.write(output)
    outputFile.close()


if __name__ == "__main__":
    main()
