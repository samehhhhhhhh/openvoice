
#ifndef OPENVOICE_NODE_GRAPH_HPP
#define OPENVOICE_NODE_GRAPH_HPP


#include <memory>
#include "nodes.hpp"
#include <array>
#include <functional>
struct node_descriptor
{
    const char* title;
    std::function<std::unique_ptr<node>(ma_node_graph&)> create;
    bool ispublic;
};


class node_graph
{

    ma_node_graph m_nodeGraph;

    // TODO: Change this from a raw value to a variable that can be set
    // btw : index of element is the ui ID.
    std::array<std::unique_ptr<node>, 20> active_nodes {nullptr};

    // Every connection that currently exists in the graph. The UI draws these.
    std::vector<link> links;

    // I got recommended doing this by people : 
    std::array<node_descriptor, 4> node_types {{
        {
            "Vocoder node",
            [](ma_node_graph& graph)
            {
                return std::make_unique<vocoder_node>(graph);
            },
            true
        },
        {
            "Exciter node",
            [](ma_node_graph& graph)
            {
                return std::make_unique<exciter_node>(graph);
            },
            false
        },
        {
            "WaveForm node",
            [](ma_node_graph& graph)
            {
                return std::make_unique<waveform_node>(graph);
            },
            true
        },
        {
            "Endpoint node",
            [](ma_node_graph& graph)
            {
                return std::make_unique<endpoint_node>(graph);
            },
            false
        }
    }};

    enum nodes {
        VOCODER_NODE = 0,
        EXCITER_NODE,
        WAVEFORM_NODE,
        ENDPOINT_NODE
    };


public:

    node_graph()
    {

        const ma_node_graph_config nodeGraphConfig = ma_node_graph_config_init(DEVICE_CHANNELS);

        result = ma_node_graph_init(&nodeGraphConfig, nullptr, &m_nodeGraph);
        check_result("Failed to initialize node graph config");        

    }


    /* Sets the exciter node content (Input) */
    void set_input_exciter(const void * Data, unsigned int frameCount)
    {
        result = ma_audio_buffer_ref_set_data(active_nodes[0]->get_audiobuffer(), Data, frameCount);
        check_result("Failed to set data to buffer");

    }

    

    // Basically just adding a pointer to a node into the array
    void add_node(node_descriptor target_node);

    // Looks in the array for the node, when found sets it to nullptr
    void remove_node(std::unique_ptr<node> target_node);

    // Returns the node owning the given pin, nullptr when no active node has it
    node* find_node_by_pin(unsigned int pin_ID);

    // Returns the pin itself, nullptr when not found
    const pin* find_pin(unsigned int pin_ID) const;

    /* Creates a link between two pins and attaches the nodes in the miniaudio graph.
     * The pins can be given in any order, one has to be an output and the other an input.
     * Returns the ID of the new link, 0 when the connection is not valid. */
    unsigned int add_link(unsigned int pin_a_ID, unsigned int pin_b_ID);

    // Removes the link from the list and detaches the output bus it was using
    void remove_link(unsigned int link_ID);

    const std::vector<link>& get_links() const { return links; }

    std::array<std::unique_ptr<node>, 20>& get_active_nodes() { return active_nodes; }

    ma_node_graph& get_nodeGraph() { return m_nodeGraph; }

    std::array<node_descriptor, 4> get_node_types() {
        return node_types;
    }

    ~node_graph()
    {
        ma_node_graph_uninit(&m_nodeGraph, nullptr);
    }

};


#endif //OPENVOICE_NODE_GRAPH_HPP
