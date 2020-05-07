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
class EqGraph    : public Component, public Slider::Listener
{
public:
    EqGraph(JoseqiAudioProcessor& p);
    ~EqGraph();

	// ==============================================================

    void paint (Graphics&) override;
    void resized() override;

	// ==============================================================

	void 	sliderValueChanged(Slider* slider) override;

	// ==============================================================

	void addSliders(const std::vector<Slider*>& newSliders);

private:
	JoseqiAudioProcessor& processor;
	std::vector<Slider*> sliders;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EqGraph)
};
