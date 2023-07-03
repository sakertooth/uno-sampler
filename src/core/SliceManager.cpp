#include "SliceManager.h"

#include <algorithm>
#include <iterator>

SliceManager::SliceManager(juce::AudioProcessorValueTreeState& state)
	: m_treeState(&state)
{
}

std::unique_ptr<juce::AudioProcessorParameterGroup> SliceManager::addSliceParametersToValueTree(int index)
{
	const auto groupID = getSliceGroupID(index);
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

auto SliceManager::getSlicePosition(int index) -> int
{
	return static_cast<int>(
		m_treeState->getRawParameterValue(getSliceParameterID(index, "position"))->load(std::memory_order_relaxed));
}

auto SliceManager::getSliceLevel(int index) -> float
{
	return static_cast<float>(
		m_treeState->getRawParameterValue(getSliceParameterID(index, "level"))->load(std::memory_order_relaxed));
}

auto SliceManager::getSliceAttack(int index) -> float
{
	return static_cast<float>(
		m_treeState->getRawParameterValue(getSliceParameterID(index, "attack"))->load(std::memory_order_relaxed));
}

auto SliceManager::getSliceRelease(int index) -> float
{
	return static_cast<float>(
		m_treeState->getRawParameterValue(getSliceParameterID(index, "release"))->load(std::memory_order_relaxed));
}

auto SliceManager::getSliceReversed(int index) -> bool
{
	return static_cast<bool>(
		m_treeState->getRawParameterValue(getSliceParameterID(index, "reversed"))->load(std::memory_order_relaxed));
}

auto SliceManager::getSliceEnabled(int index) -> bool
{
	return static_cast<bool>(
		m_treeState->getRawParameterValue(getSliceParameterID(index, "enabled"))->load(std::memory_order_relaxed));
}

auto SliceManager::setSlicePosition(int index, int position) -> void
{
	m_treeState->getRawParameterValue(getSliceParameterID(index, "position"))
		->store(static_cast<float>(position), std::memory_order_relaxed);
}

auto SliceManager::setSliceLevel(int index, float level) -> void
{
	m_treeState->getRawParameterValue(getSliceParameterID(index, "level"))
		->store(static_cast<float>(level), std::memory_order_relaxed);
}

auto SliceManager::setSliceAttack(int index, float attack) -> void
{
	m_treeState->getRawParameterValue(getSliceParameterID(index, "attack"))
		->store(static_cast<float>(attack), std::memory_order_relaxed);
}

auto SliceManager::setSliceRelease(int index, float release) -> void
{
	m_treeState->getRawParameterValue(getSliceParameterID(index, "release"))
		->store(static_cast<float>(release), std::memory_order_relaxed);
}

auto SliceManager::setSliceReversed(int index, bool reversed) -> void
{
	m_treeState->getRawParameterValue(getSliceParameterID(index, "reversed"))
		->store(static_cast<float>(reversed), std::memory_order_relaxed);
}

auto SliceManager::setSliceEnabled(int index, bool enabled) -> void
{
	m_treeState->getRawParameterValue(getSliceParameterID(index, "enabled"))
		->store(static_cast<float>(enabled), std::memory_order_relaxed);
}

auto SliceManager::resetSlice(int index) -> void
{
	setSlicePosition(index, 0);
	setSliceLevel(index, 0);
	setSliceAttack(index, 0);
	setSliceRelease(index, 0);
	setSliceReversed(index, false);
	setSliceEnabled(index, false);
}

auto SliceManager::getSliceGroupID(int index) -> std::string
{
	return "Slice" + std::to_string(index);
}

auto SliceManager::getSliceParameterID(int index, const std::string& parameter) -> std::string
{
	return getSliceGroupID(index) + "/" + parameter;
}

auto SliceManager::appendSlice(const juce::AudioSampleBuffer& buffer) -> std::optional<int>
{
	auto numEnabledSlices = countEnabledSlices();
	if (numEnabledSlices == SliceManager::MAX_NUM_SLICES) { return std::nullopt; }

	const auto numSectionsAfterSlice = 2 + numEnabledSlices;
	const auto numSamplesPerSection = buffer.getNumSamples() / numSectionsAfterSlice;

	setSliceEnabled(numEnabledSlices, true);
	for (int i = 0; i < numEnabledSlices + 1; ++i)
	{
		setSlicePosition(i, (i + 1) * numSamplesPerSection);
	}

	return numEnabledSlices;
}

auto SliceManager::addSlice(int sliceNumber, int positionInBuffer) -> void
{
	setSliceEnabled(sliceNumber, true);
	setSlicePosition(sliceNumber, positionInBuffer);
}

auto SliceManager::removeSlice(int index) -> void
{
	setSliceEnabled(index, false);
}

auto SliceManager::countEnabledSlices() -> int
{
	auto count = 0;
	for (int i = 0; i < MAX_NUM_SLICES; ++i)
	{
		if (getSliceEnabled(i)) { ++count; }
	}
	return count;
}