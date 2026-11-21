
#ifndef OPENVOICE_NODE_GRAPH_HPP
#define OPENVOICE_NODE_GRAPH_HPP

#define OPENVOICE_VERSION "0.0.1-beta"

#include "miniaudio.h"
#include "nodes.hpp"

#include <memory>
#include <array>
#include <functional>
#include <algorithm>


struct voice_descriptor
{
    std::string name;
    std::string version;
    std::string author;
    std::vector<std::shared_ptr<node>>* nodes;
    std::vector<link> links;

};
struct node_descriptor
{
    const char* title;
    std::function<std::shared_ptr<node>(ma_node_graph&)> create;
    bool ispublic;


};
class node_graph
{

    ma_node_graph m_nodeGraph;


    // This will eat alot of memory if we keep it a vector
    std::vector<std::shared_ptr<node>> active_nodes;

    // Every connection that currently exists in the graph. The UI draws these.
    std::vector<link> links;

    // I got recommended doing this by people :
    std::array<node_descriptor, 43> node_types {{
        {
            "Vocoder node",
            [](ma_node_graph& graph) -> std::shared_ptr<node>
            {
                return std::make_shared<vocoder_node>(graph);
            },
            true
        },
        {
            "Exciter node",
            [](ma_node_graph& graph) -> std::shared_ptr<node>
            {
                return std::make_shared<exciter_node>(graph);
            },
            false
        },
        {
            "WaveForm node",
            [](ma_node_graph& graph) -> std::shared_ptr<node>
            {
                return std::make_shared<waveform_node>(graph);
            },
            true
        },
        {
            "Endpoint node",
            [](ma_node_graph& graph) -> std::shared_ptr<node>
            {
                return std::make_shared<endpoint_node>(graph);
            },
            false
        },
        {
            "Low Pass Filter Node",
            [](ma_node_graph& graph) -> std::shared_ptr<node>
            {
                return std::make_shared<lpf_node>(graph);
            },
            false
        },
        {
            "Gain / Volume",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<gain_node>(graph); },
            true
        },
        {
            "Invert",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<invert_node>(graph); },
            true
        },
        {
            "Offset",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<offset_node>(graph); },
            true
        },
        {
            "Add / Mix",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<add_mix_node>(graph); },
            true
        },
        {
            "Multiply",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<multiply_node>(graph); },
            true
        },
        {
            "Clamp",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<clamp_node>(graph); },
            true
        },
        {
            "Absolute Value",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<absolute_value_node>(graph); },
            true
        },
        {
            "Math Expression",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<math_expression_node>(graph); },
            true
        },
        {
            "High-pass Filter",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<high_pass_filter_node>(graph); },
            true
        },
        {
            "Band-pass Filter",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<band_pass_filter_node>(graph); },
            true
        },
        {
            "Notch Filter",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<notch_filter_node>(graph); },
            true
        },
        {
            "Parametric EQ",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<parametric_eq_node>(graph); },
            true
        },
        {
            "Resonant Filter",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<resonant_filter_node>(graph); },
            true
        },
        {
            "Filter Bank",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<filter_bank_node>(graph); },
            true
        },
        {
            "Compressor",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<compressor_node>(graph); },
            true
        },
        {
            "Limiter",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<limiter_node>(graph); },
            true
        },
        {
            "Noise Gate",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<noise_gate_node>(graph); },
            true
        },
        {
            "Envelope Follower",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<envelope_follower_node>(graph); },
            true
        },
        {
            "Auto Gain",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<auto_gain_node>(graph); },
            true
        },
        {
            "Fade / Ramp",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<fade_ramp_node>(graph); },
            true
        },
        {
            "Delay",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<delay_node>(graph); },
            true
        },
        {
            "Feedback Delay",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<feedback_delay_node>(graph); },
            true
        },
        {
            "Reverb",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<reverb_node>(graph); },
            true
        },
        {
            "Chorus",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<chorus_node>(graph); },
            true
        },
        {
            "Flanger",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<flanger_node>(graph); },
            true
        },
        {
            "Phaser",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<phaser_node>(graph); },
            true
        },
        {
            "Soft Clip / Saturation",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<soft_clip_saturation_node>(graph); },
            true
        },
        {
            "Hard Clip",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<hard_clip_node>(graph); },
            true
        },
        {
            "Waveshaper",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<waveshaper_node>(graph); },
            true
        },
        {
            "Bit Crusher",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<bit_crusher_node>(graph); },
            true
        },
        {
            "Rectifier",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<rectifier_node>(graph); },
            true
        },
        {
            "Ring Modulator",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<ring_modulator_node>(graph); },
            true
        },
        {
            "Pitch Shifter",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<pitch_shifter_node>(graph); },
            true
        },
        {
            "Frequency Shifter",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<frequency_shifter_node>(graph); },
            true
        },
        {
            "Pitch Detector",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<pitch_detector_node>(graph); },
            true
        },
        {
            "Harmonizer",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<harmonizer_node>(graph); },
            true
        },
        {
            "Vocoder",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<vocoder_effect_node>(graph); },
            true
        },
        {
            "Time Stretcher",
            [](ma_node_graph& graph) -> std::shared_ptr<node> { return std::make_shared<time_stretcher_node>(graph); },
            true
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

        add_node(node_types[EXCITER_NODE]);
        add_node(node_types[ENDPOINT_NODE]);

    }


    /* Sets the exciter node content (Input) */
    void set_input_exciter(const void * Data, unsigned int frameCount)
    {
        result = ma_audio_buffer_ref_set_data(active_nodes[0]->get_audiobuffer(), Data, frameCount);
        check_result("Failed to set data to buffer");

    }

    // Just running the reinitialize function of every node // Broken btw but too lazy to fix
    void recompile_nodes() {

        // Removing all links between nodes

        for(auto const& i : links) {
            remove_link(i.ID, false);
        }

        // Reinitializing all nodes
        for(auto const& i : active_nodes) {
            if(i != nullptr) {
                i->reinitialize(m_nodeGraph);
            }

        }

        // Reapplying all links
        for(const auto& i : links) {


            node* m_Node = find_node_by_pin(i.output_pin_ID);

            node* target_node = find_node_by_pin(i.input_pin_ID);

            unsigned int input_pin_index = target_node->config.find_pin_by_id(i.input_pin_ID)->index;

            unsigned int output_pin_index = m_Node->config.find_pin_by_id(i.output_pin_ID)->index;

            m_Node->AttachTo(output_pin_index, &target_node, input_pin_index);
        }

    }

    void update_nodes() {
        for(auto const& i : active_nodes) {
            if(i != nullptr) {
                i->update();
            }

        }
    }

    // Basically just adding a pointer to a node into the array
    std::shared_ptr<node> add_node(node_descriptor target_node);

    // Looks in the array for the node, when found sets it to nullptr
    void remove_node(std::shared_ptr<node> target_node);

    // Returns the node owning the given pin, nullptr when no active node has it
    node* find_node_by_pin(unsigned int pin_ID);

    // Returns the pin itself, nullptr when not found
    const pin* find_pin(unsigned int pin_ID) const;

    /* Creates a link between two pins and attaches the nodes in the miniaudio graph.
     * The pins can be given in any order, one has to be an output and the other an input.
     * Returns the ID of the new link, 0 when the connection is not valid. */
    unsigned int add_link(unsigned int pin_a_ID, unsigned int pin_b_ID);

    // Removes the link from the list and detaches the output bus it was using
    void remove_link(unsigned int link_ID, const bool remove = true);

    const std::vector<link>& get_links() const { return links; }

    std::vector<std::shared_ptr<node>>& get_active_nodes() { return active_nodes; }

    ma_node_graph& get_nodeGraph() { return m_nodeGraph; }

    std::array<node_descriptor, 43> get_node_types() {
        return node_types;
    }

    ~node_graph()
    {
        ma_node_graph_uninit(&m_nodeGraph, nullptr);
    }

};


#endif //OPENVOICE_NODE_GRAPH_HPP
