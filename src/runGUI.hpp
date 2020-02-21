#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <GL/gl3w.h>
#include <iostream>
#include <fstream>
#include <string>

#include "imgui.h"
#include "examples/imgui_impl_sdl.h"
#include "examples/imgui_impl_opengl3.h"
#include "imgui_memory_editor.h"
#include "imgui_logger.h"

#include "cosproc.hpp"


class runGUI{
    private:

        void LoadIntoMemory(char *filepath);
        void DumpMemory(char *filepath);
        static void HelpMarker(const char *desc);




    public:
        int run();

};