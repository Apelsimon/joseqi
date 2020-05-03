/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JoseqiAudioProcessorEditor::JoseqiAudioProcessorEditor (JoseqiAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);

	initAndPublishSlider(bassKnob, " Bass dB", [this]() {processor.onBassGainChanged(bassKnob.getValue()); });
	initAndPublishSlider(midKnob, "Mid dB", [this]() {processor.onMidGainChanged(midKnob.getValue()); });
	initAndPublishSlider(trebleKnob, "Treble dB", [this]() {processor.onTrebleGainChanged(trebleKnob.getValue()); });
	
}

JoseqiAudioProcessorEditor::~JoseqiAudioProcessorEditor()
{
}

//==============================================================================
void JoseqiAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}

void JoseqiAudioProcessorEditor::resized()
{
	auto bounds = getBounds();
	const auto knobWidth = bounds.getWidth() / 3.f;
	bassKnob.setBounds(bounds.removeFromLeft(knobWidth));
	midKnob.setBounds(bounds.removeFromLeft(knobWidth));
	trebleKnob.setBounds(bounds);
}

void JoseqiAudioProcessorEditor::initAndPublishSlider(Slider& slider, const String& suffix,
	const std::function<void()>& onValueChanged)
{
	addAndMakeVisible(slider);
	slider.setRange({ -12.f, 12.f }, 0.1f);
	slider.setTextValueSuffix(suffix);
	slider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	slider.onValueChange = onValueChanged;
}