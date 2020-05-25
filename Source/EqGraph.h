/*
  ==============================================================================

    EqGraph.h
    Created: 4 May 2020 7:21:08pm
    Author:  Apelsimon

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"

//==============================================================================
/*
*/
class EqGraph    : public Component, public AudioProcessorValueTreeState::Listener
{
public:
    EqGraph(JoseqiAudioProcessor& p, AudioProcessorValueTreeState& parameters);
    ~EqGraph();

	// ==============================================================

    void paint (Graphics&) override;
    void resized() override;

	// ==============================================================

	void parameterChanged(const String &parameterID, float newValue) override;

	// ==============================================================

private:
	String getFrequencyString(int freq);

	JoseqiAudioProcessor& processor;
	std::vector<float> baseFrequencies;
	std::vector<float> incrementalFrequencies;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EqGraph)
};
