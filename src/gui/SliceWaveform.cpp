#include "SliceWaveform.h"

SliceWaveform::SliceWaveform(juce::AudioFormatManager& formatManager, SlicePlayer* player, SliceManager* manager)
	: m_thumbnailCache(5)
	, m_thumbnail(512, formatManager, m_thumbnailCache)
	, m_slicePlayer(player)
	, m_sliceManager(manager)
{
	for (auto& marker : m_waveformMarkers)
	{
		marker = std::make_unique<Marker>(m_slicePlayer);
		addChildComponent(*marker);
	}
}

void SliceWaveform::paint(juce::Graphics& g)
{
	g.setColour(juce::Colour{61, 60, 58});
	g.fillRect(getLocalBounds());

	g.setColour(juce::Colours::black);
	g.drawRoundedRectangle(getLocalBounds().toFloat(), 2.0f, 3.0f);

	if (m_thumbnail.getNumChannels() == 0)
	{
		g.setColour(juce::Colours::white);
		g.setFont(juce::Font{36.0f, juce::Font::bold});
		g.drawFittedText("No sample loaded", getLocalBounds(), juce::Justification::centred, 1);
	}
	else
	{
		g.setColour(juce::Colours::cornflowerblue);
		const auto playbackPosition = m_slicePlayer->getPlaybackPosition();
		m_thumbnail.drawChannel(g, getLocalBounds(), playbackPosition - durationViewRange,
			playbackPosition + durationViewRange, 1, m_zoomFactor);

		const auto area = getLocalBounds();
		g.setColour(juce::Colours::whitesmoke);
		g.fillRect(area.getCentreX(), 0, 2, area.getHeight());

		for (auto& marker : m_waveformMarkers)
		{
			if (marker->isVisible()) { marker->paint(g); }
		}

		repaint();
	}
}

auto SliceWaveform::setMarker(int index, double positionInPlayback) -> void
{
	m_waveformMarkers[static_cast<size_t>(index)]->setPositionInPlayback(positionInPlayback);
	m_waveformMarkers[static_cast<size_t>(index)]->setBounds(0, 0, 2, getHeight());
	m_waveformMarkers[static_cast<size_t>(index)]->setVisible(true);
	m_waveformMarkers[static_cast<size_t>(index)]->repaint();
	updateMarkerPositions();
}

auto SliceWaveform::removeMarker(int index) -> void
{
	m_waveformMarkers[static_cast<size_t>(index)]->setVisible(false);
	updateMarkerPositions();
}

auto SliceWaveform::updateMarkerPositions() -> void
{
	for (int i = 0; i < static_cast<int>(m_waveformMarkers.size()); ++i)
	{
		m_waveformMarkers[static_cast<size_t>(i)]->setPositionInPlayback(
			static_cast<float>(m_sliceManager->getSlicePosition(i)) / m_slicePlayer->getSampleRate());
	}
}

void SliceWaveform::thumbnailChanged()
{
	repaint();
}

juce::AudioThumbnail& SliceWaveform::thumbnail()
{
	return m_thumbnail;
}

SliceWaveform::Marker::Marker(SlicePlayer* player)
	: m_slicePlayer(player)
{
}

auto SliceWaveform::Marker::paint(juce::Graphics& g) -> void
{
	const auto playbackPosition = m_slicePlayer->getPlaybackPosition();
	const auto playbackViewRange
		= juce::Range<double>{playbackPosition - durationViewRange, playbackPosition + durationViewRange};

	if (playbackViewRange.contains(m_positionInPlayback))
	{
		const auto waveformArea = getParentComponent()->getLocalBounds();
		const auto rangeLength = playbackViewRange.getLength();
		const auto distanceFromStart = m_positionInPlayback - playbackViewRange.getStart();
		const auto relativeX = waveformArea.getWidth() * distanceFromStart / rangeLength;
		g.setColour(juce::Colours::lightgreen);
		g.fillRect(static_cast<float>(relativeX), 0.0f, 2.0f, static_cast<float>(waveformArea.getHeight()));
	}
}

auto SliceWaveform::Marker::getPositionInPlayback() const -> const double
{
	return m_positionInPlayback;
}

auto SliceWaveform::Marker::setPositionInPlayback(double positionInPlayback) -> void
{
	m_positionInPlayback = positionInPlayback;
}