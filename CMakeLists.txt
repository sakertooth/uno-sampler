cmake_minimum_required(VERSION 3.0.0)
project(uno-sampler VERSION 0.1.0)

add_subdirectory(lib/JUCE)
juce_add_plugin(UnoSampler PRODUCT_NAME "Uno Sampler")

target_sources(UnoSampler
    PRIVATE
        src/PluginProcessor.cpp
        src/PluginEditor.cpp)

target_link_libraries(UnoSampler
    PRIVATE
        juce::juce_audio_utils
    PUBLIC
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags)