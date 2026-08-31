#include "engine.hpp"

void engine::play() {

    std::cout << "Playing sound " << std::endl;

    ma_sound_start(&sound);
}

void engine::load_sound(const std::string &filename) {

    result = ma_sound_init_from_file(&audio_engine, filename.c_str(), 0, NULL, NULL, &sound);

    check_result("Failed to init from file.");
    std::cout << "Sound loaded" << std::endl;

}


void engine::data_callback(ma_device* pDevice, void* pOutput, const void* pInput, const ma_uint32 frameCount) {

    /* Recover the engine instance we stored in device_config.pUserData at init time. */
    auto* e = static_cast<engine*>(pDevice->pUserData);

    /*
    This example assumes the playback and capture sides use the same format and channel count. The
    format must be f32.
    */
    if (pDevice->capture.format != DEVICE_FORMAT || pDevice->playback.format != DEVICE_FORMAT || pDevice->capture.channels != pDevice->playback.channels || !e->stream_microphone) {

        return;
    }

    /*
    The node graph system is a pulling style of API. At the lowest level of the chain will be a
    node acting as a data source for the purpose of delivering the initial audio data. In our case,
    the data source is our pInput buffer. We need to update the underlying data source so that it
    read data from pInput.
    */

    result = ma_audio_buffer_ref_set_data(&e->g_exciteData, pInput, frameCount);
    check_result("Failed to set data to buffer");
    /* With the source buffer configured we can now read directly from the node graph. */
    result = ma_node_graph_read_pcm_frames(&e->g_nodeGraph, pOutput, frameCount, nullptr);
    check_result("Failed to read from node graph");
}



