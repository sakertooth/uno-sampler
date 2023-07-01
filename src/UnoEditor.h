#pragma once

#include <juce_audio_utils/juce_audio_utils.h>

#include "UnoProcessor.h"
#include "gui/KnobWithLabel.h"
#include "gui/SliceWaveform.h"

class UnoEditor : public juce::AudioProcessorEditor, private juce::ChangeListener
{
public:
	enum class TransportState
	{
		Stopped,
		Starting,
		Playing,
		Stopping
	};

	explicit UnoEditor(UnoProcessor&);

	void paint(juce::Graphics& g) override;
	void resized() override;

	void positionSliceButtons();
	void positionSliceSettings();
	void positionWaveform();
	void positionTopBar();

	void selectSample();
	void changeListenerCallback(juce::ChangeBroadcaster* source) override;
	void changeState(UnoEditor::TransportState newState);

	std::array<juce::Rectangle<int>, 4> getScreenRegions() const;

private:
	UnoProcessor& m_processorRef;

	std::array<juce::TextButton, 32> m_sliceButtons;
	SliceWaveform m_sliceWaveform;

	KnobWithLabel m_sliceLevel = KnobWithLabel("LEVEL");
	KnobWithLabel m_sliceAttack = KnobWithLabel("ATTACK");
	KnobWithLabel m_sliceRelease = KnobWithLabel("RELEASE");

	juce::TextButton m_playButton = juce::TextButton("PLAY");
	juce::TextButton m_loadSampleButton = juce::TextButton("LOAD");
	juce::Label m_sampleNameLabel = juce::Label("", "No sample loaded");

	juce::AudioFormatManager m_formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> m_formatReaderSource;
	juce::AudioTransportSource m_transportSource;
	TransportState m_transportState;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UnoEditor)
};