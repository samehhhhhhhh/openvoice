

#ifndef OPENVOICE_APP_H
#define OPENVOICE_APP_H

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#define GL_SILENCE_DEPRECATION

#if defined(IMGUI_IMPL_OPENGL_ES2)
#include <GLES2/gl2.h>
#endif

#include <GLFW/glfw3.h>
#include "../audio_engine/engine.hpp"
class App
{
public:
    App();
    ~App();

    engine audio_engine;

    void run();

private:
    static void glfwErrorCallback(int error, const char* description);

    GLFWwindow* window = nullptr;
    const char* glslVersion = nullptr;


    bool showAnotherWindow = false;
    ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
};

#endif // OPENVOICE_APP_H