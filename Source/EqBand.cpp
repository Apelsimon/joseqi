#include "EqBand.h"

#include "../JuceLibraryCode/JuceHeader.h"

#include <cmath>

//==============================================================================
EqBand::EqBand(JoseqiAudioProcessor& p, Type t) :
	type(t),
	gainKnob(),
	qKnob(),
	freqKnob(),
	processor(p)
{
	buildControls();
}

EqBand::~EqBand()
{
}

void EqBand::paint (Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background
	g.setColour(Colours::antiquewhite);
	g.drawRect(getLocalBounds(), 2.f);
}

void EqBand::resized()
{
	const auto height = getHeight();
	const auto width = getWidth();
	auto bandBounds = getLocalBounds();
	
	eqBandLabel.setBounds(bandBounds.removeFromTop(height * 0.1f));
	gainKnob.setBounds(bandBounds.removeFromTop(height * 2.f / 3.f));
	freqKnob.setBounds(bandBounds.removeFromLeft(width / 2.f));
	qKnob.setBounds(bandBounds);
}

void EqBand::buildControls()
{
	eqBandLabel.setJustificationType(Justification::centred);

	switch (type)
	{
	case Type::Bass:
	{
		eqBandLabel.setText("Bass", NotificationType::dontSendNotification);
		initAndPublishControl(gainKnob, Slider::SliderStyle::LinearVertical, " dB", { -12.f, 12.f }, 0.1f, 0.f, [this]() { processor.onBassGainChanged(gainKnob.getValue());  });
		initAndPublishControl(qKnob, Slider::SliderStyle::RotaryVerticalDrag, " Q", {0.1f, 10.f}, 0.1f, std::sqrt(2.f), [this]() { processor.onBassQChanged(qKnob.getValue());  });
		initAndPublishControl(freqKnob, Slider::SliderStyle::RotaryVerticalDrag, " Hz", { 100.f, 300.f }, 1.f, 150.f, [this]() { processor.onBassFreqChanged(freqKnob.getValue());  });
		break;
	}
	case Type::Mid:
	{
		eqBandLabel.setText("Mid", NotificationType::dontSendNotification);
		initAndPublishControl(gainKnob, Slider::SliderStyle::LinearVertical, " dB", { -12.f, 12.f }, 0.1f, 0.f, [this]() {  processor.onMidGainChanged(gainKnob.getValue()); });
		initAndPublishControl(qKnob, Slider::SliderStyle::RotaryVerticalDrag, " Q", { 0.1f, 10.f }, 0.1f, std::sqrt(2.f), [this]() { processor.onMidQChanged(qKnob.getValue()); });
		initAndPublishControl(freqKnob, Slider::SliderStyle::RotaryVerticalDrag, " Hz", { 300.f, 3000.f }, 1.f, 500.f, [this]() {  processor.onMidFreqChanged(freqKnob.getValue()); });
		break;
	}
	case Type::Treble:
	{
		eqBandLabel.setText("Treble", NotificationType::dontSendNotification);
		initAndPublishControl(gainKnob, Slider::SliderStyle::LinearVertical, " dB", { -12.f, 12.f }, 0.1f, 0.f, [this]() { processor.onTrebleGainChanged(gainKnob.getValue());  });
		initAndPublishControl(qKnob, Slider::SliderStyle::RotaryVerticalDrag, " Q", { 0.1f, 10.f }, 0.1f, std::sqrt(2.f), [this]() { processor.onTrebleQChanged(qKnob.getValue()); });
		initAndPublishControl(freqKnob, Slider::SliderStyle::RotaryVerticalDrag, " Hz", { 3000.f, 10000.f }, 1.f, 6000.f, [this]() { processor.onTrebleFreqChanged(freqKnob.getValue()); });
		break;
	}
	}

	addAndMakeVisible(eqBandLabel);
}

void EqBand::initAndPublishControl(Slider& slider, const Slider::SliderStyle& sliderStyle, const String& suffix, 
	Range<double> range, float rangeDelta, float defaultValue, const std::function<void()>& onValueChanged)
{
	addAndMakeVisible(slider);
	slider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 30);
	slider.setRange(range, rangeDelta);
	slider.setValue(defaultValue);
	slider.setTextValueSuffix(suffix);
	slider.setSliderStyle(sliderStyle);
	slider.onValueChange = onValueChanged;
}