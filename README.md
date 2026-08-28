
# OpenVoice

Description : 

OpenVoice is a free and opensource real time voicechanger for desktops on all platforms. It is made purely in C++.

The goal, make a simple to use app that is linked to a database of voice presets (On github) 
so that anyone can use the voice he wants,

without requiring paid subscriptions like voicemod. 

// Wiki

All voices are represented by a node graph. -> Look for https://github.com/thedmd/imgui-node-editor
There is an input node that represents the audio input, it will then get processed through multiple effects
and then be outputted through an output node. This can be compared to the FL Studio Patcher. 

The voice configuration is then stored in a file that is stored in a folder that contains all voice configuration
installed on the client. Thus, the user can either acquire his own configuration files or use the built in database
(Just a repository containing configurations made by the community)

Roadmap : 

- Show a window that contains a button that streams your microphone to your default output device. -- Done
- Make different windows (Just homepage and node editor at least) -- Done !
- Make an audio engine class. -- Done
- Make a node editor -- Done ! 
- Make sure the audio engine node graph works proprely -> Doing that now
- Make pressing shift + a open a context menu in the node editor to add a node. -> Doing that now
- Make the connection between the node editor and the audio engine. 
- Implement configuration saving.
- Work on the configuration database.

