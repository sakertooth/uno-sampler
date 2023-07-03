#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

#include "core/SliceManager.h"
#include "core/SlicePlayer.h"

class UnoProcessor : public juce::AudioProcessor
{
public:
	UnoProcessor();

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
	juce::AudioProcessorValueTreeState m_parameterValueTree
		= juce::AudioProcessorValueTreeState{*this, nullptr, "Uno Slicer Parameters", createParameterLayout()};
	SliceManager m_sliceManager = SliceManager{m_parameterValueTree};
	SlicePlayer m_slicePlayer;
	friend class UnoEditor;
	friend class SlicePlayer;
	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(UnoProcessor)
};