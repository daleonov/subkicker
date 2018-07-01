#include "DLPG_WaveGenerator.h"

namespace dlpg{
WaveGenerator::WaveGenerator(double fSampleRate){
  this-> fSampleRate = fSampleRate;
  this-> fPhaseIncrement = 0.;
}

WaveGenerator::~WaveGenerator(){
}

bool WaveGenerator::SetSampleRate(double fSampleRate){
  this-> fSampleRate = fSampleRate;
  this-> fPhaseIncrement = 0.;
}

bool WaveGenerator::GenerateSamples(std::vector<double> &vBuffer, int nSamples, double fFrequency, double fPhaseShift, WaveForm_t kWaveForm){
  double fSample;
  double fPhase = fPhaseShift;
  CalculateIncrement(fFrequency);
  // Ignoring kWaveForm and generating a sine wave
  for(int i=0; i<nSamples; i++) {
    fSample = sin(fPhase);
    vBuffer.push_back(fSample);
    fPhase += fPhaseIncrement;
    while (fPhase > fTwoPi){
      fPhase -= fTwoPi;
    }
  }
  return true;
}

bool WaveGenerator::Generate(std::vector<double> &vBuffer, double fDuration, double fFrequency, double fPhaseShift, WaveForm_t kWaveForm){
  int nSamples = fDuration * fSampleRate;
  double fSample;
  double fPhase = fPhaseShift;
  CalculateIncrement(fFrequency);
  // Ignoring kWaveForm and generating a sine wave
  for(int i=0; i<nSamples; i++) {
    fSample = sin(fPhase);
    vBuffer.push_back(fSample);
    fPhase += fPhaseIncrement;
    while (fPhase > fTwoPi){
      fPhase -= fTwoPi;
    }
  }
  return true;
}

bool WaveGenerator::CalculateIncrement(double fFrequency){
  fPhaseIncrement = fFrequency * fTwoPi / fSampleRate;
  return true;
}

} //namespace dlpg
