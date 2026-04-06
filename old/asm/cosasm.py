import sys
import math
import re


#The Instruction Set
InstructionSet = {
    "IMP NOP":0x00,
    "IMP HCF":0x01,
    "IMP PUSH":0x02,
    "IMP POP":0x03,
    "REG SWP":0x04,
    "IMM CALL":0x05,
    "ABS CALL":0x06,
    "IND CALL":0x07,
    "IMP RET":0x08,
    "IMM ADD":0x10,
    "ABS ADD":0x11,
    "IND ADD":0x12,
    "REG ADD":0x13,
    "IMM ADDX":0x14,
    "ABS ADDX":0x15,
    "IND ADDX":0x16,
    "REG ADDX":0x17,
    "IMM SUB":0x18,
    "ABS SUB":0x19,
    "IND SUB":0x1A,
    "REG SUB":0x1B,
    "IMM SUBX":0x1C,
    "ABS SUBX":0x1D,
    "IND SUBX":0x1E,
    "REG SUBX":0x1F,
    "IMM MUL":0x20,
    "ABS MUL":0x21,
    "IND MUL":0x22,
    "REG MUL":0x23,
    "IMM MULX":0x24,
    "ABS MULX":0x25,
    "IND MULX":0x26,
    "REG MULX":0x27,
    "IMM DIV":0x28,
    "ABS DIV":0x29,
    "IND DIV":0x2A,
    "REG DIV":0x2B,
    "IMM DIVX":0x2C,
    "ABS DIVX":0x2D,
    "IND DIVX":0x2E,
    "REG DIVX":0x2F,
    "IMM ABS MOV":0x30,
    "ABS ABS MOV":0x31,
    "IND ABS MOV":0x32,
    "REG ABS MOV":0x33,
    "IMM IND MOV":0x34,
    "ABS IND MOV":0x35,
    "IND IND MOV":0x36,
    "REG IND MOV":0x37,
    "IMM REG MOV":0x38,
    "ABS REG MOV":0x39,
    "IND REG MOV":0x3A,
    "REG REG MOV":0x3B,
    "IMM SHL":0x3C,
    "ABS SHL":0x3D,
    "IND SHL":0x3E,
    "REG SHL":0x3F,
    "IMM ABS MOVX":0x40,
    "ABS ABS MOVX":0x41,
    "IND ABS MOVX":0x42,
    "REG ABS MOVX":0x43,
    "IMM IND MOVX":0x44,
    "ABS IND MOVX":0x45,
    "IND IND MOVX":0x46,
    "REG IND MOVX":0x47,
    "IMM REG MOVX":0x48,
    "ABS REG MOVX":0x49,
    "IND REG MOVX":0x4A,
    "REG REG MOVX":0x4B,
    "IMM SHLX":0x4C,
    "ABS SHLX":0x4D,
    "IND SHLX":0x4E,
    "REG SHLX":0x4F,
    "IMM AND":0x50,
    "ABS AND":0x51,
    "IND AND":0x52,
    "REG AND":0x53,
    "IMM OR":0x54,
    "ABS OR":0x55,
    "IND OR":0x56,
    "REG OR":0x57,
    "IMM XOR":0x58,
    "ABS XOR":0x59,
    "IND XOR":0x5A,
    "REG XOR":0x5B,
    "IMM SHR":0x5C,
    "ABS SHR":0x5D,
    "IND SHR":0x5E,
    "REG SHR":0x5F,
    "IMM CMP":0x60,
    "ABS CMP":0x61,
    "IND CMP":0x62,
    "REG CMP":0x63,
    "IMM CMPX":0x64,
    "ABS CMPX":0x65,
    "IND CMPX":0x66,
    "REG CMPX":0x67,
    "IMM SHRX":0x6C,
    "ABS SHRX":0x6D,
    "IND SHRX":0x6E,
    "REG SHRX":0x6F,
    "IMM JMP":0x70,
    "ABS JMP":0x71,
    "IND JMP":0x72,
    "REG JMP":0x73,
    "IMM JZS":0x74,
    "ABS JZS":0x75,
    "IND JZS":0x76,
    "REG JZS":0x77,
    "IMM JNZ":0x78,
    "ABS JNZ":0x79,
    "IND JNZ":0x7A,
    "REG JNZ":0x7B,
    "IMM JCS":0x7C,
    "ABS JCS":0x7D,
    "IND JCS":0x7E,
    "REG JCS":0x7F,
    "IMM JNC":0x80,
    "ABS JNC":0x81,
    "IND JNC":0x82,
    "REG JNC":0x83,
    "IMM JOS":0x84,
    "ABS JOS":0x85,
    "IND JOS":0x86,
    "REG JOS":0x87,
    "IMM JNO":0x88,
    "ABS JNO":0x89,
    "IND JNO":0x8A,
    "REG JNO":0x8B,
    "IMM JNS":0x8C,
    "ABS JNS":0x8D,
    "IND JNS":0x8E,
    "REG JNS":0x8F,
    "IMM JNN":0x90,
    "ABS JNN":0x91,
    "IND JNN":0x92,
    "REG JNN":0x93,
    "IMM JLS":0x94,
    "ABS JLS":0x95,
    "IND JLS":0x96,
    "REG JLS":0x97,
    "IMM JNL":0x98,
    "ABS JNL":0x99,
    "IND JNL":0x9A,
    "REG JNL":0x9B,
    "IMM JES":0x9C,
    "ABS JES":0x9D,
    "IND JES":0x9E,
    "REG JES":0x9F,
    "IMP CSF":0xA0,
    "IMP CZF":0xA1,
    "IMP SZF":0xA2,
    "IMP CNF":0xA3,
    "IMP SNF":0xA4,
    "IMP CCF":0xA5,
    "IMP SCF":0xA6,
    "IMP COF":0xA7,
    "IMP SOF":0xA8,
    "IMP CLF":0xA9,
    "IMP SLF":0xAA,
    "IMP CIF":0xAB,
    "IMP SIF":0xAC,
    "IMP CEF":0xAD,
    "ABS INC":0xB0,
    "IND INC":0xB1,
    "REG INC":0xB2,
    "ABS INCX":0xB3,
    "IND INCX":0xB4,
    "REG INCX":0xB5,
    "ABS DEC":0xB6,
    "IND DEC":0xB7,
    "REG DEC":0xB8,
    "ABS DECX":0xB9,
    "IND DECX":0xBA,
    "REG DECX":0xBB
}

opcodePattern = re.compile("[A-Z,a-z]{3,4}( [#,@,R]?[0-9,A-F]{1,}([ ][#,@,R]?[0-9,A-F]{1,})?)?$")


#variableTable = {
#   "varName" = [pos,len]
#   "counter" = [5,5]
#}
variableTable = {} #Where all variables are stored
labelTable = {} #Where all labels are stored
output = bytearray() #The amount of usable bytes for program data size 32768
variables = bytearray() #The mount of usable bytes for variables size 5120
currentLine = 0 #Current line of assembly


types = { #Valid variable types
    "word","byte"
}


#Gets the addressing mode of the function by looking at a single operand
#Cannot find Implied
def getAddrMode(token):
    if(token[0] == "#"):
        return "IMM"
    if(token[0] == "@"):
        return "IND"
    if(token[0] == "R"):
        return "REG"
    return "ABS"

#Helper function for adding large variables to the variable table
def addToVariables(value, size=0):
    val = int(str(value),16)
    if(size == 0):
        size = int(math.ceil((val.bit_length())/8))
    byteArr = val.to_bytes(size,byteorder="big")
    variables.extend(byteArr)
    
    
#Variable Creation: <type> <name> = <value>
#All new types of variables can be put here
def createVar(tokens):
    #If it doesn't fit that setup, return.
    if(not tokens[2] == "="):
        return

    #byte counter = 5
    if(tokens[0] == "byte"):
        identifier = tokens[1]
        value = tokens[3]
        variableTable[identifier] = [len(variables),1]
        addToVariables(value,1)
        return
    #word points = 50
    if (tokens[0] == "word"):
        identifier = tokens[1]
        value = tokens[3]
        variableTable[identifier] = [len(variables),2]
        addToVariables(value,2)
    else:
        error("Unknown type {}".format(tokens[0]))

#Returns tokens with variables resolved
def resolveVariables(tokens):
    #Go through the length of the opcode starting with the first operand and find variables
    for i in range(1,len(tokens)):

        addrMode = getAddrMode(tokens[i])

        operator = ""
        operand = ""
        newoperand = ""

        if(addrMode == "ABS"):
            operand = tokens[i]
        else:
            operand = tokens[i][1:]
            operator = tokens[i][0]


        if(operand in variableTable):

            #print(variables[variableTable[operand][0]])


            if(variableTable[operand][1] > 2):
                warning("Variable {} is larger than opcode can handle".format(operand))
            
            print("LOCATION: {}".format((variableTable[operand][0] + 0xC800)))

            print("TEST {}".format(0x09+0x02,'x'))
            tokens[i] = operator + format((variableTable[operand][0] + 0xC800),'x')
            #Needs to return 51200
            print("AFTER LOC: {}".format(tokens[i]))

        if(operand in labelTable):
            operand = labelTable[operand]
            tokens[i] = str(operator) + str(operand)
        


    return tokens

def handleLabel(tokens):
    label = tokens[0][:-1]
    labelTable[label] = len(output)
    

def updateVar(tokens):
    print("Updating variable")

#maybe replace with regex if we're feeling brave
def assemble(tokens):
    #print(tokens)
    #If its variable creation:
    if(tokens[0] in types and tokens[2] == "="):
        createVar(tokens)

    #If its variable assigning:
    elif(tokens[0] in variableTable and tokens[1] == "="):
        updateVar(tokens)

    #If its a label
    elif(tokens[0][-1] == ":"):
        handleLabel(tokens)

    #If its a comment
    elif(tokens[0][0] == ";"):
        pass

    #Else its an opcode
    else: 
        tokens = resolveVariables(tokens)
        try:
            eval(tokens[0])(tokens)
        except NameError:
            error("Unknown Input {}".format(tokens))


def getOperand(token):
    addrMode = getAddrMode(token)
    operand = ""
    operator = ""
    if(addrMode == "IMP" or addrMode == "ABS"):
            operand = token
    else:
        operator = token[0]
        operand = token[1:]
    print(operand)
    return addrMode,operator,int(operand,16)



#Handle a Standard 8 Bit opcode

#General function for opcodes with the format [opcode] [operand] where
#the operand is 8 bits.
def handleStd8bitOpcode(tokens):
    print("Tokens in 8bit std: {}".format(tokens))
    addrMode,_,operand = getOperand(tokens[1])
    print(type(operand))
    print("Operand in 8bit: {}".format(operand))
    if(addrMode == "IMM" or addrMode == "REG"):
        output.append(InstructionSet[addrMode + " " + tokens[0]])
        output.append(operand)
    else:
        output.append(InstructionSet[addrMode + " " + tokens[0]])
        output.append((operand >> 8) & 0xFF)
        output.append(operand & 0xFF)


#Handle a Standard 16 Bit opcode

#General function for opcodes with the format [opcode] [operand] where
#the operand is 16 bits.
def handleStd16bitOpcode(tokens):
    addrMode,_,operand = getOperand(tokens[1])
    if(addrMode == "REG"):
        output.append(InstructionSet[addrMode + " " + tokens[0]])
        output.append((operand & 0xFF))
    else:       
        output.append(InstructionSet[addrMode + " " + tokens[0]])
        output.append((operand >> 8) & 0xFF)
        output.append((operand & 0xFF))


        

# -= INSTRUCTION FUNCTIONS =- #

def NOP(tokens):
    output.append(InstructionSet["IMP NOP"])

def HCF(tokens):
    output.append(InstructionSet["IMP HCF"])

def PUSH(tokens):
    output.append(InstructionSet["IMP PUSH"])

def POP(tokens):
    output.append(InstructionSet["IMP POP"])

def SWP(tokens):
    output.append(InstructionSet["REG SWP"])
    _,_,operand = getOperand(tokens[1])
    output.append(operand)
    _,_,operand = getOperand(tokens[2])
    output.append(operand)

def CALL(tokens):
    handleStd16bitOpcode(tokens)
    
def RET(tokens):
    output.append(InstructionSet["IMP RET"])

def ADD(tokens):
    handleStd8bitOpcode(tokens)
    
def ADDX(tokens):
    handleStd16bitOpcode(tokens)

def SUB(tokens):
    handleStd8bitOpcode(tokens)

def SUBX(tokens):
    handleStd16bitOpcode(tokens)

def MUL(tokens):
    handleStd8bitOpcode(tokens)

def MULX(tokens):
    handleStd16bitOpcode(tokens)

def DIV(tokens):
    handleStd8bitOpcode(tokens)

def DIVX(tokens):
    handleStd16bitOpcode(tokens)

def MOV(tokens):
    addrMode1,_,operand1 = getOperand(tokens[1])
    addrMode2,_,operand2 = getOperand(tokens[2])
    output.append(InstructionSet[addrMode1 + " " + addrMode2 + " " + tokens[0]])
    if(addrMode1 == "IMM" or addrMode1 == "REG"):
        output.append(operand1)
    else:
        output.append((operand1 >> 8) & 0xFF)
        output.append((operand1 & 0xFF))
    if(addrMode2 == "IMM" or addrMode2 == "REG"):
        output.append(operand2)
    else:
        output.append((operand2 >> 8) & 0xFF)
        output.append((operand2 & 0xFF))

def MOVX(tokens):
    addrMode1,_,operand1 = getOperand(tokens[1])
    addrMode2,_,operand2 = getOperand(tokens[2])
    output.append(InstructionSet[addrMode1 + " " + addrMode2 + " " + tokens[0]])
    if(addrMode1 == "REG"):
        output.append((operand1 & 0xFF))
    else:
        output.append((operand1 >> 8) & 0xFF)
        output.append((operand1 & 0xFF))
    if(addrMode2 == "REG"):
        output.append((operand2 & 0xFF))
    else:
        output.append((operand2 >> 8) & 0xFF)
        output.append((operand2 & 0xFF))

def SHL(tokens):
    handleStd8bitOpcode(tokens)

def SHLX(tokens):
    handleStd16bitOpcode(tokens)

def AND(tokens):
    handleStd8bitOpcode(tokens)

def OR(tokens):
    handleStd8bitOpcode(tokens)

def XOR(tokens):
    handleStd8bitOpcode(tokens)

def SHR(tokens):
    handleStd8bitOpcode(tokens)

def CMP(tokens):
    handleStd8bitOpcode(tokens)

def CMPX(tokens):
    handleStd16bitOpcode(tokens)

def SHRX(tokens):
    handleStd16bitOpcode(tokens)

def JMP(tokens):
    handleStd16bitOpcode(tokens)

def JZS(tokens):
    handleStd16bitOpcode(tokens)

def JNZ(tokens):
    handleStd16bitOpcode(tokens)

def JCS(tokens):
    handleStd16bitOpcode(tokens)

def JNC(tokens):
    handleStd16bitOpcode(tokens)

def JOS(tokens):
    handleStd16bitOpcode(tokens)

def JNO(tokens):
    handleStd16bitOpcode(tokens)

def JNS(tokens):
    handleStd16bitOpcode(tokens)

def JNN(tokens):
    handleStd16bitOpcode(tokens)

def JLS(tokens):
    handleStd16bitOpcode(tokens)

def JNL(tokens):
    handleStd16bitOpcode(tokens)

def JES (tokens):
    handleStd16bitOpcode(tokens)

def CSF(tokens):
    output.append(InstructionSet["IMP CSF"])

def CZF(tokens):
    output.append(InstructionSet["IMP CZF"])

def SZF(tokens):
    output.append(InstructionSet["IMP SZF"])

def CNF(tokens):
    output.append(InstructionSet["IMP CNF"])

def SNF(tokens):
    output.append(InstructionSet["IMP SNF"])

def CCF(tokens):
    output.append(InstructionSet["IMP CCF"])

def SCF(tokens):
    output.append(InstructionSet["IMP SCF"])

def COF(tokens):
    output.append(InstructionSet["IMP COF"])

def SOF(tokens):
    output.append(InstructionSet["IMP SOF"])

def CLF(tokens):
    output.append(InstructionSet["IMP CLF"])

def SLF(tokens):
    output.append(InstructionSet["IMP SLF"])

def CIF(tokens):
    output.append(InstructionSet["IMP CIF"])

def SIF(tokens):
    output.append(InstructionSet["IMP SIF"])

def CEF(tokens):
    output.append(InstructionSet["IMP CEF"])

def INC(tokens):
    handleStd8bitOpcode(tokens)

def INCX(tokens):
    handleStd16bitOpcode(tokens)

def DEC(tokens):
    handleStd8bitOpcode(tokens)

def DECX(tokens):
    handleStd16bitOpcode(tokens)

#Writes an error to the console. Stops exectuion
def error(msg):
    print("[Error] line {} : {} ".format(currentLine,msg))
    sys.exit()

#Writes a warning to the console
def warning(msg):
    print("[Warning] line {} : {}".format(currentLine,msg))

def main():
    global currentLine

    if(len(sys.argv) < 2):
        print("Usage: cosasm <input file> <output file>")
        return -1


    inputFile = open(sys.argv[1],'r')
    instructions = list(inputFile)
    inputFile.close()

    print("Read " + str(len(instructions)) + " lines")
    

    #-= Go through the file =-#
    for i in range(0, len(instructions)):
        tokens = instructions[i].split()
        print(tokens)
        assemble(tokens)
        currentLine += 1



    print("\n-= Output: =-")
    for i in range(0, len(output)):
        print(hex(output[i]),end=" ")
    print("")
    print('\n-= Var Table: =-')
    for i in range(0 , len(variables)):
        print(hex(variables[i]),end=" ")
    print("")

    currentSize = len(output)
    #From where output ends, to 0xC800
    for i in range (0, 0xC800-currentSize):
        output.append(0x00)

    #Put variables in
    for i in range (0, len(variables)):
        output.append(variables[i])
        print("Loc: {}  Value:{} ".format(hex(len(output)),hex(variables[i])))

    #Finish the rest of the file
    currentSize = len(output)
    for i in range (0, 0xFFFF-currentSize):
        output.append(0x00)

    currentSize = len(output)
    print("Current Size {} ".format(currentSize))

    if(len(sys.argv) == 2):
        outputFile = open('output.bin','w+b')
    else:
        outputFile = open(sys.argv[2],'w+b')

    outputFile.write(output)

    outputFile.close()

if __name__ == "__main__":
    main()
    
