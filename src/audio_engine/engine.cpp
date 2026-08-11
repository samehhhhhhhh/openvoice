
#include "engine.hpp"

void engine::play() {

    std::cout << "Playing sound " << std::endl;

    ma_sound_start(&sound);

}

void engine::load_sound(const std::string &filename) {

    result = ma_sound_init_from_file(&audio_engine, filename.c_str(), 0, NULL, NULL, &sound);

    check_errors(result, "Failed to init from file.");
    std::cout << "Sound loaded" << std::endl;

}

ma_result engine::check_errors(const ma_result &result, const std::string &msg) {
    if (result != MA_SUCCESS) {
        std::cout << "error : " << msg << std::endl;

    }
    return result;

}
#include <cstring>

void engine::data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
    if (pDevice->capture.format != pDevice->playback.format || pDevice->capture.channels != pDevice->playback.channels) {
        return;
    }

// Change to C++ version
    memcpy(pOutput, pInput, frameCount * ma_get_bytes_per_frame(pDevice->capture.format, pDevice->capture.channels));
}


