#include <string>
#include <fstream>
#include <vector>
#include <iostream>


std::string getOpcodeMneomic(std::string line){
    std::string operand = "";
    int i = 0;
    while(line[i] != ' '){
        operand += line[i];
        i++;
    }
    return operand;
}

uint8_t getOpcode(std::string opcode){

}


int main(int argc, char** argv){
    if(argc == 1){
        printf("Supply Filename\n");
        return -1;
    }
    std::vector<std::string> raw;
    std::vector<uint8_t> bytestream;
    std::string line;
    int lines = 0;

    std::ifstream infile(argv[1]);
    while(getline(infile, line)){
        raw.push_back(line);
        lines++;
    }

    printf("Read %d lines.\n",lines);

    for(int i = 0; i < lines; i++){
        std::cout << getOperand(raw[i]) <<  std::endl;
        uint8_t 

    }
    

    

}











