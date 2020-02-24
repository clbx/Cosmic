/**
 * @file runCLI.hpp
 * 
 * @author Clay Buxton  (clbx, buxtonc@etown.edu)
 * @author Kevin Carman (carmank, carmank@etown.edu)
 * 
 */

#include <stdint.h>
#include <iostream>
#include <time.h>
#include <map>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
/*
#include "imtui.h"
#include "imtui-impl-ncurses.h"
#include "imtui-impl-text.h"
*/

/**
 * A Command Line Interface for ImGUI
 */
class runCLI{    
    private:
        void LoadIntoMemory(char *filepath);
        void DumpMemory(char *filepath);

    public:
        int run();
};
