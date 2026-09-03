#ifndef OPENVOICE_ENGINE_H
#define OPENVOICE_ENGINE_H
#include <memory>

#include "../shared/includes.hpp"
#include "nodes.hpp"



class engine {


    static void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

    ma_engine audio_engine;
    ma_sound sound;
    ma_engine_config engineConfig;

    ma_device_config device_config;
    ma_device device;

    std::unique_ptr<vocoder_node> m_vocoder_node;
    std::unique_ptr<waveform_node> m_waveform_node;
    std::unique_ptr<exciter_node> m_excite_node;

public :
    ma_node_graph g_nodeGraph;

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

        // Node graph initialization
        const ma_node_graph_config nodeGraphConfig = ma_node_graph_config_init(DEVICE_CHANNELS);

        result = ma_node_graph_init(&nodeGraphConfig, nullptr, &g_nodeGraph);
        check_result("Failed to initialize node graph config");

        m_vocoder_node = std::make_unique<vocoder_node>(g_nodeGraph, device);
        m_waveform_node = std::make_unique<waveform_node>(g_nodeGraph, device);
        m_excite_node = std::make_unique<exciter_node>(g_nodeGraph, device);

        m_waveform_node->AttachTo(0, *m_vocoder_node, 0);
        m_excite_node->AttachTo(0, *m_vocoder_node, 1);

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
        ma_node_graph_uninit(&g_nodeGraph, nullptr);
        ma_device_uninit(&device);

        ma_sound_uninit(&sound);
        ma_engine_uninit(&audio_engine);

    }
};

#endif //OPENVOICE_ENGINE_H
