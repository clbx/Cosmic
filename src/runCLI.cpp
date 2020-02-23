#include "runCLI.hpp"

//This will be fixed when the processor no longer runs in the environment
uint8_t memory0[65536] = {};

//TODO: Fix this once proper memory is added
void runCLI::LoadIntoMemory(char *filepath)
{
    std::ifstream File;
    File.open(filepath);
    File.read((char *)memory0, 65536);
    File.close();
}

//TODO: Fix this once proper memory is added
void runCLI::DumpMemory(char *filepath)
{
    std::ofstream File;
    File.open(filepath);
    for (int i = 0; i < 65536; i++)
    {
        File << memory0[i];
    }

    File.close();
}



int runCLI::run() {
    printf("I don't work yet \n");
    return 0;
}