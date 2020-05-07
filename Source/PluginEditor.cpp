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
	eqGraph(p),
	bassEqBand(p, EqBand::Type::Bass),
	midEqBand(p, EqBand::Type::Mid),
	trebleEqBand(p, EqBand::Type::Treble)
{
    setSize (2 * 400, 2 * 300);
	addAndMakeVisible(eqGraph);
	addAndMakeVisible(bassEqBand);
	addAndMakeVisible(midEqBand);
	addAndMakeVisible(trebleEqBand);

	eqGraph.addSliders(bassEqBand.getSliders());
	eqGraph.addSliders(midEqBand.getSliders());
	eqGraph.addSliders(trebleEqBand.getSliders());
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
	auto eqBandbounds = bounds.removeFromBottom(getHeight() / 2.f);
	const auto eqBandWidth = eqBandbounds.getWidth() / 3.f;

	eqGraph.setBounds(bounds);
	bassEqBand.setBounds(eqBandbounds.removeFromLeft(eqBandWidth));
	midEqBand.setBounds(eqBandbounds.removeFromLeft(eqBandWidth));
	trebleEqBand.setBounds(eqBandbounds);
}