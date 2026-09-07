
#ifndef OPENVOICE_NODE_GRAPH_HPP
#define OPENVOICE_NODE_GRAPH_HPP

#include <memory>
#include "nodes.hpp"
#include <array>

class node_graph
{
    std::unique_ptr<vocoder_node> m_vocoder_node;
    std::unique_ptr<waveform_node> m_waveform_node;
    std::unique_ptr<exciter_node> m_excite_node;


    ma_node_graph m_nodeGraph;

    // TODO: Change this from a raw value to a variable that can be set
    // btw : index of element is the ui ID.
    std::array<node*, 20> active_nodes {nullptr};

public:

    node_graph()
    {

        const ma_node_graph_config nodeGraphConfig = ma_node_graph_config_init(DEVICE_CHANNELS);

        result = ma_node_graph_init(&nodeGraphConfig, nullptr, &m_nodeGraph);
        check_result("Failed to initialize node graph config");

        /////

        m_vocoder_node = std::make_unique<vocoder_node>(m_nodeGraph);
        m_waveform_node = std::make_unique<waveform_node>(m_nodeGraph);
        m_excite_node = std::make_unique<exciter_node>(m_nodeGraph);

        add_node(m_vocoder_node.get());
        add_node(m_waveform_node.get());
        add_node(m_excite_node.get());

        // Connection
        m_waveform_node->AttachTo(0, *m_vocoder_node, 0);
        m_excite_node->AttachTo(0, *m_vocoder_node, 1);
    }

    /* Sets the exciter node content (Input) */
    void set_input_exciter(const void * Data, unsigned int frameCount)
    {

        result = ma_audio_buffer_ref_set_data(&m_excite_node->g_exciteData, Data, frameCount);
        check_result("Failed to set data to buffer");

    }

    // Basically just adding a pointer to a node into the array
    void add_node(node* target_node);

    // Looks in the array for the node, when found sets it to nullptr
    void remove_node(node* target_node);

    std::array<node*, 20> get_active_nodes() const { return active_nodes; }

    ma_node_graph& get_nodeGraph() { return m_nodeGraph; }

    ~node_graph()
    {
        ma_node_graph_uninit(&m_nodeGraph, nullptr);
    }

};


#endif //OPENVOICE_NODE_GRAPH_HPP
