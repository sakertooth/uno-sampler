#include "SampleSlice.h"

#include <algorithm>
#include <iterator>

SampleSlice::SampleSlice(const juce::AudioSampleBuffer& buffer)
	: m_sample(buffer)
{
}

auto SampleSlice::getSlice(size_t index) -> Slice*
{
	if (index < 0 || index > m_slices.size()) { return nullptr; }
	return &m_slices[index];
}

auto SampleSlice::disableAllSlices() -> void
{
	for (auto& slice : m_slices)
	{
		slice.setEnabled(false);
	}
}

auto SampleSlice::getNumFramesForSlice(size_t index) -> int
{
	if (index < 0 || index >= m_slices.size()) { throw std::out_of_range("SampleSlice::getNumFramesForSlice: Index of out range"); }
	if (index == m_slices.size() - 1) { return getNumFramesInBuffer() - getSlice(index)->getFramePosition(); }
	return getSlice(index + 1)->getFramePosition() - getSlice(index)->getFramePosition();
}

auto SampleSlice::getNumFramesInBuffer() const -> int
{
	return m_sample.getNumSamples() / m_sample.getNumChannels();
}

auto SampleSlice::getSample() const -> const juce::AudioSampleBuffer&
{
	return m_sample;
}

auto SampleSlice::setSample(const juce::AudioSampleBuffer& sample) -> void
{
	m_sample = sample;
}

auto SampleSlice::Slice::getFramePosition() -> int
{
	return m_framePosition.load(std::memory_order_relaxed);
}

auto SampleSlice::Slice::getLevel() -> float
{
	return m_level.load(std::memory_order_relaxed);
}

auto SampleSlice::Slice::getAttack() -> float
{
	return m_attack.load(std::memory_order_relaxed);
}

auto SampleSlice::Slice::getRelease() -> float
{
	return m_release.load(std::memory_order_relaxed);
}

auto SampleSlice::Slice::getReversed() -> bool
{
	return m_reversed.load(std::memory_order_relaxed);
}

auto SampleSlice::Slice::setFramePosition(int framePosition) -> void
{
	m_framePosition.store(framePosition, std::memory_order_relaxed);
}

auto SampleSlice::Slice::setLevel(float level) -> void
{
	m_level.store(level, std::memory_order_relaxed);
}

auto SampleSlice::Slice::setAttack(float attack) -> void
{
	m_attack.store(attack, std::memory_order_relaxed);
}

auto SampleSlice::Slice::setRelease(float release) -> void
{
	return m_release.store(release, std::memory_order_relaxed);
}

auto SampleSlice::Slice::setReversed(bool reversed) -> void
{
	return m_reversed.store(reversed, std::memory_order_relaxed);
}

auto SampleSlice::Slice::setEnabled(bool enabled) -> void
{
	return m_enabled.store(enabled, std::memory_order_relaxed);
}
