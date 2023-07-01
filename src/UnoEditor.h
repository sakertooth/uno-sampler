#pragma once

#include <juce_audio_utils/juce_audio_utils.h>

#include "UnoProcessor.h"
#include "gui/BeatPad.h"
#include "gui/KnobWithLabel.h"
#include "gui/SliceWaveform.h"

class UnoEditor : public juce::AudioProcessorEditor, private juce::ChangeListener
{
public:
	enum class PlayState
	{
		Stopped,
		Playing,
		Paused
	};

	explicit UnoEditor(UnoProcessor&);

	void paint(juce::Graphics& g) override;
	void resized() override;

	void positionSliceButtons();
	void positionSliceSettings();
	void positionWaveform();
	void positionTopBar();

	void selectSample();
	void onPlayButtonClick();

	std::array<juce::Rectangle<int>, 4> getScreenRegions() const;

	void changeListenerCallback(juce::ChangeBroadcaster* source) override;
	void selectPad(BeatPad::Pad* pad);

private:
	UnoProcessor& m_processorRef;

	BeatPad m_beatPad;
	SliceWaveform m_sliceWaveform;

	KnobWithLabel m_sliceLevel = KnobWithLabel("LEVEL");
	KnobWithLabel m_sliceAttack = KnobWithLabel("ATTACK");
	KnobWithLabel m_sliceRelease = KnobWithLabel("RELEASE");

	juce::TextButton m_playButton = juce::TextButton("PLAY");
	juce::TextButton m_loadSampleButton = juce::TextButton("LOAD");
	juce::Label m_sampleNameLabel = juce::Label("", "No sample loaded");
	juce::Label m_selectedPadLabel;

	juce::AudioFormatManager m_formatManager;
	std::unique_ptr<juce::AudioFormatReaderSource> m_formatReaderSource;
	PlayState m_playState = PlayState::Stopped;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UnoEditor)
};