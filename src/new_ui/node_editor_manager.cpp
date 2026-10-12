//
// Created by samehh on 11‏/8‏/2026.
//

#include "node_editor_manager.hpp"
#include <algorithm>
#include <functional>

void node_editor_man::OnFrame(float deltaTime, node_graph& ng) {
    

    // Create node editor

    // Button to add a node from the available nodes

    const auto& graph_nodes {ng.get_active_nodes()};

    for (int i = 0; i < graph_nodes.size(); i++)
    {
        if (graph_nodes.at(i) != nullptr)
            DrawNode(graph_nodes[i].get());

    }

    // The node graph owns the links, the editor only mirrors them
    DrawLinks(ng);

    HandleLinkCreation(ng);
    HandleLinkDeletion(ng);

}

#include <utility>

std::pair<pin, pin> FindSameIndexPin(node* ActiveNode, unsigned int index)
{
    pin a;
    pin b;
    for (const auto& i : ActiveNode->config.pins)
    {
        if (i.index == index)
        {
            if (i.type == pin_types::INPUT)
            {
                a = i;
            } else
            {
                b = i;
            }
        }

        if (a.index == index && b.index == index)
        {
            break;
        }
    }

    return {a, b};
}

int max_index(node * ActiveNode)
{

    int max_index = 0;

    for (const auto& i : ActiveNode->config.pins)
    {
        if (i.index > max_index) max_index = i.index;
    }

    return max_index;
}

void node_editor_man::DrawNode(node* ActiveNode)
{


    for (int i = 0; i <= max_index(ActiveNode); i++)
    {
        std::pair <pin, pin> a = FindSameIndexPin(ActiveNode, i);

        // We make sure that this is not a one pin line
        if (a.first.ID != 0)
        {
            // Draw pin
        }
        // On the same line
        if (a.second.ID != 0)
        {
            // Draw the second line
        }

    }
}

void node_editor_man::DrawLinks(const node_graph& ng)
{
    
        
}

void node_editor_man::HandleLinkCreation(node_graph& ng)
{
    
}

void node_editor_man::HandleLinkDeletion(node_graph& ng)
{
    
}
