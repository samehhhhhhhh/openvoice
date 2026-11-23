
#include "app.hpp"
#include <imgui.h>



App::App() {

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << "\n";
        std::exit(1);
    }

    window = SDL_CreateWindow(
        "OpenVoice",
        1280,
        800,
        SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << "\n";
        SDL_Quit();
        std::exit(1);
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << "\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
        std::exit(1);
    }

    SDL_SetRenderVSync(renderer, 1);

    const float mainScale = SDL_GetWindowDisplayScale(window);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();

    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    ImGui::StyleColorsDark();

    ImGuiStyle &style = ImGui::GetStyle();
    style.ScaleAllSizes(mainScale);
    style.FontScaleDpi = mainScale;

    ImGui_ImplSDL3_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer3_Init(renderer);

}


App::~App()
{
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
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
    if (ImGui::Button("Stream microphone")) {
        audio_engine.toggle_microphone_stream();
    }
    if (ImGui::Button("Play sound")) {
        audio_engine.play();
    }

    if (ImGui::Button("Open Node editor")) {
        nodeEditorOpen = true;
    }

    if(ImGui::Button("Select Input Device")) {
        ImGui::OpenPopup("input_select");
    }
    if(ImGui::Button("Select Monitor Device")) {
        ImGui::OpenPopup("output_select");
    }

    if(ImGui::BeginPopup("input_select")) {
        ImGui::SeparatorText("Devices : ");

        devices.update_devices();
        for(int i = 0; i < devices.captureDeviceCount; i++) {

            if(ImGui::Selectable(devices.pCaptureDeviceInfos[i].name)) {
                audio_engine.set_dev(&devices.pCaptureDeviceInfos[i].id, DEVICE_CAPTURE);
            }
        }
        ImGui::EndPopup();
    }


    if(ImGui::BeginPopup("output_select")) {
        ImGui::SeparatorText("Devices : ");
        devices.update_devices();
        for(int i = 0; i < devices.playbackDeviceCount; i++) {
            if(ImGui::Selectable(devices.pPlaybackDeviceInfos[i].name)) {
                audio_engine.set_dev(&devices.pPlaybackDeviceInfos[i].id, DEVICE_PLAYBACK);
            }

        }
        ImGui::EndPopup();

    }

    ImGui::BeginChild("Voices",ImVec2(ImGui::GetContentRegionAvail().x * 0.5f, 260), ImGuiChildFlags_Borders);

    path voices_dir = "voices";

    if(!exists(voices_dir)) {
        create_directory(voices_dir);
    }

    for(const auto& i : directory_iterator(voices_dir)) {
        if(i.path().extension() == ".conf" ) {

            std::string token;
            std::vector<std::string> file_content;

            std::ifstream file(i.path());

            for(int a = 0; a <=2; a++) {
                std::getline(file, token);
                file_content.push_back(token);
            }

            ImGui::BeginChild(file_content[1].c_str(), ImVec2(ImGui::GetContentRegionAvail().x * 0.2f, 200), ImGuiChildFlags_Borders);

            ImGui::Text(file_content[1].c_str());

            std::string authour_label = "Made by " + file_content[2];
            ImGui::Text(authour_label.c_str());

            if(ImGui::Button("Load")) {
                save_manager::load(i.path(), audio_engine.nodeGraph);
            }

            ImGui::EndChild();

        }
    }


    ImGui::EndChild();

    ImGui::End();

}

void App::renderNodeEditor() {
    ImGuiIO& io = ImGui::GetIO();
    ImGui::Begin("Node editor", nullptr, ImGuiWindowFlags_NoDecoration);

    ImGui::Text("This is the node editor");

    if (ImGui::Button("Return")) {
        nodeEditorOpen = false;
    }

    n_manager.OnFrame(60, audio_engine.nodeGraph);
    // Use the editor class here

    ImGui::End();

}

void App::run()
{


    bool running = true;
    SDL_Event event;
    while (running)
    {
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        if ((SDL_GetWindowFlags(window) & SDL_WINDOW_MINIMIZED) != 0)
        {
            SDL_Delay(10);
            continue;
        }

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
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

        SDL_SetRenderDrawColor(
            renderer,
            static_cast<Uint8>(clearColor.x * clearColor.w * 255.0f),
            static_cast<Uint8>(clearColor.y * clearColor.w * 255.0f),
            static_cast<Uint8>(clearColor.z * clearColor.w * 255.0f),
            static_cast<Uint8>(clearColor.w * 255.0f));
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

}
