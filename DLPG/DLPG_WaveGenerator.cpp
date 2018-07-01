#include "DLPG_WaveGenerator.h"

namespace dlpg{
WaveGenerator::WaveGenerator(double fSampleRate){
  this-> fSampleRate = fSampleRate;
}

WaveGenerator::~WaveGenerator(){
}

bool WaveGenerator::SetSampleRate(double fSampleRate){
  this-> fSampleRate = fSampleRate;
}

bool WaveGenerator::Generate(std::vector<double>& vBuffer, WaveForm_t kWaveForm){
  return true;
}

} //namespace dlpg
