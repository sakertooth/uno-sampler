#pragma once

#include <juce_audio_utils/juce_audio_utils.h>

#include "PluginProcessor.h"

class AudioPluginAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
	explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor&);

	void paint(juce::Graphics& g) override;
	void paintSampleWaveform(juce::Graphics& g);

	void resized() override;

	void calculateInitialRegions();

	void positionSliceButtons();
	void positionSliceSettings();
	void positionTopBar();

private:
	AudioPluginAudioProcessor& m_processorRef;

	juce::Rectangle<int> m_sliceButtonsRegion;
	std::array<juce::TextButton, 32> m_sliceButtons;

	juce::Rectangle<int> m_sliceSettingsRegion;
	juce::Slider m_sliceAttack;
	juce::Slider m_sliceRelease;
	juce::Slider m_sliceKeyShift;
	juce::Slider m_sliceTimeStretch;
	juce::TextButton m_reverseButton;

	juce::Rectangle<int> m_sampleWaveformRegion;
	std::unique_ptr<juce::AudioThumbnail> m_sampleWaveform;
	std::unique_ptr<juce::AudioThumbnailCache> m_sampleWaveformCache;

	juce::Rectangle<int> m_topBarRegion;
	juce::TextButton m_playButton;
	juce::Label m_sampleNameLabel;
	juce::TextButton m_loadSampleButton;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};