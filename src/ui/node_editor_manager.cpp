//
// Created by samehh on 11‏/8‏/2026.
//

#include "node_editor_manager.hpp"

#include <algorithm>

void node_editor_man::OnFrame(float deltaTime, node_graph& ng) {
    auto& io = ImGui::GetIO();

    ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);

    ImGui::Separator();

    ed::SetCurrentEditor(m_Context);
    ed::Begin("My Editor", ImVec2(0.0, 0.0f));
    int uniqueId = 1;

    auto graph_nodes {ng.get_active_nodes()};

    for (int i = 0; i < graph_nodes.size(); i++)
    {
        if (graph_nodes.at(i) != nullptr)
            DrawNode(graph_nodes[i]->ID);

    }

    ed::End();
    ed::SetCurrentEditor(nullptr);

    //ImGui::ShowMetricsWindow();
}

void node_editor_man::DrawNode(unsigned int& uniqueId)
{

    ed::BeginNode(uniqueId);
    ImGui::Text("Node A");
    /*
        ed::BeginPin(uniqueId++ , ed::PinKind::Input);
        ImGui::Text("-> In");
        ed::EndPin();
        ImGui::SameLine();
        ed::BeginPin(uniqueId++ , ed::PinKind::Output);
        ImGui::Text("Out ->");
        ed::EndPin();
        */

    ed::EndNode();

}
