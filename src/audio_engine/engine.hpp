#ifndef OPENVOICE_ENGINE_H
#define OPENVOICE_ENGINE_H
#include <memory>

#include "../shared/includes.hpp"
#include "nodes.hpp"
#include "node_graph.hpp"


class engine {

    static void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

    ma_engine audio_engine;
    ma_sound sound;
    ma_engine_config engineConfig;

    ma_device_config device_config;
    ma_device device;

public :

    node_graph nodeGraph;
    // Enabled by default
    bool stream_microphone = true;

    void toggle_microphone_stream() {
        stream_microphone = !stream_microphone;
        std::cout << "Toggled microphone to " << stream_microphone << std::endl;
    }

    engine()

{

        // Duplex audio device
        device_config = ma_device_config_init(ma_device_type_duplex);
        device_config.capture.pDeviceID  = nullptr;
        device_config.capture.format     = DEVICE_FORMAT;
        device_config.capture.channels   = DEVICE_CHANNELS;
        device_config.capture.shareMode  = ma_share_mode_shared;
        device_config.playback.pDeviceID = nullptr;
        device_config.playback.format    = DEVICE_FORMAT;
        device_config.playback.channels  = DEVICE_CHANNELS;
        device_config.dataCallback       = data_callback;
        device_config.pUserData          = this;

        result = ma_device_init(nullptr, &device_config, &device);
        check_result("Failed to initialize device");

        // initialize nodeGraph here

        result = ma_device_start(&device);
        check_result("Failed to start device");
        // Miniaudio engine initialization for generic sound playing
        engineConfig = ma_engine_config_init();

        result = ma_engine_init(&engineConfig, &audio_engine);

        check_result("Initialize the engine");

    }

    void play();
    void load_sound(const std::string& filename);

    ~engine() {
        ma_device_stop(&device);
        ma_device_uninit(&device);
        ma_sound_uninit(&sound);
        ma_engine_uninit(&audio_engine);

    }
};

#endif //OPENVOICE_ENGINE_H
