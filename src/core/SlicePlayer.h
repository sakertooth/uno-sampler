#pragma once

#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_devices/juce_audio_devices.h>
#include <mutex>
#include <shared_mutex>

class SliceManager;

class SlicePlayer : public juce::AudioSource
{
public:
	auto start() -> void;
	auto stop() -> void;
	auto prepareToPlay(int samplesPerBlockExpected, double sampleRate) -> void override;
	auto releaseResources() -> void override;
	auto isPlaying() -> bool;

	auto getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) -> void override;
	auto getAudioSource() -> juce::MemoryAudioSource*;
	auto getSampleBuffer() const -> const juce::AudioSampleBuffer&;
	auto getNumSamplesForSlice(SliceManager& sliceManager, int index) -> int;

	auto setSampleBuffer(std::unique_ptr<juce::AudioSampleBuffer>&& sampleBuffer, double sampleRate) -> void;
	auto setSliceToPlay(SliceManager& sliceManager, int index) -> void;

private:
	double m_sampleRate = 0;
	std::unique_ptr<juce::AudioSampleBuffer> m_sampleBuffer;
	std::unique_ptr<juce::MemoryAudioSource> m_audioSource;
	juce::Range<double> m_positionRange;
	juce::AudioTransportSource m_transportSource;
};