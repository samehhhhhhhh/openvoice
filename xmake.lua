
add_rules("mode.debug", "mode.release")

target("miniaudio")
    add_includedirs("vendor/miniaudio", {public = true})

    add_files("vendor/miniaudio/*.c")

    set_kind("shared")

target("audio_engine")
    add_includedirs("src/audio_engine", {public = true})
    add_files("src/audio_engine/*.cpp")
    set_kind("shared")
    add_includedirs("vendor/voclib")

    add_deps("miniaudio")



target("openvoice")

-- New UI stuff

    add_includedirs("src/new_ui", {public = true})
    
    add_files("src/new_ui/*.cpp")
    
    add_deps("audio_engine")

-- This file is only for testing purposes
   after_build(function (target)
        os.cp("audio.wav", target:targetdir())
    end)
   add_files("src/main.cpp")

   add_rules("qt.widgetapp")

    
    set_kind("binary")