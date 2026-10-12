//
// Created by samehh on 11‏/8‏/2026.
//

#ifndef OPENVOICE_NODE_EDITOR_MANAGER_HPP
#define OPENVOICE_NODE_EDITOR_MANAGER_HPP


#include "node_graph.hpp"


class node_editor_man {

    

    void DrawNode(node* ActiveNode);

    void GetNodesFromNodeGraph();

    // Draws every link the node graph currently holds
    void DrawLinks(const node_graph& ng);

    // Turns a link dragged in the editor into a real node attachment
    void HandleLinkCreation(node_graph& ng);

    // Removes the link from the graph and detaches the nodes
    void HandleLinkDeletion(node_graph& ng);

public:
    node_editor_man()
    {
        
    }

    void OnFrame(float deltaTime, node_graph& ng);


    ~node_editor_man() {
        
    }
};


#endif //OPENVOICE_NODE_EDITOR_MANAGER_HPP
