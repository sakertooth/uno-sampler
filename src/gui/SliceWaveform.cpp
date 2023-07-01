#include "SliceWaveform.h"

SliceWaveform::SliceWaveform(juce::AudioFormatManager& formatManager)
	: m_thumbnailCache(5)
	, m_thumbnail(512, formatManager, m_thumbnailCache)
{
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
		g.setColour(juce::Colours::white);
		m_thumbnail.drawChannel(g, getLocalBounds(), 0, m_thumbnail.getTotalLength() / 20, 1, 5.0f);
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