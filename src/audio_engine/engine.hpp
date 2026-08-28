#ifndef OPENVOICE_ENGINE_H
#define OPENVOICE_ENGINE_H
#include "../shared/includes.hpp"
#include "ma_vocoder_node.h"

#include "utils.h"
#include <array>

/*
 *
 *engine is used for playing audio
 *device is used for the node graph
 *
 */

/* Data Format */
#define FORMAT              ma_format_f32   /* Must always be f32. */
#define CHANNELS            2
#define SAMPLE_RATE         48000

/* Effect Properties */
#define LPF_BIAS            0.9f    /* Higher values means more bias towards the low pass filter (the low pass filter will be more audible). Lower values means more bias towards the echo. Must be between 0 and 1. */
#define LPF_CUTOFF_FACTOR   80      /* High values = more filter. */
#define LPF_ORDER           8
#define DELAY_IN_SECONDS    0.2f
#define DECAY               0.5f    /* Volume falloff for each echo. */

#define DEVICE_FORMAT      ma_format_f32    /* Must always be f32 for this example because the node graph system only works with this. */
#define DEVICE_CHANNELS    1                /* For this example, always set to 1. */


static ma_node_graph g_nodeGraph;
static ma_audio_buffer_ref g_exciteData;    /* The underlying data source of the excite node. */

// Enabled by default
static bool stream_microphone = true;

class engine {


    static void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount);

    void create_node();

    ma_engine audio_engine;
    ma_sound sound;
    ma_engine_config engineConfig;

    ma_device_config device_config;
    ma_device device;

    ma_vocoder_node_config vocoderNodeConfig;
    ma_data_source_node_config sourceNodeConfig;
    ma_data_source_node_config exciteNodeConfig;
    ma_waveform_config waveformConfig;

    ma_waveform         g_sourceData;    /* The underlying data source of the source node. */

    ma_data_source_node g_sourceNode;    /* A data source node containing the source data we'll be sending through to the vocoder. This will be routed into the first bus of the vocoder node. */
    ma_data_source_node g_exciteNode;    /* A data source node containing the excite data we'll be sending through to the vocoder. This will be routed into the second bus of the vocoder node. */
    ma_vocoder_node     g_vocoderNode;   /* The vocoder node. */

public :

    // https://miniaud.io/docs/examples/duplex_effect.html

// implement function that streams audio from input to ouput

    void toggle_microphone_stream() {
        stream_microphone = !stream_microphone;
        std::cout << "Toggled microphone to " << stream_microphone << std::endl;
    }

    engine() {

        // Duplex audio device
        device_config = ma_device_config_init(ma_device_type_duplex);
        device_config.capture.pDeviceID  = NULL;
        device_config.capture.format     = DEVICE_FORMAT;
        device_config.capture.channels   = DEVICE_CHANNELS;
        device_config.capture.shareMode  = ma_share_mode_shared;
        device_config.playback.pDeviceID = NULL;
        device_config.playback.format    = DEVICE_FORMAT;
        device_config.playback.channels  = DEVICE_CHANNELS;
        device_config.dataCallback       = data_callback;

        result = ma_device_init(NULL, &device_config, &device);
        check_result("Failed to initialize device");

        // Node graph initialization
        ma_node_graph_config nodeGraphConfig = ma_node_graph_config_init(device.capture.channels);

        result = ma_node_graph_init(&nodeGraphConfig, NULL, &g_nodeGraph);
        check_result("Failed to initialize node graph");

        /* Vocoder. Attached straight to the endpoint. */
        vocoderNodeConfig = ma_vocoder_node_config_init(device.capture.channels, device.sampleRate);

        result = ma_vocoder_node_init(&g_nodeGraph, &vocoderNodeConfig, NULL, &g_vocoderNode);
        check_result("Failed to initialize vocoder node");

        ma_node_attach_output_bus(&g_vocoderNode, 0, ma_node_graph_get_endpoint(&g_nodeGraph), 0);

        /* Amplify the volume of the vocoder output because in my testing it is a bit quiet. */
        ma_node_set_output_bus_volume(&g_vocoderNode, 0, 4);


        /* Source/carrier. Attached to input bus 0 of the vocoder node. */
        waveformConfig = ma_waveform_config_init(device.capture.format, device.capture.channels, device.sampleRate, ma_waveform_type_sawtooth, 1.0, 50);

        result = ma_waveform_init(&waveformConfig, &g_sourceData);
        check_result("Failed to initialize waveform for excite node.");

        sourceNodeConfig = ma_data_source_node_config_init(&g_sourceData);

        result = ma_data_source_node_init(&g_nodeGraph, &sourceNodeConfig, NULL, &g_sourceNode);
        check_result("Failed to initialize excite node.");

        ma_node_attach_output_bus(&g_sourceNode, 0, &g_vocoderNode, 0);


        /* Excite/modulator. Attached to input bus 1 of the vocoder node. */
        result = ma_audio_buffer_ref_init(device.capture.format, device.capture.channels, NULL, 0, &g_exciteData);
        check_result("Failed to initialize audio buffer for source.");

        exciteNodeConfig = ma_data_source_node_config_init(&g_exciteData);

        result = ma_data_source_node_init(&g_nodeGraph, &exciteNodeConfig, NULL, &g_exciteNode);
        check_result("Failed to initialize source node.");

        ma_node_attach_output_bus(&g_exciteNode, 0, &g_vocoderNode, 1);

        result = ma_device_start(&device);
        check_result("Failed to start device");
        // Miniaudio engine initialization for generic sound playing
        engineConfig = ma_engine_config_init();

        result = ma_engine_init(&engineConfig, &audio_engine);

        check_result("Initialize the engine");

    };

    void play();
    void load_sound(const std::string& filename);

    // Destructor
    ~engine() {
        ma_device_stop(&device);

        ma_data_source_node_uninit(&g_exciteNode, nullptr);
        ma_data_source_node_uninit(&g_sourceNode, nullptr);
        ma_vocoder_node_uninit(&g_vocoderNode, nullptr);
        ma_node_graph_uninit(&g_nodeGraph, nullptr);
        ma_device_uninit(&device);

        ma_sound_uninit(&sound);
        ma_engine_uninit(&audio_engine);

    }
};

#endif //OPENVOICE_ENGINE_H
