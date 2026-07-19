
#ifndef OPENVOICE_ENGINE_H
#define OPENVOICE_ENGINE_H
#include "../shared/includes.hpp"
#include <miniaudio.h>

class engine {

    ma_result check_errors(ma_result& result, std::string msg) {
        if (result != MA_SUCCESS) return result;
    }

public :

    ma_engine audio_engine;
    ma_sound sound;
    ma_engine_config engineConfig;
    ma_result result;
    ma_resource_manager_config config;
    ma_resource_manager resourceManager;
// implement function that streams audio from input to ouput

    engine() {

        config = ma_resource_manager_config_init();
        result =  ma_resource_manager_init(&config, &resourceManager);

        if (result != MA_SUCCESS) {
            std::cout << "Failed to init the resource manager" << std::endl;

        }

        engineConfig = ma_engine_config_init();

        result = ma_engine_init(&engineConfig, &audio_engine);
        check_errors(result, "Initialize the engine");

    };

    void play(std::string filename);

    // Destructor
    ~engine() {
        ma_sound_uninit(&sound);
        ma_engine_uninit(&audio_engine);
    }
};


#endif //OPENVOICE_ENGINE_H
