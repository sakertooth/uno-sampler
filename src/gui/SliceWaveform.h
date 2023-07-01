#pragma once
#include <juce_audio_utils/juce_audio_utils.h>
#include <juce_gui_basics/juce_gui_basics.h>
#include <memory>

class SliceWaveform : public juce::Component
{
public:
    enum class PlayState
    {
        Stopped,
        Starting,
        Playing,
        Stopping
    };

    SliceWaveform(juce::AudioFormatManager& formatManager);
    
    void paint(juce::Graphics& g) override;
    void thumbnailChanged();

    juce::AudioThumbnail& thumbnail();

private:
	juce::AudioThumbnailCache m_thumbnailCache;
	juce::AudioThumbnail m_thumbnail;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SliceWaveform)
};