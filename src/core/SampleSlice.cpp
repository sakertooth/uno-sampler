#include "SampleSlice.h"

#include <algorithm>
#include <iterator>

SampleSlice::SampleSlice(const juce::AudioSampleBuffer& buffer)
	: m_sample(buffer)
{
}

auto SampleSlice::getNumFramesForSlice(juce::AudioProcessorValueTreeState& state, int index) -> int
{
	if (index < 0 || index > MAX_NUM_SLICES - 1)
	{
		throw std::out_of_range("SampleSlice::getNumFramesForSlice, out of range");
	}

	if (index == MAX_NUM_SLICES - 1)
	{
		const auto slicePosition = getSlicePosition(state, index);
		return m_sample.getNumSamples() / m_sample.getNumChannels() - static_cast<int>(slicePosition);
	}

	const auto slicePosition = getSlicePosition(state, index);
	const auto nextSlicePosition = getSlicePosition(state, index + 1);
	return static_cast<int>(nextSlicePosition - slicePosition);
}

std::unique_ptr<juce::AudioProcessorParameterGroup> SampleSlice::addSliceParametersToValueTree(int index)
{
	const auto groupID = "Slice" + std::to_string(index);

	auto sliceParameterGroup = std::make_unique<juce::AudioProcessorParameterGroup>(groupID, groupID, "/");
	sliceParameterGroup->addChild(std::make_unique<juce::AudioParameterInt>(
		getSliceParameterID(index, "position"), "Position", 0, std::numeric_limits<int>::max(), 0));
	sliceParameterGroup->addChild(
		std::make_unique<juce::AudioParameterFloat>(getSliceParameterID(index, "level"), "Level", -12.0f, 12.0f, 0.0f));
	sliceParameterGroup->addChild(std::make_unique<juce::AudioParameterFloat>(
		getSliceParameterID(index, "attack"), "Attack", 0.0f, 10000.0f, 0.0f));
	sliceParameterGroup->addChild(std::make_unique<juce::AudioParameterFloat>(
		getSliceParameterID(index, "release"), "Release", 0.0f, 10000.0f, 0.0f));
	sliceParameterGroup->addChild(
		std::make_unique<juce::AudioParameterBool>(getSliceParameterID(index, "reversed"), "Reversed", false));
	sliceParameterGroup->addChild(
		std::make_unique<juce::AudioParameterBool>(getSliceParameterID(index, "enabled"), "Enabled", false));

	return sliceParameterGroup;
}

auto SampleSlice::getSlicePosition(juce::AudioProcessorValueTreeState& state, int index) -> int
{
	return static_cast<int>(state.getRawParameterValue(getSliceParameterID(index, "position"))->load(std::memory_order_relaxed));
}

auto SampleSlice::getSliceLevel(juce::AudioProcessorValueTreeState& state, int index) -> float
{
	return static_cast<float>(state.getRawParameterValue(getSliceParameterID(index, "level"))->load(std::memory_order_relaxed));
}

auto SampleSlice::getSliceAttack(juce::AudioProcessorValueTreeState& state, int index) -> float
{
	return static_cast<float>(
		state.getRawParameterValue(getSliceParameterID(index, "attack"))->load(std::memory_order_relaxed));
}

auto SampleSlice::getSliceRelease(juce::AudioProcessorValueTreeState& state, int index) -> float
{
	return static_cast<float>(
		state.getRawParameterValue(getSliceParameterID(index, "release"))->load(std::memory_order_relaxed));
}

auto SampleSlice::getSliceReversed(juce::AudioProcessorValueTreeState& state, int index) -> bool
{
	return static_cast<bool>(
		state.getRawParameterValue(getSliceParameterID(index, "reversed"))->load(std::memory_order_relaxed));
}

auto SampleSlice::getSliceEnabled(juce::AudioProcessorValueTreeState& state, int index) -> bool
{
	return static_cast<bool>(
		state.getRawParameterValue(getSliceParameterID(index, "enabled"))->load(std::memory_order_relaxed));
}

auto SampleSlice::setSlicePosition(juce::AudioProcessorValueTreeState& state, int index, int position) -> void
{
	state.getRawParameterValue(getSliceParameterID(index, "position"))
		->store(static_cast<float>(position), std::memory_order_relaxed);
}

auto SampleSlice::setSliceLevel(juce::AudioProcessorValueTreeState& state, int index, float level) -> void
{
	state.getRawParameterValue(getSliceParameterID(index, "level"))
		->store(static_cast<float>(level), std::memory_order_relaxed);
}

auto SampleSlice::setSliceAttack(juce::AudioProcessorValueTreeState& state, int index, float attack) -> void
{
	state.getRawParameterValue(getSliceParameterID(index, "attack"))
		->store(static_cast<float>(attack), std::memory_order_relaxed);
}

auto SampleSlice::setSliceRelease(juce::AudioProcessorValueTreeState& state, int index, float release) -> void
{
	state.getRawParameterValue(getSliceParameterID(index, "release"))
		->store(static_cast<float>(release), std::memory_order_relaxed);
}

auto SampleSlice::setSliceReversed(juce::AudioProcessorValueTreeState& state, int index, bool reversed) -> void
{
	state.getRawParameterValue(getSliceParameterID(index, "reversed"))
		->store(static_cast<float>(reversed), std::memory_order_relaxed);
}

auto SampleSlice::setSliceEnabled(juce::AudioProcessorValueTreeState& state, int index, bool enabled) -> void
{
	state.getRawParameterValue(getSliceParameterID(index, "enabled"))
		->store(static_cast<float>(enabled), std::memory_order_relaxed);
}

auto SampleSlice::resetSlice(juce::AudioProcessorValueTreeState& state, int index) -> void
{
	setSlicePosition(state, index, 0);
	setSliceLevel(state, index, 0);
	setSliceAttack(state, index, 0);
	setSliceRelease(state, index, 0);
	setSliceReversed(state, index, false);
	setSliceEnabled(state, index, false);
}

auto SampleSlice::getSliceGroupID(int index) -> std::string
{
	return "Slice" + std::to_string(index);
}

auto SampleSlice::getSliceParameterID(int index, const std::string& parameter) -> std::string
{
	return getSliceGroupID(index) + "/" + parameter;
}