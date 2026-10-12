

#ifndef OPENVOICE_APP_H
#define OPENVOICE_APP_H

#include "engine.hpp"
#include "device_manager.hpp"
#include "node_editor_manager.hpp"


#include <QtWidgets/QtWidgets>

class App
{
public:
    
    App();
    ~App();

    engine audio_engine;
    node_editor_man n_manager;

    device_manager devices;

    void run();

private:
    QWidget window;
    // variables :
    const char* glslVersion = nullptr;


    bool nodeEditorOpen = false;
    bool showAnotherWindow = false;
    void renderHomePage();
    void renderNodeEditor();

};

#endif // OPENVOICE_APP_H