
#include "miniaudio.h"
#include <string>
#include <iostream>

void check_errors(ma_result& result, std::string msg) {

    if (result != MA_SUCCESS) std::cout << "Error ! : Failed to " <<  msg << "\n";
}

void init(ma_engine &engine, ma_engine_config engineConfig) {
    engineConfig = ma_engine_config_init();
    ma_result result;

    result = ma_engine_init(&engineConfig, &engine);
    check_errors(result, "Initialize the engine");
}

int main()
{
    
    ma_engine engine;
    ma_engine_config engineConfig;
    init(engine, engineConfig);
    ma_sound sound;
    
    ma_result result = ma_sound_init_from_file(&engine, "audio.wav", 0, NULL, NULL, &sound);

    check_errors(result, "Failed to init from file bruh");

    ma_sound_start(&sound);

    std::cout << "Press Enter to quit...";
    std::cin.get();   // keep the program alive while audio plays

    ma_sound_uninit(&sound);
    ma_engine_uninit(&engine);
}