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

  GetParam(kBypassSwitch)->InitEnum("Bypass", DLPG_DEFAULT_BYPASS_SWITCH_STATE, DLPG_SWITCH_STATES);
  GetParam(kBypassSwitch)->SetDisplayText(0, "Normal");
  GetParam(kBypassSwitch)->SetDisplayText(1, "Bypassed");
  GetParam(kTrigSwitch)->InitEnum("Trigger", DLPG_DEFAULT_TRIG_SWITCH_STATE, DLPG_SWITCH_STATES);
  GetParam(kTrigSwitch)->SetDisplayText(0, "Internal");
  GetParam(kTrigSwitch)->SetDisplayText(1, "External");
  GetParam(kSnapSwitch)->InitEnum("Frequency snap", DLPG_DEFAULT_SNAP_SWITCH_STATE, DLPG_SWITCH_STATES);
  GetParam(kSnapSwitch)->SetDisplayText(0, "to Hz");
  GetParam(kSnapSwitch)->SetDisplayText(1, "to notes");
  GetParam(kFlipSwitch)->InitEnum("Phase flip", DLPG_DEFAULT_FLIP_SWITCH_STATE, DLPG_SWITCH_STATES);
  GetParam(kFlipSwitch)->SetDisplayText(0, "off");
  GetParam(kFlipSwitch)->SetDisplayText(1, "invert");
  GetParam(kFreezeSwitch)->InitEnum("Freeze", DLPG_DEFAULT_FREEZE_SWITCH_STATE, DLPG_SWITCH_STATES);
  GetParam(kFreezeSwitch)->SetDisplayText(0, "off");
  GetParam(kFreezeSwitch)->SetDisplayText(1, "on");

  IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
  const IColor *tBgColor = new IColor(255, 40, 40, 40);
  pGraphics->AttachPanelBackground(tBgColor);

  IBitmap tBmp = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, kKnobFrames);
  pGraphics->AttachControl(new IKnobMultiControl(this, kGainX, kGainY, kGain, &tBmp));

  tBmp = pGraphics->LoadIBitmap(DLPG_BYPASS_SWITCH_ID, DLPG_BYPASS_SWITCH_FN, DLPG_SWITCH_STATES);
  tBypassSwitch = new ISwitchControl(this, kBypassSwitchX, kBypassSwitchY, kBypassSwitch, &tBmp);
  pGraphics->AttachControl(tBypassSwitch);
  tBmp = pGraphics->LoadIBitmap(DLPG_TRIG_SWITCH_ID, DLPG_TRIG_SWITCH_FN, DLPG_SWITCH_STATES);
  tTrigSwitch = new ISwitchControl(this, kTrigSwitchX, kTrigSwitchY, kTrigSwitch, &tBmp);
  pGraphics->AttachControl(tTrigSwitch);
  tBmp = pGraphics->LoadIBitmap(DLPG_SNAP_SWITCH_ID, DLPG_SNAP_SWITCH_FN, DLPG_SWITCH_STATES);
  tSnapSwitch = new ISwitchControl(this, kSnapSwitchX, kSnapSwitchY, kSnapSwitch, &tBmp);
  pGraphics->AttachControl(tSnapSwitch);
  tBmp = pGraphics->LoadIBitmap(DLPG_FLIP_SWITCH_ID, DLPG_FLIP_SWITCH_FN, DLPG_SWITCH_STATES);
  tFlipSwitch = new ISwitchControl(this, kFlipSwitchX, kFlipSwitchY, kFlipSwitch, &tBmp);
  pGraphics->AttachControl(tFlipSwitch);
  tBmp = pGraphics->LoadIBitmap(DLPG_FREEZE_SWITCH_ID, DLPG_FREEZE_SWITCH_FN, DLPG_SWITCH_STATES);
  tFreezeSwitch = new ISwitchControl(this, kFreezeSwitchX, kFreezeSwitchY, kFreezeSwitch, &tBmp);
  pGraphics->AttachControl(tFreezeSwitch);

  double fDuration = 300./1000;
  double fAttack = 20./1000;
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
