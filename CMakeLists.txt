cmake_minimum_required(VERSION 3.0.0)
project(uno-sampler VERSION 0.1.0)

add_subdirectory(lib/JUCE)
juce_add_plugin(UnoSampler PRODUCT_NAME "Uno" FORMATS AU VST3 Standalone)

target_sources(UnoSampler
    PRIVATE
        src/UnoProcessor.cpp
        src/UnoEditor.cpp
        src/gui/BeatPad.cpp
        src/gui/KnobWithLabel.cpp
        src/gui/SliceWaveform.cpp
        src/core/SlicePlayer.cpp
        src/core/SliceManager.cpp)

target_compile_definitions(UnoSampler
    PUBLIC
        JUCE_WEB_BROWSER=0
        JUCE_USE_CURL=0
        JUCE_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(UnoSampler
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)