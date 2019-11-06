import sys
import re 


def ADD(tokens):
    pattern = re.compile("ADD (([#][0-9,A-F]{1,2})|([@][0-9,A-F]{1,4})|([0-9,A-F]{1,4})|([R][0-7]{1}))")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False
    

def ADDX(tokens):
    pattern = re.compile("ADDX (([#][0-9,A-F]{1,4})|([@][0-9,A-F]{1,4})|([0-9,A-F]{1,4})|([R][0-3]{1}))")
    p = pattern.match(' '.join(tokens))
    if(p.group != (' '.join(tokens))):
        return True


    return False

def SUB(tokens):
    pattern = re.compile("SUB (([#][0-9,A-F]{1,2})|([@][0-9,A-F]{1,4})|([0-9,A-F]{1,4})|([R][0-7]{1}))")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True
    
    return False

def SUBX(tokens):
    pattern = re.compile("SUBX (([#][0-9,A-F]{1,4})|([@][0-9,A-F]{1,4})|([0-9,A-F]{1,4})|([R][0-3]{1}))")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True
    
    return False

def MUL(tokens):
    pattern = re.compile("MUL (([#][0-9,A-F]{1,2})|([@][0-9,A-F]{1,4})|([0-9,A-F]{1,4})|([R][0-7]{1}))")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True
    
    return False

def MULX(tokens):
    pattern = re.compile("MULX (([#][0-9,A-F]{1,4})|([@][0-9,A-F]{1,4})|([0-9,A-F]{1,4})|([R][0-3]{1}))")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True
    
    return False

def DIV(tokens):
    pattern = re.compile("DIV (([#][0-9,A-F]{1,2})|([@][0-9,A-F]{1,4})|([0-9,A-F]{1,4})|([R][0-7]{1}))")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True
    
    return False

def DIVX(tokens):
    pattern = re.compile("DIVX (([#][0-9,A-F]{1,4})|([@][0-9,A-F]{1,4})|([0-9,A-F]{1,4})|([R][0-3]{1}))")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True
    
    return False

def SHL(tokens):
    pattern = re.compile("SHL")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False

def SHR(tokens):
    pattern = re.compile("SHR")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False

def INC(tokens):
    pattern = re.compile("INC")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False

def DEC(tokens):
    pattern = re.compile("INC")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False

def AND(tokens):
    pattern = re.compile("AND (([#][0-9,A-F]{1,2})|([@][0-9,A-F]{1,4})|([0-9,A-F]{1,4})|([R][0-7]{1}))")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False

def OR(tokens):
    pattern = re.compile("OR (([#][0-9,A-F]{1,2})|([@][0-9,A-F]{1,4})|([0-9,A-F]{1,4})|([R][0-7]{1}))")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False

def XOR(tokens):
    pattern = re.compile("XOR (([#][0-9,A-F]{1,2})|([@][0-9,A-F]{1,4})|([0-9,A-F]{1,4})|([R][0-7]{1}))")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False

def CMP(tokens):
    pattern = re.compile("CMP (([#][0-9,A-F]{1,2})|([@][0-9,A-F]{1,4})|([0-9,A-F]{1,4})|([R][0-7]{1}))")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False

def JMP(tokens):
    pattern = re.compile("JMP (([#][0-9,A-F]{1,2})|([@][0-9,A-F]{1,4})|([0-9,A-F]{1,4}))")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False

def JZS(tokens):
    pattern = re.compile("")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False

def JNZ(tokens):
    pattern = re.compile("")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False

def JNC(tokens):
    pattern = re.compile("")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False

def JCS(tokens):
    pattern = re.compile("")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False

def JNS(tokens):
    pattern = re.compile("")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False

def JOS(tokens):
    pattern = re.compile("")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False

def SID(tokens):
    pattern = re.compile("")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False

def CALL(tokens):
    pattern = re.compile("")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False

def RET(tokens):
    pattern = re.compile("")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False

def HCF(tokens):
    pattern = re.compile("")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False

def MOV(tokens):
    pattern = re.compile("")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False

def MOVX(tokens):
    pattern = re.compile("")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False

def SWP(tokens):
    pattern = re.compile("")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False

def PUSH(tokens):
    pattern = re.compile("")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False

def POP(tokens):
    pattern = re.compile("")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False

def NOP(tokens):
    pattern = re.compile("")
    p = pattern.match(' '.join(tokens))
    if(p.group() != (' '.join(tokens))):
        return True

    return False

def other(tokens):
    return False

def main():
    if(len(sys.argv) < 2):
        print("Please supply a file")
        return -1

    #Put's all read instructions into a list by line
    file = open(sys.argv[1],'r')
    instructions = list(file)

    #Go through the instructions
    for i in range(0, len(instructions)):
        #Tokenize
        tokens = instructions[i].split()
        print(tokens)
        error = False

        if tokens[0] == "ADD":
            error = ADD(tokens)
        elif tokens[0] == "ADDX":
            error = ADDX(tokens)
        elif tokens[0] == "SUB":
            error = SUB(tokens)
        elif tokens[0] == "SUBX":
            error = SUBX(tokens)
        elif tokens[0] == "MUL":
            error = MUL(tokens)
        elif tokens[0] == "MULX":
            error = MULX(tokens)
        elif tokens[0] == "DIV":
            error = DIV(tokens)
        elif tokens[0] == "DIVX":
            error = DIVX(tokens)
        elif tokens[0] == "SHL":
            error = SHL(tokens)
        elif tokens[0] == "SHR":
            error = SHR(tokens)
        elif tokens[0] == "INC":
            error = INC(tokens)
        elif tokens[0] == "DEC":
            error = DEC(tokens)
        elif tokens[0] == "AND":
            error = AND(tokens)
        elif tokens[0] == "OR":
            error = OR(tokens)
        elif tokens[0] == "XOR":
            error = XOR(tokens)
        elif tokens[0] == "CMP":
            error = CMP(tokens)
        elif tokens[0] == "JMP":
            error = JMP(tokens)
        elif tokens[0] == "JZS":
            error = JZS(tokens)
        elif tokens[0] == "JNZ":
            error = JNZ(tokens)
        elif tokens[0] == "JNC":
            error = JNC(tokens)
        elif tokens[0] == "JCS":
            error = JCS(tokens)
        elif tokens[0] == "JNS":
            error = JNS(tokens)
        elif tokens[0] == "JOS":
            error = JOS(tokens)
        elif tokens[0] == "SID":
            error = SID(tokens)
        elif tokens[0] == "CALL":
            error = CALL(tokens)
        elif tokens[0] == "RET":
            error = RET(tokens)
        elif tokens[0] == "HCF":
            error = HCF(tokens)
        elif tokens[0] == "MOV":
            error = MOV(tokens)
        elif tokens[0] == "MOVX":
            error = MOVX(tokens)
        elif tokens[0] == "SWP":
            error = SWP(tokens)
        elif tokens[0] == "PUSH":
            error = PUSH(tokens)
        elif tokens[0] == "POP":
            error = POP(tokens)
        elif tokens[0] == "NOP":
            error = NOP(tokens)
        else:
            error = other(tokens)

        if(error == True):
            print("Syntax error on line ", i+1)
            return

        


    file.close()

if __name__ == "__main__":
    main()




