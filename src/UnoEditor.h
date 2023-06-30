#pragma once

#include <juce_audio_utils/juce_audio_utils.h>

#include "UnoProcessor.h"
#include "gui/KnobWithLabel.h"

class UnoEditor : public juce::AudioProcessorEditor
{
public:
	explicit UnoEditor(UnoProcessor&);

	void paint(juce::Graphics& g) override;
	void resized() override;

	void positionSliceButtons();
	void positionSliceSettings();
	void positionTopBar();

	void selectSample();
	std::array<juce::Rectangle<int>, 4> getScreenRegions() const;

private:
	UnoProcessor& m_processorRef;
	std::array<juce::TextButton, 32> m_sliceButtons;
	std::unique_ptr<juce::AudioThumbnail> m_sampleWaveform;
	std::unique_ptr<juce::AudioThumbnailCache> m_sampleWaveformCache;
	KnobWithLabel m_sliceLevel = KnobWithLabel("LEVEL");
	KnobWithLabel m_sliceAttack = KnobWithLabel("ATTACK");
	KnobWithLabel m_sliceRelease = KnobWithLabel("RELEASE");
	juce::TextButton m_playButton = juce::TextButton("PLAY");
	juce::TextButton m_loadSampleButton = juce::TextButton("LOAD");
	juce::Label m_sampleNameLabel = juce::Label("", "No sample loaded");
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UnoEditor)
};