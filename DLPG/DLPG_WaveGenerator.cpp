#include "DLPG_WaveGenerator.h"

namespace dlpg{
WaveGenerator::WaveGenerator(double fSampleRate){
  this-> fSampleRate = fSampleRate;
  // Phase increment is calculated later
  this-> fPhaseIncrement = 0.;
}

WaveGenerator::~WaveGenerator(){
}

bool WaveGenerator::SetSampleRate(double fSampleRate){
  this-> fSampleRate = fSampleRate;
  // Phase increment is calculated later
  this-> fPhaseIncrement = 0.;
  return true;
}

bool WaveGenerator::Generate(std::vector<double> &vBuffer, double fDuration, double fFrequency, double fPhaseShift, WaveForm_t kWaveForm){
  int nSamples = fDuration * fSampleRate;
  double fSample;
  double fPhase = fPhaseShift;
  CalculateIncrement(fFrequency);
  switch(kWaveForm){
  case kSine:
    for(int i=0; i<nSamples; i++) {
      fSample = sin(fPhase);
      vBuffer.push_back(fSample);
      fPhase += fPhaseIncrement;
      while (fPhase > fTwoPi){
        fPhase -= fTwoPi;
      }
    }
    break;
  case kTriangle:
    // Shift the phase, so that triangle wave is in phase with sine, not cosine. 
    fPhase -= fPi/2;
    // Convert negative phase to positive for correct waveform generation. 
    while (fPhase < 0.){
      fPhase += fTwoPi;
    }
    for(int i=0; i<nSamples; i++) {
      fSample = 2. * fabs((fPhase / fPi) - 1.) - 1.;
      vBuffer.push_back(fSample);
      fPhase += fPhaseIncrement;
      while (fPhase > fTwoPi){
        fPhase -= fTwoPi;
      }
    }
    break;
  } // switch
  return true;
}

bool WaveGenerator::CalculateIncrement(double fFrequency){
  fPhaseIncrement = fFrequency * fTwoPi / fSampleRate;
  return true;
}

EnvelopeGenerator::EnvelopeGenerator(double fSampleRate){
  this-> fSampleRate = fSampleRate;
}

EnvelopeGenerator::~EnvelopeGenerator(){
}

bool EnvelopeGenerator::Generate(std::vector<double> &vBuffer, double fDuration, EnvelopeStage_t kEnvelopeStage, EnvelopeShape_t kEnvelopeShape){
  int nSamples = fDuration * fSampleRate;
  double fSampleMultiplier;
  switch(kEnvelopeStage){
  case kAttack:
    for(int i=0; i<nSamples; i++) {
      if(kEnvelopeShape == kLogarithmic)
        // Log
        fSampleMultiplier = log(((fExp-1)/(nSamples-1))*i + 1);
      else
        // Linear
        fSampleMultiplier = (double)i/(nSamples-1);

      vBuffer.push_back(fSampleMultiplier);
    }
    break;
  case kRelease:
    for(int i=0; i<nSamples; i++) {
      if(kEnvelopeShape == kLogarithmic)
        // Log
        fSampleMultiplier = 1.-log(((fExp-1)/(nSamples-1))*i + 1);
      else
        // Linear
        fSampleMultiplier = 1.-((double)i/(nSamples-1));

      vBuffer.push_back(fSampleMultiplier);
    }
    break;
  default:
    break;
  } //switch
  return true;
}

bool EnvelopeGenerator::SetSampleRate(double fSampleRate){
  this-> fSampleRate = fSampleRate;
  return true;
}

} //namespace dlpg
