
#ifndef _DLPG_WAVEGENERATOR_H
#define _DLPG_WAVEGENERATOR_H
#include <vector>
#include <math.h>

#ifndef DLPG_DEFAULT_SAMPLERATE_HZ_F
#define DLPG_DEFAULT_SAMPLERATE_HZ_F 44100.
#endif

namespace dlpg{

const double fPi = 3.14159265359;
const double fTwoPi = 6.28318530718;
const double fExp = 2.718281828;

#ifndef DLPG_DEG_TO_RADIANS
#define DLPG_DEG_TO_RADIANS(alpha) (alpha * dlpg::fPi/180.)
#endif

typedef enum WaveForm_t{
  kSine,
  kTriangle,
}WaveForm_t;

typedef enum EnvelopeShape_t{
  kLinear,
  kLogarithmic,
}EnvelopeShape_t;

typedef enum EnvelopeStage_t{
  kAttack,
  kHold,
  kDecay,
  kSustain,
  kRelease
}EnvelopeStage_t;

class WaveGenerator{

public:
  WaveGenerator(double fSampleRate = DLPG_DEFAULT_SAMPLERATE_HZ_F);
  ~WaveGenerator();
  /*
  @param Graphic part. Sizes and colours are based on macros defined in the header. 
  */
  bool GenerateSamples(std::vector<double> &vBuffer, int nSamples, double fFrequency, double fPhase = 0., WaveForm_t kWaveForm = kSine);

  /*
  @brief Fills a buffer with a generated signal.
  @param vBuffer buffer to fill with values (-1..1 range)
  @param fDuration signal duration in seconds
  @param fFrequency desired signal frequency in Hz
  @param fPhase phase shift of signal in radians
  @param kWaveForm waveform type; for suture use, in this version it's always a sine wave.
  @retval always true
  */
  bool Generate(std::vector<double> &vBuffer, double fDuration, double fFrequency, double fPhase = 0., WaveForm_t kWaveForm = kSine);

  /*
  @brief Set sample rate so that the generator can take time-based parameters correctly 
  @param fSampleRate sample rate in Hz (e.g. 44100.)
  */
  bool SetSampleRate(double fSampleRate);
private:
  bool CalculateIncrement(double fFrequency);
  double fSampleRate;
  double fPhaseIncrement;

};

class EnvelopeGenerator{

public:
  EnvelopeGenerator(double fSampleRate = DLPG_DEFAULT_SAMPLERATE_HZ_F);
  ~EnvelopeGenerator();

  /*
  @brief Fills a buffer with a generated signal.
  @param vBuffer buffer to fill with values (-1..1 range)
  @param fDuration envelope duration in seconds
  @param kEnvelopeStage envelope stage (A, H, D, S, R) (see @ref EnvelopeStage_t)
  @param kEnvelopeShape envelope shape (see @ref EnvelopeShape_t)
  @retval always true
  */
  bool Generate(std::vector<double> &vBuffer, double fDuration, EnvelopeStage_t kEnvelopeStage, EnvelopeShape_t kEnvelopeShape = kLogarithmic);

  /*
  @brief Set sample rate so that the generator can take time-based parameters correctly 
  @param fSampleRate sample rate in Hz (e.g. 44100.)
  */
  bool SetSampleRate(double fSampleRate);

private:
  double fSampleRate;
};

} //namespace dlpg

#endif //_PLUG_WAVEGENERATOR_H
