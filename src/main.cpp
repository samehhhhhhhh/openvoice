
#include "audio_engine/engine.hpp"
#include "shared/includes.hpp"
#include "ui/app.hpp"

int main()
{
    App my_app;
    my_app.run();
    engine my_engine;
    std::string filename = "audio.wav";
    my_engine.play(filename);

}