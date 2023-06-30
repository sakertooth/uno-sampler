#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <list>

class SampleSlice
{
    struct Slice
    {
        Slice(int framePosition);

        std::atomic<int> framePosition = 0;
        std::atomic<float> level = 0;
        std::atomic<float> attack = 0;
        std::atomic<float> release = 0;
        std::atomic<bool> reversed = 0;
    };

    SampleSlice() = default;
    explicit SampleSlice(const juce::AudioSampleBuffer& buffer, int maxNumSlices);

    bool addSlice(const Slice& slice);
    bool appendSlice();

    bool removeSlice(int index);
    void removeInvalidSlices();

    Slice* getSlice(int index);
    int getNumFramesForSlice(int index);
    int getNumFramesInBuffer() const;

    void clearSlices();

    const juce::AudioSampleBuffer& getSample() const;
    const std::list<Slice>& getSlices() const;
    int getMaxNumSlices() const;

    void setSample(const juce::AudioSampleBuffer& buffer);
private:
	juce::AudioSampleBuffer m_sample;
    std::list<Slice> m_slices;
    int m_maxNumSlices;
};