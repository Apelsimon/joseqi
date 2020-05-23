#pragma once

#include "../JuceLibraryCode/JuceHeader.h"

namespace EqParameters
{
	static const String BassGainId{ "bassGain" };
	static const String BassQId{"bassQ"};
	static const String BassFreqId{ "bassFreq" };
	static const String MidGainId{ "midGain" };
	static const String MidQId{ "midQ" };
	static const String MidFreqId{ "midFreq" };
	static const String TrebleGainId{ "trebleGain" };
	static const String TrebleQId{ "trebleQ" };
	static const String TrebleFreqId{ "trebleFreq" };
	
	static const Range<float> BassFreqRange{100.f, 300.f};
	static const Range<float> MidFreqRange{300.f, 3000.f};
	static const Range<float> TrebleFreqRange{3000.f, 10000.f};
	static const Range<float> GainRange{-12.f, 12.f};
	static const Range<float> QRange{0.1f, 10.f};

	static const float BassFreqDefault{150.f};
	static const float MidFreqDefault{500.f};
	static const float TrebleFreqDefault{6000.f};
	static const float GainDefault{ 0 };
	static const float QDefault{ std::sqrt(2.f) };

	static void addEqParametersListener(AudioProcessorValueTreeState& parameters, AudioProcessorValueTreeState::Listener* listener)
	{
		parameters.addParameterListener(EqParameters::BassGainId, listener);
		parameters.addParameterListener(EqParameters::BassQId, listener);
		parameters.addParameterListener(EqParameters::BassFreqId, listener);
		parameters.addParameterListener(EqParameters::MidGainId, listener);
		parameters.addParameterListener(EqParameters::MidQId, listener);
		parameters.addParameterListener(EqParameters::MidFreqId, listener);
		parameters.addParameterListener(EqParameters::TrebleGainId, listener);
		parameters.addParameterListener(EqParameters::TrebleQId, listener);
		parameters.addParameterListener(EqParameters::TrebleFreqId, listener);
	}
} 

