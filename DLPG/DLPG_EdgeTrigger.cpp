#include "DLPG_EdgeTrigger.h"

namespace dlpg{

EdgeTrigger::EdgeTrigger(double fSampleRate, double fHoldTime, double fThresholdLinear):
	fPreviousSampleLinear(0.),
	fSampleRate(fSampleRate),
	fThresholdLinear(fThresholdLinear)
{
	nHoldSamples = std::floor(fSampleRate * fHoldTime);
	nHoldSampleCounter = nHoldSamples;
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
	nHoldSampleCounter = nHoldSamples;
	return true;
}

bool EdgeTrigger::SetThresholdLinear(double fThresholdLinear){
	this->fThresholdLinear = fabs(fThresholdLinear);
	return true;
}

TriggerState_t EdgeTrigger::ProcessMonoSampleLinear(double fSampleLinear){
	/*
	How the states should work:
	... off off off RISING on on on on on FALLING off off off off RISING on on ...
	i.e. rising and falling edge states last for only 1 sample, on and off states
	can last fo any amount of samples.

	Trigger switches on instantly after signal crosses threshold in upward direction.
	Trigger switches off when:
		a. Hold time ends and the signal is below threshold at that moment.
		b. Hold time ends when signal us above threshold - it's still on, and turns off
		   once the signal gets below the threshold. 

	Scenarios:
	1. Sample value higher than thresh, previous sample is lower than thresh and nHoldSampleCounter = nHoldSamples -> kTriggerRisingEdge
	2. Sample has any value, but nHoldSampleCounter > 1 -> kTriggerOn
	3. Sample is lower than thresh, nHoldSampleCounter = 1 -> kTriggerFallingEdge, reset nHoldSampleCounter
	4. Sample is higher than thresh, nHoldSampleCounter = 1 -> kTriggerOn, and stop decreasing nHoldSampleCounter
	5. Sample value is below the thresh and nHoldSampleCounter = 0 -> kTriggerOff
	*/
	TriggerState_t eState = kNone;

	// Rectify the input signal
	fSampleLinear = fabs(fSampleLinear);

	// Scenario #1
	if(
		(nHoldSampleCounter == nHoldSamples) && \
		(fSampleLinear >= fThresholdLinear) && \
		(fPreviousSampleLinear < fThresholdLinear)
		){
		eState = kTriggerRisingEdge;
		nHoldSampleCounter--;
		fPreviousSampleLinear = fSampleLinear;
		return eState;
	}

	// Scenario #2
	if((nHoldSampleCounter > 1) && (nHoldSampleCounter < nHoldSamples)){
		eState = kTriggerOn;
		nHoldSampleCounter--;
		fPreviousSampleLinear = fSampleLinear;
		return eState;
	}

	if(nHoldSampleCounter == 1){
		if(fSampleLinear < fThresholdLinear){
		// Scenario #3
			eState = kTriggerFallingEdge;
			// Reset the hold sample counter
			nHoldSampleCounter = nHoldSamples;
			fPreviousSampleLinear = fSampleLinear;
			return eState;
		}
		else{
			// Scenario #4
			eState = kTriggerOn;
			// Not changing nHoldSampleCounter, waiting for scenario #3 to occur
			fPreviousSampleLinear = fSampleLinear;
			return eState;
		}
	}

	// Scenario #5
	if((nHoldSampleCounter == nHoldSamples) && (fSampleLinear < fThresholdLinear)){
		eState = kTriggerOff;
		// Not changing nHoldSampleCounter, waiting for scenario #1 to occur
		fPreviousSampleLinear = fSampleLinear;
		return eState;
	}

	/*
	In theory, the program should never reach this point. If it does, there's
	more scenarios then described above - one should try to find and cover it.
	*/
	return eState;
}

TriggerState_t EdgeTrigger::ProcessStereoSampleLinear(double fSampleLeftLinear, double fSampleRightLinear){
	// Well, it's still a mono trigger.
	double fSampleMonoLinear = (fabs(fSampleLeftLinear) + fabs(fSampleRightLinear)) / 2;
	return ProcessMonoSampleLinear(fSampleMonoLinear);
}

bool EdgeTrigger::Reset(){
	nHoldSampleCounter = nHoldSamples;
	fPreviousSampleLinear = 0.;
}

} //namespace
