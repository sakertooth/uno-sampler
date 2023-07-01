#include "UnoProcessor.h"

#include "UnoEditor.h"

UnoProcessor::UnoProcessor()
	: m_parameterValueTree(*this, nullptr, "Uno Slicer Parameters", createParameterLayout())
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
	return 0.0;
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

void UnoProcessor::prepareToPlay([[maybe_unused]] double sampleRate, [[maybe_unused]] int samplesPerBlock)
{
}

void UnoProcessor::releaseResources()
{
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

	juce::ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	// In case we have more outputs than inputs, this code clears any output
	// channels that didn't contain input data, (because these aren't
	// guaranteed to be empty - they may contain garbage).
	// This is here to avoid people getting screaming feedback
	// when they first compile a plugin, but obviously you don't need to keep
	// this code if your algorithm always overwrites all the output channels.
	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	// This is the place where you'd normally do the guts of your plugin's
	// audio processing...
	// Make sure to reset the state if your inner loop is processing
	// the samples and the outer loop is handling the channels.
	// Alternatively, you can process the samples with the channels
	// interleaved by keeping the same state.
	for (int channel = 0; channel < totalNumInputChannels; ++channel)
	{
		auto* channelData = buffer.getWritePointer(channel);
		juce::ignoreUnused(channelData);
		// ..do something to the data...
	}
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
	for (int i = 0; i < SampleSlice::MAX_NUM_SLICES; ++i)
	{
		auto sliceGroup = m_sampleSlice.addSliceParametersToValueTree(i);
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