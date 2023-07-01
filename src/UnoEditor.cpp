#include "UnoEditor.h"

UnoEditor::UnoEditor(UnoProcessor& p)
	: AudioProcessorEditor(&p)
	, m_processorRef(p)
	, m_sliceWaveform(m_formatManager)
{
	addAndMakeVisible(m_beatPad);
	addAndMakeVisible(m_sliceLevel);
	addAndMakeVisible(m_sliceAttack);
	addAndMakeVisible(m_sliceRelease);
	addAndMakeVisible(m_sliceWaveform);
	addAndMakeVisible(m_sampleNameLabel);
	addAndMakeVisible(m_selectedPadLabel);
	addAndMakeVisible(m_playButton);
	addAndMakeVisible(m_loadSampleButton);
	setSize(820, 560);

	m_sampleNameLabel.setColour(juce::Label::ColourIds::backgroundColourId, juce::Colour{34, 34, 34});
	m_sampleNameLabel.setColour(juce::Label::ColourIds::outlineColourId, juce::Colours::whitesmoke);
	m_sampleNameLabel.setFont(juce::Font{12.0f, juce::Font::bold});
	m_sampleNameLabel.setEditable(false);
	m_sampleNameLabel.setJustificationType(juce::Justification::left);

	m_selectedPadLabel.setFont(juce::Font{24.0f, juce::Font::bold});
	m_selectedPadLabel.setJustificationType(juce::Justification::centred);
	m_selectedPadLabel.setEditable(false);

	m_formatManager.registerBasicFormats();

	m_sliceWaveform.thumbnail().addChangeListener(this);

	m_playButton.setEnabled(false);
	m_playButton.onClick = [this] { onPlayButtonClick(); };

	m_loadSampleButton.onClick = [this] { selectSample(); };
}

void UnoEditor::paint(juce::Graphics& g)
{
	auto [sliceButtonsRegion, sliceSettingsRegion, sampleWaveformRegion, topBarRegion] = getScreenRegions();
	g.setColour(juce::Colour{34, 34, 34});
	g.fillRect(sliceButtonsRegion);

	g.setColour(juce::Colours::black);
    g.drawRoundedRectangle(sliceButtonsRegion.toFloat(), 1.0f, 1.0f);

	g.setColour(juce::Colour{51, 51, 51});
	g.fillRect(sliceSettingsRegion);

	g.setColour(juce::Colours::black);
	g.drawRoundedRectangle(sliceSettingsRegion.toFloat(), 1.0f, 1.0f);

	m_sliceWaveform.paint(g);

	g.setColour(juce::Colour{51, 51, 51});
	g.fillRect(topBarRegion);

	g.setColour(juce::Colours::black);
	g.drawRoundedRectangle(topBarRegion.toFloat(), 1.0f, 1.0f);
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
	m_beatPad.setBounds(sliceButtonsRegion);
}

void UnoEditor::positionSliceSettings()
{
	auto [sliceButtonsRegion, sliceSettingsRegion, sampleWaveformRegion, topBarRegion] = getScreenRegions();
	const auto width = sliceSettingsRegion.getWidth();
	const auto numControlsGroup1 = 3;
	const auto spacing = width / 2 / numControlsGroup1;
	const auto margin = 2;

	m_sliceLevel.setBounds(sliceSettingsRegion.removeFromLeft(spacing).reduced(margin));
	m_sliceAttack.setBounds(sliceSettingsRegion.removeFromLeft(spacing).reduced(margin));
	m_sliceRelease.setBounds(sliceSettingsRegion.removeFromLeft(spacing).reduced(margin));
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
	m_selectedPadLabel.setBounds(topBarRegion.removeFromRight(m_sampleNameLabel.getWidth() + margin).reduced(margin));
}

void UnoEditor::selectSample()
{
	auto fileChooser = std::make_shared<juce::FileChooser>(
		"Choose sample...", juce::File::getSpecialLocation(juce::File::userHomeDirectory), "*.wav .*ogg *.mp3");

	fileChooser->launchAsync(juce::FileBrowserComponent::openMode, [this, fileChooser](const juce::FileChooser&) {
		auto sampleFile = fileChooser->getResult();
		if (sampleFile == juce::File{}) { return; }

		auto reader = std::unique_ptr<juce::AudioFormatReader>(m_formatManager.createReaderFor(sampleFile));
		if (reader == nullptr) { return; }

		auto& buffer = m_processorRef.m_sampleSlice.getSample();
		buffer.setSize(static_cast<int>(reader->numChannels), static_cast<int>(reader->lengthInSamples));
		reader->read(&buffer, 0, static_cast<int>(reader->lengthInSamples), 0, true, true);

		m_sliceWaveform.thumbnail().setSource(new juce::FileInputSource{sampleFile});
		m_playButton.setEnabled(true);
		m_sampleNameLabel.setText(sampleFile.getFileNameWithoutExtension(), juce::dontSendNotification);
	});
}

void UnoEditor::onPlayButtonClick()
{
	if (!m_playButton.isEnabled()) { return; }
	switch (m_playState)
	{
	case PlayState::Stopped:
		m_playState = PlayState::Playing;
		m_playButton.setButtonText("PAUSE");
		break;
	case PlayState::Playing:
		m_playState = PlayState::Paused;
		m_playButton.setButtonText("PLAY");
		break;
	case PlayState::Paused:
		m_playState = PlayState::Playing;
		m_playButton.setButtonText("PAUSE");
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

void UnoEditor::changeListenerCallback(juce::ChangeBroadcaster* source)
{
	if (source == &m_sliceWaveform.thumbnail()) { m_sliceWaveform.thumbnailChanged(); }
}

void UnoEditor::selectPad(BeatPad::Pad* pad)
{
	auto selectedSlice = m_beatPad.findSlice(pad);
	if (!selectedSlice.has_value()) { return; }
	m_selectedPadLabel.setText("Pad " + std::to_string(*selectedSlice + 1), juce::dontSendNotification);
}