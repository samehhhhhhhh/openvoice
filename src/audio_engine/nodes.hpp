//
// Created by samehh on 8/26/26.
//

#ifndef OPENVOICE_NODES_H
#define OPENVOICE_NODES_H

#include "node.hpp"
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
#define DEVICE_CHANNELS    1               /* For this example, always set to 1. */



struct vocoder_node : public node
{
    ma_vocoder_node_config vocoderNodeConfig;
    ma_vocoder_node     g_vocoderNode;   /* The vocoder node. */

    vocoder_node(ma_node_graph &g_nodeGraph)
    {

        config.title = "Vocoder node";
        config.description = "This node lets you modulate an audio stream with a waveform.";

        config.add_pin("Carrier", "This is where the waveform goes into", pin_types::INPUT, 0, id_generator::get_id());
        config.add_pin("Modulator", "This is where the voice goes into", pin_types::INPUT, 1, id_generator::get_id());
        config.add_pin("Output", "An output brah", pin_types::OUTPUT, 0, id_generator::get_id());

        m_Node = &g_vocoderNode;

        vocoderNodeConfig = ma_vocoder_node_config_init(1, SAMPLE_RATE);

        result = ma_vocoder_node_init(&g_nodeGraph, &vocoderNodeConfig, nullptr, &g_vocoderNode);
        check_result("Failed to initialize vocoder node");

        SetOutputBusVolume(0, 4);
    }

    ~vocoder_node()
    {
        ma_vocoder_node_uninit(&g_vocoderNode, nullptr);
    }
};


struct endpoint_node : public node
{
    endpoint_node(ma_node_graph &g_nodeGraph)
    {
        config.title = "Endpoint node";
        config.description = "Endpoint node";

        config.add_pin("Input", "This audio will get sent to the output device.", pin_types::INPUT, 0, id_generator::get_id());

        m_Node = ma_node_graph_get_endpoint(&g_nodeGraph);


    }
};

struct waveform_node : public node
{
    ma_waveform_config waveformConfig;
    ma_waveform         g_sourceData;    /* The underlying data source of the source node. */
    ma_data_source_node_config sourceNodeConfig;
    ma_data_source_node g_sourceNode;    /* A data source node containing the source data we'll be sending through to the vocoder. This will be routed into the first bus of the vocoder node. */

    float frequencyva = 60;
    node_widget frequency {
        widget_types::WIDGET_FLOAT_INPUT,
        "Frequency",
        &frequencyva
    };

    void reinitialize(ma_node_graph &g_nodeGraph) override{



    }

    void update() override {
        ma_waveform_set_frequency(&g_sourceData, frequencyva);
    }
    waveform_node(ma_node_graph &g_nodeGraph)
    {

        config.title = "Waveform node";
        config.description = "A waveform node";
        config.widgets.push_back(frequency);

        config.add_pin("Output", "An output brah", pin_types::OUTPUT, 0, id_generator::get_id());

        m_Node = &g_sourceNode;

        waveformConfig = ma_waveform_config_init(DEVICE_FORMAT, DEVICE_CHANNELS, SAMPLE_RATE, ma_waveform_type_sawtooth, 1.0, 50);

        result = ma_waveform_init(&waveformConfig, &g_sourceData);
        check_result("Failed to initialize waveform.");

        sourceNodeConfig = ma_data_source_node_config_init(&g_sourceData);

        // Actual source node for the carrier
        result = ma_data_source_node_init(&g_nodeGraph, &sourceNodeConfig, NULL, &g_sourceNode);
        check_result("Failed to initialize source node.");

        reinitialize(g_nodeGraph);
        // I forgot this and got stuck for a moment
        // Peak experience


    }

    ~waveform_node()
    {
        ma_data_source_node_uninit(&g_sourceNode, nullptr);
    }

};


struct exciter_node : public node
{

    ma_data_source_node_config exciteNodeConfig;
    ma_data_source_node g_exciteNode;    /* A data source node containing the excite data we'll be sending through to the vocoder. This will be routed into the second bus of the vocoder node. */
    ma_audio_buffer_ref g_exciteData;    /* The underlying data source of the excite node. */

    exciter_node(ma_node_graph &g_nodeGraph)
    {

        config.title = "Excite node";
        config.description = "Litteraly your microphone";

        config.add_pin("Output", "An output brah", pin_types::OUTPUT, 0, id_generator::get_id());
        m_Node = &g_exciteNode;

        /* Excite/modulator. This is where is the microphone voice coming from */
        result = ma_audio_buffer_ref_init(DEVICE_FORMAT, DEVICE_CHANNELS, nullptr, 0, &g_exciteData);
        check_result("Failed to initialize audio buffer for source.");

        exciteNodeConfig = ma_data_source_node_config_init(&g_exciteData);

        // We set the excite as a member of the graph and its config
        result = ma_data_source_node_init(&g_nodeGraph, &exciteNodeConfig, nullptr, &g_exciteNode);
        check_result("Failed to initialize source node.");

    }

    ma_audio_buffer_ref* get_audiobuffer() override {
        return &g_exciteData;
    }



    ~exciter_node()
    {
        ma_data_source_node_uninit(&g_exciteNode, nullptr);
    }

};

// Low pass filter
struct lpf_node : public node
{
    ma_lpf_node_config lpfConfig;
    ma_lpf_node         g_sourceData;    /* The underlying data source of the source node. */
    ma_data_source_node_config sourceNodeConfig;
    ma_data_source_node g_sourceNode;    /* A data source node containing the source data we'll be sending through to the vocoder. This will be routed into the first bus of the vocoder node. */

    lpf_node(ma_node_graph &g_nodeGraph)
    {

        config.title = "Low Pass Filter Node";
        config.description = "A LPF node";

        config.add_pin("Output", "An output brah", pin_types::OUTPUT, 0, id_generator::get_id());

        m_Node = &g_sourceNode;

        lpfConfig = ma_lpf_node_config_init(DEVICE_CHANNELS, SAMPLE_RATE, 6000, 1000);

        result = ma_lpf_node_init(&g_nodeGraph, &lpfConfig,NULL,&g_sourceData);
        check_result("Failed to initialize waveform.");

        sourceNodeConfig = ma_data_source_node_config_init(&g_sourceData);


        result = ma_data_source_node_init(&g_nodeGraph, &sourceNodeConfig, NULL, &g_sourceNode);
        // Create an allocation callback with the node graph i guess as pUser ?????
        check_result("Failed to initialize source node.");
    }

    ~lpf_node()
    {
        ma_data_source_node_uninit(&g_sourceNode, nullptr);
    }

};


#endif //OPENVOICE_NODES_H
