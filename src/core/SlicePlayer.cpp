#include "SlicePlayer.h"

#include <cassert>

#include "SliceManager.h"
#include "SlicePlayer.h"

auto SlicePlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate) -> void
{
	m_transportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

auto SlicePlayer::releaseResources() -> void
{
	m_transportSource.releaseResources();
}

auto SlicePlayer::isPlaying() -> bool
{
	return m_transportSource.isPlaying();
}

auto SlicePlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) -> void
{
	if (m_audioSource == nullptr)
	{
		bufferToFill.clearActiveBufferRegion();
		return;
	}

	m_transportSource.getNextAudioBlock(bufferToFill);
}

auto SlicePlayer::getNumSamplesForSlice(SliceManager& sliceManager, int index) -> int
{
	const auto slicePosition = sliceManager.getSlicePosition(index);
	const auto numSamplesInBuffer = m_sampleBuffer->getNumSamples();
	return index == sliceManager.countEnabledSlices() - 1 ? numSamplesInBuffer - slicePosition
														  : sliceManager.getSlicePosition(index + 1) - slicePosition;
}

auto SlicePlayer::setSliceToPlay(SliceManager& sliceManager, int index) -> void
{
	const auto startPosition = static_cast<float>(sliceManager.getSlicePosition(index));
	const auto endPosition
		= static_cast<float>(startPosition + static_cast<float>(getNumSamplesForSlice(sliceManager, index)));
	m_positionRange = {startPosition / m_sampleRate, endPosition / m_sampleRate};
	m_transportSource.setPosition(m_positionRange.getStart());
	m_transportSource.setGain(juce::Decibels::decibelsToGain(sliceManager.getSliceLevel(index)));
}

auto SlicePlayer::getSampleBuffer() const -> const juce::AudioSampleBuffer&
{
	return *m_sampleBuffer;
}

auto SlicePlayer::getPlaybackPosition() const -> double
{
	return m_transportSource.getCurrentPosition();
}

auto SlicePlayer::getSampleRate() const -> double
{
	return m_sampleRate;
}

auto SlicePlayer::setSampleBuffer(std::unique_ptr<juce::AudioSampleBuffer>&& sampleBuffer, double sampleRate) -> void
{
	if (m_audioSource.get() != nullptr) { m_transportSource.setSource(nullptr); }
	m_sampleBuffer = std::move(sampleBuffer);
	m_audioSource = std::make_unique<juce::MemoryAudioSource>(*m_sampleBuffer, false);
	m_transportSource.setSource(m_audioSource.get(), 0, nullptr, sampleRate, m_sampleBuffer->getNumChannels());
	m_sampleRate = sampleRate;
}

auto SlicePlayer::setPlaybackPosition(double position) -> void
{
	m_transportSource.setPosition(position);
}

auto SlicePlayer::start() -> void
{
	m_transportSource.start();
}

auto SlicePlayer::stop() -> void
{
	m_transportSource.stop();
}