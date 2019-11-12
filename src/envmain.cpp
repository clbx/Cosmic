/**
 * @breif The main class when running in the debugging simulation environtment. Shows status of the processor when running
 * 
 * @author Clay Buxton (buxtonc@etown.edu)
 * @author Kevin Carman (carmank@etown.edu)
 * 
 */

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


#define BYTE_TO_BINARY_PATTERN "%c %c %c %c  %c %c %c %c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 
/*fuck std for having hex and octal but no binary */


/*Probably will be moved elsewhere in the future becauese it doesn't belong here */
/**
* -= Memory and Address Bus=-
*  
* These two functions act as a memory and address bus would on a physical machine
* They pull from a postision in memory and return to wherever calls them.
* Callbacks make for a wonderful way to emulate a system that is connected 
* that avoids using a centralized stationary memory location. 
*/

uint8_t memory[65536] = { };

void MemoryWrite(uint16_t address, uint8_t value){
    //TODO: Actually put memory here
    memory[address] = value;
    printf("Wrote %X to %X\n",value,address);
}

uint8_t MemoryRead(uint16_t address){
    printf("READ: %X from %X\n",memory[address],address);
    return memory[address];
}


//TODO: Fix this once proper memory is added
void LoadIntoMemory(char* filepath){
    std::ifstream File;
    File.open(filepath);
    int i = 0;

    while(!File.eof()){
        File >> memory[i];
        i++;
    }

    File.close();
}

//TODO: Fix this once proper memory is added
void DumpMemory(char* filepath){
    std::ofstream File;
    File.open(filepath);
    for(int i = 0; i < 65536; i++){
        File << memory[i];
    }

    File.close();
}


static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}


static MemoryEditor ram_edit;

void runCMD(char* filepath){
    LoadIntoMemory(filepath);
    //TODO: Fix when memory is not only 256bytes
    int k = 0; 
    for(int i = 0; i < 256/8; i++){
        for(int j = 0; j < 8; j++){
            printf("%X ",memory[k]);
            k++;
        }
        printf("\n");
    }

}

int runGUI(){
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0){
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    const char* glsl_version = "#version 150";
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    // Create window with graphics context
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Cosmic", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync


    bool err = gl3wInit() != 0;
    if (err){
        fprintf(stderr, "Failed to initialize OpenGL loader!\n");
        return 1;
    }

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::StyleColorsDark();


    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL3_Init(glsl_version);


    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);



    //System setup
    

    cosproc proc = cosproc(MemoryRead, MemoryWrite);    


    bool done = false;
    while (!done){
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
            if (event.type == SDL_KEYDOWN){
                switch (event.key.keysym.sym){
                    case SDLK_SPACE:
                        proc.cycle();
                        break;
                    case SDLK_r:
                        proc.reset();
                        break;
                    case SDLK_m:	
                        memset(memory,0,sizeof(memory));
                        break;
                }
            }
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        //ImGui::ShowTestWindow();
        
        std::string menu_action = "";
        if (ImGui::BeginMainMenuBar()){
            if (ImGui::BeginMenu("File")){
                //Load File Into Memory
                if(ImGui::MenuItem("Load into Memory")){
                    menu_action = "loadbin";
                }   
                //Dump Memory to a File
                if(ImGui::MenuItem("Dump Memory")){
                    menu_action = "dumpmem";
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")){
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if(menu_action == "loadbin"){ImGui::OpenPopup("Load Binary");}
        if(menu_action == "dumpmem"){ImGui::OpenPopup("Dump Memory");}

        if (ImGui::BeginPopupModal("Load Binary",NULL)){
            ImGui::Text("Insert Filepath Here: ");
            static char filepath[128] = "";
            ImGui::InputText("input text", filepath, IM_ARRAYSIZE(filepath));
            if (ImGui::Button("Open")){
                LoadIntoMemory(filepath);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")){
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        if (ImGui::BeginPopupModal("Dump Memory",NULL)){
            ImGui::Text("Insert Filepath Here: ");
            static char filepath[128] = "";
            ImGui::InputText("input text", filepath, IM_ARRAYSIZE(filepath));
            if (ImGui::Button("Save")){
                DumpMemory(filepath);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")){
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }



        /**  -= Debug Window =-
        *  This window holds debug info
        *       About the gui.
        */
        ImGui::SetNextWindowPos(ImVec2(630,30), ImGuiCond_Once);
        ImGui::Begin("Debug");
            ImVec2 mousePos = ImGui::GetMousePos();
            ImGui::Text("%f, %f",mousePos.x,mousePos.y);
        ImGui::End();

        
        /**  -= Status Window =-
        *  Shows the status of the registers
        *  And other basic information about
        *        the processor
        */
        ImGui::SetNextWindowSize(ImVec2(290,350),ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImVec2(10,30),ImGuiCond_Once);
        ImGui::Begin("Status");
            ImGui::Text("Registers");
            ImGui::SameLine();
            HelpMarker("Registers are the most basic unit of storage in a processor.\n The amount, size, and configuration of registers depends on the architecture of the processor. \n\n General Registers: Cosmic contains 8 8-bit general registers that can also be referenced as 4 16-bit registers. These registers are used for temporarily storing data for operations \n\n Program Counter: A 16-bit register that holds the current position where code is being executed.\n\n Stack Pointer: The location of the top of the stack.\n\n Status Register: Holds current execution data about the system.");
            ImGui::Separator();
            ImGui::Columns(2,"8bitreg");    
            //Left Side
            ImGui::SetColumnWidth(0,145);
                ImGui::Text("General Registers");
                ImGui::Text("A: %X (%d) (%hhd)",proc.r[0],proc.r[0],proc.r[0]);
                ImGui::Text("B: %X (%d) (%hhd)",proc.r[1],proc.r[1],proc.r[1]);
                ImGui::Text("C: %X (%d) (%hhd)",proc.r[2],proc.r[2],proc.r[2]);
                ImGui::Text("D: %X (%d) (%hhd)",proc.r[3],proc.r[3],proc.r[3]);
                ImGui::Text("E: %X (%d) (%hhd)",proc.r[4],proc.r[4],proc.r[4]);
                ImGui::Text("F: %X (%d) (%hhd)",proc.r[5],proc.r[5],proc.r[5]);
                ImGui::Text("G: %X (%d) (%hhd)",proc.r[6],proc.r[6],proc.r[6]);
                ImGui::Text("H: %X (%d) (%hhd)",proc.r[7],proc.r[7],proc.r[7]);
            //Right Side
            ImGui::NextColumn();
                ImGui::Text("PC: %X (%d)",proc.pc,proc.pc);
                ImGui::Text("SP: %X (%d)",proc.sp,proc.sp); 
            ImGui::Columns(1);
            ImGui::Separator();
            ImGui::Columns(1, "16bitreg", false);
            ImGui::Text("A/B: %X (%d) (%hd)",(proc.r[0] << 8 | proc.r[1]),(proc.r[0] << 8 | proc.r[1]),(short)(proc.r[0] << 8 | proc.r[1]));
            ImGui::Text("C/D: %X (%d) (%hd)",(proc.r[2] << 8 | proc.r[3]),(proc.r[2] << 8 | proc.r[3]),(short)(proc.r[2] << 8 | proc.r[3]));
            ImGui::Text("E/F: %X (%d) (%hd)",(proc.r[4] << 8 | proc.r[5]),(proc.r[4] << 8 | proc.r[5]),(short)(proc.r[4] << 8 | proc.r[5]));
            ImGui::Text("G/H: %X (%d) (%hd)",(proc.r[6] << 8 | proc.r[7]),(proc.r[6] << 8 | proc.r[7]),(short)(proc.r[6] << 8 | proc.r[7]));
            ImGui::Columns(1);

            ImGui::Separator();
            /*
            
            for(int i = 7; i >= 0; i--){
                if(i == 3){
                    ImGui::SameLine();
                    ImGui::Text(" ");
                }
                ImGui::SameLine();
                if(proc.st[i]){
                    ImGui::Text("1");
                }
                else{
                    ImGui::Text("0");
                }
            }
            ImGui::SameLine();
            
            ImGui::Text("              I P   O C N Z");
            */
            ImGui::Text("Status: ");
            ImGui::SameLine();
            HelpMarker("I: Interrupt Disable\nP: Parity\nO: Overflow\nC: Carry\nN: Negative\nZ: Zero\n");
            char statusRegisterNames[] = {'Z','N','C','O','P','I',' ',' '};
            ImGui::Columns(8,"statusregister",false);
            for(int i = 7; i >= 0; i--){
                char current = statusRegisterNames[i];
                ImGui::Text(&current);
                if(proc.st[i]){
                    ImGui::Text("1");
                }
                else{
                    ImGui::Text("0");
                }
                ImGui::NextColumn();
            }



        ImGui::End();


        /**  -= Memory Editor =-
        *  Shows and allows the editing of
        *  Memory
        */
        ImGui::SetNextWindowSize(ImVec2(530,280),ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImVec2(305,100),ImGuiCond_Once);
        ram_edit.DrawWindow("Memory Editor", memory, sizeof(uint8_t)*65536);
        ram_edit.Highlight(proc.pc,proc.pc+1,ImGui::ColorConvertFloat4ToU32(ImVec4(0.75f,0.75f,0.25f,1.0f)));


        /**  -= Control Window =-
        *   Control the Processor.
        */
        ImGui::SetNextWindowSize(ImVec2(300,60),ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImVec2(305,30),ImGuiCond_Once);
        ImGui::Begin("Control");
            if(ImGui::Button("Step")){
                proc.cycle();
            }
            ImGui::SameLine();
            if(ImGui::Button("Processor Reset")){
                proc.reset();
            }
            ImGui::SameLine();
            if(ImGui::Button("Memory Reset")){
                memset(memory,0,sizeof(memory));
            }
                
        ImGui::End();


        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);

        


    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


int main(int argc, char** argv){

    if(argc > 1){
        runCMD(argv[1]);
    }else{
        runGUI();
    }

    
}