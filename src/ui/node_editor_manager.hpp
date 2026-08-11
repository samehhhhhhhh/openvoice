//
// Created by samehh on 11‏/8‏/2026.
//

#ifndef OPENVOICE_NODE_EDITOR_MANAGER_HPP
#define OPENVOICE_NODE_EDITOR_MANAGER_HPP


#include <imgui_node_editor.h>

namespace ed = ax::NodeEditor;
class node_editor_manager {

    ed::EditorContext* m_Context = nullptr;

    node_editor_manager() {
        ed::Config config;
        config.SettingsFile = "Simple.json";
        m_Context = ed::CreateEditor(&config);
    }
    ~node_editor_manager() {
        ed::DestroyEditor(m_Context);
    }

    void OnFrame(float deltaTime);

};


#endif //OPENVOICE_NODE_EDITOR_MANAGER_HPP
