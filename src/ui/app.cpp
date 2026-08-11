
#include "app.hpp"

App::App() {
    std::cout << "HEY";

    glfwInit();

#if defined(__APPLE__)

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

    const float mainScale =
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

void App::renderHomePage() {
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Begin("Openvoice", nullptr, ImGuiWindowFlags_NoDecoration);

    ImGui::Text("Welcome to openvoice");


    ImGui::Text(
        "Application average %.3f ms/frame (%.1f FPS)",
        1000.0f / io.Framerate,
        io.Framerate);

    if (ImGui::Button("Load sound")) {
        audio_engine.load_sound("audio.wav");
    }
    if (ImGui::Button("Play sound")) {
        audio_engine.play();
    }

    if (ImGui::Button("Open Node editor")) {
        nodeEditorOpen = true;
    }

    // Display all locally saved voice files.
    // Make a card for every sound.

    ImGui::End();

}

void App::renderNodeEditor() {
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Begin("Node editor", nullptr, ImGuiWindowFlags_NoDecoration);

    ImGui::Text("This is the node editor");

    if (ImGui::Button("Return")) {
        nodeEditorOpen = false;
    }

    // Use the node_editor_manager class here

    ImGui::End();

}

void App::run()
{


    while (!glfwWindowShouldClose(window))
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

        #ifdef IMGUI_HAS_VIEWPORT
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->GetWorkPos());
        ImGui::SetNextWindowSize(viewport->GetWorkSize());
        ImGui::SetNextWindowViewport(viewport->ID);
        #else
        ImGui::SetNextWindowPos(ImVec2(0.0f, 0.0f));
        ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
        #endif

        // make a navbar here with the project name, a + button to create a voice and that gets you in the node editor
        // Also a search bar to search locally for voices.
        if (nodeEditorOpen) {
            renderNodeEditor();
        } else {
            renderHomePage();
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

}

