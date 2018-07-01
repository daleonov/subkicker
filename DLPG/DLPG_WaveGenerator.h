
#ifndef _DLPG_WAVEGENERATOR_H
#define _DLPG_WAVEGENERATOR_H
#include <vector>

#ifndef DLPG_DEFAULT_SAMPLERATE_F
#define DLPG_DEFAULT_SAMPLERATE_F 44100.
#endif

namespace dlpg{

const double fPi = 3.14159265359;
const double fTwoPi = 6.28318530718;

typedef enum WaveForm_t{
  kSine = 1,
}WaveForm_t;

class WaveGenerator{

public:
  WaveGenerator(double fSampleRate = DLPG_DEFAULT_SAMPLERATE_F);

  ~WaveGenerator();
  /*
  @param Graphic part. Sizes and colours are based on macros defined in the header. 
  */
  bool Generate(std::vector<double>& vBuffer, WaveForm_t kWaveForm);
  bool SetSampleRate(double fSampleRate);
private:
  double fSampleRate;

};

} //namespace dlpg

#endif //_PLUG_WAVEGENERATOR_H
