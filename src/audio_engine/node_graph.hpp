
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
    std::unique_ptr<endpoint_node> m_endpoint_node;


    ma_node_graph m_nodeGraph;

    // TODO: Change this from a raw value to a variable that can be set
    // btw : index of element is the ui ID.
    std::array<node*, 20> active_nodes {nullptr};

    // Every connection that currently exists in the graph. The UI draws these.
    std::vector<link> links;


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
        m_endpoint_node = std::make_unique<endpoint_node>(m_nodeGraph);

        add_node(m_vocoder_node.get());
        add_node(m_waveform_node.get());
        add_node(m_excite_node.get());
        add_node(m_endpoint_node.get());

        // Connection
        add_link(m_waveform_node->config.find_pin(pin_types::OUTPUT, 0)->ID,
                 m_vocoder_node->config.find_pin(pin_types::INPUT, 0)->ID);

        add_link(m_excite_node->config.find_pin(pin_types::OUTPUT, 0)->ID,
                 m_vocoder_node->config.find_pin(pin_types::INPUT, 1)->ID);
        
        add_link(m_vocoder_node->config.find_pin(pin_types::OUTPUT, 0)->ID,
                 m_endpoint_node->config.find_pin(pin_types::INPUT, 0)->ID);
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

    // Returns the node owning the given pin, nullptr when no active node has it
    node* find_node_by_pin(unsigned int pin_ID) const;

    // Returns the pin itself, nullptr when not found
    const pin* find_pin(unsigned int pin_ID) const;

    /* Creates a link between two pins and attaches the nodes in the miniaudio graph.
     * The pins can be given in any order, one has to be an output and the other an input.
     * Returns the ID of the new link, 0 when the connection is not valid. */
    unsigned int add_link(unsigned int pin_a_ID, unsigned int pin_b_ID);

    // Removes the link from the list and detaches the output bus it was using
    void remove_link(unsigned int link_ID);

    const std::vector<link>& get_links() const { return links; }

    std::array<node*, 20> get_active_nodes() const { return active_nodes; }

    ma_node_graph& get_nodeGraph() { return m_nodeGraph; }

    ~node_graph()
    {
        ma_node_graph_uninit(&m_nodeGraph, nullptr);
    }

};


#endif //OPENVOICE_NODE_GRAPH_HPP
