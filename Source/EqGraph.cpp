#include "EqGraph.h"

#include "EqParameters.h"

//==============================================================================
EqGraph::EqGraph(JoseqiAudioProcessor& p, AudioProcessorValueTreeState& parameters) :
	processor(p),
	baseFrequencies({ 20.f, 50.f, 100.f, 200.f, 500.f, 1000.f, 2000.f, 5000.f, 10000.f, 20000.f }),
	incrementalFrequencies()
{
	EqParameters::addEqParametersListener(parameters, this);

	const auto maxFreq = baseFrequencies.back();
	const auto minFreq = baseFrequencies.front();

	auto inc = 10;
	auto targetFreq = 100;

	for (int freq = minFreq; freq <= maxFreq; freq += inc)
	{
		incrementalFrequencies.push_back(freq);
		if (freq % targetFreq == 0)
		{
			inc *= 10;
			targetFreq *= 10;
		}
	}
}

EqGraph::~EqGraph()
{
}

void EqGraph::paint (Graphics& g)
{
	g.fillAll(Colours::black);

	constexpr auto Padding = 30.f;
	auto bounds = getLocalBounds().reduced(Padding);

	g.setColour(Colours::grey);
    g.drawRect (bounds, 1);
	g.drawRect(getLocalBounds(), 1);

	constexpr auto TextWidth = 20.f;
	constexpr auto TextHeight = TextWidth;
	const auto width = bounds.getWidth();
	const auto height = bounds.getHeight();
	const auto left = bounds.getTopLeft().getX();
	const auto top = bounds.getTopLeft().getY();
	const auto right = bounds.getRight();
	const auto bottom = bounds.getBottom();

	const auto minFreq = incrementalFrequencies.front();
	const auto maxFreq = incrementalFrequencies.back();
	const auto freqLogBase = std::pow(maxFreq / minFreq, 1 / static_cast<float>(width));
	for(auto freq : incrementalFrequencies)
	{
		auto drawFreqNumber = std::find(baseFrequencies.begin(), baseFrequencies.end(), freq) != baseFrequencies.end();
		auto colour = drawFreqNumber ? Colours::grey : Colours::darkgrey;
		g.setColour(colour);

		const auto x = left + std::log(freq / minFreq) / std::log(freqLogBase);
		g.drawVerticalLine(x, top, bottom);

		if (drawFreqNumber)
		{
			g.drawFittedText(getFrequencyString(freq), x - 10, bottom, TextWidth, TextHeight, Justification::centred, 1);
		}
	}

	constexpr auto MaxDb = 24.f;
	constexpr auto MinDb = -MaxDb;
	const auto justification = Justification::centred;

	for(auto db = 0.f; db < MaxDb; db += 6.f)
	{ 
		const auto nrOfSignificantNumbers = db < 10 ? 1 : 2;
		const auto posY = jmap<float>(db, MinDb, MaxDb, bottom, top);
		const auto negY = jmap<float>(-db, MinDb, MaxDb, bottom, top);

		g.setColour(Colours::darkgrey);
		g.drawHorizontalLine(posY, left, right);
		g.drawHorizontalLine(negY, left, right); 

		g.setColour(Colours::grey);
		g.drawFittedText((db > 0 ? "+" : "") + String::toDecimalStringWithSignificantFigures(+db, nrOfSignificantNumbers), 0, posY - 10, TextWidth, TextHeight, justification, 1);
		g.drawFittedText(String::toDecimalStringWithSignificantFigures(-db, nrOfSignificantNumbers), 0, negY - 10, TextWidth, TextHeight, justification, 1);
	}
	g.drawFittedText("+" + String::toDecimalStringWithSignificantFigures(MaxDb, 2), 0, top - 10, TextWidth, TextHeight, justification, 1);
	g.drawFittedText(String::toDecimalStringWithSignificantFigures(MinDb, 2), 0, bottom - 10, TextWidth, TextHeight, justification, 1);
	
	auto bassFilterCoeffs = processor.getBassFilterCoeffs();
	auto midFilterCoeffs = processor.getMidFilterCoeffs();
	auto trebleFilterCoeffs = processor.getTrebleFilterCoeffs();

	Path bassPath;
	Path midPath;
	Path treblePath;
	
	for(auto x = left; x < right; ++x)
	{
		const float freq = minFreq * std::pow((maxFreq / minFreq), ((x - left) / static_cast<float>(width)));
		
		auto bassGain = bassFilterCoeffs.getMagnitudeForFrequency(freq, 44100.f);
		auto midGain = midFilterCoeffs.getMagnitudeForFrequency(freq, 44100.f);
		auto trebleGain = trebleFilterCoeffs.getMagnitudeForFrequency(freq, 44100.f);
		
		auto bassDb = jlimit<float>(MinDb, MaxDb, Decibels::gainToDecibels(bassGain));
		auto midDb = jlimit<float>(MinDb, MaxDb, Decibels::gainToDecibels(midGain));
		auto trebleDb = jlimit<float>(MinDb, MaxDb, Decibels::gainToDecibels(trebleGain));
		
		auto bassY = jmap<float>(bassDb, MinDb, MaxDb, bottom, top);
		auto midY = jmap<float>(midDb, MinDb, MaxDb, bottom, top);
		auto trebleY = jmap<float>(trebleDb, MinDb, MaxDb, bottom, top);

		if(x == left)
		{
			bassPath.startNewSubPath(x, bassY);
			midPath.startNewSubPath(x, midY);
			treblePath.startNewSubPath(x, trebleY);
		}
		else
		{
			bassPath.lineTo(x, bassY);
			midPath.lineTo(x, midY);
			treblePath.lineTo(x, trebleY);
		}
		
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