/*
  ==============================================================================

    EqGraph.cpp
    Created: 4 May 2020 7:21:08pm
    Author:  Apelsimon

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "EqGraph.h"

//==============================================================================
EqGraph::EqGraph(JoseqiAudioProcessor& p) :
	processor(p)
{
}

EqGraph::~EqGraph()
{
}

void EqGraph::paint (Graphics& g)
{
    g.fillAll (Colours::black);   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component
    
	const auto width = getWidth();
	std::vector<float> baseFreqs{ 20.f, 50.f, 100.f, 200.f, 500.f, 1000.f, 2000.f, 5000.f, 10000.f, 20000.f };
	std::vector<float> freqs{ 20.f, 30.f, 40.f, 50.f, 60.f, 70.f, 80.f, 90.f, 100.f, 200.f, 300.f, 400.f, 500.f, 
		600.f, 700.f, 800.f, 900.f, 1000.f, 2000.f, 3000.f, 4000.f, 5000.f, 6000.f, 7000.f, 8000.f, 9000.f, 10000.f, 20000.f };
	const auto minFreq = freqs.front();
	const auto maxFreq = freqs.back();
	const auto freqLogBase = std::pow(maxFreq / minFreq, 1 / static_cast<float>(width));
	for(auto freq : freqs)
	{
		g.setColour(Colours::darkgrey);
		if (std::find(baseFreqs.begin(), baseFreqs.end(), freq) != baseFreqs.end())
		{
			g.setColour(Colours::grey);
		}
		const auto x = std::log(freq / minFreq) / std::log(freqLogBase);
		g.drawVerticalLine(x, 0, getHeight());
	}

	g.setColour(Colours::darkgrey);
	g.drawHorizontalLine(jmap<float>(12.f, -12.f, 12.f, getHeight(), 0.f), 0, getWidth());
	g.drawHorizontalLine(jmap<float>(6, -12.f, 12.f, getHeight(), 0.f), 0, getWidth());
	g.drawHorizontalLine(jmap<float>(0, -12.f, 12.f, getHeight(), 0.f), 0, getWidth());
	g.drawHorizontalLine(jmap<float>(-6.f, -12.f, 12.f, getHeight(), 0.f), 0, getWidth());
	g.drawHorizontalLine(jmap<float>(-12.f, -12.f, 12.f, getHeight(), 0.f), 0, getWidth());
	
	auto bassCoeffs = processor.getBassFilterCoeffs();
	auto midCoeffs = processor.getMidFilterCoeffs();
	auto trebleCoeffs = processor.getTrebleFilterCoeffs();

	Path bassPath;
	Path midPath;
	Path treblePath;

	auto bassGain = bassCoeffs.getMagnitudeForFrequency(0, 44100.f);
	auto midGain = midCoeffs.getMagnitudeForFrequency(0, 44100.f);
	auto trebleGain = trebleCoeffs.getMagnitudeForFrequency(0, 44100.f);

	auto bassDb = jlimit<float>(-12.f, 12.f, Decibels::gainToDecibels(bassGain));
	auto midDb = jlimit<float>(-12.f, 12.f, Decibels::gainToDecibels(midGain));
	auto trebleDb = jlimit<float>(-12.f, 12.f, Decibels::gainToDecibels(trebleGain));

	auto bassY = jmap<float>(bassDb, -12.f, 12.f, getHeight(), 0);
	auto midY = jmap<float>(midDb, -12.f, 12.f, getHeight(), 0);
	auto trebleY = jmap<float>(trebleDb, -12.f, 12.f, getHeight(), 0);
	
	bassPath.startNewSubPath(0, bassY);
	midPath.startNewSubPath(0, midY);
	treblePath.startNewSubPath(0, trebleY);
	
	//for (auto freq = 1; freq < MaxFrequenzy; ++freq)
	for(auto x = 0; x < getWidth(); ++x)
	{
		//auto x = jmap<float>(std::log(freq), 0, std::log(MaxFrequenzy), 0, getWidth());//std::log(freq) * delta;
		const float freq = minFreq * std::pow((maxFreq / minFreq), (x / (float)getWidth()));
		
		bassGain = bassCoeffs.getMagnitudeForFrequency(freq, 44100.f);
		midGain = midCoeffs.getMagnitudeForFrequency(freq, 44100.f);
		trebleGain = trebleCoeffs.getMagnitudeForFrequency(freq, 44100.f);
		
		bassDb = jlimit<float>(-12.f, 12.f, Decibels::gainToDecibels(bassGain));
		midDb = jlimit<float>(-12.f, 12.f, Decibels::gainToDecibels(midGain));
		trebleDb = jlimit<float>(-12.f, 12.f, Decibels::gainToDecibels(trebleGain));
		
		bassY = jmap<float>(bassDb, -12.f, 12.f, getHeight(), 0);
		midY = jmap<float>(midDb, -12.f, 12.f, getHeight(), 0);
		trebleY = jmap<float>(trebleDb, -12.f, 12.f, getHeight(), 0);

		bassPath.lineTo(x, bassY);
		midPath.lineTo(x, midY);
		treblePath.lineTo(x, trebleY);
	}
	
	const auto strokeType = PathStrokeType{ 4.f };
	g.setColour(Colours::blue);
	g.strokePath(bassPath, strokeType);
	
	g.setColour(Colours::green);
	g.strokePath(midPath, strokeType);

	g.setColour(Colours::red);
	g.strokePath(treblePath, strokeType);
}

void EqGraph::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..
}

void EqGraph::sliderValueChanged(Slider* slider)
{
	auto res = std::find_if(sliders.begin(), sliders.end(), [slider](auto s) { return s == slider; });
	if (res != sliders.end())
	{
		repaint();
	}
}

void EqGraph::addSliders(const std::vector<Slider*>& newSliders)
{
	for (auto newSlider : newSliders)
	{
		newSlider->addListener(this);
		sliders.push_back(newSlider);
	}
}
