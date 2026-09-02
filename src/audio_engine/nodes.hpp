//
// Created by samehh on 8/26/26.
//

#ifndef OPENVOICE_NODES_H
#define OPENVOICE_NODES_H

#include "ma_vocoder_node.h"
#include "utils.h"

class node
{
protected:
    node() = default;


    ma_node* m_Node = nullptr;

public :

    void AttachTo(const unsigned int OutputBus, ma_node* target_node, const unsigned int TargetBus) const
    {
        result = ma_node_attach_output_bus(m_Node, OutputBus, target_node, TargetBus);
        check_result("Failed to attach vocoder output bus");
    }


    void AttachTo(const unsigned int OutputBus, const node& target_node, const unsigned int TargetBus) const
    {
        result = ma_node_attach_output_bus(m_Node, OutputBus, target_node.m_Node, TargetBus);
        check_result("Failed to attach vocoder output bus");
    }

    void SetOutputBusVolume(const unsigned int OutputBus,const float volume) const
    {
        result = ma_node_set_output_bus_volume(m_Node, OutputBus, volume);
        check_result("Failed to set node output bus volume");
    }


};


struct vocoder_node : public node
{
    ma_vocoder_node_config vocoderNodeConfig;
    ma_vocoder_node     g_vocoderNode;   /* The vocoder node. */
    vocoder_node(ma_node_graph &g_nodeGraph, const ma_device& device)
    {

        m_Node = &g_vocoderNode;

        vocoderNodeConfig = ma_vocoder_node_config_init(1, device.sampleRate);

        result = ma_vocoder_node_init(&g_nodeGraph, &vocoderNodeConfig, nullptr, &g_vocoderNode);
        check_result("Failed to initialize vocoder node");

        AttachTo(0, ma_node_graph_get_endpoint(&g_nodeGraph), 0);

        SetOutputBusVolume(0, 4);
    }

    ~vocoder_node()
    {
        ma_vocoder_node_uninit(&g_vocoderNode, nullptr);
    }
};

struct waveform_node : public node
{
    ma_waveform_config waveformConfig;
    ma_waveform         g_sourceData;    /* The underlying data source of the source node. */
    ma_data_source_node_config sourceNodeConfig;
    ma_data_source_node g_sourceNode;    /* A data source node containing the source data we'll be sending through to the vocoder. This will be routed into the first bus of the vocoder node. */

    waveform_node(ma_node_graph &g_nodeGraph, const ma_device& device)
    {
        m_Node = &g_sourceNode;

        waveformConfig = ma_waveform_config_init(device.capture.format, device.capture.channels, device.sampleRate, ma_waveform_type_sawtooth, 1.0, 50);

        result = ma_waveform_init(&waveformConfig, &g_sourceData);
        check_result("Failed to initialize waveform.");

        sourceNodeConfig = ma_data_source_node_config_init(&g_sourceData);

        // Actual source node for the carrier
        result = ma_data_source_node_init(&g_nodeGraph, &sourceNodeConfig, NULL, &g_sourceNode);
        check_result("Failed to initialize source node.");
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

    exciter_node(ma_node_graph &g_nodeGraph, const ma_device& device)
    {
        m_Node = &g_exciteNode;

        /* Excite/modulator. This is where is the microphone voice coming from */
        result = ma_audio_buffer_ref_init(device.capture.format, device.capture.channels, nullptr, 0, &g_exciteData);
        check_result("Failed to initialize audio buffer for source.");

        exciteNodeConfig = ma_data_source_node_config_init(&g_exciteData);

        // We set the excite as a member of the graph and its config
        result = ma_data_source_node_init(&g_nodeGraph, &exciteNodeConfig, nullptr, &g_exciteNode);
        check_result("Failed to initialize source node.");

    }

    ~exciter_node()
    {
        ma_data_source_node_uninit(&g_exciteNode, nullptr);
    }

};








#endif //OPENVOICE_NODES_H
