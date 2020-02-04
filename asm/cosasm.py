import sys
import math
import re


#The Instruction Set
InstructionSet = {
    "IMP NOP":"0x00",
    "IMP HCF":"0x01",
    "IMP PUSH":"0x02",
    "IMP POP":"0x03",
    "IMP SWP":"0x04",
    "IMM CALL":"0x05",
    "ABS CALL":"0x06",
    "IND CALL":"0x07",
    "IMP RET":"0x08",
    "IMM ADD":"0x10",
    "ABS ADD":"0x11",
    "IND ADD":"0x12",
    "REG ADD":"0x13",
    "IMM ADDX":"0x14",
    "ABS ADDX":"0x15",
    "IND ADDX":"0x16",
    "REG ADDX":"0x17",
    "IMM SUB":"0x18",
    "ABS SUB":"0x19",
    "IND SUB":"0x1A",
    "REG SUB":"0x1B",
    "IMM SUBX":"0x1C",
    "ABS SUBX":"0x1D",
    "IND SUBX":"0x1E",
    "REG SUBX":"0x1F",
    "IMM MUL":"0x20",
    "ABS MUL":"0x21",
    "IND MUL":"0x22",
    "REG MUL":"0x23",
    "IMM MULX":"0x24",
    "ABS MULX":"0x25",
    "IND MULX":"0x26",
    "REG MULX":"0x27",
    "IMM DIV":"0x28",
    "ABS DIV":"0x29",
    "IND DIV":"0x2A",
    "REG DIV":"0x2B",
    "IMM DIVX":"0x2C",
    "ABS DIVX":"0x2D",
    "IND DIVX":"0x2E",
    "REG DIVX":"0x2F",
    "IMM MOV":"0x30",
    "ABS MOV":"0x31",
    "IND MOV":"0x32",
    "REG MOV":"0x33",
    "IMM MOV":"0x34",
    "ABS MOV":"0x35",
    "IND MOV":"0x36",
    "REG MOV":"0x37",
    "IMM MOV":"0x38",
    "ABS MOV":"0x39",
    "IND MOV":"0x3A",
    "IMM MOV":"0x3B",
    "IMM SHL":"0x3C",
    "ABS SHL":"0x3D",
    "IND SHL":"0x3E",
    "REG SHL":"0x3F",
    "IMM MOVX":"0x40",
    "ABS MOVX":"0x41",
    "IND MOVX":"0x42",
    "REG MOVX":"0x43",
    "IMM MOVX":"0x44",
    "ABS MOVX":"0x45",
    "IND MOVX":"0x46",
    "REG MOVX":"0x47",
    "IMM MOVX":"0x48",
    "ABS MOVX":"0x49",
    "IND MOVX":"0x4A",
    "REG MOVX":"0x4B",
    "IMM SHLX":"0x4C",
    "ABS SHLX":"0x4D",
    "IND SHLX":"0x4E",
    "REG SHLX":"0x4F",
    "IMM AND":"0x50",
    "ABS AND":"0x51",
    "IND AND":"0x52",
    "REG AND":"0x53",
    "IMM OR":"0x54",
    "ABS OR":"0x55",
    "IND OR":"0x56",
    "REG OR":"0x57",
    "IMM XOR":"0x58",
    "ABS XOR":"0x59",
    "IND XOR":"0x5A",
    "REG XOR":"0x5B",
    "IMM SHR":"0x5C",
    "ABS SHR":"0x5D",
    "IND SHR":"0x5E",
    "REG SHR":"0x5F",
    "IMM CMP":"0x60",
    "ABS CMP":"0x61",
    "IND CMP":"0x62",
    "REG CMP":"0x63",
    "IMM CMPX":"0x64",
    "ABS CMPX":"0x65",
    "IND CMPX":"0x66",
    "REG CMPX":"0x67",
    "IMP INC":"0x68",
    "IMP INCX":"0x69",
    "IMP DEC":"0x6A",
    "IMP DECX":"0x6B",
    "IMM SHRX":"0x6C",
    "ABS SHRX":"0x6D",
    "IND SHRX":"0x6E",
    "REG SHRX":"0x6F",
    "IMM JMP":"0x70",
    "ABS JMP":"0x71",
    "IND JMP":"0x72",
    "REG JMP":"0x73",
    "IMM JZS":"0x74",
    "ABS JZS":"0x75",
    "IND JZS":"0x76",
    "REG JZS":"0x77",
    "IMM JNZ":"0x78",
    "ABS JNZ":"0x79",
    "IND JNZ":"0x7A",
    "REG JNZ":"0x7B",
    "IMM JCS":"0x7C",
    "ABS JCS":"0x7D",
    "IND JCS":"0x7E",
    "REG JCS":"0x7F",
    "IMM JNC":"0x80",
    "ABS JNC":"0x81",
    "IND JNC":"0x82",
    "REG JNC":"0x83",
    "IMM JOS":"0x84",
    "ABS JOS":"0x85",
    "IND JOS":"0x86",
    "REG JOS":"0x87",
    "IMM JNO":"0x88",
    "ABS JNO":"0x89",
    "IND JNO":"0x8A",
    "REG JNO":"0x8B",
    "IMM JNS":"0x8C",
    "ABS JNS":"0x8D",
    "IND JNS":"0x8E",
    "REG JNS":"0x8F",
    "IMM JNN":"0x90",
    "ABS JNN":"0x91",
    "IND JNN":"0x92",
    "REG JNN":"0x93",
    "IMM JLS":"0x94",
    "ABS JLS":"0x95",
    "IND JLS":"0x96",
    "REG JLS":"0x97",
    "IMM JNL":"0x98",
    "ABS JNL":"0x99",
    "IND JNL":"0x9A",
    "REG JNL":"0x9B",
    "IMP CSF":"0xA0",
    "IMP CZF":"0xA1",
    "IMP SZF":"0xA2",
    "IMP CNF":"0xA3",
    "IMP SNF":"0xA4",
    "IMP COF":"0xA5",
    "IMP SOF":"0xA6",
    "IMP CCF":"0xA7",
    "IMP SCF":"0xA8",
    "IMP CLF":"0xA9",
    "IMP SLF":"0xAA",
    "IMP CIF":"0xAB",
    "IMP SIF":"0xAC"
}

opcodePattern = re.compile("[A-Z,a-z]{3,4}( [#,@,R]?[0-9,A-F]{1,}([ ][#,@,R]?[0-9,A-F]{1,})?)?$")


#variableTable = {
#   "varName" = [pos,len]
#   "counter" = [5,5]
#}
variableTable = {} #Where all variables are stored
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
    elif(token[0] == "@"):
        return "IND"
    elif(token[0] == "R"):
        return "REG"
    else:
        return "ABS"

#Helper function for adding large variables to the variable table
def addToVariables(value):
    val = int(value)
    bytesNeeded = int(math.ceil((val.bit_length())/8))
    variables.extend(val.to_bytes(bytesNeeded,byteorder="big"))
    
    
#Variable Creation: <type> <name> = <value>
#All new types of variables can be put here
def createVar(tokens):
    #If it doesn't fit that setup, return.
    if(not tokens[2] == "="):
        return

    #byte counter = 5
    if(tokens[0] == "byte"):
        identifier = tokens[1]
        value = int(tokens[3],16)
        variableTable[identifier] = [len(variables),1]
        addToVariables(value)
        return
    #word points = 50
    elif (tokens[0] == "word"):
        identifier = tokens[1]
        value = tokens[3]
        variableTable[identifier] = [len(variables),2]
        addToVariables(value)
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
            print("HIT")
            print(variables[variableTable[operand][0]])

            if(variableTable[operand][1] > 2):
                warning("Variable {} is larger than opcode can handle".format(operand))
            for j in range(variableTable[operand][1]): #Add all of the opcode
                newoperand += str(variables[variableTable[operand][0] + j])
            tokens[i] = operator + newoperand
        print("TOK " + tokens[i])

    print(tokens)
    return tokens

def handleLabel(tokens):
    print("handling label")  

def updateVar(tokens):
    print("Updating variable")

#maybe replace with regex if we're feeling brave
def assemble(tokens):
    #If its variable creation:
    if(tokens[0] in types and tokens[2] == "="):
        createVar(tokens)

    #If its variable assigning:
    elif(tokens[0] in variableTable and tokens[1] == "="):
        updateVar(tokens)

    #If its a label
    elif(tokens[0][:-1] == ":"):
        handleLabel(tokens)

    #If its a comment
    elif(tokens[0][0] == ";"):
        pass

    #Else its an opcode
    else: 
        tokens = resolveVariables(tokens)
        print(tokens)
        eval(tokens[0])(tokens)


        

# -= INSTRUCTION FUNCTIONS =- #
def NOP(tokens):
    print(tokens)
def HCF(tokens):
    print(tokens)
def PUSH(tokens):
    print(tokens)
def POP(tokens):
    print(tokens)
def SWP(tokens):
    print(tokens)
def CALL(tokens):
    print(tokens)
def RET(tokens):
    print(tokens)
def ADD(tokens):
    print(tokens)
def ADDX(tokens):
    print(tokens)
def SUB(tokens):
    print(tokens)
def SUBX(tokens):
    print(tokens)
def MUL(tokens):
    print(tokens)
def MULX(tokens):
    print(tokens)
def DIV(tokens):
    print(tokens)
def DIVX(tokens):
    print(tokens)
def MOV(tokens):
    print(tokens)
def SHL(tokens):
    print(tokens)
def MOVX(tokens):
    print(tokens)
def SHLX(tokens):
    print(tokens)
def AND(tokens):
    print(tokens)
def OR(tokens):
    print(tokens)
def XOR(tokens):
    print(tokens)
def SHR(tokens):
    print(tokens)
def CMP(tokens):
    print(tokens)
def CMPX(tokens):
    print(tokens)
def INC(tokens):
    print(tokens)
def INCX(tokens):
    print(tokens)
def DEC(tokens):
    print(tokens)
def DECX(tokens):
    print(tokens)
def SHRX(tokens):
    print(tokens)
def JMP(tokens):
    print(tokens)
def JZS(tokens):
    print(tokens)
def JNZ(tokens):
    print(tokens)
def JCS(tokens):
    print(tokens)
def JNC(tokens):
    print(tokens)
def JOS(tokens):
    print(tokens)
def JNO(tokens):
    print(tokens)
def JNS(tokens):
    print(tokens)
def JNN(tokens):
    print(tokens)
def JLS(tokens):
    print(tokens)
def JNL(tokens):
    print(tokens)
def CSF(tokens):
    print(tokens)
def CZF(tokens):
    print(tokens)
def SZF(tokens):
    print(tokens)
def CNF(tokens):
    print(tokens)
def SNF(tokens):
    print(tokens)
def COF(tokens):
    print(tokens)
def SOF(tokens):
    print(tokens)
def CCF(tokens):
    print(tokens)
def SCF(tokens):
    print(tokens)
def CLF(tokens):
    print(tokens)
def SLF(tokens):
    print(tokens)
def CIF(tokens):
    print(tokens)
def SI(tokens):
    print(tokens)

#Writes an error to the console. Stops exectuion
def error(msg):
    print("[Error] line {} : {} ".format(currentLine,msg))
    sys.exit()

#Writes a warning to the console
def warning(msg):
    print("[Warning] line {} : {}".format(currentLine,msg))

def main():
    if(len(sys.argv) < 2):
        print("Usage: cosasm <input file> <output file>")
        return -1


    inputFile = open(sys.argv[1],'r')
    instructions = list(inputFile)
    inputFile.close()

    print("Read " + str(len(instructions)) + " lines\n")
    

    #-= Pass 2: Add Instructions =-#
    for i in range(0, len(instructions)):
        tokens = instructions[i].split()
        assemble(tokens)

    print(variableTable)


    print("\n\n-= Output: =-")
    for i in range(0, len(output)):
        print(hex(output[i]))
    
    print('\n\n-= Var Table: =-')
    for i in range(0 , len(variables)):
        print(hex(variables[i]))
    #outputFile = open('output.bin','w+b')
    #outputFile.write(output)
    #outputFile.close()

if __name__ == "__main__":
    main()


