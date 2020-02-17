#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <GL/gl3w.h>
#include <iostream>
#include <fstream>
#include <string>

#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl3.h"
#include "imgui_memory_editor.h"
#include "imgui_logger.h"

#include "cosproc.hpp"


class runGUI{
    private:
        uint8_t memory[65538];
        static Logger debugLog;
        static MemoryEditor ramEdit;

        void MemoryWrite(uint16_t address, uint8_t value);
        uint8_t MemoryRead(uint16_t address);
        void LoadIntoMemory(char *filepath);
        void DumpMemory(char *filepath);
        static void HelpMarker(const char *desc);




    public:
        int run();

};