
add_rules("mode.debug", "mode.release")

-- Packages required for the project
add_requires("imgui", {configs = {glfw_opengl3 = true}})
add_requires("opengl", "glfw")

target("openvoice")

-- This file is only for testing purposes
   after_build(function (target)
        os.cp("audio.wav", target:targetdir())
    end)

-- Adding the required packages to target
    add_packages("imgui", "glfw", "opengl")

-- Adding files to compilation
    add_includedirs("thirdparty", {public = true})
    add_includedirs("src")
    add_files("thirdparty/miniaudio.c")
    add_files("src/**.cpp")

    set_kind("binary")


