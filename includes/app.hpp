

#ifndef OPENVOICE_APP_H
#define OPENVOICE_APP_H
#include <SDL3/SDL.h>
#include "engine.hpp"
#include "device_manager.hpp"
#include <imgui.h>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <filesystem>
#include "node_editor_manager.hpp"
#include <string>
#include <fstream>
#include "saves.hpp"

using namespace std::filesystem;

struct voice_desc {
    std::string name;
    std::string authour;
};

class App
{
public:
    App();
    ~App();

    engine audio_engine;
    node_editor_man n_manager;

    device_manager devices;

    void run();

private:

    // variables :
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;


    bool nodeEditorOpen = false;
    bool showAnotherWindow = false;
    ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    void renderHomePage();
    void renderNodeEditor();



};

#endif // OPENVOICE_APP_H
