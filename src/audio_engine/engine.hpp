
#ifndef OPENVOICE_ENGINE_H
#define OPENVOICE_ENGINE_H
#include "../shared/includes.hpp"
#include <miniaudio.h>

class engine {

    void check_errors(ma_result& result, std::string msg) {
        if (result != MA_SUCCESS) std::cout << "Error ! : Failed to " <<  msg << "\n";
    }

public :

    ma_engine audio_engine;
    ma_sound sound;
    ma_engine_config engineConfig;
    ma_result result;

    engine() {
        engineConfig = ma_engine_config_init();

        result = ma_engine_init(&engineConfig, &audio_engine);
        check_errors(result, "Initialize the engine");

    };

    void play(std::string& filename);

    // Destructor
    ~engine() {
        ma_sound_uninit(&sound);
        ma_engine_uninit(&audio_engine);
    }
};


#endif //OPENVOICE_ENGINE_H
