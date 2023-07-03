#pragma once
#include <juce_audio_basics/juce_audio_basics.h>
#include <juce_audio_utils/juce_audio_utils.h>
#include <list>

class SlicePlayer;

class SliceManager
{
public:
	static constexpr auto MAX_NUM_SLICES = 32;
	static constexpr auto NUM_PARAMETERS_PER_SLICE = 6;

	explicit SliceManager(juce::AudioProcessorValueTreeState& state);

	auto addSliceParametersToValueTree(int index) -> std::unique_ptr<juce::AudioProcessorParameterGroup>;
	auto appendSlice(const juce::AudioSampleBuffer& buffer) -> std::optional<int>;
	auto addSlice(int sliceNumber, int positionInBuffer) -> void;
	auto removeSlice(int index) -> void;
	auto countEnabledSlices() -> int;
	auto resetSlice(int index) -> void;

	auto getSlicePosition(int index) -> int;
	auto getSliceLevel(int index) -> float;
	auto getSliceAttack(int index) -> float;
	auto getSliceRelease(int index) -> float;
	auto getSliceReversed(int index) -> bool;
	auto getSliceEnabled(int index) -> bool;

	auto setSlicePosition(int index, int position) -> void;
	auto setSliceLevel(int index, float level) -> void;
	auto setSliceAttack(int index, float attack) -> void;
	auto setSliceRelease(int index, float release) -> void;
	auto setSliceReversed(int index, bool reversed) -> void;
	auto setSliceEnabled(int index, bool enabled) -> void;

	auto getSliceGroupID(int index) -> std::string;
	auto getSliceParameterID(int index, const std::string& parameter) -> std::string;

private:
	juce::AudioProcessorValueTreeState* m_treeState;
};