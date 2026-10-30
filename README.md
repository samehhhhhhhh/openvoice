
# OpenVoice

![A node editor](images/phone_voice_setup.gif)

OpenVoice is a free and opensource real time voicechanger for desktops on all platforms. It is made purely in C++.

The goal, make a simple to use app that is linked to a database of voice presets (On github) 
so that anyone can use the voice he wants,

without requiring paid subscriptions like certain paid software. 

## Download

Clone the repository:

```bash
git clone https://github.com/samehhhhhhhh/openvoice.git
cd openvoice
```

## Requirements

- C++ compiler (GCC or MinGW-w64)
- [xmake](https://xmake.io/)
- Git

### Windows

1. Install [MSYS2](https://www.msys2.org/).
2. Open the **MSYS2 UCRT64** terminal.
3. Install GCC, Git and xmake:

```bash
pacman -S --needed mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-git mingw-w64-ucrt-x86_64-xmake
```

### Linux

Install GCC, Git and xmake using your distribution's package manager.

## Build

From the project directory, run:

```bash
xmake
```

To build in Release mode:

```bash
xmake f -m release
xmake
```

## Run

```bash
xmake run openvoice
```
## Usage

You must use VB cable on windows or qpwgraph on linux to route audio to your desired app. 
Select an audio input and output in the homepage and then make your node graph setup 

When changing values on nodes, don't forget to update nodes so the changes are applied. 



