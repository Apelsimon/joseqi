#include "EqBand.h"

#include "EqParameters.h"

#include <cmath>

//==============================================================================
EqBand::EqBand(JoseqiAudioProcessor& p, AudioProcessorValueTreeState& parameters, Type t) :
	parameters(parameters),
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
	addAndMakeVisible(eqBandLabel);

	initAndPublishControl(gainKnob, Slider::SliderStyle::LinearVertical, " db");
	initAndPublishControl(qKnob, Slider::SliderStyle::RotaryVerticalDrag, " q");
	initAndPublishControl(freqKnob, Slider::SliderStyle::RotaryVerticalDrag, " hz");

	const String* gainId;
	const String* qId;
	const String* freqId;
	
	switch (type)
	{
	case Type::Bass:
	{
		eqBandLabel.setText("Bass", NotificationType::dontSendNotification);
		gainId = &EqParameters::BassGainId;
		qId = &EqParameters::BassQId;
		freqId = &EqParameters::BassFreqId;
		break;
	}
	case Type::Mid:
	{
		eqBandLabel.setText("Mid", NotificationType::dontSendNotification);
		gainId = &EqParameters::MidGainId;
		qId = &EqParameters::MidQId;
		freqId = &EqParameters::MidFreqId;
		break;
	}
	case Type::Treble:
	{
		eqBandLabel.setText("Treble", NotificationType::dontSendNotification);
		gainId = &EqParameters::TrebleGainId;
		qId = &EqParameters::TrebleQId;
		freqId = &EqParameters::TrebleFreqId;
		break;
	}
	}

	gainAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
		parameters, *gainId, gainKnob);
	qAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
		parameters, *qId, qKnob);
	freqAttachment = std::make_unique<AudioProcessorValueTreeState::SliderAttachment>(
		parameters, *freqId, freqKnob);
}

void EqBand::initAndPublishControl(Slider& slider, const Slider::SliderStyle& sliderStyle, const String& suffix)
{
	addAndMakeVisible(slider);
	slider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 30);
	slider.setTextValueSuffix(suffix);
	slider.setSliderStyle(sliderStyle);
}