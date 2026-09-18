//
// Created by samehh on 11‏/8‏/2026.
//

#include "node_editor_manager.hpp"

#include <algorithm>
#include <functional>

void node_editor_man::OnFrame(float deltaTime, node_graph& ng) {
    auto& io = ImGui::GetIO();

    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

    ImGui::Separator();

    ed::SetCurrentEditor(m_Context);
    ed::Begin("My Editor", ImVec2(0.0, 0.0f));

    auto graph_nodes {ng.get_active_nodes()};

    for (int i = 0; i < graph_nodes.size(); i++)
    {
        if (graph_nodes.at(i) != nullptr)
            DrawNode(graph_nodes[i]);

    }

    // The node graph owns the links, the editor only mirrors them
    DrawLinks(ng);

    HandleLinkCreation(ng);
    HandleLinkDeletion(ng);

    ed::End();
    ed::SetCurrentEditor(nullptr);

    //ImGui::ShowMetricsWindow();
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

    ed::BeginNode(ActiveNode->ID);
    ImGui::Text(ActiveNode->config.title.c_str());

    for (int i = 0; i <= max_index(ActiveNode); i++)
    {
        std::pair <pin, pin> a = FindSameIndexPin(ActiveNode, i);

        // We make sure that this is not a one pin line
        if (a.first.ID != 0)
        {
            ed::BeginPin(a.first.ID , ed::PinKind::Input);
            ImGui::Text(a.first.title.c_str());
            ed::EndPin();
        }
        ImGui::SameLine();
        if (a.second.ID != 0)
        {
            ed::BeginPin(a.second.ID , ed::PinKind::Output);
            ImGui::Text(a.second.title.c_str());
            ed::EndPin();
        }

    }
    ed::EndNode();
}

void node_editor_man::DrawLinks(const node_graph& ng)
{
    for (const auto& linkInfo : ng.get_links())
        ed::Link(ed::LinkId(linkInfo.ID),
                 ed::PinId(linkInfo.output_pin_ID),
                 ed::PinId(linkInfo.input_pin_ID));
}

void node_editor_man::HandleLinkCreation(node_graph& ng)
{
    if (ed::BeginCreate())
    {
        ed::PinId startPinId, endPinId;
        if (ed::QueryNewLink(&startPinId, &endPinId))
        {
            if (startPinId && endPinId)
            {
                const pin* startPin = ng.find_pin(static_cast<unsigned int>(startPinId.Get()));
                const pin* endPin   = ng.find_pin(static_cast<unsigned int>(endPinId.Get()));

                // One side has to be an output and the other an input, and never the same node
                const bool valid = startPin != nullptr && endPin != nullptr
                    && startPin->type != endPin->type
                    && ng.find_node_by_pin(startPin->ID) != ng.find_node_by_pin(endPin->ID);

                if (!valid)
                {
                    ed::RejectNewItem();
                }
                else if (ed::AcceptNewItem())
                {
                    // This both stores the link and attaches the nodes in the audio graph
                    ng.add_link(startPin->ID, endPin->ID);
                }
            }
        }
    }
    ed::EndCreate();
}

void node_editor_man::HandleLinkDeletion(node_graph& ng)
{
    if (ed::BeginDelete())
    {
        ed::LinkId deletedLinkId;
        while (ed::QueryDeletedLink(&deletedLinkId))
        {
            if (ed::AcceptDeletedItem())
            {
                // Drops the link and detaches the output bus it was using
                ng.remove_link(static_cast<unsigned int>(deletedLinkId.Get()));
            }

        }
    }
    ed::EndDelete();
}
