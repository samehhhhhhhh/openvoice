#ifndef OPENVOICE_ENGINE_H
#define OPENVOICE_ENGINE_H
#include "../shared/includes.hpp"
#include <miniaudio.h>

class engine {

    static ma_result check_errors(const ma_result& result,const std::string& msg);

    static void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

    bool stream_microphone = false;

public :

    ma_engine audio_engine;
    ma_sound sound;
    ma_engine_config engineConfig;
    ma_result result;
    ma_resource_manager_config config;
    ma_resource_manager resourceManager;
    ma_device_config device_config;
    ma_device device;

// implement function that streams audio from input to ouput

    void toggle_microphone_stream() {
        stream_microphone = !stream_microphone;
    }

    engine() {


        device_config = ma_device_config_init(ma_device_type_duplex);
        device_config.capture.pDeviceID  = NULL;
        device_config.capture.format     = ma_format_s16;
        device_config.capture.channels   = 2;
        device_config.capture.shareMode  = ma_share_mode_shared;
        device_config.playback.pDeviceID = NULL;
        device_config.playback.format    = ma_format_s16;
        device_config.playback.channels  = 2;
        device_config.dataCallback       = data_callback;
        device_config.pUserData          = this;

        if (ma_device_init(NULL, &device_config, &device) != MA_SUCCESS) {
            std::cout << "Failed to initialize the duplex device" << std::endl;
        }
        ma_device_start(&device);

        config = ma_resource_manager_config_init();
        result =  ma_resource_manager_init(&config, &resourceManager);

        if (result != MA_SUCCESS) {
            std::cout << "Failed to init the resource manager" << std::endl;

        }

        engineConfig = ma_engine_config_init();

        result = ma_engine_init(&engineConfig, &audio_engine);

        check_errors(result, "Initialize the engine");

    };

    void play();
    void load_sound(const std::string& filename);

    // Destructor
    ~engine() {
        ma_device_uninit(&device);
        ma_sound_uninit(&sound);
        ma_engine_uninit(&audio_engine);

    }
};

#endif //OPENVOICE_ENGINE_H
