#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <list>

class SampleSlice
{
public:
	static constexpr auto MAX_NUM_SLICES = 32;
	static constexpr auto NUM_PARAMETERS_PER_SLICE = 6;

	SampleSlice() = default;
	explicit SampleSlice(const juce::AudioSampleBuffer& buffer);

	auto addSliceParametersToValueTree(int index) -> std::unique_ptr<juce::AudioProcessorParameterGroup>;

	auto getSlicePosition(juce::AudioProcessorValueTreeState& state, int index) -> int;
	auto getSliceLevel(juce::AudioProcessorValueTreeState& state, int index) -> float;
	auto getSliceAttack(juce::AudioProcessorValueTreeState& state, int index) -> float;
	auto getSliceRelease(juce::AudioProcessorValueTreeState& state, int index) -> float;
	auto getSliceReversed(juce::AudioProcessorValueTreeState& state, int index) -> bool;
	auto getSliceEnabled(juce::AudioProcessorValueTreeState& state, int index) -> bool;
	auto getNumFramesForSlice(juce::AudioProcessorValueTreeState& state, int index) -> int;

	auto setSlicePosition(juce::AudioProcessorValueTreeState& state, int index, int position) -> void;
	auto setSliceLevel(juce::AudioProcessorValueTreeState& state, int index, float level) -> void;
	auto setSliceAttack(juce::AudioProcessorValueTreeState& state, int index, float attack) -> void;
	auto setSliceRelease(juce::AudioProcessorValueTreeState& state, int index, float release) -> void;
	auto setSliceReversed(juce::AudioProcessorValueTreeState& state, int index, bool reversed) -> void;
	auto setSliceEnabled(juce::AudioProcessorValueTreeState& state, int index, bool enabled) -> void;

	auto resetSlice(juce::AudioProcessorValueTreeState& state, int index) -> void;
private:
	auto getSliceGroupID(int index) -> std::string;
	auto getSliceParameterID(int index, const std::string& parameter)
		-> std::string;
	juce::AudioSampleBuffer m_sample;
};