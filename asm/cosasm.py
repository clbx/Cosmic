import sys
import re 

output = bytearray()


def other(tokens):
    return False

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

    
    outputFile = open('output.bin','w+b')
    outputFile.write(output)
    outputFile.close()


if __name__ == "__main__":
    main()
