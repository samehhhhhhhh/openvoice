//
// Created by samehh on 11‏/8‏/2026.
//

#ifndef OPENVOICE_NODE_EDITOR_MANAGER_HPP
#define OPENVOICE_NODE_EDITOR_MANAGER_HPP


#include <imgui_node_editor.h>
#include "../audio_engine/node_graph.hpp"
/*
 * Option 1 :
 * we read the mini audio graph g_Graph in audio engine starting from the endpoint and making our way all the way to
 * the top every frame.
 *
 * We get the name of the node, draw it with its input and outputs,
 *
 * Option 2 :
 *
 * We make a wrapper for the nodeGraph object in the audio engine so that we manage
 * all the data and don't use mini audio methods here. Easier ?
 *
 *
 */

namespace ed = ax::NodeEditor;
class node_editor_man {

    ed::EditorContext* m_Context = nullptr;

    void DrawNode(node* ActiveNode);

    void GetNodesFromNodeGraph();



public:
    node_editor_man()
    {
        ed::Config config;
        config.SettingsFile = "Simple.json";
        m_Context = ed::CreateEditor(&config);
    }


    void OnFrame(float deltaTime, node_graph& ng);


    ~node_editor_man() {
        ed::DestroyEditor(m_Context);
    }
};


#endif //OPENVOICE_NODE_EDITOR_MANAGER_HPP
