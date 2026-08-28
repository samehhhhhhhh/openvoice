//
// Created by samehh on 8/26/26.
//

#ifndef OPENVOICE_NODES_H
#define OPENVOICE_NODES_H

#include "miniaudio.h"
#include "utils.h"


// Low pass filter
struct lpf_node
{
    ma_lpf_node_config lpfNodeConfig;
    ma_lpf_node g_lpfNode;
    lpf_node(ma_node_graph g_nodeGraph)
    {
        lpfNodeConfig = ma_lpf_node_config_init(CHANNELS, SAMPLE_RATE, SAMPLE_RATE / LPF_CUTOFF_FACTOR, LPF_ORDER);

        ma_lpf_node_init(&g_nodeGraph, &lpfNodeConfig, NULL, &g_lpfNode);

        check_result("Error : Failed to init Node.");

        /* Connect the output bus of the low pass filter node to the input bus of the endpoint. */
        ma_node_attach_output_bus(&g_lpfNode, 0, ma_node_graph_get_endpoint(&g_nodeGraph), 0);

        /* Set the volume of the low pass filter to make it more of less impactful. */
        ma_node_set_output_bus_volume(&g_lpfNode, 0, LPF_BIAS);
    }

};

struct delay_node
{
    ma_delay_node_config delayNodeConfig = ma_delay_node_config_init(CHANNELS, SAMPLE_RATE, (ma_uint32)(SAMPLE_RATE * DELAY_IN_SECONDS), DECAY);
    ma_delay_node g_delayNode;
    delay_node(ma_node_graph g_nodeGraph)
    {
        result = ma_delay_node_init(&g_nodeGraph, &delayNodeConfig, NULL, &g_delayNode);
        check_result("Error : Failed to init Node.");

        /* Connect the output bus of the delay node to the input bus of the endpoint. */
        ma_node_attach_output_bus(&g_delayNode, 0, ma_node_graph_get_endpoint(&g_nodeGraph), 0);

        /* Set the volume of the delay filter to make it more of less impactful. */
        ma_node_set_output_bus_volume(&g_delayNode, 0, 1 - LPF_BIAS);
    }

};

// Splitter
struct splitter_node
{

    delay_node g_delayNode;
    lpf_node g_lpfNode;

    ma_splitter_node g_splitterNode;
    ma_splitter_node_config splitterNodeConfig = ma_splitter_node_config_init(CHANNELS);
    splitter_node(ma_node_graph g_nodeGraph)
        : g_delayNode(g_nodeGraph), g_lpfNode(g_nodeGraph)
    {
        result = ma_splitter_node_init(&g_nodeGraph, &splitterNodeConfig, NULL, &g_splitterNode);

        check_result("Error : Failed to init Node.");

        /* Connect output bus 0 to the input bus of the low pass filter node, and output bus 1 to the input bus of the delay node. */
        ma_node_attach_output_bus(&g_splitterNode, 0, &g_lpfNode,   0);
        ma_node_attach_output_bus(&g_splitterNode, 1, &g_delayNode, 0);
    }
};

struct sound_node
{
    ma_data_source_node node;
    ma_decoder decoder;
};



struct data_source_node
{

    sound_node* g_pSoundNodes;
    int g_soundNodeCount;

    data_source_node(ma_node_graph g_nodeGraph)
    {


    }


};








#endif //OPENVOICE_NODES_H
