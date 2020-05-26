/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/**
*/
class JoseqiAudioProcessor  : public AudioProcessor, public AudioProcessorValueTreeState::Listener
{
public:
    //==============================================================================
    JoseqiAudioProcessor();
    ~JoseqiAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

	//==============================================================================
	 
	void parameterChanged(const String &parameterID, float newValue) override;

	//==============================================================================

	float getSampleRate() const { return sampleRate; }
	dsp::IIR::Coefficients<float> getBassFilterCoeffs();
	dsp::IIR::Coefficients<float> getMidFilterCoeffs();
	dsp::IIR::Coefficients<float> getTrebleFilterCoeffs();
	
private:

	enum
	{
		BassFilterIndex,
		MidFilterIndex,
		TrebleFilterIndex
	};

	void initFilters();

	void onBassGainChanged(float newValue);
	void onBassQChanged(float newValue);
	void onBassFreqChanged(float newValue);

	void onMidGainChanged(float newValue);
	void onMidQChanged(float newValue);
	void onMidFreqChanged(float newValue);

	void onTrebleGainChanged(float newValue);
	void onTrebleQChanged(float newValue);
	void onTrebleFreqChanged(float newValue);

	using Filter = dsp::ProcessorDuplicator <dsp::IIR::Filter<float>, dsp::IIR::Coefficients<float>>;
	dsp::ProcessorChain<Filter, Filter, Filter> processChain;
	
	AudioProcessorValueTreeState parameters;
 
	std::atomic<float> sampleRate;

	float bassGain;
	float bassQ;
	float bassFreq;

	float midGain;
	float midQ;
	float midFreq;

	float trebleGain;
	float trebleQ;
	float trebleFreq;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JoseqiAudioProcessor)
};
