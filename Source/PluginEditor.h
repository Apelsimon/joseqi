/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class JoseqiAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    JoseqiAudioProcessorEditor (JoseqiAudioProcessor&);
    ~JoseqiAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
	void initAndPublishSlider(Slider& slider, const String& suffix,
		const std::function<void()>& onValueChanged);
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JoseqiAudioProcessor& processor;

	Slider bassKnob;
	Slider midKnob;
	Slider trebleKnob;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JoseqiAudioProcessorEditor)
};
