###
#
#
#   this was my first attempt on writing an assembler for this language
#   i struggled a lot and the code quality got bad fast and I made a lot of mistakes
#   hopefully re-writing from scratch will go a bit better.
# 
#
#  im taking a different approach and instead of assembling for a broad, i'm assembling individually
#  for each opcode. This might be less clean, but makes it much easier to manage and also makes it more
#  exntensible in the future for when our instruction set inevitably changes.
#
#
#
##


import sys
import re 

output = bytearray()



bitnessPattern = re.compile("[A-Z]{3}[X].*$")
#Addressing Modes
impliedPattern = re.compile("[A-Z]{3,4}$")
immmediatePattern = re.compile("[A-Z]{3,4} [#][0-9,A-Z,a-z]{1,}$")
absolutePattern = re.compile("[A-Z]{3,4} [0-9,A-Z,a-z]{1,}$")
indirectPattern = re.compile("[A-Z]{3,4} [@][0-9,A-Z,a-z]{1,}$")
registerPattern = re.compile("[A-Z]{3,4} [R][0-7]$")
constantPattern = re.compile("(word|byte) [A-Z,a-z]{1}[A-Z,a-z,0-9]{0,128} [=] (0x)?[0-9,A-F,a-f]{1,4}")
variablePattern = re.compile("var (word|byte) [A-Z,a-z]{1}[A-Z,a-z,0-9]{0,128} [=] (0x)?[0-9,A-F,a-f]{1,4}")
hasVarConstPattern = re.compile("[A-Z]{3,4} (#|@|R)?[0-9,A-Z,a-z]{1,}$")
hexNumberPattern = re.compile("[A-F,0-9]{1,4}")
opcodePattern = re.compile("[A-Z,a-z]{3,4} [#,@,R]?[0-9,A-Z,a-z]{1,}([ ][#,@,R]?[0-9,A-Z,a-z]{1,})?$")

labelPattern = re.compile("[A-Z,a-z,0-9]{1,}:$")

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
    'CMPX':[0x64,0x65,0x66,0x67],
    'INC':[0x68,0x69,0x6A,0x6B],
    'DEC':[0x6C,0x6D,0x6E,0x6F],
    'JMP':[0x70,0x71,0x72,0x73],
    'JZS':[0x74,0x75,0x76,0x77],
    'JNZ':[0x78,0x79,0x7A,0x7B],
    'JCS':[0x7C,0x7D,0x7E,0x7F],
    'JNC':[0x80,0x81,0x82,0x83],
    'JOS':[0x84,0x85,0x86,0x87],
    'JNO':[0x88,0x89,0x8A,0x8B],
    'JNS':[0x8C,0x8D,0x8E,0x8F],
    'JNN':[0x90,0x91,0x92,0x93],
    'JLS':[0x94,0x95,0x96,0x97],
    'JNL':[0x98,0x99,0x9A,0x9B],
    'MOV':[0xFF],
    'MOVX':[0xFF]
}

#All of the instructions that use 16 bit operands
x16Instructions = {
    'PUSH','CALL','ADDX','SUBX','MULX',
    'DIVX','SHLX','JMP','JZS','JNZ','JCS',
    'JNC','JOS','JNS','MOVXA','MOVXI','MOVXR'
}

#All of the jump instructions
Jumps  = {
    'JMP','JZS','JCS','JNC','JOS','JNO','JNS',
    'JNN','JLS','JNL'
}

#All of the move instructions
Moves = {
    'MOVA','MOVI','MOVR'
}

Movesx16 = {
    'MOVXA','MOVXI','MOVXR'
}

MovSet = [
    [0xFF],                #This is to pad out immediate
    [0x30,0x31,0x32,0x33], #MOVA Moving to an Absolute Location
    [0x34,0x35,0x36,0x37], #MOVI Moving to an Indirect Location
    [0x38,0x39,0x3A,0x3B]  #MOVR Moving to a Register
]

MovSetx16 = [
    [0xFF],
    [0x40,0x41,0x42,0x43], #MOVXA Moving 16 bits to an Absolute Location
    [0x44,0x45,0x46,0x47], #MOVXI Moving 16 bits to an Indirect Location
    [0x48,0x49,0x4A,0x4B]  #MOVXR Moving 16 bits to a register
]
    
#Returns the addressing mode
def getAddrMode(tokens):
    instruction = " ".join(tokens)
    if(impliedPattern.match(instruction)):
        return 0
    
    elif(immmediatePattern.match(instruction)):
        return 1
    #Register is moved out front because absolute was shorting it, since it technically fits the regex.
    #I should be fixed somehow as well.
    elif(registerPattern.match(instruction)):
        return 4
    elif(absolutePattern.match(instruction)):
        return 2
    elif(indirectPattern.match(instruction)):
        return 3
   
    else:
        return -1


'''
Goes through the assembly code and picks out any variable declarations, then adds
them to the front of the program to be used later
'''
def getVariables(tokens):
    if(tokens[0] == "var"):
        if(tokens[2] in variableTable):
            error("Variable {} already initalized".format(tokens[2]))   
        if(tokens[1] == "byte"):
            variableTable[tokens[2]] = len(output)
            output.append(int(tokens[4],16))
            print("Added byte var {} val {} at {}".format(tokens[2],tokens[4],len(output)-1))
        elif(tokens[1] == "word"):
            variableTable[tokens[2]] = len(output)
            val = int(tokens[4],16)
            output.append((val >> 8) & 0xff)
            output.append((val & 0xff))
            print("Added word {} val {} at {}".format(tokens[2],tokens[4],len(output)-2))
        else:
            error("Type {} is not a valid type (byte|word)".format(tokens[1]))     



'''
When a label declaration is found, it sets that labvel equal to the current position
'''
def handleLabel(tokens):
    label = tokens[0][:-1]
    if(label in labelTable):
        error("Label {} alread in label table".format(label))
    else:
        print("Added label {} to the label table".format(label))
        labelTable[label] = len(output)

'''
Checks if a variable or label is present in a opcode call and replaces it with the correct value
Don't touch it otherwise
'''
def checkVar(tokens):
    #Check if it's a MOV instruciton, if it is the variables could be in different places and an extra place
    #Come back to me and add for addressing modes
    if('MOV' in tokens[0]):
        print("HIT")
        print(variableTable)

        dstMode = getAddrMode([tokens[0],tokens[2]])
        srcMode = getAddrMode([tokens[0],tokens[1]])
        
        dstOperator = ""
        dstOperand = ""
        srcOperator = ""
        srcOperand = ""
        print(srcMode)

        if(dstMode == 0 or dstMode == 2):
            dstOperand = tokens[2]
        else:
            dstOperator = tokens[2][0]
            dstOperand = tokens[2][1:]

        if(srcMode == 0 or srcMode == 2):
            srcOperand = tokens[1]
        else:
            srcOperator = tokens[1][0]
            srcOperand = tokens[1][1:]

        if(srcOperand in variableTable):
            tokens[1] = str(srcOperator) + str(output[variableTable[srcOperand]])

        if(dstOperand in variableTable):
            tokens[2] =  str(dstOperator) + str(output[variableTable[dstOperand]])

        print(tokens)

        return tokens

    #For all the other opcodes
    else:
        #We want to break down the operand, so we want to separate the #,R or @ from the rest of the operand
        #We'll re add it at the end, unless its a label.
        addrmode = getAddrMode(tokens)
        operator = ""
        operand = ""


        if(addrmode == 0 or addrmode == 2):
            operand = tokens[1]
        else:
            operator = tokens[1][0]
            operand = tokens[1][1:]
        #If the operand is a hex number, we can hop off the bus now
        if(hexNumberPattern.match(operand)):
            return tokens

        #Check if it's a jump to check for lables, Jumps should be the only opcode using labels
        if(tokens[0] in Jumps):
            if(operand in labelTable):
                tokens[1] = str(operator) + str(labelTable[operand])
                return tokens
            else:
                error("Label {} not in label table".format(operand))
            return




        #Now for everyone else
        elif(operand in variableTable):
            if(operand in variableTable):
                print(variableTable)
                tokens[1] = str(operator) + str(output[variableTable[operand]])
                return tokens
            else:
                error("Variable {} not in variable table".format(operand))

        else:
            return tokens
     




def handleOpcode(tokens):
    #Check if it has a variable in it
    tokens = checkVar(tokens)
    print("TOKENS" + str(tokens))
    #handle the rest here
    #if its a mov
    if("MOV" in tokens[0]):
        dstMode = getAddrMode([tokens[0],tokens[2]])
        srcMode = getAddrMode([tokens[0],tokens[1]])


        dstOperand = ""
        srcOperand = ""

        #Fix me too PLEASE
        if(dstMode == 1):
            dstMode = 0
        
        if(srcMode == 1):
            srcMode = 0
        
        if(dstMode == 2):
            dstMode = 1
        
        if(srcMode == 2):
            srcMode = 1
        
        if(dstMode == 3):
            dstMode = 2
        
        if(srcMode == 3):
            srcMode = 2
        
        if(dstMode == 4):
            dstMode = 3
        
        if(srcMode == 4):
            srcMode = 3

        if(dstMode == 0 or dstMode == 1):
            dstOperand = tokens[2]
        else:
            dstOperand = tokens[2][1:]
        
        if(srcMode == 0 or srcMode == 1):
            srcOperand = tokens[1]
        else:
            srcOperand = tokens[1][1:]         

        #This will need to be improved to allow for 16 bit operations. though preferably when im not tired
        output.append(MovSet[dstMode][srcMode])
        if(srcMode == 1 or srcMode == 2):
            val = int(srcOperand,16)
            output.append((val >> 8) & 0xFF)
            output.append((val & 0xFF))
        else:
            output.append(int(srcOperand,16))


        if(dstMode == 1 or dstMode == 2):
            val = int(dstOperand,16)
            output.append((val >> 8) & 0xFF)
            output.append((val & 0xFF))
        else:
            output.append(int(dstOperand,16))



    #otherwise
    else:
        if(tokens[0] not in InstructionSet):
            error("Unknown opcode {}".format(tokens[0]))
        
        #Done because i changed how it return addr mode.
        #fix me later PLEASE
        '''
        before i forget. it was changed so that I could reference the position in the
        instruction set lists, but that fucks with getting the prefix in
        so I changed the prefix, so now it works, but it's really just a band-aid. A more solid
        solution will need to be made.
        '''
        addrmode = getAddrMode(tokens)

        print(addrmode)

        if(addrmode == 1):
            addrmode = 0
        elif(addrmode == 2):
            addrmode = 1
        elif(addrmode == 3):
            addrmode = 2
        elif(addrmode == 4):
            addrmode = 3

        output.append(InstructionSet[tokens[0]][addrmode])
        
        if(len(tokens) == 1):
            return

        print("addr:" + str(addrmode))
        if(addrmode == 1):
            operand = tokens[1]
        else:
            operand = tokens[1][1:]

        print(tokens)
        


        #Need to pad for 16 bit instructions        
        if(tokens[0] in x16Instructions):
            val = int(operand,16)

            output.append((val >> 8) & 0xFF)
            output.append((val & 0xFF))
        else:
            output.append(int(operand,16))



        

    


def assemble(tokens):
    #It can be a label, lables always end with ":"
    if(tokens[0][-1:] == ":"):
        handleLabel(tokens)
    #If its an opcode
    elif(opcodePattern.match(" ".join(tokens))):
        handleOpcode(tokens)
    #If its a comment
    elif(tokens[0][0] == ";"):
        pass
    #If its a variable assignement skip it
    elif(tokens[0] == "var"):
        pass
    else:
        error("Unknown thing")
    



#Writes an error to the console. Stops exectuion
def error(msg):
    print("[Error] line {} : {} ".format(currentLine,msg))
    sys.exit()

#Writes a warning to the console
def warning(msg):
    print("[Warning] line {} : {}".format(currentLine,msg))




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
