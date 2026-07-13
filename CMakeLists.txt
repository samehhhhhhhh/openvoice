cmake_minimum_required(VERSION 3.10.0)

project(openvoice CXX C)

# --- Dependencies ---
find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

set(IMGUI_DIR ${CMAKE_SOURCE_DIR}/third_party/imgui)

set(IMGUI_SOURCES
        ${IMGUI_DIR}/imgui.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})
target_include_directories(imgui PUBLIC
        ${IMGUI_DIR}
        ${IMGUI_DIR}/backends
)
target_link_libraries(imgui PUBLIC glfw OpenGL::GL)

set(MINIAUDIO_DIR ${CMAKE_SOURCE_DIR}/thirdparty)

# --- Main project sources ---
set(SOURCES
        src/ui/app.cpp
        src/main.cpp
        src/audio_engine/engine.cpp
        thirdparty/miniaudio.c
)

add_executable(openvoice ${SOURCES})

target_include_directories(openvoice PRIVATE
        src/audio_engine
        src/ui
        ${MINIAUDIO_DIR}
)

target_link_libraries(openvoice PRIVATE imgui)