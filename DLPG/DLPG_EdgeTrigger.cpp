#include "DLPG_EdgeTrigger.h"

namespace dlpg{

EdgeTrigger::EdgeTrigger(double fSampleRate, double fHoldTime, double fThresholdLinear):
	fSampleRate(fSampleRate),
	fThresholdLinear(fThresholdLinear)
{
	nHoldSamples = std::floor(fSampleRate * fHoldTime);
}

EdgeTrigger::~EdgeTrigger(){
}

bool EdgeTrigger::SetSampleRate(double fSampleRate){
	// In Hz
	this->fSampleRate = fSampleRate;
	// Refresh time-related values
	SetHoldTime(this->fHoldSeconds);
	return true;
}

bool EdgeTrigger::SetHoldTime(double fHoldTime){
	this->fHoldSeconds = fHoldTime;
	this->nHoldSamples = std::floor(fSampleRate * fHoldTime);
	return true;
}

bool EdgeTrigger::SetThresholdLinear(double fThresholdLinear){
	this->fThresholdLinear = fThresholdLinear;
	return true;
}

TriggerState_t EdgeTrigger::ProcessMonoSampleLinear(double fSampleLinear){
	/*
	How the states should work:
	... off off off RISING on on on on on FALLING off off off off RISING on on ...
	i.e. rising and falling edge states last for only 1 sample, on and off states
	can last fo any amount of samples.

	Scenarios:
	1. Sample value higher than thresh, previous sample is lower than thresh and hold = 0 -> kTriggerRisingEdge
	2. Sample has any value, but hold > 1 -> kTriggerOn
	3. Sample has any value, but hold = 1 -> kTriggerFalling edge
	4. Sample value is below the thresh and hold = 0 -> kTriggerOff
	*/

	return kNone;
}

TriggerState_t EdgeTrigger::ProcessStereoSampleLinear(double fSampleLeftLinear, double fSampleRightLinear){
	return kNone;
}

} //namespace
