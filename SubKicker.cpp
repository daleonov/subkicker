#include "SubKicker.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

SubKicker::SubKicker(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo), mGain(1.)
{
  TRACE;
  IBitmap tBmp;

  //arguments are: name, defaultVal, minVal, maxVal, step, label
  GetParam(kTrigNoteKnob)->InitInt("Trigger (ext) | Midi note", -1, -1, 127, "");
  GetParam(kTrigChKnob)->InitInt("Trigger (ext) | Midi channel", 0, 0, 16, "");
  GetParam(kTrigAttackKnob)->InitDouble("Trigger (int) | Attack", 10., 0.1, 100., 0.1, "ms");
  GetParam(kTrigThreshKnob)->InitDouble("Trigger (int) | Threshold", -6., -60., 0., 0.1, "dB");

  GetParam(kSubFreqKnob)->InitDouble("Sub | Frequency", 100., 20., 500., 0.1, "Hz");
  // And another freq knob for snapped mode?
  GetParam(kSubPhaseKnob)->InitDouble("Sub | Phase", 0., -180., 180., 0.1, "Deg");

  GetParam(kEnvelopeAttackKnob)->InitDouble("Envelope | Attack", 10., 0.1, 100., 0.1, "ms");
  GetParam(kEnvelopeHoldKnob)->InitDouble("Envelope | Hold", 100., 0.1, 1000., 0.1, "ms");
  GetParam(kEnvelopeReleaseKnob)->InitDouble("Envelope | Release", 10., 0.1, 100., 0.1, "ms");

  GetParam(kVolKnob)->InitDouble("Volume", 0., -60., 12., 0.1, "dB");
  GetParam(kVolKnob)->SetShape(DLPG_VOL_KNOB_SHAPE);

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
  GetParam(kTrigInputSwitch)->InitEnum("Input source", DLPG_DEFAULT_TRIG_INPUT_SWITCH_STATE, DLPG_SWITCH_STATES);
  GetParam(kTrigInputSwitch)->SetDisplayText(0, "ch. 1-2");
  GetParam(kTrigInputSwitch)->SetDisplayText(1, "ch. 3-4");
  GetParam(kTrigInpMuteSwitch)->InitEnum("Input mute", DLPG_DEFAULT_TRIG_INPMUTE_SWITCH_STATE, DLPG_SWITCH_STATES);
  GetParam(kTrigInpMuteSwitch)->SetDisplayText(0, "don't mute");
  GetParam(kTrigInpMuteSwitch)->SetDisplayText(1, "mute");
  // Background
  IGraphics* pGraphics = MakeGraphics(this, kWidth, kHeight);
  //const IColor *tBgColor = new IColor(255, 40, 40, 40);
  //pGraphics->AttachPanelBackground(tBgColor);
  pGraphics->AttachBackground(DLPG_BACKGROUND_ID, DLPG_BACKGROUND_FN);


  // *** Knobs - start
  // Trig section

  tBmp = pGraphics->LoadIBitmap(DLPG_TRIG_NOTE_KNOB_ID, DLPG_TRIG_NOTE_KNOB_FN, DLPG_TRIG_NOTE_KNOB_FRAMES);
  tTrigNoteKnob = new IKnobMultiControl(this, DLPG_KNOB_GRID(1, 1), kTrigNoteKnob, &tBmp);
  pGraphics->AttachControl(tTrigNoteKnob);

  tBmp = pGraphics->LoadIBitmap(DLPG_TRIG_CH_KNOB_ID, DLPG_TRIG_CH_KNOB_FN, DLPG_TRIG_CH_KNOB_FRAMES);
  tTrigChKnob = new IKnobMultiControl(this, DLPG_KNOB_GRID(1, 2), kTrigChKnob, &tBmp);
  pGraphics->AttachControl(tTrigChKnob);

  tBmp = pGraphics->LoadIBitmap(DLPG_TRIG_THRESH_KNOB_ID, DLPG_TRIG_THRESH_KNOB_FN, DLPG_STANDARD_KNOB_FRAMES);
  tTrigThreshKnob = new IKnobMultiControl(this, DLPG_KNOB_GRID(1, 1), kTrigThreshKnob, &tBmp);
  pGraphics->AttachControl(tTrigThreshKnob);
  tTrigThreshKnob->Hide(true);

  tBmp = pGraphics->LoadIBitmap(DLPG_TRIG_ATTACK_KNOB_ID, DLPG_TRIG_ATTACK_KNOB_FN, DLPG_STANDARD_KNOB_FRAMES);
  tTrigAttackKnob = new IKnobMultiControl(this, DLPG_KNOB_GRID(1, 2), kTrigAttackKnob, &tBmp);
  pGraphics->AttachControl(tTrigAttackKnob);
  tTrigAttackKnob->Hide(true);

  // Sub section
  tBmp = pGraphics->LoadIBitmap(DLPG_SUB_FREQ_KNOB_ID, DLPG_SUB_FREQ_KNOB_FN, DLPG_STANDARD_KNOB_FRAMES);
  tSubFreqKnob = new IKnobMultiControl(this, DLPG_KNOB_GRID(1, 3), kSubFreqKnob, &tBmp);
  pGraphics->AttachControl(tSubFreqKnob);

  tBmp = pGraphics->LoadIBitmap(DLPG_SUB_PHASE_KNOB_ID, DLPG_SUB_PHASE_KNOB_FN, DLPG_STANDARD_KNOB_FRAMES);
  tSubPhaseKnob = new IKnobMultiControl(this, DLPG_KNOB_GRID(1, 4), kSubPhaseKnob, &tBmp);
  pGraphics->AttachControl(tSubPhaseKnob);

  // Envelope section
  tBmp = pGraphics->LoadIBitmap(DLPG_ENVELOPE_ATTACK_KNOB_ID, DLPG_ENVELOPE_ATTACK_KNOB_FN, DLPG_STANDARD_KNOB_FRAMES);
  tEnvelopeAttackKnob = new IKnobMultiControl(this, DLPG_KNOB_GRID(2, 1), kEnvelopeAttackKnob, &tBmp);
  pGraphics->AttachControl(tEnvelopeAttackKnob);

  tBmp = pGraphics->LoadIBitmap(DLPG_ENVELOPE_HOLD_KNOB_ID, DLPG_ENVELOPE_HOLD_KNOB_FN, DLPG_STANDARD_KNOB_FRAMES);
  tEnvelopeHoldKnob = new IKnobMultiControl(this, DLPG_KNOB_GRID(2, 2), kEnvelopeHoldKnob, &tBmp);
  pGraphics->AttachControl(tEnvelopeHoldKnob);

  tBmp = pGraphics->LoadIBitmap(DLPG_ENVELOPE_RELEASE_KNOB_ID, DLPG_ENVELOPE_RELEASE_KNOB_FN, DLPG_STANDARD_KNOB_FRAMES);
  tEnvelopeReleaseKnob = new IKnobMultiControl(this, DLPG_KNOB_GRID(2, 3), kEnvelopeReleaseKnob, &tBmp);
  pGraphics->AttachControl(tEnvelopeReleaseKnob);

  // Volume
  tBmp = pGraphics->LoadIBitmap(DLPG_VOL_KNOB_ID, DLPG_VOL_KNOB_FN, DLPG_STANDARD_KNOB_FRAMES);
  tVolKnob = new IKnobMultiControl(this, DLPG_KNOB_GRID(2, 4), kVolKnob, &tBmp);
  pGraphics->AttachControl(tVolKnob);

  // *** Knobs - end

  // *** Switches - start
  tBmp = pGraphics->LoadIBitmap(DLPG_BYPASS_SWITCH_ID, DLPG_BYPASS_SWITCH_FN, DLPG_SWITCH_STATES);
  tBypassSwitch = new ISwitchControl(this, DLPG_SWITCH_GRID(1, 1), kBypassSwitch, &tBmp);
  pGraphics->AttachControl(tBypassSwitch);
  tBmp = pGraphics->LoadIBitmap(DLPG_SNAP_SWITCH_ID, DLPG_SNAP_SWITCH_FN, DLPG_SWITCH_STATES);
  tSnapSwitch = new ISwitchControl(this, DLPG_SWITCH_GRID(2, 1), kSnapSwitch, &tBmp);
  pGraphics->AttachControl(tSnapSwitch);
  tBmp = pGraphics->LoadIBitmap(DLPG_FLIP_SWITCH_ID, DLPG_FLIP_SWITCH_FN, DLPG_SWITCH_STATES);
  tFlipSwitch = new ISwitchControl(this, DLPG_SWITCH_GRID(3, 1), kFlipSwitch, &tBmp);
  pGraphics->AttachControl(tFlipSwitch);

  tBmp = pGraphics->LoadIBitmap(DLPG_TRIG_SWITCH_ID, DLPG_TRIG_SWITCH_FN, DLPG_SWITCH_STATES);
  tTrigSwitch = new ISwitchControl(this, DLPG_SWITCH_GRID(1, 2), kTrigSwitch, &tBmp);
  pGraphics->AttachControl(tTrigSwitch);
  tBmp = pGraphics->LoadIBitmap(DLPG_TRIG_INPUT_SWITCH_ID, DLPG_TRIG_INPUT_SWITCH_FN, DLPG_SWITCH_STATES);
  tTrigInputSwitch = new ISwitchControl(this, DLPG_SWITCH_GRID(2, 2), kTrigInputSwitch, &tBmp);
  pGraphics->AttachControl(tTrigInputSwitch);
  tBmp = pGraphics->LoadIBitmap(DLPG_TRIG_INPMUTE_SWITCH_ID, DLPG_TRIG_INPMUTE_SWITCH_FN, DLPG_SWITCH_STATES);
  tTrigInpMuteSwitch = new ISwitchControl(this, DLPG_SWITCH_GRID(3, 2), kTrigInpMuteSwitch, &tBmp);
  pGraphics->AttachControl(tTrigInpMuteSwitch);

  // *** Switches - end

  double fDuration = 300./1000;
  double fAttack = 20./1000;
  double fSampleRate = 44100.;
  double fFrequency = 100.;

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
