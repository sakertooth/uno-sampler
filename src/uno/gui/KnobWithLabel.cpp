#include "KnobWithLabel.h"

KnobWithLabel::KnobWithLabel(const juce::String& labelText) :
    m_label(labelText, labelText)
{
	m_dial.setSliderStyle(juce::Slider::Rotary);
	m_dial.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);

	m_label.setColour(juce::Label::ColourIds::textColourId, juce::Colours::white);
	m_label.setFont(juce::Font(16.0f, juce::Font::bold));
	m_label.setJustificationType(juce::Justification::centredTop);

	addAndMakeVisible(m_dial);
	addAndMakeVisible(m_label);
}

void KnobWithLabel::resized()
{
	m_dial.setBoundsRelative(0, 0, 1, 0.8f);
	m_label.setBoundsRelative(0, 0.8f, 1, 0.2f);
}

juce::Slider& KnobWithLabel::slider()
{
	return m_dial;
}

juce::Label& KnobWithLabel::label()
{
	return m_label;
}