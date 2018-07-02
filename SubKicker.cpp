#include "SubKicker.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

SubKicker::SubKicker(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo), mGain(1.)
{
  TRACE;

  //arguments are: name, defaultVal, minVal, maxVal, step, label
  GetParam(kGain)->InitDouble("Gain", 50., 0., 100.0, 0.01, "%");
  GetParam(kGain)->SetShape(2.);

  IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
  const IColor *tBgColor = new IColor(255, 40, 40, 40);
  pGraphics->AttachPanelBackground(tBgColor);

  IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, kKnobFrames);

  pGraphics->AttachControl(new IKnobMultiControl(this, kGainX, kGainY, kGain, &knob));

  double fDuration = 300./1000;
  double fAttack = 50./1000;
  double fSampleRate = 44100.;
  double fFrequency = 400.;

  // Wave stuff
  std::vector<double> vWaveform(0), vAttackEnvelope(0);
  tWaveGenerator = new dlpg::WaveGenerator();
  tEnvelopeGenerator = new dlpg::EnvelopeGenerator();
  tWaveGenerator->Generate(vWaveform, fDuration, fFrequency);
  tEnvelopeGenerator->Generate(vAttackEnvelope, fAttack, dlpg::kAttack, dlpg::kLogarithmic);

  for(std::vector<double>::size_type i = 0; i != vAttackEnvelope.size(); i++){
    vWaveform[i] *= vAttackEnvelope[i];
  }

  // Scope
  tScope = new dlpg::IWavScopeControl(this, PLUG_ScopeIrect, kScope, vWaveform);
  tScope->UpdateScale(fDuration, fSampleRate);
  pGraphics->AttachControl(tScope);

  AttachGraphics(pGraphics);

  //tScope->LoadWave(&vWaveform);
  //MakePreset("preset 1", ... );
  MakeDefaultPreset((char *) "-", kNumPrograms);
}

SubKicker::~SubKicker() {}

void SubKicker::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
  // Mutex is already locked for us.

  double* in1 = inputs[0];
  double* in2 = inputs[1];
  double* out1 = outputs[0];
  double* out2 = outputs[1];

  for (int s = 0; s < nFrames; ++s, ++in1, ++in2, ++out1, ++out2)
  {
    *out1 = *in1 * mGain;
    *out2 = *in2 * mGain;
  }
}

void SubKicker::Reset()
{
  TRACE;
  IMutexLock lock(this);
}

void SubKicker::OnParamChange(int paramIdx)
{
  IMutexLock lock(this);

  switch (paramIdx)
  {
    case kGain:
      mGain = GetParam(kGain)->Value() / 100.;
      break;

    default:
      break;
  }
}
