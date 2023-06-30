#include "UnoEditor.h"

UnoEditor::UnoEditor(UnoProcessor& p)
	: AudioProcessorEditor(&p)
	, m_processorRef(p)
{
	juce::ignoreUnused(m_processorRef);

	for (size_t i = 0; i < 32; ++i)
	{
		m_sliceButtons[i].setButtonText(juce::String{i + 1});
		addAndMakeVisible(m_sliceButtons[i]);
	}

	m_sampleNameLabel.setColour(juce::Label::ColourIds::backgroundColourId, juce::Colour{34, 34, 34});
	m_sampleNameLabel.setColour(juce::Label::ColourIds::outlineColourId, juce::Colours::whitesmoke);
	m_sampleNameLabel.setFont(juce::Font{12.0f, juce::Font::bold});
	m_sampleNameLabel.setEditable(false);
	m_sampleNameLabel.setJustificationType(juce::Justification::left);

	addAndMakeVisible(m_sliceLevel);
	addAndMakeVisible(m_sliceAttack);
	addAndMakeVisible(m_sliceRelease);
	addAndMakeVisible(m_sampleNameLabel);
	addAndMakeVisible(m_playButton);
	addAndMakeVisible(m_loadSampleButton);
	setSize(820, 560);
}

void UnoEditor::paint(juce::Graphics& g)
{
	auto [sliceButtonsRegion, sliceSettingsRegion, sampleWaveformRegion, topBarRegion] = getScreenRegions();
	g.setColour(juce::Colour{34, 34, 34});
	g.fillRect(sliceButtonsRegion);

	g.setColour(juce::Colour{51, 51, 51});
	g.fillRect(sliceSettingsRegion);

	g.setColour(juce::Colours::black);
	g.fillRect(sampleWaveformRegion);

	g.setColour(juce::Colour{51, 51, 51});
	g.fillRect(topBarRegion);

	g.setColour(juce::Colours::black);
	if (m_sampleWaveform.get() == nullptr || m_sampleWaveform->getNumChannels() == 0)
	{
		g.fillRect(sampleWaveformRegion);
		g.setColour(juce::Colours::white);
		g.drawFittedText("No File Loaded", sampleWaveformRegion, juce::Justification::centred, 1);
	}
	else
	{
		g.fillRect(sampleWaveformRegion);
		g.setColour(juce::Colours::darkblue);
		m_sampleWaveform->drawChannels(g, sampleWaveformRegion, 0.0, m_sampleWaveform->getTotalLength(), 1.0f);
	}
}

void UnoEditor::resized()
{
	positionSliceButtons();
	positionSliceSettings();
	positionTopBar();
}

void UnoEditor::positionSliceButtons()
{
	auto [sliceButtonsRegion, sliceSettingsRegion, sampleWaveformRegion, topBarRegion] = getScreenRegions();
	const auto numRows = 2;
	const auto regionMargin = 10;
	const auto sliceButtonSize = std::array<int, 2>{40, 32};
	const auto numButtonsPerRow = static_cast<int>(m_sliceButtons.size() / numRows);
	const auto rowOne = sliceButtonsRegion.removeFromTop(sliceButtonsRegion.getHeight() / 2);
	const auto rowTwo = sliceButtonsRegion.removeFromTop(sliceButtonsRegion.getHeight() / 2);

	for (auto row = 0; row < numRows; ++row)
	{
		auto rowRegion = row == 0 ? rowOne : rowTwo;
		auto spacing = rowRegion.getWidth() / numButtonsPerRow;
		for (auto button = 0; button < numButtonsPerRow; ++button)
		{
			auto buttonRect = rowRegion.removeFromLeft(spacing).reduced(regionMargin);
			buttonRect.setSize(sliceButtonSize[0], sliceButtonSize[1]);
			m_sliceButtons[static_cast<size_t>(row * numButtonsPerRow + button)].setBounds(buttonRect);
		}
	}
}

void UnoEditor::positionSliceSettings()
{
	auto [sliceButtonsRegion, sliceSettingsRegion, sampleWaveformRegion, topBarRegion] = getScreenRegions();
	const auto width = sliceSettingsRegion.getWidth();
	const auto numControlsGroup1 = 3;
	const auto spacingGroup1 = width / 2 / numControlsGroup1;
	const auto margin = 2;

	m_sliceLevel.setBounds(sliceSettingsRegion.removeFromLeft(spacingGroup1).reduced(margin));
	m_sliceAttack.setBounds(sliceSettingsRegion.removeFromLeft(spacingGroup1).reduced(margin));
	m_sliceRelease.setBounds(sliceSettingsRegion.removeFromLeft(spacingGroup1).reduced(margin));
}

void UnoEditor::positionTopBar()
{
	auto [sliceButtonsRegion, sliceSettingsRegion, sampleWaveformRegion, topBarRegion] = getScreenRegions();
	const auto margin = 10;

	m_playButton.setSize(topBarRegion.proportionOfWidth(0.2f), topBarRegion.getHeight() - margin);
	m_playButton.setBounds(topBarRegion.removeFromLeft(m_playButton.getWidth() + margin).reduced(margin));

	m_sampleNameLabel.setSize(topBarRegion.proportionOfWidth(0.4f), topBarRegion.getHeight() - margin);
	m_sampleNameLabel.setBounds(topBarRegion.removeFromLeft(m_sampleNameLabel.getWidth() + margin).reduced(margin));

	m_loadSampleButton.setSize(topBarRegion.proportionOfWidth(0.4f), topBarRegion.getHeight() - margin);
	m_loadSampleButton.setBounds(topBarRegion.removeFromLeft(m_loadSampleButton.getWidth() + margin).reduced(margin));
}

std::array<juce::Rectangle<int>, 4> UnoEditor::getScreenRegions() const
{
	auto area = getLocalBounds();
	auto sliceButtonsRegion = area.removeFromBottom(area.getHeight() / 5);
	auto sliceSettingsRegion = area.removeFromBottom(area.getHeight() / 5);
	auto sampleWaveformRegion = area.removeFromBottom(area.getHeight() - 50);
	auto topBarRegion = area.removeFromTop(50);
	return {sliceButtonsRegion, sliceSettingsRegion, sampleWaveformRegion, topBarRegion};
}