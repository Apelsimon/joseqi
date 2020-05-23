/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "EqBand.h"
#include "EqGraph.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class JoseqiAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    JoseqiAudioProcessorEditor (JoseqiAudioProcessor& p, AudioProcessorValueTreeState& parameters);
    ~JoseqiAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JoseqiAudioProcessor& processor;

	EqGraph eqGraph;
	EqBand bassEqBand;
	EqBand midEqBand;
	EqBand trebleEqBand;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JoseqiAudioProcessorEditor)
};
