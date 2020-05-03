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
    : AudioProcessorEditor (&p), 
	processor (p),
	bassEqBand(p, EqBand::Type::Bass),
	midEqBand(p, EqBand::Type::Mid),
	trebleEqBand(p, EqBand::Type::Treble)
{
    setSize (400, 300);
	addAndMakeVisible(bassEqBand);
	addAndMakeVisible(midEqBand);
	addAndMakeVisible(trebleEqBand);
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
	const auto eqBandWidth = bounds.getWidth() / 3.f;
	bassEqBand.setBounds(bounds.removeFromLeft(eqBandWidth));
	midEqBand.setBounds(bounds.removeFromLeft(eqBandWidth));
	trebleEqBand.setBounds(bounds);
}