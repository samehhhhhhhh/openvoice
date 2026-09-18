
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
            // The links of a node that is gone have to go with it
            for (auto it = links.begin(); it != links.end();)
            {
                if (target_node->config.find_pin_by_id(it->output_pin_ID) != nullptr
                    || target_node->config.find_pin_by_id(it->input_pin_ID) != nullptr)
                {
                    const unsigned int link_ID = it->ID;
                    remove_link(link_ID);
                    it = links.begin();
                }
                else
                {
                    ++it;
                }
            }

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

node* node_graph::find_node_by_pin(const unsigned int pin_ID) const
{
    for (const auto& i : active_nodes)
    {
        if (i == nullptr) continue;

        if (i->config.find_pin_by_id(pin_ID) != nullptr) return i;
    }

    return nullptr;
}

const pin* node_graph::find_pin(const unsigned int pin_ID) const
{
    for (const auto& i : active_nodes)
    {
        if (i == nullptr) continue;

        if (const pin* found = i->config.find_pin_by_id(pin_ID)) return found;
    }

    return nullptr;
}

unsigned int node_graph::add_link(const unsigned int pin_a_ID, const unsigned int pin_b_ID)
{
    const pin* pin_a = find_pin(pin_a_ID);
    const pin* pin_b = find_pin(pin_b_ID);

    if (pin_a == nullptr || pin_b == nullptr)
    {
        std::cout << "Cannot link, one of the pins does not belong to an active node. \n";
        return 0;
    }

    if (pin_a->type == pin_b->type)
    {
        std::cout << "Cannot link two pins of the same kind. \n";
        return 0;
    }

    // The drag can start from either side, so we sort them out here
    const pin* output_pin = pin_a->type == pin_types::OUTPUT ? pin_a : pin_b;
    const pin* input_pin  = pin_a->type == pin_types::INPUT  ? pin_a : pin_b;

    node* source_node = find_node_by_pin(output_pin->ID);
    node* target_node = find_node_by_pin(input_pin->ID);

    if (source_node == target_node)
    {
        std::cout << "Cannot link a node to itself. \n";
        return 0;
    }

    // An output bus can only feed one target, so the old link goes away first
    for (auto it = links.begin(); it != links.end(); ++it)
    {
        if (it->output_pin_ID == output_pin->ID)
        {
            const unsigned int old_link_ID = it->ID;
            remove_link(old_link_ID);
            break;
        }
    }

    source_node->AttachTo(output_pin->index, *target_node, input_pin->index);

    const link new_link {.ID = id_generator::get_id(),
                         .output_pin_ID = output_pin->ID,
                         .input_pin_ID = input_pin->ID};

    links.push_back(new_link);

    return new_link.ID;
}

void node_graph::remove_link(const unsigned int link_ID)
{
    for (auto it = links.begin(); it != links.end(); ++it)
    {
        if (it->ID != link_ID) continue;

        const pin* output_pin = find_pin(it->output_pin_ID);
        node* source_node = find_node_by_pin(it->output_pin_ID);

        if (output_pin != nullptr && source_node != nullptr)
        {
            source_node->DetachFrom(output_pin->index);
        }

        links.erase(it);
        return;
    }

    std::cout << "Link that is trying to be removed was not found in the node graph. \n";
}
