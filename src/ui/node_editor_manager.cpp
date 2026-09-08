//
// Created by samehh on 11‏/8‏/2026.
//

#include "node_editor_manager.hpp"

#include <algorithm>
#include <functional>

void node_editor_man::update_links(node_graph& ng)
{
    // loop through the graph:
    // - make sure that all node connections are in the list.
    // - Also make sure that all links created are actually linked (Rerun attach method)
}


void node_editor_man::OnFrame(float deltaTime, node_graph& ng) {
    auto& io = ImGui::GetIO();

    update_links(ng);

    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

    ImGui::Separator();

    ed::SetCurrentEditor(m_Context);
    ed::Begin("My Editor", ImVec2(0.0, 0.0f));
    int uniqueId = 1;

    auto graph_nodes {ng.get_active_nodes()};

    for (int i = 0; i < graph_nodes.size(); i++)
    {
        if (graph_nodes.at(i) != nullptr)
            DrawNode(graph_nodes[i]);

    }

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


// Drawing exiting links
    for (auto& linkInfo : m_Links)
        ed::Link(linkInfo.Id, linkInfo.InputId, linkInfo.OutputId);

    // Handle creation action
    if (ed::BeginCreate())
        {
            ed::PinId inputPinId, outputPinId;
            if (ed::QueryNewLink(&inputPinId, &outputPinId))
            {
                if (inputPinId && outputPinId)
                {
                    if (ed::AcceptNewItem())
                    {
                        m_Links.push_back({ ed::LinkId(id_generator::get_id()), inputPinId, outputPinId });

                        ed::Link(m_Links.back().Id, m_Links.back().InputId, m_Links.back().OutputId);
                    }
                }
            }
        }
    ed::EndCreate();

    if (ed::BeginDelete())
    {
        ed::LinkId deletedLinkId;
        while (ed::QueryDeletedLink(&deletedLinkId))
        {
            if (ed::AcceptDeletedItem())
            {
                for (auto it = m_Links.begin(); it != m_Links.end(); ++it)
                {
                    if (it->Id == deletedLinkId)
                    {
                        m_Links.erase(it);
                        break;
                    }
                }
            }

        }
    }
    ed::EndDelete();





}
