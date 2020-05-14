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
//#include <GL/gl3w.h>
#include <GL/glew.h>
#include <iostream>
#include <fstream>
#include <string>


#include "runGUI.hpp"
#include "runCLI.hpp"

/*
* -= Memory and Address Bus=-
*  
* These two functions act as a memory and address bus would on a physical machine
* They pull from a postision in memory and return to wherever calls them.
* Callbacks make for a wonderful way to emulate a system that is connected 
* that avoids using a centralized stationary memory location. 
*/


static MemoryEditor ramEdit;
static Logger debugLog;
runGUI gui;
runCLI cli;

//TODO: Implement arguments
int main(int argc, char **argv){
    if (argc > 1){
        cli.run();
    }
    else{
        gui.run();
    }
}