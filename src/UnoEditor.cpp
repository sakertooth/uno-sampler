#include "UnoEditor.h"

UnoEditor::UnoEditor(UnoProcessor& p)
	: AudioProcessorEditor(&p)
	, m_processorRef(p)
	, m_sliceWaveform(m_formatManager)
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

	m_formatManager.registerBasicFormats();
	m_transportSource.addChangeListener(this);
	m_sliceWaveform.thumbnail().addChangeListener(this);
	m_playButton.setEnabled(false);
	m_loadSampleButton.onClick = [this] { selectSample(); };

	addAndMakeVisible(m_sliceLevel);
	addAndMakeVisible(m_sliceAttack);
	addAndMakeVisible(m_sliceRelease);
	addAndMakeVisible(m_sliceWaveform);
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

	m_sliceWaveform.paint(g);

	g.setColour(juce::Colour{51, 51, 51});
	g.fillRect(topBarRegion);
}

void UnoEditor::resized()
{
	positionSliceButtons();
	positionSliceSettings();
	positionWaveform();
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

void UnoEditor::positionWaveform()
{
	auto [sliceButtonsRegion, sliceSettingsRegion, sampleWaveformRegion, topBarRegion] = getScreenRegions();
	m_sliceWaveform.setBounds(sampleWaveformRegion);
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

void UnoEditor::selectSample()
{
	auto fileChooser = std::make_shared<juce::FileChooser>(
		"Choose sample...", juce::File::getSpecialLocation(juce::File::userHomeDirectory), "*.wav .*ogg *.mp3");

	fileChooser->launchAsync(juce::FileBrowserComponent::openMode, [this, fileChooser](const juce::FileChooser&) {
		auto sampleFile = fileChooser->getResult();
		if (sampleFile == juce::File{}) { return; }

		auto reader = m_formatManager.createReaderFor(sampleFile);
		if (reader == nullptr) { return; }

		auto& buffer = m_processorRef.m_sampleSlice.getSample();
		buffer.setSize(static_cast<int>(reader->numChannels), static_cast<int>(reader->lengthInSamples));
		reader->read(&buffer, 0, static_cast<int>(reader->lengthInSamples), 0, true, true);

		auto newSource = std::make_unique<juce::AudioFormatReaderSource>(reader, true);
		m_transportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
		m_sliceWaveform.thumbnail().setSource(new juce::FileInputSource{sampleFile});
		m_playButton.setEnabled(true);
		m_formatReaderSource.reset(newSource.release());
	});
}

void UnoEditor::changeListenerCallback(juce::ChangeBroadcaster* source)
{
	if (source == &m_transportSource)
	{
		changeState(m_transportSource.isPlaying() ? TransportState::Playing : TransportState::Stopped);
	}
	else if (source == &m_sliceWaveform.thumbnail())
	{
		m_sliceWaveform.thumbnailChanged();
	}
}

void UnoEditor::changeState(UnoEditor::TransportState newState)
{
	if (m_transportState == newState) { return; }
	switch (newState)
	{
	case TransportState::Starting:
		m_transportSource.start();
		break;
	case TransportState::Playing:
		m_transportSource.start();
		break;
	case TransportState::Stopping:
		m_transportSource.stop();
		break;
	case TransportState::Stopped:
		break;
	default:
		break;
	}
}

std::array<juce::Rectangle<int>, 4> UnoEditor::getScreenRegions() const
{
	auto area = getLocalBounds();
	const auto windowHeight = area.getHeight();

	auto sliceButtonsRegion = area.removeFromBottom(windowHeight / 5);
	auto sliceSettingsRegion = area.removeFromBottom(windowHeight / 5);
	auto sampleWaveformRegion = area.removeFromBottom(2 * windowHeight / 5 + 50);
	auto topBarRegion = area;
	return {sliceButtonsRegion, sliceSettingsRegion, sampleWaveformRegion, topBarRegion};
}