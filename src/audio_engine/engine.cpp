
#include "engine.hpp"

void engine::play(std::string filename) {
    result = ma_sound_init_from_file(&audio_engine, filename.c_str(), 0, NULL, NULL, &sound);

    check_errors(result, "Failed to init from file.");

    std::cout << "Playing sound : " << filename << std::endl;

    ma_sound_start(&sound);

}