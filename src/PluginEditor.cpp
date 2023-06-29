#include "PluginEditor.h"

#include "PluginProcessor.h"

AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor& p)
	: AudioProcessorEditor(&p)
	, m_processorRef(p)
{
	juce::ignoreUnused(m_processorRef);

	for (size_t i = 0; i < 32; ++i)
	{
		m_sliceButtons[i].setButtonText(juce::String{i + 1});
		addAndMakeVisible(m_sliceButtons[i]);
	}

	m_reverseButton.setButtonText("REVERSE");
	m_playButton.setButtonText("PLAY");
	m_loadSampleButton.setButtonText("LOAD SAMPLE");

	addAndMakeVisible(m_sliceAttack);
	addAndMakeVisible(m_sliceRelease);
	addAndMakeVisible(m_reverseButton);
	addAndMakeVisible(m_sliceKeyShift);
	addAndMakeVisible(m_sliceTimeStretch);
	addAndMakeVisible(m_playButton);
	addAndMakeVisible(m_sampleNameLabel);
	addAndMakeVisible(m_loadSampleButton);
	setSize(820, 560);
}

void AudioPluginAudioProcessorEditor::paint(juce::Graphics& g)
{
	calculateInitialRegions();
	g.fillAll(juce::Colours::darkgrey);

	g.setColour(juce::Colours::black);
	g.fillRect(m_sliceButtonsRegion);

	g.setColour(juce::Colours::grey);
	g.fillRect(m_sliceSettingsRegion);

	g.setColour(juce::Colours::black);
	g.fillRect(m_sampleWaveformRegion);

	g.setColour(juce::Colours::grey);
	g.fillRect(m_topBarRegion);
	calculateInitialRegions();

	paintSampleWaveform(g);
}

void AudioPluginAudioProcessorEditor::resized()
{
	positionSliceButtons();
	positionSliceSettings();
	positionTopBar();
}

void AudioPluginAudioProcessorEditor::positionSliceButtons()
{
	calculateInitialRegions();

	const auto numRows = 2;
	const auto regionMargin = 10;
	const auto sliceButtonSize = std::array<int, 2>{32, 32};
	const auto numButtonsPerRow = static_cast<int>(m_sliceButtons.size() / numRows);
	const auto rowOne = m_sliceButtonsRegion.removeFromTop(m_sliceButtonsRegion.getHeight() / 2);
	const auto rowTwo = m_sliceButtonsRegion.removeFromTop(m_sliceButtonsRegion.getHeight() / 2);

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

	calculateInitialRegions();
}

void AudioPluginAudioProcessorEditor::positionSliceSettings()
{
	calculateInitialRegions();

	const auto width = m_sliceSettingsRegion.getWidth();
	const auto numControls = 5;
	const auto spacing = width / numControls;
	const auto margin = 10;

	m_sliceAttack.setBounds(m_sliceSettingsRegion.removeFromLeft(spacing).reduced(margin));
	m_sliceRelease.setBounds(m_sliceSettingsRegion.removeFromLeft(spacing).reduced(margin));
	m_reverseButton.setBounds(m_sliceSettingsRegion.removeFromLeft(spacing).reduced(margin));
	m_sliceKeyShift.setBounds(m_sliceSettingsRegion.removeFromLeft(spacing).reduced(margin));
	m_sliceTimeStretch.setBounds(m_sliceSettingsRegion.removeFromLeft(spacing).reduced(margin));

	calculateInitialRegions();
}

void AudioPluginAudioProcessorEditor::paintSampleWaveform(juce::Graphics& g)
{
	calculateInitialRegions();
	g.setColour(juce::Colours::black);
	if (m_sampleWaveform.get() == nullptr || m_sampleWaveform->getNumChannels() == 0)
	{
		g.fillRect(m_sampleWaveformRegion);
		g.setColour(juce::Colours::white);
		g.drawFittedText("No File Loaded", m_sampleWaveformRegion, juce::Justification::centred, 1);
	}
	else
	{
		g.fillRect(m_sampleWaveformRegion);
		g.setColour(juce::Colours::darkblue);
		m_sampleWaveform->drawChannels(g, m_sampleWaveformRegion, 0.0, m_sampleWaveform->getTotalLength(), 1.0f);
	}
	calculateInitialRegions();
}

void AudioPluginAudioProcessorEditor::positionTopBar()
{
	calculateInitialRegions();

	const auto width = m_topBarRegion.getWidth();
	const auto numControls = 3;
	const auto spacing = width / numControls;
	const auto margin = 10;

	auto playRect = m_topBarRegion.removeFromLeft(spacing).reduced(margin);
	playRect.setSize(100, 50);
	m_playButton.setBounds(playRect);

	m_sampleNameLabel.setBounds(m_topBarRegion.removeFromLeft(spacing).reduced(margin));
	m_loadSampleButton.setBounds(m_topBarRegion.removeFromLeft(spacing).reduced(margin));

	calculateInitialRegions();
}

void AudioPluginAudioProcessorEditor::calculateInitialRegions()
{
	auto area = getLocalBounds();
	m_sliceButtonsRegion = area.removeFromBottom(getHeight() / 5);
	m_sliceSettingsRegion = area.removeFromBottom(getHeight() / 8);
	m_sampleWaveformRegion = area.removeFromBottom(static_cast<int>(getHeight() / 1.82));
	m_topBarRegion = area.removeFromBottom(getHeight() / 8);
}