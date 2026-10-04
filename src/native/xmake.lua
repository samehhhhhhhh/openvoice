
add_rules("mode.debug", "mode.release")

-- Packages required for the project
add_requires("imgui", {configs = {glfw_opengl3 = true}})
add_requires("opengl", "glfw")


target("miniaudio")
    add_includedirs("vendor/miniaudio", {public = true})

    add_files("vendor/miniaudio/*.c")

    set_kind("shared")

target("audio_engine")
    add_includedirs("audio_engine", {public = true})
    add_files("audio_engine/*.cpp")
    set_kind("shared")
    add_includedirs("vendor/voclib")

    add_deps("miniaudio")


target("ui")

    -- Adding the required packages to target
    add_packages("imgui", "glfw", "opengl")

    add_includedirs("vendor/imgui-node-editor", {public = true})
    add_files("vendor/imgui-node-editor/**.cpp")

    add_includedirs("old_ui")
    add_files("old_ui/*.cpp")
    set_kind("shared")
    add_packages("imgui", {public = true})
    add_deps("audio_engine")



target("openvoice")

-- This file is only for testing purposes
   after_build(function (target)
        os.cp("../../assets/audio.wav", target:targetdir())
    end)
   add_files("main.cpp")

-- Adding files to compilation
    add_deps("ui")
    set_kind("binary")


