#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <list>

class SampleSlice
{
    class Slice
    {
    public:
        auto getFramePosition() -> int;
        auto getLevel() -> float;
        auto getAttack() -> float;
        auto getRelease() -> float;
        auto getReversed() -> bool;

        auto setFramePosition(int framePosition) -> void;
        auto setLevel(float level) -> void;
        auto setAttack(float attack) -> void;
        auto setRelease(float release) -> void;
        auto setReversed(bool reversed) -> void;
        auto setEnabled(bool enabled) -> void;

    private:
        std::atomic<int> m_framePosition = 0;
        std::atomic<float> m_level = 0;
        std::atomic<float> m_attack = 0;
        std::atomic<float> m_release = 0;
        std::atomic<bool> m_reversed = 0;
        std::atomic<bool> m_enabled = 0;
    };

    static constexpr auto MAX_NUM_SLICES = 32;

    SampleSlice() = default;
    explicit SampleSlice(const juce::AudioSampleBuffer& buffer);

    auto getSlice(size_t index) -> Slice*;
    auto disableAllSlices() -> void;
    auto getNumFramesForSlice(size_t index) -> int;
    auto getNumFramesInBuffer() const -> int;
    auto getSample() const -> const juce::AudioSampleBuffer&;
    auto setSample(const juce::AudioSampleBuffer& sample) -> void;
private:
	juce::AudioSampleBuffer m_sample;
    std::array<Slice, MAX_NUM_SLICES> m_slices;
};