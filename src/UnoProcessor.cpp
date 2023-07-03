#include "UnoProcessor.h"

#include "UnoEditor.h"

UnoProcessor::UnoProcessor()
	: AudioProcessor(BusesProperties()
#if !JucePlugin_IsMidiEffect
#if !JucePlugin_IsSynth
						 .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
						 .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
	)
{
}

const juce::String UnoProcessor::getName() const
{
	return JucePlugin_Name;
}

bool UnoProcessor::acceptsMidi() const
{
	return true;
}

bool UnoProcessor::producesMidi() const
{
	return false;
}

bool UnoProcessor::isMidiEffect() const
{
	return false;
}

double UnoProcessor::getTailLengthSeconds() const
{
	return std::numeric_limits<double>::max();
}

int UnoProcessor::getNumPrograms()
{
	return 1; // NB: some hosts don't cope very well if you tell them there are 0
			  // programs, so this should be at least 1, even if you're not really
			  // implementing programs.
}

int UnoProcessor::getCurrentProgram()
{
	return 0;
}

void UnoProcessor::setCurrentProgram([[maybe_unused]] int index)
{
}

const juce::String UnoProcessor::getProgramName([[maybe_unused]] int index)
{
	return "";
}

void UnoProcessor::changeProgramName([[maybe_unused]] int index, [[maybe_unused]] const juce::String& newName)
{
}

void UnoProcessor::prepareToPlay(double sampleRate, int samplesPerBlock)
{
	m_slicePlayer.prepareToPlay(samplesPerBlock, sampleRate);
}

void UnoProcessor::releaseResources()
{
	m_slicePlayer.releaseResources();
}

bool UnoProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
	// This is the place where you check if the layout is supported.
	// In this template code we only support mono or stereo.
	// Some plugin hosts, such as certain GarageBand versions, will only
	// load plugins that support stereo bus layouts.
	if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
		&& layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
		return false;

		// This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
	if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet()) return false;
#endif

	return true;
}

void UnoProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
	juce::ignoreUnused(midiMessages);
	m_slicePlayer.getNextAudioBlock(juce::AudioSourceChannelInfo{buffer});
}

bool UnoProcessor::hasEditor() const
{
	return true;
}

juce::AudioProcessorEditor* UnoProcessor::createEditor()
{
	return new UnoEditor(*this);
}

juce::AudioProcessorValueTreeState::ParameterLayout UnoProcessor::createParameterLayout()
{
	auto layout = juce::AudioProcessorValueTreeState::ParameterLayout{};
	for (int i = 0; i < SliceManager::MAX_NUM_SLICES; ++i)
	{
		auto sliceGroup = m_sliceManager.addSliceParametersToValueTree(i);
		layout.add(std::move(sliceGroup));
	}

	return layout;
}

void UnoProcessor::getStateInformation(juce::MemoryBlock& destData)
{
	// You should use this method to store your parameters in the memory block.
	// You could do that either as raw data, or use the XML or ValueTree classes
	// as intermediaries to make it easy to save and load complex data.
	juce::ignoreUnused(destData);
}

void UnoProcessor::setStateInformation(const void* data, int sizeInBytes)
{
	// You should use this method to restore your parameters from this memory
	// block, whose contents will have been created by the getStateInformation()
	// call.
	juce::ignoreUnused(data, sizeInBytes);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
	return new UnoProcessor();
}