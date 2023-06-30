#pragma once
#include <juce_gui_basics/juce_gui_basics.h>

class KnobWithLabel : public juce::Component
{
public:
    explicit KnobWithLabel(const juce::String& labelText = "");
    void resized() override;
    juce::Slider& slider();
    juce::Label& label();
private:
    juce::Slider m_dial;
    juce::Label m_label;
};