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
    ma_lpf_node         g_filter;
    float cutoff = 6000.0f;
    float order = 8.0f;
    node_widget cutoff_widget {widget_types::WIDGET_FLOAT_INPUT, "Cutoff", &cutoff};
    node_widget order_widget {widget_types::WIDGET_FLOAT_INPUT, "Order", &order};

    lpf_node(ma_node_graph& graph)
    {
        config.title = "Low Pass Filter Node";
        config.description = "Reduces high frequencies.";
        config.widgets.push_back(cutoff_widget);
        config.widgets.push_back(order_widget);

        config.add_pin("Input", "Audio input", pin_types::INPUT, 0, id_generator::get_id());
        config.add_pin("Output", "Filtered audio", pin_types::OUTPUT, 0, id_generator::get_id());

        lpfConfig = ma_lpf_node_config_init(DEVICE_CHANNELS, SAMPLE_RATE, cutoff, order);
        result = ma_lpf_node_init(&graph, &lpfConfig, nullptr, &g_filter);
        check_result("Failed to initialize low-pass filter");
        m_Node = reinterpret_cast<ma_node*>(&g_filter);
    }

    ~lpf_node()
    {
        ma_lpf_node_uninit(&g_filter, nullptr);
    }

    void update() override
    {
        const auto config = ma_lpf_config_init(ma_format_f32, DEVICE_CHANNELS, SAMPLE_RATE,
                                               cutoff, static_cast<ma_uint32>(order < 1.0f ? 1.0f : order));
        result = ma_lpf_node_reinit(&config, &g_filter);
        check_result("Failed to update low-pass filter");
    }

};

enum class basic_operation
{
    GAIN,
    INVERT,
    OFFSET,
    MIX,
    MULTIPLY,
    CLAMP,
    ABSOLUTE_VAL, // For some reason, the preprocessor on windows (using MinGW) replaces ABSOLUTE
    EXPRESSION
};

struct basic_dsp_state
{
    ma_node_base node;
    basic_operation operation;
    float value = 1.0f;
    float secondary_value = 0.0f;
    float minimum = -1.0f;
    float maximum = 1.0f;
};

struct basic_dsp_node : public node
{
    basic_dsp_state state;
    ma_uint32 input_channels[2] {DEVICE_CHANNELS, DEVICE_CHANNELS};
    ma_uint32 output_channels[1] {DEVICE_CHANNELS};

    static void process(ma_node* raw_node, const float** inputs, ma_uint32* input_frames,
                        float** outputs, ma_uint32* output_frames)
    {
        auto* state = reinterpret_cast<basic_dsp_state*>(raw_node);
        const ma_uint32 frames = *input_frames < *output_frames ? *input_frames : *output_frames;
        const ma_uint32 sample_count = frames * DEVICE_CHANNELS;

        for (ma_uint32 i = 0; i < sample_count; ++i)
        {
            const float first = inputs[0] == nullptr ? 0.0f : inputs[0][i];
            const bool needs_second_input = state->operation == basic_operation::MIX ||
                                            state->operation == basic_operation::MULTIPLY;
            const float second = !needs_second_input || inputs[1] == nullptr ? 0.0f : inputs[1][i];
            float value = first;

            switch (state->operation)
            {
                case basic_operation::GAIN:
                    value = first * state->value;
                    break;
                case basic_operation::INVERT:
                    value = -first;
                    break;
                case basic_operation::OFFSET:
                    value = first + state->value;
                    break;
                case basic_operation::MIX:
                    value = first + second * state->value;
                    break;
                case basic_operation::MULTIPLY:
                    value = first * second;
                    break;
                case basic_operation::CLAMP:
                    value = first < state->minimum ? state->minimum :
                            (first > state->maximum ? state->maximum : first);
                    break;
                case basic_operation::ABSOLUTE_VAL:
                    value = first < 0.0f ? -first : first;
                    break;
                case basic_operation::EXPRESSION:
                    value = first * state->value + state->secondary_value;
                    break;
            }

            outputs[0][i] = value;
        }

        *input_frames = frames;
        *output_frames = frames;
    }

    static inline const ma_node_vtable one_input_vtable {
        process, nullptr, 1, 1, 0
    };

    static inline const ma_node_vtable two_input_vtable {
        process, nullptr, 2, 1, 0
    };

    basic_dsp_node(ma_node_graph& graph, const char* title, const char* description,
                   basic_operation operation, bool two_inputs = false)
        : state {}
    {
        state.operation = operation;
        config.title = title;
        config.description = description;
        config.add_pin("Input", "Audio input", pin_types::INPUT, 0, id_generator::get_id());
        if (two_inputs)
            config.add_pin("Input 2", "Second audio input", pin_types::INPUT, 1, id_generator::get_id());
        config.add_pin("Output", "Audio output", pin_types::OUTPUT, 0, id_generator::get_id());

        ma_node_config node_config = ma_node_config_init();
        node_config.vtable = two_inputs ? &two_input_vtable : &one_input_vtable;
        node_config.pInputChannels = input_channels;
        node_config.pOutputChannels = output_channels;

        result = ma_node_init(&graph, &node_config, nullptr, &state.node);
        check_result("Failed to initialize basic DSP node");
        m_Node = reinterpret_cast<ma_node*>(&state.node);
    }

    ~basic_dsp_node()
    {
        ma_node_uninit(&state.node, nullptr);
    }
};

struct gain_node : public basic_dsp_node
{
    float gain = 1.0f;
    node_widget gain_widget {widget_types::WIDGET_FLOAT_INPUT, "Gain", &gain};

    explicit gain_node(ma_node_graph& graph)
        : basic_dsp_node(graph, "Gain / Volume", "Multiply the signal by a value.",
                         basic_operation::GAIN)
    {
        config.widgets.push_back(gain_widget);
        state.value = gain;
    }

    void update() override { state.value = gain; }
};

struct invert_node : public basic_dsp_node
{
    explicit invert_node(ma_node_graph& graph)
        : basic_dsp_node(graph, "Invert", "Flip the waveform polarity.", basic_operation::INVERT) {}
};

struct offset_node : public basic_dsp_node
{
    float offset = 0.0f;
    node_widget offset_widget {widget_types::WIDGET_FLOAT_INPUT, "Offset", &offset};

    explicit offset_node(ma_node_graph& graph)
        : basic_dsp_node(graph, "Offset", "Add a constant to every sample.", basic_operation::OFFSET)
    {
        config.widgets.push_back(offset_widget);
        state.value = offset;
    }

    void update() override { state.value = offset; }
};

struct add_mix_node : public basic_dsp_node
{
    float second_gain = 1.0f;
    node_widget gain_widget {widget_types::WIDGET_FLOAT_INPUT, "Input 2 Gain", &second_gain};

    explicit add_mix_node(ma_node_graph& graph)
        : basic_dsp_node(graph, "Add / Mix", "Add two audio inputs.", basic_operation::MIX, true)
    {
        config.widgets.push_back(gain_widget);
        state.value = second_gain;
    }

    void update() override { state.value = second_gain; }
};

struct multiply_node : public basic_dsp_node
{
    explicit multiply_node(ma_node_graph& graph)
        : basic_dsp_node(graph, "Multiply", "Multiply two audio inputs sample by sample.",
                         basic_operation::MULTIPLY, true) {}
};

struct clamp_node : public basic_dsp_node
{
    float minimum = -1.0f;
    float maximum = 1.0f;
    node_widget minimum_widget {widget_types::WIDGET_FLOAT_INPUT, "Minimum", &minimum};
    node_widget maximum_widget {widget_types::WIDGET_FLOAT_INPUT, "Maximum", &maximum};

    explicit clamp_node(ma_node_graph& graph)
        : basic_dsp_node(graph, "Clamp", "Constrain samples to a range.", basic_operation::CLAMP)
    {
        config.widgets.push_back(minimum_widget);
        config.widgets.push_back(maximum_widget);
    }

    void update() override
    {
        state.minimum = minimum < maximum ? minimum : maximum;
        state.maximum = maximum > minimum ? maximum : minimum;
    }
};

struct absolute_value_node : public basic_dsp_node
{
    explicit absolute_value_node(ma_node_graph& graph)
        : basic_dsp_node(graph, "Absolute Value", "Make negative samples positive.",
                         basic_operation::ABSOLUTE_VAL) {}
};

struct math_expression_node : public basic_dsp_node
{
    float scale = 1.0f;
    float offset = 0.0f;
    node_widget scale_widget {widget_types::WIDGET_FLOAT_INPUT, "Scale", &scale};
    node_widget offset_widget {widget_types::WIDGET_FLOAT_INPUT, "Offset", &offset};

    explicit math_expression_node(ma_node_graph& graph)
        : basic_dsp_node(graph, "Math Expression", "Apply y = x * scale + offset.",
                         basic_operation::EXPRESSION)
    {
        config.widgets.push_back(scale_widget);
        config.widgets.push_back(offset_widget);
        state.value = scale;
        state.secondary_value = offset;
    }

    void update() override
    {
        state.value = scale;
        state.secondary_value = offset;
    }
};

struct pending_dsp_node : public basic_dsp_node
{
    float parameter_1 = 0.0f;
    float parameter_2 = 0.0f;
    float parameter_3 = 0.0f;

    pending_dsp_node(ma_node_graph& graph, const char* title, bool two_inputs = false)
        : basic_dsp_node(graph, title, "DSP implementation scheduled for a later phase.",
                         basic_operation::GAIN, two_inputs) {}

    void add_pending_parameters(const char* parameter_1_label,
                                const char* parameter_2_label,
                                const char* parameter_3_label)
    {
        config.widgets.push_back(
            {widget_types::WIDGET_FLOAT_INPUT, parameter_1_label, &parameter_1});
        config.widgets.push_back(
            {widget_types::WIDGET_FLOAT_INPUT, parameter_2_label, &parameter_2});
        config.widgets.push_back(
            {widget_types::WIDGET_FLOAT_INPUT, parameter_3_label, &parameter_3});
    }
};

struct high_pass_filter_node : public node
{
    ma_hpf_node_config filter_config;
    ma_hpf_node filter;
    float cutoff = 1000.0f;
    node_widget cutoff_widget {widget_types::WIDGET_FLOAT_INPUT, "Cutoff", &cutoff};

    explicit high_pass_filter_node(ma_node_graph& graph)
    {
        config.title = "High-pass Filter";
        config.description = "Reduce frequencies below the cutoff.";
        config.widgets.push_back(cutoff_widget);
        config.add_pin("Input", "Audio input", pin_types::INPUT, 0, id_generator::get_id());
        config.add_pin("Output", "Audio output", pin_types::OUTPUT, 0, id_generator::get_id());
        filter_config = ma_hpf_node_config_init(DEVICE_CHANNELS, SAMPLE_RATE, cutoff, 2);
        result = ma_hpf_node_init(&graph, &filter_config, nullptr, &filter);
        check_result("Failed to initialize high-pass filter");
        m_Node = reinterpret_cast<ma_node*>(&filter);
    }

    ~high_pass_filter_node() { ma_hpf_node_uninit(&filter, nullptr); }

    void update() override
    {
        const auto config = ma_hpf_config_init(ma_format_f32, DEVICE_CHANNELS, SAMPLE_RATE,
                                               cutoff, filter_config.hpf.order);
        result = ma_hpf_node_reinit(&config, &filter);
        check_result("Failed to update high-pass filter");
    }
};

struct band_pass_filter_node : public node
{
    ma_bpf_node_config filter_config;
    ma_bpf_node filter;
    float cutoff = 1000.0f;
    float order = 2.0f;
    node_widget cutoff_widget {widget_types::WIDGET_FLOAT_INPUT, "Cutoff", &cutoff};
    node_widget order_widget {widget_types::WIDGET_FLOAT_INPUT, "Order", &order};

    explicit band_pass_filter_node(ma_node_graph& graph)
    {
        config.title = "Band-pass Filter";
        config.description = "Keep frequencies around the cutoff.";
        config.widgets.push_back(cutoff_widget);
        config.widgets.push_back(order_widget);
        config.add_pin("Input", "Audio input", pin_types::INPUT, 0, id_generator::get_id());
        config.add_pin("Output", "Audio output", pin_types::OUTPUT, 0, id_generator::get_id());
        filter_config = ma_bpf_node_config_init(DEVICE_CHANNELS, SAMPLE_RATE, cutoff, order);
        result = ma_bpf_node_init(&graph, &filter_config, nullptr, &filter);
        check_result("Failed to initialize band-pass filter");
        m_Node = reinterpret_cast<ma_node*>(&filter);
    }

    ~band_pass_filter_node() { ma_bpf_node_uninit(&filter, nullptr); }

    void update() override
    {
        const auto config = ma_bpf_config_init(ma_format_f32, DEVICE_CHANNELS, SAMPLE_RATE,
                                               cutoff, static_cast<ma_uint32>(order));
        result = ma_bpf_node_reinit(&config, &filter);
        check_result("Failed to update band-pass filter");
    }
};

struct notch_filter_node : public node
{
    ma_notch_node_config filter_config;
    ma_notch_node filter;
    float frequency = 1000.0f;
    float q = 1.0f;
    node_widget frequency_widget {widget_types::WIDGET_FLOAT_INPUT, "Frequency", &frequency};
    node_widget q_widget {widget_types::WIDGET_FLOAT_INPUT, "Q", &q};

    explicit notch_filter_node(ma_node_graph& graph)
    {
        config.title = "Notch Filter";
        config.description = "Remove a narrow frequency region.";
        config.widgets.push_back(frequency_widget);
        config.widgets.push_back(q_widget);
        config.add_pin("Input", "Audio input", pin_types::INPUT, 0, id_generator::get_id());
        config.add_pin("Output", "Audio output", pin_types::OUTPUT, 0, id_generator::get_id());
        filter_config = ma_notch_node_config_init(DEVICE_CHANNELS, SAMPLE_RATE, q, frequency);
        result = ma_notch_node_init(&graph, &filter_config, nullptr, &filter);
        check_result("Failed to initialize notch filter");
        m_Node = reinterpret_cast<ma_node*>(&filter);
    }

    ~notch_filter_node() { ma_notch_node_uninit(&filter, nullptr); }

    void update() override
    {
        const auto config = ma_notch2_config_init(ma_format_f32, DEVICE_CHANNELS, SAMPLE_RATE,
                                                  q, frequency);
        result = ma_notch_node_reinit(&config, &filter);
        check_result("Failed to update notch filter");
    }
};

struct parametric_eq_node : public node
{
    ma_peak_node_config filter_config;
    ma_peak_node filter;
    float gain_db = 0.0f;
    float frequency = 1000.0f;
    float q = 1.0f;
    node_widget gain_widget {widget_types::WIDGET_FLOAT_INPUT, "Gain dB", &gain_db};
    node_widget frequency_widget {widget_types::WIDGET_FLOAT_INPUT, "Frequency", &frequency};
    node_widget q_widget {widget_types::WIDGET_FLOAT_INPUT, "Q", &q};

    explicit parametric_eq_node(ma_node_graph& graph)
    {
        config.title = "Parametric EQ";
        config.description = "Boost or cut a frequency range.";
        config.widgets.push_back(gain_widget);
        config.widgets.push_back(frequency_widget);
        config.widgets.push_back(q_widget);
        config.add_pin("Input", "Audio input", pin_types::INPUT, 0, id_generator::get_id());
        config.add_pin("Output", "Audio output", pin_types::OUTPUT, 0, id_generator::get_id());
        filter_config = ma_peak_node_config_init(DEVICE_CHANNELS, SAMPLE_RATE, gain_db, q, frequency);
        result = ma_peak_node_init(&graph, &filter_config, nullptr, &filter);
        check_result("Failed to initialize parametric EQ");
        m_Node = reinterpret_cast<ma_node*>(&filter);
    }

    ~parametric_eq_node() { ma_peak_node_uninit(&filter, nullptr); }

    void update() override
    {
        const auto config = ma_peak2_config_init(ma_format_f32, DEVICE_CHANNELS, SAMPLE_RATE,
                                                 gain_db, q, frequency);
        result = ma_peak_node_reinit(&config, &filter);
        check_result("Failed to update parametric EQ");
    }
};

struct resonant_filter_node : public pending_dsp_node
{
    explicit resonant_filter_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Resonant Filter")
    {
        add_pending_parameters("Cutoff", "Resonance", "Mix");
    }
};
struct filter_bank_node : public pending_dsp_node
{
    explicit filter_bank_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Filter Bank")
    {
        add_pending_parameters("Low Band", "High Band", "Mix");
    }
};
struct compressor_node : public pending_dsp_node
{
    explicit compressor_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Compressor")
    {
        add_pending_parameters("Threshold", "Ratio", "Attack");
    }
};
struct limiter_node : public pending_dsp_node
{
    explicit limiter_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Limiter")
    {
        add_pending_parameters("Ceiling", "Release", "Mix");
    }
};
struct noise_gate_node : public pending_dsp_node
{
    explicit noise_gate_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Noise Gate")
    {
        add_pending_parameters("Threshold", "Attack", "Release");
    }
};
struct envelope_follower_node : public pending_dsp_node
{
    explicit envelope_follower_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Envelope Follower")
    {
        add_pending_parameters("Attack", "Release", "Sensitivity");
    }
};
struct auto_gain_node : public pending_dsp_node
{
    explicit auto_gain_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Auto Gain")
    {
        add_pending_parameters("Target", "Speed", "Maximum");
    }
};
struct fade_ramp_node : public pending_dsp_node
{
    explicit fade_ramp_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Fade / Ramp")
    {
        add_pending_parameters("Start", "End", "Duration");
    }
};
struct delay_node : public pending_dsp_node
{
    explicit delay_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Delay")
    {
        add_pending_parameters("Time", "Feedback", "Mix");
    }
};
struct feedback_delay_node : public pending_dsp_node
{
    explicit feedback_delay_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Feedback Delay")
    {
        add_pending_parameters("Time", "Feedback", "Mix");
    }
};
struct reverb_node : public pending_dsp_node
{
    explicit reverb_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Reverb")
    {
        add_pending_parameters("Room Size", "Damping", "Mix");
    }
};
struct chorus_node : public pending_dsp_node
{
    explicit chorus_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Chorus")
    {
        add_pending_parameters("Rate", "Depth", "Mix");
    }
};
struct flanger_node : public pending_dsp_node
{
    explicit flanger_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Flanger")
    {
        add_pending_parameters("Rate", "Depth", "Mix");
    }
};
struct phaser_node : public pending_dsp_node
{
    explicit phaser_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Phaser")
    {
        add_pending_parameters("Rate", "Depth", "Stages");
    }
};
struct soft_clip_saturation_node : public pending_dsp_node
{
    explicit soft_clip_saturation_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Soft Clip / Saturation")
    {
        add_pending_parameters("Drive", "Threshold", "Mix");
    }
};
struct hard_clip_node : public pending_dsp_node
{
    explicit hard_clip_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Hard Clip")
    {
        add_pending_parameters("Ceiling", "Floor", "Mix");
    }
};
struct waveshaper_node : public pending_dsp_node
{
    explicit waveshaper_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Waveshaper")
    {
        add_pending_parameters("Drive", "Curve", "Mix");
    }
};
struct bit_crusher_node : public pending_dsp_node
{
    explicit bit_crusher_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Bit Crusher")
    {
        add_pending_parameters("Bit Depth", "Sample Rate", "Mix");
    }
};
struct rectifier_node : public pending_dsp_node
{
    explicit rectifier_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Rectifier")
    {
        add_pending_parameters("Mode", "Threshold", "Mix");
    }
};
struct ring_modulator_node : public pending_dsp_node
{
    explicit ring_modulator_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Ring Modulator", true)
    {
        add_pending_parameters("Frequency", "Depth", "Mix");
    }
};
struct pitch_shifter_node : public pending_dsp_node
{
    explicit pitch_shifter_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Pitch Shifter")
    {
        add_pending_parameters("Semitones", "Window", "Mix");
    }
};
struct frequency_shifter_node : public pending_dsp_node
{
    explicit frequency_shifter_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Frequency Shifter")
    {
        add_pending_parameters("Frequency", "Range", "Mix");
    }
};
struct pitch_detector_node : public pending_dsp_node
{
    explicit pitch_detector_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Pitch Detector")
    {
        add_pending_parameters("Minimum Hz", "Maximum Hz", "Sensitivity");
    }
};
struct harmonizer_node : public pending_dsp_node
{
    explicit harmonizer_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Harmonizer")
    {
        add_pending_parameters("Intervals", "Detune", "Mix");
    }
};
struct vocoder_effect_node : public pending_dsp_node
{
    explicit vocoder_effect_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Vocoder", true)
    {
        add_pending_parameters("Bands", "Attack", "Release");
    }
};
struct time_stretcher_node : public pending_dsp_node
{
    explicit time_stretcher_node(ma_node_graph& graph)
        : pending_dsp_node(graph, "Time Stretcher")
    {
        add_pending_parameters("Ratio", "Window", "Mix");
    }
};


#endif //OPENVOICE_NODES_H
