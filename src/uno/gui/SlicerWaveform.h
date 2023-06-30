#pragma once
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>

class SlicerWaveform : public juce::Component
{
public:
    SlicerWaveform();
    void paint(juce::Graphics& g) override;
    void resized() override;
private:
	std::unique_ptr<juce::AudioThumbnail> m_waveform;
	std::unique_ptr<juce::AudioThumbnailCache> m_waveformCache;
};