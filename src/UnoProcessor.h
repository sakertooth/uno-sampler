#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "core/SampleSlice.h"

class UnoProcessor : public juce::AudioProcessor
{
public:
	UnoProcessor();
	~UnoProcessor() override;

	void prepareToPlay(double sampleRate, int samplesPerBlock) override;
	void releaseResources() override;

	bool isBusesLayoutSupported(const BusesLayout& layouts) const override;

	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
	using AudioProcessor::processBlock;

	juce::AudioProcessorEditor* createEditor() override;
	juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
	bool hasEditor() const override;

	const juce::String getName() const override;

	bool acceptsMidi() const override;
	bool producesMidi() const override;
	bool isMidiEffect() const override;
	double getTailLengthSeconds() const override;

	int getNumPrograms() override;
	int getCurrentProgram() override;
	void setCurrentProgram(int index) override;
	const juce::String getProgramName(int index) override;
	void changeProgramName(int index, const juce::String& newName) override;

	void getStateInformation(juce::MemoryBlock& destData) override;
	void setStateInformation(const void* data, int sizeInBytes) override;

private:
	juce::AudioProcessorValueTreeState m_parameterValueTree;
	SampleSlice m_sampleSlice;
	std::array<std::array<std::atomic<float>*, SampleSlice::NUM_PARAMETERS_PER_SLICE>, SampleSlice::MAX_NUM_SLICES>
		m_parameterValues;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UnoProcessor)
	friend class UnoEditor;
};