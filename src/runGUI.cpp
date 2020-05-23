#include "runGUI.hpp"
#include <fstream>


uint8_t memory[65536] = {};


static MemoryEditor ramEdit;
static Logger debugLog;

static char editorText[256 * 1000] = "";

/** Globals to show and hide windows **/
bool showGraphics = false;
bool showDemo = false;
bool logram = false;
bool verbose = true;

void MemoryWrite(uint16_t address, uint8_t value){
    if(verbose && logram){
        debugLog.AddLog("Wrote %X to %X\n", value, address);
    }
    memory[address] = value;
}

uint8_t MemoryRead(uint16_t address){
    if(verbose && logram){
        debugLog.AddLog("READ: %X from %X\n", memory[address], address);
    }
    return memory[address];
}

#ifdef __arm__
void runGUI::handlePins(){
    verbose = false;
    int base = 0xC402;
    for(int i = 0; i <= 28; i++){
        int byte = MemoryRead(base+i);
        if(byte & 0x80){ //Write
            pinMode(i,OUTPUT);
            if(byte & 0x7F){
                digitalWrite(1,i);
            }
            else{
                digitalWrite(0,i);
            }
        }
        else{ //Read
            pinMode(i,INPUT);
            int tmp = digitalRead(i);
            MemoryWrite(base+i,tmp&0x7F);
        }
    }
    verbose = true;
}
#endif

void runGUI::LoadIntoMemory(const char *filepath)
{
    std::ifstream File;
    File.open(filepath);
    File.read((char *)memory, 65536);
    File.close();
}

void runGUI::DumpMemory(const char *filepath)
{
    std::ofstream File;
    File.open(filepath);
    for (int i = 0; i < 65536; i++)
    {
        File << memory[i];
    }

    File.close();
}

void runGUI::HelpMarker(const char *desc){
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

void runGUI::Assemble(){
    std::string result = "";
    char buffer[128];
    const char* tmpAssmbly = "tmp.asm";
    const char* tmpBin = "tmp.bin";

    std::ofstream file(tmpAssmbly);
    file << editorText;
    file.close();

    FILE* pipe = popen("python3 asm/cosasm.py tmp.asm tmp.bin","r");

    while(!feof(pipe)){
        if(fgets(buffer,128,pipe) != NULL)
            result += buffer;
    }

    pclose(pipe);
    LoadIntoMemory(tmpBin);

    remove(tmpBin);
    remove(tmpAssmbly);

    debugLog.AddLog("%s",result.c_str());

}

void runGUI::MemoryEditor(cosproc proc){
    ImGui::SetNextWindowSize(ImVec2(530, 280), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(305, 120), ImGuiCond_Once);
    ramEdit.DrawWindow("Memory Editor", memory, sizeof(uint8_t) * 65536);
    ramEdit.Highlight(proc.pc, proc.pc + 1, ImGui::ColorConvertFloat4ToU32(ImVec4(0.75f, 0.75f, 0.25f, 1.0f)));

}

void runGUI::VideoOut(){
    ImGui::SetNextWindowSize(ImVec2(650, 450), ImGuiCond_Once);
    ImGui::SetNextWindowPos(ImVec2(500, 300), ImGuiCond_Once);
    ImGui::Begin("Video Out");
    pgu.show();
    ImGui::End();
}

void runGUI::Assembler(cosproc proc){
    ImGui::SetNextWindowPos(ImVec2(845, 30), ImGuiCond_Once);
    ImGui::Begin("Editor");

    if (ImGui::Button("Assemble")){
        debugLog.AddLog("Assembling...\n");
        Assemble();
    }

    static ImGuiInputTextFlags editorFlags = ImGuiInputTextFlags_AllowTabInput;
    ImGui::InputTextMultiline("##source", editorText, IM_ARRAYSIZE(editorText), ImVec2(400, 625), editorFlags);

    ImGui::End();
}

void runGUI::ShowTopMenu(){
    std::string menu_action = "";
        if (ImGui::BeginMainMenuBar()){
            if (ImGui::BeginMenu("File")){
                //Load File Into Memory
                if (ImGui::MenuItem("Load Binary")){
                    menu_action = "loadbin";
                }
                //Dump Memory to a File
                if (ImGui::MenuItem("Save Binary")){
                    menu_action = "savebin";
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Load Assembly")){
                    menu_action = "loadasm";
                }
                //Dump Memory to a File
                if (ImGui::MenuItem("Save Assembly")){
                    menu_action = "saveasm";
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Swap PGU")){
                  menu_action = "swappgu";
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit")){
                if (ImGui::MenuItem("Undo", "CTRL+Z")){}
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)){} // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X")){}
                if (ImGui::MenuItem("Copy", "CTRL+C")){}
                if (ImGui::MenuItem("Paste", "CTRL+V")){}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Window")){
                ImGui::Checkbox("Show Video Out", &showGraphics);
                ImGui::Checkbox("Show Demo Window", &showDemo);
                ImGui::Checkbox("Log Ram Access", &logram);
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        if (menu_action == "loadbin"){
            ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_Once);
            ImGui::OpenPopup("Load Binary");
            
        }
        if (menu_action == "savebin"){
            ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_Once);
            ImGui::OpenPopup("Dump Memory");
        }
        if (menu_action == "loadasm"){
            ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_Once);
            ImGui::OpenPopup("Load Assembly");
        }
        if (menu_action == "saveasm"){
            ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_Once);
            ImGui::OpenPopup("Save Assembly");
        }
        if (menu_action == "swappgu"){
            ImGui::SetNextWindowSize(ImVec2(500, 100), ImGuiCond_Once);
            ImGui::OpenPopup("Swap PGU");
        }

        if (ImGui::BeginPopupModal("Load Binary", NULL)){
            ImGui::Text("Choose File: ");
            static char loadBinFilepath[128] = "";
            ImGui::PushItemWidth(400);
            ImGui::InputText("", loadBinFilepath, IM_ARRAYSIZE(loadBinFilepath));
            ImGui::SameLine();
            const bool browseBinButtonPressed = ImGui::Button("...");
            static ImGuiFs::Dialog loadbinFsInstance;
            loadbinFsInstance.chooseFileDialog(browseBinButtonPressed,"./");
            strcpy(loadBinFilepath,loadbinFsInstance.getChosenPath()); //TODO: https://i.imgur.com/xZrKmAS.jpg
            if (strlen(loadBinFilepath)>0) {
            }
           if (ImGui::Button("Open")){
                LoadIntoMemory(loadBinFilepath);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")){
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        if (ImGui::BeginPopupModal("Dump Memory", NULL)){
            ImGui::Text("Save File: ");
            static char saveBinFilepath[128] = "";
            ImGui::PushItemWidth(400);
            ImGui::InputText("", saveBinFilepath, IM_ARRAYSIZE(saveBinFilepath));
            ImGui::SameLine();
            const bool saveBinButtonPressed = ImGui::Button("...");
            static ImGuiFs::Dialog savebinFsInstance;
            savebinFsInstance.saveFileDialog(saveBinButtonPressed,"myROM.bin");
            strcpy(saveBinFilepath,savebinFsInstance.getChosenPath()); //TODO: https://i.imgur.com/xZrKmAS.jpg
            if (strlen(saveBinFilepath)>0) {
            }
           if (ImGui::Button("Save")){
                DumpMemory(saveBinFilepath);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")){
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        if (ImGui::BeginPopupModal("Load Assembly", NULL)){
            ImGui::Text("Choose File: ");
            static char loadAsmFilepath[128] = "";
            ImGui::PushItemWidth(400);
            ImGui::InputText("", loadAsmFilepath, IM_ARRAYSIZE(loadAsmFilepath));
            ImGui::SameLine();
            const bool browseAsmButtonPressed = ImGui::Button("...");
            static ImGuiFs::Dialog loadAsmFsInstance;
            loadAsmFsInstance.chooseFileDialog(browseAsmButtonPressed,"./");
            strcpy(loadAsmFilepath,loadAsmFsInstance.getChosenPath()); //TODO: https://i.imgur.com/xZrKmAS.jpg
            if (strlen(loadAsmFilepath)>0) {
            }
           if (ImGui::Button("Open")){
                memset(editorText,0,256 * 1000);
                std::ifstream file(loadAsmFilepath);
                int i = 0;
                while(!file.eof()){
                    file.get(editorText[i]); 
                    i++;
                }
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")){
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }

        if (ImGui::BeginPopupModal("Save Assembly", NULL)){
            ImGui::Text("Save File: ");
            static char saveAsmFilepath[128] = "";
            ImGui::PushItemWidth(400);
            ImGui::InputText("", saveAsmFilepath, IM_ARRAYSIZE(saveAsmFilepath));
            ImGui::SameLine();
            const bool saveAsmButtonPressed = ImGui::Button("...");
            static ImGuiFs::Dialog saveAsmFsInstance;
            saveAsmFsInstance.saveFileDialog(saveAsmButtonPressed,"assembler.asm");
            strcpy(saveAsmFilepath,saveAsmFsInstance.getChosenPath()); //TODO: https://i.imgur.com/xZrKmAS.jpg
            if (strlen(saveAsmFilepath)>0) {
            }
           if (ImGui::Button("Save")){
                std::ofstream file;
                file.open(saveAsmFilepath);
                file << editorText;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")){
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
        
        if (ImGui::BeginPopupModal("Swap PGU", NULL)){
            ImGui::Text("Choose File: ");
            static char swapPGUFilepath[4096];
            ImGui::PushItemWidth(400);
            ImGui::InputText("", swapPGUFilepath, IM_ARRAYSIZE(swapPGUFilepath));
            ImGui::SameLine();
            const bool browseBinButtonPressed = ImGui::Button("...");
            static ImGuiFs::Dialog swapPGUFsInstance;
            swapPGUFsInstance.chooseFileDialog(browseBinButtonPressed,"./pgu/");
            strcpy(swapPGUFilepath,swapPGUFsInstance.getChosenPath()); //TODO: https://i.imgur.com/xZrKmAS.jpg
            if (strlen(swapPGUFilepath)>0) {
            }
            if (ImGui::Button("Open")){
                pgu.swap(swapPGUFilepath,&debugLog);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("Cancel")){
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }        
}

int runGUI::run(){
    #ifdef __arm__ 

        // Setup SDL
        // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
        // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
        {
            printf("Error: %s\n", SDL_GetError());
            return -1;
        }

        // Setup window
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        SDL_Window* window = SDL_CreateWindow("Cosmic", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
        SDL_GLContext gl_context = SDL_GL_CreateContext(window);
        SDL_GL_MakeCurrent(window, gl_context);
        SDL_GL_SetSwapInterval(1); // Enable vsync

        // Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();

        // Setup Platform/Renderer bindings
        ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
        ImGui_ImplOpenGL2_Init();

    #else
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0){
            printf("Error: %s\n", SDL_GetError());
            return -1;
        }

        const char *glsl_version = "#version 150";
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG); // Always required on Mac
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);


        // Create window with graphics context
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
        SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        SDL_Window *window = SDL_CreateWindow("Cosmic", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
        SDL_GLContext gl_context = SDL_GL_CreateContext(window);
        SDL_GL_MakeCurrent(window, gl_context);
        SDL_GL_SetSwapInterval(1); // Enable vsync

        bool err = glewInit() != 0;
        if (err){
            fprintf(stderr, "Failed to initialize OpenGL loader!\n");
            return 1;
        }

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO &io = ImGui::GetIO();
        (void)io;

        ImGui::StyleColorsDark();

        ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
        ImGui_ImplOpenGL3_Init(glsl_version);
    #endif

    //Setup Raspberry Pi Pins
    #ifdef __arm__ //RPi
        wiringPiSetup();
    #endif


    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    glEnable(GL_DEBUG_OUTPUT);
    //System setup
    cosproc proc = cosproc(MemoryRead, MemoryWrite);
    pgu.init(&debugLog);

    cosproc::Debug debugPackage;
    bool running = false;
    int procFrequency = 3000;
    int procFraction = 0;

    bool done = false;

    bool ctrlState = false;

    ImGuiContext* context = ImGui::GetCurrentContext();
    const char* focused = "";
    int result;

    while (!done){
        result = strcmp(focused, "Video Out");
        if(result == 0){
            for (int i = 0; i < IM_ARRAYSIZE(io.KeysDown); i++){
                if (ImGui::IsKeyPressed(i)){
                    MemoryWrite(0xC400, 0xFF);
                    MemoryWrite(0xC401, i);
                }
            }
        }
        SDL_Event event;
        while (SDL_PollEvent(&event)){
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
            if (event.type == SDL_KEYDOWN){
                //TODO: Add scope to this with only affecting the active window
                // debugLog.AddLog("Keys mods: %s%s%s%s\n", io.KeyCtrl ? "CTRL " : "", io.KeyShift ? "SHIFT " : "", io.KeyAlt ? "ALT " : "", io.KeySuper ? "SUPER " : "");
                switch (event.key.keysym.sym){
                    case SDLK_SPACE:
                        if (ctrlState)
                        {
                            debugPackage = proc.cycle();
                            debugLog.AddLog("[%04X] %s\n", debugPackage.pc, debugPackage.instruction.mnemonic);
                        }
                        break;
                    case SDLK_r:
                        if (ctrlState)
                        {
                            proc.reset();
                            debugLog.Clear();
                            debugLog.AddLog("Processor Reset\n");
                        }
                        break;
                    case SDLK_m:
                        if (ctrlState)
                        {
                            memset(memory, 0, sizeof(memory));
                            debugLog.AddLog("Memory Reset\n");
                        }
                        break;
                    case SDLK_c:
                        if (ctrlState)
                        {
                            debugLog.Clear();
                        }
                        break;
                    case SDLK_RCTRL:
                        ctrlState = true;
                        break;
                    case SDLK_LCTRL:
                        ctrlState = true;
                        break;
                }
            }
            if (event.type == SDL_KEYUP){
                switch (event.key.keysym.sym){
                    case SDLK_RCTRL:
                        ctrlState = false;
                        break;
                    case SDLK_LCTRL:
                        ctrlState = false;
                        // debugLog.AddLog(ImGui::IsWindowFocused());
                        break;
                }
            }
        }
        
        #ifdef __arm__
        handlePins();
        #endif

        pgu.copy(memory+0x8000);
        #ifdef __arm__
            ImGui_ImplOpenGL2_NewFrame();
        #else
            ImGui_ImplOpenGL3_NewFrame();
        #endif

        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

        /**  -= Test Window =-
        *   Useful for figuring out how
        *   To do new things
        */
        if(showDemo){
            ImGui::ShowDemoWindow();
        }

        /**  -= Menu Bar =-
        *     Top Menu bar.
        */
        ShowTopMenu();

        /**  -= GUI Debug Window =-
        *  This window holds debug info
        *       About the gui.
        */
        ImGui::SetNextWindowPos(ImVec2(630, 30), ImGuiCond_Once);
        ImGui::Begin("Debug");
        ImVec2 mousePos = ImGui::GetMousePos();
        ImGui::Text("%f, %f", mousePos.x, mousePos.y);
        ImGui::End();

        /**  -= Status Window =-
        *  Shows the status of the registers
        *  And other basic information about
        *        the processor
        */
        ImGui::SetNextWindowSize(ImVec2(290, 370), ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImVec2(10, 30), ImGuiCond_Once);
        ImGui::Begin("Status");
        ImGui::Text("Registers");
        ImGui::SameLine();
        HelpMarker("Registers are the most basic unit of storage in a processor.\n The amount, size, and configuration of registers depends on the architecture of the processor. \n\n General Registers: Cosmic contains 8 8-bit general registers that can also be referenced as 4 16-bit registers. These registers are used for temporarily storing data for operations \n\n Program Counter: A 16-bit register that holds the current position where code is being executed.\n\n Stack Pointer: The location of the top of the stack.\n\n Status Register: Holds current execution data about the system.");
        //"
        ImGui::Separator();
        ImGui::Columns(2, "8bitreg");
        //Left Side
        ImGui::SetColumnWidth(0, 145);
        ImGui::Text("General Registers");
        ImGui::Text("A: %X (%d) (%hhd)", proc.r[0], proc.r[0], proc.r[0]);
        ImGui::Text("B: %X (%d) (%hhd)", proc.r[1], proc.r[1], proc.r[1]);
        ImGui::Text("C: %X (%d) (%hhd)", proc.r[2], proc.r[2], proc.r[2]);
        ImGui::Text("D: %X (%d) (%hhd)", proc.r[3], proc.r[3], proc.r[3]);
        ImGui::Text("E: %X (%d) (%hhd)", proc.r[4], proc.r[4], proc.r[4]);
        ImGui::Text("F: %X (%d) (%hhd)", proc.r[5], proc.r[5], proc.r[5]);
        ImGui::Text("G: %X (%d) (%hhd)", proc.r[6], proc.r[6], proc.r[6]);
        ImGui::Text("H: %X (%d) (%hhd)", proc.r[7], proc.r[7], proc.r[7]);
        //Right Side
        ImGui::NextColumn();
        ImGui::Text("PC: %X (%d)", proc.pc, proc.pc);
        ImGui::Text("SP: %X (%d)", proc.sp, proc.sp);
        ImGui::Columns(1);
        ImGui::Separator();
        ImGui::Columns(1, "16bitreg", false);
        ImGui::Text("A/B: %X (%d) (%hd)", (proc.r[0] << 8 | proc.r[1]), (proc.r[0] << 8 | proc.r[1]), (short)(proc.r[0] << 8 | proc.r[1]));
        ImGui::Text("C/D: %X (%d) (%hd)", (proc.r[2] << 8 | proc.r[3]), (proc.r[2] << 8 | proc.r[3]), (short)(proc.r[2] << 8 | proc.r[3]));
        ImGui::Text("E/F: %X (%d) (%hd)", (proc.r[4] << 8 | proc.r[5]), (proc.r[4] << 8 | proc.r[5]), (short)(proc.r[4] << 8 | proc.r[5]));
        ImGui::Text("G/H: %X (%d) (%hd)", (proc.r[6] << 8 | proc.r[7]), (proc.r[6] << 8 | proc.r[7]), (short)(proc.r[6] << 8 | proc.r[7]));
        ImGui::Columns(1);

        ImGui::Separator();
        ImGui::Text("Status: ");
        ImGui::SameLine();
        HelpMarker("E: Error\nI: Interrupt Disable\nL: Less\nO: Overflow\nC: Carry\nN: Negative\nZ: Zero\n");
        char statusRegisterNames[] = {'Z', 'N', 'C', 'O', 'L', 'I', 'E', ' '};
        ImGui::Columns(8, "statusregister", false);
        for (int i = 7; i >= 0; i--){
            ImGui::Text("%c", statusRegisterNames[i]);
            if (proc.st[i]){
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
        MemoryEditor(proc);

        /**  -= Control Window =-
        *   Control the Processor.
        */
        ImGui::SetNextWindowSize(ImVec2(300, 85), ImGuiCond_Once);
        ImGui::SetNextWindowPos(ImVec2(305, 30), ImGuiCond_Once);
        ImGui::Begin("Control");
        if (ImGui::Button("Step")){
            debugPackage = proc.cycle();
            debugLog.AddLog("[%04X] %s\n", debugPackage.pc, debugPackage.instruction.mnemonic);
        }
        ImGui::SameLine();
        if (ImGui::Button("Processor Reset")){
            proc.reset();
            debugLog.Clear();
            debugLog.AddLog("Processor Reset\n");
            running = false;
        }
        ImGui::SameLine();
        if (ImGui::Button("Memory Reset")){
            memset(memory, 0, sizeof(memory));
            debugLog.AddLog("Memory Reset\n");
            running = false;
        }
        if (ImGui::Button("Run")){
            running = true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Pause")){
            running = false;
        }
        ImGui::SameLine();
        ImGui::PushItemWidth(75);
        const char *speeds[] = {"3000", "1500", "600", "300", "60", "1","1000000"};
        static const char *current_speed = speeds[0];
        static ImGuiComboFlags flags = 0;
        if (ImGui::BeginCombo("Hz", current_speed, flags)){
            for (int n = 0; n < IM_ARRAYSIZE(speeds); n++){
                bool is_selected = (current_speed == speeds[n]);
                if (ImGui::Selectable(speeds[n], is_selected)){
                    current_speed = speeds[n];
                    procFrequency = atoi(speeds[n]);
                }
                if (is_selected)
                    ImGui::SetItemDefaultFocus(); // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
            }
            ImGui::EndCombo();
        }
        ImGui::End();

        /**  -= Editor + Assembler Window =-
        *   Allows the writing of Assembly Code
        *   And assembles it in the enviroment
        */
       Assembler(proc);

        //Run if the run button it pushed
        if (running){
            procFraction+=procFrequency;
            while (0<procFraction){
                debugPackage = proc.cycle();
                debugLog.AddLog("[%04X] %s\n", debugPackage.pc, debugPackage.instruction.mnemonic);
                procFraction-=60;
            }
        }

        /**  -= Debug Log =-
        *   Prints out Debug Information about
        *       The system
        */
        ImGui::SetNextWindowPos(ImVec2(10, 405), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(825, 310), ImGuiCond_Once);
        debugLog.Draw("Debug Log");

        /** -= Graphics Window =-
        *   Shows the video out of
        *   Cosmic, VRAM is memory mapped
        */
        if (showGraphics){
            VideoOut();
        }

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        #ifdef __arm__
            ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        #else
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        #endif
        SDL_GL_SwapWindow(window);

        focused = context->WindowsFocusOrder.back()->Name; //Set at end to guarentee size > 0 on init
    }

    pgu.kill();

    #ifdef __arm__
        ImGui_ImplOpenGL2_Shutdown();
    #else
        ImGui_ImplOpenGL3_Shutdown();
    #endif
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
