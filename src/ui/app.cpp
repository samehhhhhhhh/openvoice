//
// Created by samehh on 6‏/7‏/2026.
//

#include "app.hpp"

#include <cstdio>

#ifdef __EMSCRIPTEN__
#include "../libs/emscripten/emscripten_mainloop_stub.h"
#endif

void App::glfwErrorCallback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}

App::App()
{
    glfwSetErrorCallback(glfwErrorCallback);
    glfwInit();

#if defined(IMGUI_IMPL_OPENGL_ES2)

    glslVersion = "#version 100";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

#elif defined(IMGUI_IMPL_OPENGL_ES3)

    glslVersion = "#version 300 es";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

#elif defined(__APPLE__)

    glslVersion = "#version 150";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

#else

    glslVersion = "#version 130";

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

#endif

    float mainScale =
        ImGui_ImplGlfw_GetContentScaleForMonitor(glfwGetPrimaryMonitor());

    window = glfwCreateWindow(
        (int)(1280 * mainScale),
        (int)(800 * mainScale),
        "OpenVoice",
        nullptr,
        nullptr);

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    style.ScaleAllSizes(mainScale);
    style.FontScaleDpi = mainScale;

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);
}

App::~App()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();
}

void App::run()
{
    ImGuiIO& io = ImGui::GetIO();

#ifdef __EMSCRIPTEN__
    io.IniFilename = nullptr;
    EMSCRIPTEN_MAINLOOP_BEGIN
#else
    while (!glfwWindowShouldClose(window))
#endif
    {
        glfwPollEvents();

        if (glfwGetWindowAttrib(window, GLFW_ICONIFIED))
        {
            ImGui_ImplGlfw_Sleep(10);
            continue;
        }

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (showDemoWindow)
            ImGui::ShowDemoWindow(&showDemoWindow);

        {

            ImGui::Begin("Openvoice");

            ImGui::Text("Welcome to openvoice");
            ImGui::Checkbox("Demo Window", &showDemoWindow);

            ImGui::Text(
                "Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / io.Framerate,
                io.Framerate);

            if (ImGui::Button("Play sound")) {
                audio_engine.play("audio.wav");
            }


            ImGui::End();
        }

        ImGui::Render();

        int displayW, displayH;
        glfwGetFramebufferSize(window, &displayW, &displayH);

        glViewport(0, 0, displayW, displayH);

        glClearColor(
            clearColor.x * clearColor.w,
            clearColor.y * clearColor.w,
            clearColor.z * clearColor.w,
            clearColor.w);

        glClear(GL_COLOR_BUFFER_BIT);

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwSwapBuffers(window);
    }

#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif
}

