#include "EqGraph.h"

#include "EqParameters.h"

//==============================================================================
EqGraph::EqGraph(JoseqiAudioProcessor& p, AudioProcessorValueTreeState& parameters) :
	processor(p)
{
	EqParameters::addEqParametersListener(parameters, this);
}

EqGraph::~EqGraph()
{
}

void EqGraph::paint (Graphics& g)
{
    g.fillAll (Colours::black);

	auto bounds = getLocalBounds().reduced(30);
    g.setColour (Colours::grey);
    g.drawRect (bounds, 1);
	g.drawRect(getLocalBounds(), 1);
      
	constexpr auto MaxDb = 24.f;
	constexpr auto MinDb = -MaxDb;
	const auto width = bounds.getWidth();
	const auto height = bounds.getHeight();
	const auto left = bounds.getTopLeft().getX();
	const auto top = bounds.getTopLeft().getY();
	const auto right = bounds.getRight();
	const auto bottom = bounds.getBottom();

	std::vector<float> baseFreqs{ 20.f, 50.f, 100.f, 200.f, 500.f, 1000.f, 2000.f, 5000.f, 10000.f, 20000.f };
	std::vector<float> freqs{ 20.f, 30.f, 40.f, 50.f, 60.f, 70.f, 80.f, 90.f, 100.f, 200.f, 300.f, 400.f, 500.f, 
		600.f, 700.f, 800.f, 900.f, 1000.f, 2000.f, 3000.f, 4000.f, 5000.f, 6000.f, 7000.f, 8000.f, 9000.f, 10000.f, 20000.f };

	const auto minFreq = freqs.front();
	const auto maxFreq = freqs.back();
	const auto freqLogBase = std::pow(maxFreq / minFreq, 1 / static_cast<float>(width));
	for(auto freq : freqs)
	{
		auto drawFreqNumber = std::find(baseFreqs.begin(), baseFreqs.end(), freq) != baseFreqs.end();
		auto colour = drawFreqNumber ? Colours::grey : Colours::darkgrey;
		g.setColour(colour);

		const auto x = left + std::log(freq / minFreq) / std::log(freqLogBase);
		g.drawVerticalLine(x, top, bottom);
		if (drawFreqNumber)
		{
			g.drawFittedText(getFrequencyString(freq), x - 10, bottom, 20, 20, Justification::centred, 1);
		}
	}

	
	for(auto db = 0.f; db < MaxDb; db += 6.f)
	{ 
		const auto nrOfSignificantNumbers = db < 10 ? 1 : 2;
		const auto posY = jmap<float>(db, MinDb, MaxDb, bottom, top);
		const auto negY = jmap<float>(-db, MinDb, MaxDb, bottom, top);

		g.setColour(Colours::darkgrey);
		g.drawHorizontalLine(posY, left, right);
		g.drawHorizontalLine(negY, left, right); 
		g.setColour(Colours::grey);
		g.drawFittedText((db > 0 ? "+" : "") + String::toDecimalStringWithSignificantFigures(+db, nrOfSignificantNumbers), 0, posY - 10, 20, 20, Justification::centred, 1);
		g.drawFittedText(String::toDecimalStringWithSignificantFigures(-db, nrOfSignificantNumbers), 0, negY - 10, 20, 20, Justification::centred, 1);
	}
	g.drawFittedText("+" + String::toDecimalStringWithSignificantFigures(MaxDb, 2), 0, top - 10, 20, 20, Justification::centred, 1);
	g.drawFittedText(String::toDecimalStringWithSignificantFigures(MinDb, 2), 0, bottom - 10, 20, 20, Justification::centred, 1);
	
	auto bassCoeffs = processor.getBassFilterCoeffs();
	auto midCoeffs = processor.getMidFilterCoeffs();
	auto trebleCoeffs = processor.getTrebleFilterCoeffs();

	Path bassPath;
	Path midPath;
	Path treblePath;

	auto bassGain = bassCoeffs.getMagnitudeForFrequency(minFreq, 44100.f);
	auto midGain = midCoeffs.getMagnitudeForFrequency(minFreq, 44100.f);
	auto trebleGain = trebleCoeffs.getMagnitudeForFrequency(minFreq, 44100.f);

	auto bassDb = jlimit<float>(MinDb, MaxDb, Decibels::gainToDecibels(bassGain));
	auto midDb = jlimit<float>(MinDb, MaxDb, Decibels::gainToDecibels(midGain));
	auto trebleDb = jlimit<float>(MinDb, MaxDb, Decibels::gainToDecibels(trebleGain));

	auto bassY = jmap<float>(bassDb, MinDb, MaxDb, bottom, top);
	auto midY = jmap<float>(midDb, MinDb, MaxDb, bottom, top);
	auto trebleY = jmap<float>(trebleDb, MinDb, MaxDb, bottom, top);
	
	bassPath.startNewSubPath(left, bassY);
	midPath.startNewSubPath(left, midY);
	treblePath.startNewSubPath(left, trebleY);
	
	for(auto x = left; x < right; ++x)
	{
		const float freq = minFreq * std::pow((maxFreq / minFreq), ((x - left) / static_cast<float>(width)));
		
		bassGain = bassCoeffs.getMagnitudeForFrequency(freq, 44100.f);
		midGain = midCoeffs.getMagnitudeForFrequency(freq, 44100.f);
		trebleGain = trebleCoeffs.getMagnitudeForFrequency(freq, 44100.f);
		
		auto bassDb = jlimit<float>(MinDb, MaxDb, Decibels::gainToDecibels(bassGain));
		auto midDb = jlimit<float>(MinDb, MaxDb, Decibels::gainToDecibels(midGain));
		auto trebleDb = jlimit<float>(MinDb, MaxDb, Decibels::gainToDecibels(trebleGain));
		
		bassY = jmap<float>(bassDb, MinDb, MaxDb, bottom, top);
		midY = jmap<float>(midDb, MinDb, MaxDb, bottom, top);
		trebleY = jmap<float>(trebleDb, MinDb, MaxDb, bottom, top);

		bassPath.lineTo(x, bassY);
		midPath.lineTo(x, midY);
		treblePath.lineTo(x, trebleY);
	}
	
	const auto strokeType = PathStrokeType{ 3.f };
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

void EqGraph::parameterChanged(const String &parameterID, float newValue)
{
	DBG("param change");
	repaint();
}

String EqGraph::getFrequencyString(int freq)
{
	if (freq < 1000)
	{
		return String::toDecimalStringWithSignificantFigures(freq, 1);
	}

	switch (freq)
	{
	case 1000:
		return { "1k" };
	case 2000:
		return { "2k" };
	case 5000:
		return { "5k" };
	case 10000:
		return { "10k" };
	case 20000:
		return { "20k" };
	default:
		return {};
	}
}