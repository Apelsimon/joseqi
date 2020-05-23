/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"

#include "EqParameters.h"
#include "PluginEditor.h"

//==============================================================================
JoseqiAudioProcessor::JoseqiAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
	: AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
		.withInput("Input", AudioChannelSet::stereo(), true)
#endif
		.withOutput("Output", AudioChannelSet::stereo(), true)
#endif
	),
#endif
	parameters(*this, nullptr, Identifier{ "EqParameters" }, {
		std::make_unique<AudioParameterFloat>(EqParameters::BassGainId, "Gain", EqParameters::GainRange.getStart(), EqParameters::GainRange.getEnd(), EqParameters::GainDefault),
		std::make_unique<AudioParameterFloat>(EqParameters::BassQId, "Q", EqParameters::QRange.getStart(), EqParameters::QRange.getEnd(), EqParameters::QDefault),
		std::make_unique<AudioParameterFloat>(EqParameters::BassFreqId, "Hz", EqParameters::BassFreqRange.getStart(), EqParameters::BassFreqRange.getEnd(), EqParameters::BassFreqDefault),
		std::make_unique<AudioParameterFloat>(EqParameters::MidGainId, "Gain",  EqParameters::GainRange.getStart(), EqParameters::GainRange.getEnd(), EqParameters::GainDefault),
		std::make_unique<AudioParameterFloat>(EqParameters::MidQId, "Q", EqParameters::QRange.getStart(), EqParameters::QRange.getEnd(), EqParameters::QDefault),
		std::make_unique<AudioParameterFloat>(EqParameters::MidFreqId, "Hz", EqParameters::MidFreqRange.getStart(), EqParameters::MidFreqRange.getEnd(), EqParameters::MidFreqDefault),
		std::make_unique<AudioParameterFloat>(EqParameters::TrebleGainId, "Gain", EqParameters::GainRange.getStart(), EqParameters::GainRange.getEnd(), EqParameters::GainDefault),
		std::make_unique<AudioParameterFloat>(EqParameters::TrebleQId, "Q", EqParameters::QRange.getStart(), EqParameters::QRange.getEnd(), EqParameters::QDefault),
		std::make_unique<AudioParameterFloat>(EqParameters::TrebleFreqId, "Hz", EqParameters::TrebleFreqRange.getStart(), EqParameters::TrebleFreqRange.getEnd(), EqParameters::TrebleFreqDefault),
		}),
	bassGain(Decibels::decibelsToGain(*parameters.getRawParameterValue(EqParameters::BassGainId))),
	midGain(Decibels::decibelsToGain(*parameters.getRawParameterValue(EqParameters::MidGainId))),
	trebleGain(Decibels::decibelsToGain(*parameters.getRawParameterValue(EqParameters::TrebleGainId))),
	bassQ(*parameters.getRawParameterValue(EqParameters::BassQId)),
	midQ(*parameters.getRawParameterValue(EqParameters::MidQId)),
	trebleQ(*parameters.getRawParameterValue(EqParameters::TrebleQId)),
	bassFreq(*parameters.getRawParameterValue(EqParameters::BassFreqId)),
	midFreq(*parameters.getRawParameterValue(EqParameters::MidFreqId)),
	trebleFreq(*parameters.getRawParameterValue(EqParameters::TrebleFreqId))
{
	EqParameters::addEqParametersListener(parameters, this);
}

JoseqiAudioProcessor::~JoseqiAudioProcessor()
{
}

//==============================================================================
const String JoseqiAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JoseqiAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JoseqiAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JoseqiAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JoseqiAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JoseqiAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JoseqiAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JoseqiAudioProcessor::setCurrentProgram (int index)
{
}

const String JoseqiAudioProcessor::getProgramName (int index)
{
    return {};
}

void JoseqiAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void JoseqiAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
	this->sampleRate = sampleRate;

	dsp::ProcessSpec spec{ sampleRate, samplesPerBlock, getTotalNumInputChannels() };
	processChain.prepare(spec);
	processChain.reset();

	
	DBG("gain: " << bassGain << ", Q: " << bassQ << ", freq: " << bassFreq);
	*processChain.template get<BassFilterIndex>().state = *dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, bassFreq, bassQ, bassGain);
	*processChain.template get<MidFilterIndex>().state = *dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, midFreq, midQ, midGain);
	*processChain.template get<TrebleFilterIndex>().state = *dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, trebleFreq, trebleQ, trebleGain);
}

void JoseqiAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JoseqiAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void JoseqiAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
	ScopedNoDenormals noDenormals;
	auto totalNumInputChannels = getTotalNumInputChannels();
	auto totalNumOutputChannels = getTotalNumOutputChannels();

	for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
		buffer.clear(i, 0, buffer.getNumSamples());

	dsp::AudioBlock <float> block{ buffer };
	processChain.process(dsp::ProcessContextReplacing<float>{block});
}

//==============================================================================
bool JoseqiAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* JoseqiAudioProcessor::createEditor()
{
    return new JoseqiAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void JoseqiAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void JoseqiAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void JoseqiAudioProcessor::parameterChanged(const String &parameterID, float newValue)
{ 
	if (parameterID == EqParameters::BassGainId)
	{
		onBassGainChanged(newValue);
	}	
	else if (parameterID == EqParameters::BassQId)
	{
		onBassQChanged(newValue);
	}
	else if (parameterID == EqParameters::BassFreqId)
	{
		onBassFreqChanged(newValue);
	}
	else if (parameterID == EqParameters::MidGainId)
	{
		onMidGainChanged(newValue);
	}
	else if (parameterID == EqParameters::MidQId)
	{
		onMidQChanged(newValue);
	}
	else if (parameterID == EqParameters::MidFreqId)
	{
		onMidFreqChanged(newValue);
	}
	else if (parameterID == EqParameters::TrebleGainId)
	{
		onTrebleGainChanged(newValue);
	}
	else if (parameterID == EqParameters::TrebleQId)
	{
		onTrebleQChanged(newValue);
	}
	else if (parameterID == EqParameters::TrebleFreqId)
	{
		onTrebleFreqChanged(newValue);
	}
}

void JoseqiAudioProcessor::onBassGainChanged(float newValue)
{
	DBG("change bass gain: " << newValue);
	bassGain = Decibels::decibelsToGain(newValue);
	*processChain.template get<BassFilterIndex>().state = *dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, bassFreq, bassQ, bassGain);
}

void JoseqiAudioProcessor::onBassQChanged(float newValue)
{
	DBG("change bass Q: " << newValue);
	bassQ = newValue;
	*processChain.template get<BassFilterIndex>().state = *dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, bassFreq, bassQ, bassGain);
}

void JoseqiAudioProcessor::onBassFreqChanged(float newValue)
{
	DBG("change bass freq: " << newValue);
	bassFreq = newValue;
	*processChain.template get<BassFilterIndex>().state = *dsp::IIR::Coefficients<float>::makeLowShelf(sampleRate, bassFreq, bassQ, bassGain);
}

void JoseqiAudioProcessor::onMidGainChanged(float newValue)
{
	DBG("change mid gain: " << newValue);
	midGain = Decibels::decibelsToGain(newValue);
	*processChain.template get<MidFilterIndex>().state = *dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, midFreq, midQ, midGain);
}

void JoseqiAudioProcessor::onMidQChanged(float newValue)
{
	DBG("change mid Q: " << newValue);
	midQ = newValue;
	*processChain.template get<MidFilterIndex>().state = *dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, midFreq, midQ, midGain);
}

void JoseqiAudioProcessor::onMidFreqChanged(float newValue)
{
	DBG("change mid freq gain: " << newValue);
	midFreq = newValue;
	*processChain.template get<MidFilterIndex>().state = *dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate, midFreq, midQ, midGain);
}

void JoseqiAudioProcessor::onTrebleGainChanged(float newValue)
{
	DBG("change treble gain: " << newValue);
	trebleGain = Decibels::decibelsToGain(newValue);
	*processChain.template get<TrebleFilterIndex>().state = *dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, trebleFreq, trebleQ, trebleGain);
}

void JoseqiAudioProcessor::onTrebleQChanged(float newValue)
{
	DBG("change treble q: " << newValue);
	trebleQ = newValue;
	*processChain.template get<TrebleFilterIndex>().state = *dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, trebleFreq, trebleQ, trebleGain);
}

void JoseqiAudioProcessor::onTrebleFreqChanged(float newValue)
{
	DBG("change treble freq: " << newValue);
	trebleFreq = newValue;
	*processChain.template get<TrebleFilterIndex>().state = *dsp::IIR::Coefficients<float>::makeHighShelf(sampleRate, trebleFreq, trebleQ, trebleGain);
}

dsp::IIR::Coefficients<float> JoseqiAudioProcessor::getBassFilterCoeffs()
{
	return *processChain.template get<BassFilterIndex>().state;
}

dsp::IIR::Coefficients<float> JoseqiAudioProcessor::getMidFilterCoeffs()
{
	return *processChain.template get<MidFilterIndex>().state;
}

dsp::IIR::Coefficients<float> JoseqiAudioProcessor::getTrebleFilterCoeffs()
{
	return *processChain.template get<TrebleFilterIndex>().state;
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JoseqiAudioProcessor();
}
