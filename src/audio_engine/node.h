
#ifndef OPENVOICE_NODE_H
#define OPENVOICE_NODE_H

#include "ma_vocoder_node.h"
#include "utils.h"
#include <vector>
#include "id_generator.hpp"

enum class pin_types
{
    INPUT = 1,
    OUTPUT
};
struct pin
{
    std::string title;
    std::string description;
    pin_types type;
    unsigned int index = 100; // TODO: This will be a problem, that i will fix :(
    unsigned int ID = 0; // default for verification later on
};

struct node_config
{
    std::string title = "Untitled node";
    std::string description;
    std::string color;
    std::vector<pin> pins;

    void add_pin(std::string title, std::string description, pin_types type, unsigned int index, unsigned int ID)
    {
        const pin m_pin {.title = title, .description = description, .type = type, .index = index, .ID = ID};
        pins.push_back(m_pin);
    }

};

class node
{
protected:
    node() = default;

    ma_node* m_Node = nullptr;

public :

    node_config config;
    unsigned int ID;
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


#endif //OPENVOICE_NODE_H
