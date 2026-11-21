
add_rules("mode.debug", "mode.release")
set_defaultmode("debug")

-- Packages required for the project
add_requires("imgui", {configs = {sdl3 = true, sdl3_renderer = true}})
add_requires("libsdl3")

includes("@builtin/xpack")



target("miniaudio")
    add_includedirs("vendor/miniaudio", {public = true})

    add_files("vendor/miniaudio/*.c")

    set_kind("static")

target("audio_engine")
    add_includedirs("includes", {public = true})
    add_files("src/audio_engine/*.cpp")
    set_kind("static")
    add_includedirs("vendor/voclib")

    add_deps("miniaudio")

target("ui")

    -- Adding the required packages to target
    add_packages("imgui", "libsdl3")

    add_includedirs("vendor/imgui-node-editor", {public = true})
    add_files("vendor/imgui-node-editor/**.cpp")

    add_includedirs("includes", {public = true})
    add_files("src/ui/*.cpp")
    set_kind("static")
    add_packages("imgui", {public = true})
    add_deps("audio_engine")


target("openvoice")

-- This file is only for testing purposes
   after_build(function (target)
        os.cp("audio.wav", target:targetdir())
    end)

-- Adding files to compilation
    add_files("src/main.cpp")

    add_deps("ui")
    set_kind("binary")

    if is_plat("mingw") then
    add_ldflags("-static", "-static-libgcc", "-static-libstdc++", {force = true})

    end



xpack("openvoice pack")
    set_formats("nsis", "zip", "targz")
    set_title("Openvoice")
    set_homepage("https://github.com/samehhhhhhhh/openvoice")
    set_author("Samehh.")
    add_targets("openvoice")
