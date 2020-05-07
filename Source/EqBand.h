#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

#include "PluginProcessor.h"

class EqBand    : public Component
{
public:
	enum class Type
	{
		Bass,
		Mid,
		Treble
	};

    EqBand(JoseqiAudioProcessor& p, Type t);
    ~EqBand();

    void paint (Graphics&) override;
    void resized() override;

	std::vector<Slider*> getSliders() { return std::vector<Slider*>{&gainKnob, &qKnob, &freqKnob }; }

private:
	void buildControls();
	void initAndPublishControl(Slider& slider, const Slider::SliderStyle& sliderStyle, const String& suffix,
		Range<double> range, float rangeDelta, float defaultValue, const std::function<void()>& onValueChanged);

	Type type;

	Label eqBandLabel;
	Slider gainKnob;
	Slider qKnob;
	Slider freqKnob;

	JoseqiAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (EqBand)
};
