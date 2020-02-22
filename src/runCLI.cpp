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
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    auto screen0 = ImTui_ImplNcurses_Init(true);
    ImTui_ImplText_Init();

    bool demo = true;
    int nframes = 0;
    float fval = 1.23f;

    while (true) {
        ImTui_ImplNcurses_NewFrame();
        ImTui_ImplText_NewFrame();

        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(4, 2), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(50.0, 10.0), ImGuiCond_Once);
        ImGui::Begin("Hello, world!");
        ImGui::Text("NFrames = %d", nframes++);
        ImGui::Text("Mouse Pos : x = %g, y = %g", ImGui::GetIO().MousePos.x, ImGui::GetIO().MousePos.y);
        ImGui::Text("Time per frame %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::Text("Float:");
        ImGui::SameLine();
        ImGui::SliderFloat("##float", &fval, 0.0f, 10.0f);
        ImGui::End();

        ImGui::SetNextWindowPos(ImVec2(630, 30), ImGuiCond_Once);
        ImGui::Begin("Debug");
        ImVec2 mousePos = ImGui::GetMousePos();
        ImGui::Text("%f, %f", mousePos.x, mousePos.y);
        ImGui::End();

        ImGui::Render();

        ImTui_ImplText_RenderDrawData(ImGui::GetDrawData(), screen0);
        ImTui_ImplNcurses_DrawScreen();
    }

    ImTui_ImplText_Shutdown();
    ImTui_ImplNcurses_Shutdown();

    return 0;
}