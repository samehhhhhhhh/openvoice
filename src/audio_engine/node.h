
#ifndef OPENVOICE_NODE_H
#define OPENVOICE_NODE_H

#include "ma_vocoder_node.h"
#include "utils.h"

class node
{
protected:
    node() = default;


    ma_node* m_Node = nullptr;
    unsigned int inputBusCount = 0;
    unsigned int outputBusCount = 0;





public :

    // TODO: Also make pin IDS and configuration
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
    unsigned int GetInputBusCount() const { return inputBusCount; }
    unsigned int GetOutputBusCount() const { return outputBusCount; }



};


#endif //OPENVOICE_NODE_H
