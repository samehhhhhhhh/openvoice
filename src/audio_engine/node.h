
#ifndef OPENVOICE_NODE_H
#define OPENVOICE_NODE_H


class node
{

    node() = default;

    ~node() = default;


    void AttachTo(unsigned int OutputBus, unsigned int TargetBus)
    {

    }

    void SetOutputBusVolume(unsigned int OutputBus, int volume);
};


#endif //OPENVOICE_NODE_H
