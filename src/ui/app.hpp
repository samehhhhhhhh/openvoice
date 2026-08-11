

#ifndef OPENVOICE_APP_H
#define OPENVOICE_APP_H
#include <GLFW/glfw3.h>
#include "../audio_engine/engine.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>


#include "node_editor_manager.hpp"
class App
{
public:
    App();
    ~App();

    engine audio_engine;
    node_editor_manager n_manager;

    void run();

private:

    // variables :
    GLFWwindow* window = nullptr;
    const char* glslVersion = nullptr;


    bool nodeEditorOpen = false;
    bool showAnotherWindow = false;
    ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
    void renderHomePage();
    void renderNodeEditor();

};

#endif // OPENVOICE_APP_H