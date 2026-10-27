
#ifndef OPENVOICE_NODE_GRAPH_HPP
#define OPENVOICE_NODE_GRAPH_HPP

#include "miniaudio.h"
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
    std::array<node_descriptor, 43> node_types {{
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
        },
        {
            "Low Pass Filter Node",
            [](ma_node_graph& graph)
            {
                return std::make_unique<lpf_node>(graph);
            },
            false
        },
        {
            "Gain / Volume",
            [](ma_node_graph& graph) { return std::make_unique<gain_node>(graph); },
            true
        },
        {
            "Invert",
            [](ma_node_graph& graph) { return std::make_unique<invert_node>(graph); },
            true
        },
        {
            "Offset",
            [](ma_node_graph& graph) { return std::make_unique<offset_node>(graph); },
            true
        },
        {
            "Add / Mix",
            [](ma_node_graph& graph) { return std::make_unique<add_mix_node>(graph); },
            true
        },
        {
            "Multiply",
            [](ma_node_graph& graph) { return std::make_unique<multiply_node>(graph); },
            true
        },
        {
            "Clamp",
            [](ma_node_graph& graph) { return std::make_unique<clamp_node>(graph); },
            true
        },
        {
            "Absolute Value",
            [](ma_node_graph& graph) { return std::make_unique<absolute_value_node>(graph); },
            true
        },
        {
            "Math Expression",
            [](ma_node_graph& graph) { return std::make_unique<math_expression_node>(graph); },
            true
        },
        {
            "High-pass Filter",
            [](ma_node_graph& graph) { return std::make_unique<high_pass_filter_node>(graph); },
            true
        },
        {
            "Band-pass Filter",
            [](ma_node_graph& graph) { return std::make_unique<band_pass_filter_node>(graph); },
            true
        },
        {
            "Notch Filter",
            [](ma_node_graph& graph) { return std::make_unique<notch_filter_node>(graph); },
            true
        },
        {
            "Parametric EQ",
            [](ma_node_graph& graph) { return std::make_unique<parametric_eq_node>(graph); },
            true
        },
        {
            "Resonant Filter",
            [](ma_node_graph& graph) { return std::make_unique<resonant_filter_node>(graph); },
            true
        },
        {
            "Filter Bank",
            [](ma_node_graph& graph) { return std::make_unique<filter_bank_node>(graph); },
            true
        },
        {
            "Compressor",
            [](ma_node_graph& graph) { return std::make_unique<compressor_node>(graph); },
            true
        },
        {
            "Limiter",
            [](ma_node_graph& graph) { return std::make_unique<limiter_node>(graph); },
            true
        },
        {
            "Noise Gate",
            [](ma_node_graph& graph) { return std::make_unique<noise_gate_node>(graph); },
            true
        },
        {
            "Envelope Follower",
            [](ma_node_graph& graph) { return std::make_unique<envelope_follower_node>(graph); },
            true
        },
        {
            "Auto Gain",
            [](ma_node_graph& graph) { return std::make_unique<auto_gain_node>(graph); },
            true
        },
        {
            "Fade / Ramp",
            [](ma_node_graph& graph) { return std::make_unique<fade_ramp_node>(graph); },
            true
        },
        {
            "Delay",
            [](ma_node_graph& graph) { return std::make_unique<delay_node>(graph); },
            true
        },
        {
            "Feedback Delay",
            [](ma_node_graph& graph) { return std::make_unique<feedback_delay_node>(graph); },
            true
        },
        {
            "Reverb",
            [](ma_node_graph& graph) { return std::make_unique<reverb_node>(graph); },
            true
        },
        {
            "Chorus",
            [](ma_node_graph& graph) { return std::make_unique<chorus_node>(graph); },
            true
        },
        {
            "Flanger",
            [](ma_node_graph& graph) { return std::make_unique<flanger_node>(graph); },
            true
        },
        {
            "Phaser",
            [](ma_node_graph& graph) { return std::make_unique<phaser_node>(graph); },
            true
        },
        {
            "Soft Clip / Saturation",
            [](ma_node_graph& graph) { return std::make_unique<soft_clip_saturation_node>(graph); },
            true
        },
        {
            "Hard Clip",
            [](ma_node_graph& graph) { return std::make_unique<hard_clip_node>(graph); },
            true
        },
        {
            "Waveshaper",
            [](ma_node_graph& graph) { return std::make_unique<waveshaper_node>(graph); },
            true
        },
        {
            "Bit Crusher",
            [](ma_node_graph& graph) { return std::make_unique<bit_crusher_node>(graph); },
            true
        },
        {
            "Rectifier",
            [](ma_node_graph& graph) { return std::make_unique<rectifier_node>(graph); },
            true
        },
        {
            "Ring Modulator",
            [](ma_node_graph& graph) { return std::make_unique<ring_modulator_node>(graph); },
            true
        },
        {
            "Pitch Shifter",
            [](ma_node_graph& graph) { return std::make_unique<pitch_shifter_node>(graph); },
            true
        },
        {
            "Frequency Shifter",
            [](ma_node_graph& graph) { return std::make_unique<frequency_shifter_node>(graph); },
            true
        },
        {
            "Pitch Detector",
            [](ma_node_graph& graph) { return std::make_unique<pitch_detector_node>(graph); },
            true
        },
        {
            "Harmonizer",
            [](ma_node_graph& graph) { return std::make_unique<harmonizer_node>(graph); },
            true
        },
        {
            "Vocoder",
            [](ma_node_graph& graph) { return std::make_unique<vocoder_effect_node>(graph); },
            true
        },
        {
            "Time Stretcher",
            [](ma_node_graph& graph) { return std::make_unique<time_stretcher_node>(graph); },
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
    void remove_link(unsigned int link_ID, const bool remove = true);

    const std::vector<link>& get_links() const { return links; }

    std::array<std::unique_ptr<node>, 20>& get_active_nodes() { return active_nodes; }

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
