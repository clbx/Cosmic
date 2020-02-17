/**
 * @breif The main class when running in the debugging simulation environtment. Shows status of the processor when running
 * 
 * @author Clay Buxton  (clbx, buxtonc@etown.edu)
 * @author Kevin Carman (carmank, carmank@etown.edu)
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <GL/gl3w.h>
#include <iostream>
#include <fstream>
#include <string>


#include "runGUI.hpp"

/*
* -= Memory and Address Bus=-
*  
* These two functions act as a memory and address bus would on a physical machine
* They pull from a postision in memory and return to wherever calls them.
* Callbacks make for a wonderful way to emulate a system that is connected 
* that avoids using a centralized stationary memory location. 
*/

uint8_t memory[65536] = {};

static MemoryEditor ramEdit;
static Logger debugLog;
runGUI gui;

void MemoryWrite(uint16_t address, uint8_t value)
{
    debugLog.AddLog("Wrote %X to %X\n", value, address);
    memory[address] = value;
}

uint8_t MemoryRead(uint16_t address)
{
    debugLog.AddLog("READ: %X from %X\n", memory[address], address);
    return memory[address];
}

//TODO: Fix this once proper memory is added
void LoadIntoMemory(char *filepath)
{
    std::ifstream File;
    File.open(filepath);
    File.read((char *)memory, 65536);
    File.close();
}

//TODO: Fix this once proper memory is added
void DumpMemory(char *filepath)
{
    std::ofstream File;
    File.open(filepath);
    for (int i = 0; i < 65536; i++)
    {
        File << memory[i];
    }

    File.close();
}

void runCMD(char *filepath)
{
    LoadIntoMemory(filepath);
    //TODO: Fix when memory is not only 256bytes
    int k = 0;
    for (int i = 0; i < 256 / 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            printf("%X ", memory[k]);
            k++;
        }
        printf("\n");
    }
}

int main(int argc, char **argv){
    if (argc > 1){
        runCMD(argv[1]);
    }
    else{
        gui.run();
    }
}