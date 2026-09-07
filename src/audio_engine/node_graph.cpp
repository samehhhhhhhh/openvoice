
#include "node_graph.hpp"

void node_graph::add_node(node* target_node)
{
    for (auto& i : active_nodes)
    {
        // Is free
        if (i == nullptr)
        {
            i = target_node;
            target_node->ID = id_generator::get_id();
            break;
        }

        if (i == active_nodes.back())
        {
            std::cout << "Array is full. \n";
        }
    }
}

void node_graph::remove_node(node* target_node)
{
    for (auto& i : active_nodes)
    {
        if (i == target_node)
        {
            i = nullptr;
            delete(target_node);
            break;
        }
        if (i == active_nodes.back())
        {
            std::cout << "Element that is trying to be removed not found in the engine's active node array. \n";
        }
    }
}
