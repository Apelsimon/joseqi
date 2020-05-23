#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"

#include <memory>

class EqBand    : public Component
{
public:
	enum class Type
	{
		Bass,
		Mid,
		Treble
	};

    EqBand(JoseqiAudioProcessor& p, AudioProcessorValueTreeState& parameters, Type t);
    ~EqBand();

    void paint (Graphics&) override;
    void resized() override;

	std::vector<Slider*> getSliders() { return std::vector<Slider*>{&gainKnob, &qKnob, &freqKnob }; }

private:
	void buildControls();
	void initAndPublishControl(Slider& slider, const Slider::SliderStyle& sliderStyle, const String& suffix);

	AudioProcessorValueTreeState& parameters;
	
	Type type;

	Label eqBandLabel;
	Slider gainKnob;
	Slider qKnob;
	Slider freqKnob;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> qAttachment;
	std::unique_ptr<AudioProcessorValueTreeState::SliderAttachment> freqAttachment;

	JoseqiAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EqBand)
};
