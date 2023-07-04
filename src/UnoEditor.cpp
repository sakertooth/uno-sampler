#include "UnoEditor.h"

UnoEditor::UnoEditor(UnoProcessor& p)
	: AudioProcessorEditor(&p)
	, m_sliceManager(&p.m_sliceManager)
	, m_slicePlayer(&p.m_slicePlayer)
	, m_sliceWaveform(m_formatManager, m_slicePlayer, m_sliceManager)
{
	addAndMakeVisible(m_beatPad);
	addAndMakeVisible(m_sliceLevel);
	addAndMakeVisible(m_sliceAttack);
	addAndMakeVisible(m_sliceRelease);
	addAndMakeVisible(m_sliceWaveform);
	addAndMakeVisible(m_sampleNameLabel);
	addAndMakeVisible(m_selectedPadLabel);
	addAndMakeVisible(m_playButton);
	addAndMakeVisible(m_addSliceButton);
	addAndMakeVisible(m_removeSliceButton);
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
	m_addSliceButton.setEnabled(false);
	m_removeSliceButton.setEnabled(false);

	m_playButton.onClick = [this] { onPlayButtonClick(); };
	m_loadSampleButton.onClick = [this] { selectSample(); };
	m_addSliceButton.onClick = [this] { onAddButtonClick(); };
	m_removeSliceButton.onClick = [this] { onRemoveButtonClick(); };
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

	auto addRemoveSliceRegion = sliceSettingsRegion.removeFromRight(spacing).reduced(margin);
	auto addRemoveSliceRegionHeight = addRemoveSliceRegion.getHeight();

	m_addSliceButton.setBounds(addRemoveSliceRegion.removeFromTop(addRemoveSliceRegionHeight / 2).reduced(margin));
	m_removeSliceButton.setBounds(addRemoveSliceRegion.removeFromTop(addRemoveSliceRegionHeight / 2).reduced(margin));
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

		const auto numChannels = static_cast<int>(reader->numChannels);
		const auto numSamples = static_cast<int>(reader->lengthInSamples);

		auto buffer = std::make_unique<juce::AudioSampleBuffer>(numChannels, numSamples);
		reader->read(buffer.get(), 0, numSamples, 0, true, true);
		m_slicePlayer->setSampleBuffer(std::move(buffer), reader->sampleRate);

		m_sliceWaveform.thumbnail().setSource(new juce::FileInputSource{sampleFile});
		m_sampleNameLabel.setText(sampleFile.getFileNameWithoutExtension(), juce::dontSendNotification);
		m_playButton.setEnabled(true);
		m_addSliceButton.setEnabled(true);
		m_removeSliceButton.setEnabled(true);
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
		m_slicePlayer->start();
		break;
	case PlayState::Playing:
		m_playState = PlayState::Paused;
		m_playButton.setButtonText("PLAY");
		m_slicePlayer->stop();
		break;
	case PlayState::Paused:
		m_playState = PlayState::Playing;
		m_playButton.setButtonText("PAUSE");
		m_slicePlayer->start();
		break;
	default:
		break;
	}
}

void UnoEditor::onAddButtonClick()
{
	BeatPad::Pad* firstEmptyPad = nullptr;
	auto index = 0;

	for (; index < SliceManager::MAX_NUM_SLICES; ++index)
	{
		firstEmptyPad = m_beatPad.getPad(index);
		if (firstEmptyPad->getState() == BeatPad::Pad::State::Empty) break;
	}

	if (firstEmptyPad == nullptr) { return; }
	auto sliceNumber = m_sliceManager->appendSlice(m_slicePlayer->getSampleBuffer());

	if (sliceNumber.has_value())
	{
		firstEmptyPad->setSliceToPlay(*sliceNumber);
		firstEmptyPad->setState(BeatPad::Pad::State::Filled);
		m_sliceWaveform.setMarker(
			*sliceNumber, m_sliceManager->getSlicePosition(*sliceNumber) / m_slicePlayer->getSampleRate());
	}
}

void UnoEditor::onRemoveButtonClick()
{
	BeatPad::Pad* lastFilledPad = nullptr;
	auto index = SliceManager::MAX_NUM_SLICES - 1;

	for (; index >= 0; --index)
	{
		lastFilledPad = m_beatPad.getPad(index);
		if (lastFilledPad->getState() == BeatPad::Pad::State::Filled) break;
	}

	if (lastFilledPad == nullptr) return;

	m_sliceManager->resetSlice(index);
	lastFilledPad->setState(BeatPad::Pad::State::Empty);
	m_sliceWaveform.removeMarker(index);
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

void UnoEditor::padPressed(BeatPad::Pad* pad)
{
	auto selectedSlice = m_beatPad.getIndexOfPad(pad);
	if (!selectedSlice.has_value()) { return; }
	m_selectedPadLabel.setText("Pad " + std::to_string(*selectedSlice + 1), juce::dontSendNotification);
	m_beatPad.setSelectedPad(pad);

	auto sliceToPlay = pad->getSliceToPlay();
	if (!sliceToPlay.has_value()) { return; }

	m_slicePlayer->setSliceToPlay(*m_sliceManager, *sliceToPlay);
	onPlayButtonClick();
}

void UnoEditor::padUnpressed([[maybe_unused]] BeatPad::Pad* pad)
{
	onPlayButtonClick();
}