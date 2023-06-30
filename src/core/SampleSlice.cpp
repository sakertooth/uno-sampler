#include "SampleSlice.h"

#include <algorithm>
#include <iterator>

SampleSlice::SampleSlice(const juce::AudioSampleBuffer& buffer, int maxNumSlices)
	: m_sample(buffer)
	, m_maxNumSlices(maxNumSlices)
{
}

bool SampleSlice::addSlice(const Slice& slice)
{
	if (m_slices.size() == static_cast<size_t>(m_maxNumSlices)) { return false; }
	m_slices.push_back(slice);
	return true;
}

bool SampleSlice::appendSlice()
{
	;
	const auto newSliceIndex = static_cast<int>(m_slices.size());
	auto newSlicePosition = newSliceIndex * getNumFramesInBuffer() / (newSliceIndex + 1);
	return addSlice(Slice{newSlicePosition});
}

SampleSlice::Slice* SampleSlice::getSlice(int index)
{
	if (static_cast<size_t>(index) > m_slices.size() - 1 || index < 0) { return nullptr; }
	auto it = m_slices.begin();
	std::advance(it, index);
	return &*it;
}

int SampleSlice::getNumFramesForSlice(int index)
{
	auto slice = getSlice(index);
	if (slice == nullptr) { return -1; }

	if (static_cast<size_t>(index) == m_slices.size() - 1) { return getNumFramesInBuffer() - slice->framePosition; }

	auto sliceIt = m_slices.begin();
	std::advance(sliceIt, index);
	auto nextSliceIt = std::next(sliceIt, 1);
	return nextSliceIt->framePosition - sliceIt->framePosition;
}

int SampleSlice::getNumFramesInBuffer() const
{
	return m_sample.getNumSamples() / m_sample.getNumChannels();
}

bool SampleSlice::removeSlice(int index)
{
    if (static_cast<size_t>(index) > m_slices.size() - 1 || index < 0) { return false; }
	auto it = m_slices.begin();
	std::advance(it, index);
	m_slices.erase(it);
}

void SampleSlice::clearSlices()
{
	m_slices.clear();
}

void SampleSlice::removeInvalidSlices()
{
	m_slices.remove_if([this](const Slice& slice) {
		return slice.framePosition > m_sample.getNumSamples() / m_sample.getNumChannels() || slice.framePosition < 0;
	});
}

const juce::AudioSampleBuffer& SampleSlice::getSample() const
{
	return m_sample;
}

void SampleSlice::setSample(const juce::AudioSampleBuffer& sample)
{
	m_sample = sample;
	clearSlices();
}