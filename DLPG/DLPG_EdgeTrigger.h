#ifndef __DLPG_EDGE_TRIGGER_H
#define __DLPG_EDGE_TRIGGER_H

#include <cmath>

#ifndef DLPG_DEFAULT_SAMPLERATE_HZ_F
#define DLPG_DEFAULT_SAMPLERATE_HZ_F 44100.
#endif

namespace dlpg{
typedef enum TriggerState_t{
	kNone,
	kTriggerRisingEdge,
	kTriggerOn,
	kTriggerFallingEdge,
	kTriggerOff
}TriggerState_t;

class EdgeTrigger{
public:
	EdgeTrigger(double fSampleRate, double fHoldTime, double fThresholdLinear);
	~EdgeTrigger();
	/*
	@brief Set sample rate so that the trigger can take time-based parameters correctly 
  @param fSampleRate sample rate in Hz (not kHz!)
	@param fHoldTime hold time in seconds (not ms!)
	@param fThresholdLinear linear threshold level, 0..1 range
  @retval always true
	*/
	bool SetSampleRate(double fSampleRate);

	/*
	@brief Set hold time
	@param fHoldTime hold time in seconds (not ms!)
  @retval always true
	*/
	bool SetHoldTime(double fHoldTime);

	/*
	@brief Set threshold level
	@param fThresholdLinear linear threshold level, 0..1 range
  @retval always true
	*/
	bool SetThresholdLinear(double fThresholdLinear);

	/*
	@brief Add sample for mono signal
	@param fSampleLinear Sample value in linear domain, -1..1 range
  @retval State of the trigger after the sample was processed, see @ref TriggerState_t
	*/
	TriggerState_t ProcessMonoSampleLinear(double fSampleLinear);

	/*
	@brief Add sample for mono signal
	@param fSampleLeftLinear Sample value (left channel) in linear domain, -1..1 range
	@param fSampleRightLinear Sample value (right channel) in linear domain, -1..1 range
  @retval State of the trigger after the sample was processed, see @ref TriggerState_t
	*/
	TriggerState_t ProcessStereoSampleLinear(double fSampleLeftLinear, double fSampleRightLinear);

private:
	double fSampleRate;
	double fThresholdLinear;
	double fHoldSeconds;
	unsigned long nHoldSamples;
};

} //namespace dlpg
#endif // __DLPG_EDGE_TRIGGER_H
