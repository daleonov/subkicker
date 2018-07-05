#include "DLPG_Version.h"
#include "DLPG_NoteNames.h"
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
  GetParam(kTrigNoteKnob)->InitInt("Trigger (ext) | Midi note", DLPG_TRIG_ANY_NOTE, DLPG_TRIG_NOTE_RANGE, "");
  GetParam(kTrigChKnob)->InitInt("Trigger (ext) | Midi channel", DLPG_TRIG_ANY_CH, DLPG_TRIG_CH_RANGE, "");
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

  GetParam(kOutputMeter)->InitDouble(
    "[Output level]",
    DLPG_OUTPUT_METER_RANGE_MIN,
    DLPG_OUTPUT_METER_RANGE_MIN,
    DLPG_OUTPUT_METER_RANGE_MAX,
    0.1,
    "dB"
    );

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
  tTrigInputSwitch->GrayOut(true);
  tTrigInpMuteSwitch->GrayOut(true);
  // *** Switches - end

  // Text label with current version of the plug
  // TODO: Make it clickable so it leads to a website or something
  static IText tTextVersion = IText(DLPG_VERSION_TEXT_LABEL_STRING_SIZE);
  char sDisplayedVersion[DLPG_VERSION_TEXT_LABEL_STRING_SIZE];
  #ifdef _DLPG_VERSION_H
  sprintf(
    sDisplayedVersion,
    DLPG_VERSTION_TEXT,
    VST3_VER_STR,
    &sPlugVersionGitHead,
    &sPlugVersionDate
    );
  #else
  sprintf(
    sDisplayedVersion,
    DLPG_VERSTION_TEXT,
    VST3_VER_STR
    );
  #endif
  tTextVersion.mColor = tTextVersionColor;
  tTextVersion.mSize = DLPG_VERSION_TEXT_LABEL_FONT_SIZE;
  tTextVersion.mAlign = tTextVersion.kAlignNear;
  pGraphics->AttachControl(
    new ITextControl(
      this,
      tTextVersionIrect,
      &tTextVersion,
      (const char*)&sDisplayedVersion
      )
    );

  static IText tBugreportLabel = IText(DLPG_BUGREPORT_LABEL_STRING_SIZE);
  tBugreportLabel.mColor = tBugreportLabelColor;
  tBugreportLabel.mSize = DLPG_BUGREPORT_LABEL_FONT_SIZE;
  tBugreportLabel.mAlign = tBugreportLabel.kAlignNear;
  pGraphics->AttachControl(
    new ITextControl(
      this,
      tBugreportLabelIrect,
      &tBugreportLabel,
      DLPG_BUGREPORT_LABEL_TEXT
      )
    );

  tOutputMeter = new Plug::ILevelMeteringBar(
    this,
    kOutputMeterX,
    kOutputMeterY,
    tOutputMeterIrect,
    kOutputMeter,
    false,
    &tOutputMeterFgIcolor
    );
  tOutputMeter->SetNotchValue(DLPG_OUTPUT_METER_NOTCH);
  pGraphics->AttachControl(tOutputMeter);

  // *** Knob labels - begin
  IText tKnobLabelCommon = IText(DLPG_KNOB_LABEL_STRING_SIZE);
  tKnobLabelCommon.mColor = tKnobLabelColor;
  tKnobLabelCommon.mSize = DLPG_KNOB_LABEL_FONT_SIZE;
  tKnobLabelCommon.mAlign = tKnobLabelCommon.DLPG_KNOB_LABEL_ALIGN;
  // Trig section
  tTrigNoteLabel = new ITextControl(this, DLPG_KNOB_LABEL_GRID_IRECT(1, 1), &tKnobLabelCommon, "Trig Note");
  tTrigChLabel = new ITextControl(this, DLPG_KNOB_LABEL_GRID_IRECT(1, 2), &tKnobLabelCommon, "Trig Ch");
  tTrigAttackLabel = new ITextControl(this, DLPG_KNOB_LABEL_GRID_IRECT(1, 1), &tKnobLabelCommon, "Trig Attack");
  tTrigThreshLabel = new ITextControl(this, DLPG_KNOB_LABEL_GRID_IRECT(1, 2), &tKnobLabelCommon, "Trig Thresh");
  pGraphics->AttachControl(tTrigNoteLabel);
  pGraphics->AttachControl(tTrigChLabel);
  pGraphics->AttachControl(tTrigAttackLabel);
  pGraphics->AttachControl(tTrigThreshLabel);
  tTrigAttackLabel->Hide(true);
  tTrigThreshLabel->Hide(true);
  // Sub section
  tSubFreqLabel = new ITextControl(this, DLPG_KNOB_LABEL_GRID_IRECT(1, 3), &tKnobLabelCommon, "Sub Freq");
  tSubPhaseLabel = new ITextControl(this, DLPG_KNOB_LABEL_GRID_IRECT(1, 4), &tKnobLabelCommon, "Sub Phase");
  pGraphics->AttachControl(tSubFreqLabel);
  pGraphics->AttachControl(tSubPhaseLabel);
  // Envelope section
  tEnvelopeAttackLabel = new ITextControl(this, DLPG_KNOB_LABEL_GRID_IRECT(2, 1), &tKnobLabelCommon, "Envelope Attack");
  tEnvelopeHoldLabel = new ITextControl(this, DLPG_KNOB_LABEL_GRID_IRECT(2, 2), &tKnobLabelCommon, "Envelope Hold");
  tEnvelopeReleaseLabel = new ITextControl(this, DLPG_KNOB_LABEL_GRID_IRECT(2, 3), &tKnobLabelCommon, "Envelope Release");
  pGraphics->AttachControl(tEnvelopeAttackLabel);
  pGraphics->AttachControl(tEnvelopeHoldLabel);
  pGraphics->AttachControl(tEnvelopeReleaseLabel);
  // Volume
  tVolLabel = new ITextControl(this, DLPG_KNOB_LABEL_GRID_IRECT(2, 4), &tKnobLabelCommon, "Volume");
  pGraphics->AttachControl(tVolLabel);
/*
  double fAttack = 20./1000;
  double fHold = 100./1000;
  double fRelease = 20./1000;
  double fDuration = fAttack + fHold + fRelease;
  double fSampleRate = 44100.;
  double fFrequency = 100.;
  // Wave stuff
  std::vector<double> vWaveform(0), vAttackEnvelope(0), vReleaseEnvelope(0);
  tWaveGenerator = new dlpg::WaveGenerator();
  tEnvelopeGenerator = new dlpg::EnvelopeGenerator();
  tWaveGenerator->Generate(vWaveform, fDuration, fFrequency);
  tEnvelopeGenerator->Generate(vAttackEnvelope, fAttack, dlpg::kAttack, dlpg::DLPG_ENVELOPE_ATTACK_SHAPE);
  tEnvelopeGenerator->Generate(vReleaseEnvelope, fRelease, dlpg::kRelease, dlpg::DLPG_ENVELOPE_RELEASE_SHAPE);
  std::vector<double>::size_type i, j;
  for(i = 0; i != vAttackEnvelope.size(); i++){
    vWaveform[i] *= vAttackEnvelope[i];
  }
  for(i = vWaveform.size()-vReleaseEnvelope.size(), j=0; i != vWaveform.size(); i++, j++){
    vWaveform[i] *= vReleaseEnvelope[j];
  }
*/
  // Scope
  tScope = new dlpg::IWavScopeControl(this, PLUG_ScopeIrect, kScope, vSubkickWaveform);
  tScope->UpdateScale(0.1, 44100.);
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

bool SubKicker::UpdateWaveform(){
  /*
  Note: do not forget to convert all velues to Si! 
  e.g. ms -> s, kHz -> Hz etc.
  */
  double fAttack = GetParam(kEnvelopeAttackKnob)->Value() / 1000.;
  double fHold = GetParam(kEnvelopeHoldKnob)->Value() / 1000.;
  double fRelease = GetParam(kEnvelopeReleaseKnob)->Value() / 1000.;
  double fDuration = fAttack + fHold + fRelease;
  double fSampleRate = GetSampleRate();
  double fFrequency = GetParam(kSubFreqKnob)->Value();
  double fPhaseFlip = GetParam(kFlipSwitch)->Bool() ? dlpg::fPi : 0.;
  double fPhase = DLPG_DEG_TO_RADIANS(GetParam(kSubPhaseKnob)->Value()) + fPhaseFlip;

  // Delete old waveform
  vSubkickWaveform.clear();

  std::vector<double> vAttackEnvelope(0), vReleaseEnvelope(0);
  tWaveGenerator = new dlpg::WaveGenerator();
  tEnvelopeGenerator = new dlpg::EnvelopeGenerator();

  // Generate wave
  tWaveGenerator->Generate(vSubkickWaveform, fDuration, fFrequency, fPhase);

  // Apply Attack and Release envelopes 
  tEnvelopeGenerator->Generate(vAttackEnvelope, fAttack, dlpg::kAttack, dlpg::DLPG_ENVELOPE_ATTACK_SHAPE);
  tEnvelopeGenerator->Generate(vReleaseEnvelope, fRelease, dlpg::kRelease, dlpg::DLPG_ENVELOPE_RELEASE_SHAPE);
  std::vector<double>::size_type i, j;
  for(i = 0; i != vAttackEnvelope.size(); i++){
    vSubkickWaveform[i] *= vAttackEnvelope[i];
  }
  for(i = vSubkickWaveform.size()-vReleaseEnvelope.size(), j=0; i != vSubkickWaveform.size(); i++, j++){
    vSubkickWaveform[i] *= vReleaseEnvelope[j];
  }

  tScope->UpdateScale(fDuration, fSampleRate);
  tScope->LoadWave(vSubkickWaveform);
  tScope->SetDirty(true);
  return true;
}

void SubKicker::OnParamChange(int paramIdx)
{
  IMutexLock lock(this);
  char sKnobLabelString[DLPG_KNOB_LABEL_STRING_SIZE];
  int nKnobValue;

  switch (paramIdx)
  {
    case kTrigNoteKnob:
      // Display knob's value with a text label
      nKnobValue = GetParam(kTrigNoteKnob)->Int();
      if(nKnobValue == DLPG_TRIG_ANY_NOTE)
        sprintf(sKnobLabelString, DLPG_TRIG_ANY_NOTE_STR);
      else
        sprintf(
          sKnobLabelString,
          DLPG_TRIG_NOTE_LABEL_STR,
          nKnobValue,
          DLPG_MIDI_NOTE_NAME(nKnobValue),
          DLPG_MIDI_OCTAVE_NUMBER(nKnobValue)
        );
      tTrigNoteLabel->SetTextFromPlug(sKnobLabelString);
      break;
    case kTrigChKnob:
      // Display knob's value with a text label
      nKnobValue = GetParam(kTrigChKnob)->Int();
      if(nKnobValue == DLPG_TRIG_ANY_CH)
        sprintf(sKnobLabelString, DLPG_TRIG_ANY_CH_STR);
      else
        sprintf(sKnobLabelString, DLPG_TRIG_CH_LABEL_STR, nKnobValue);
      tTrigChLabel->SetTextFromPlug(sKnobLabelString);
      break;
    case kSubFreqKnob:
      // Display knob's value with a text label
      DLPG_SET_LABEL_GENERIC(sKnobLabelString, DLPG_SUB_FREQ_LABEL_STR, kSubFreqKnob, tSubFreqLabel);
      UpdateWaveform();
      break;
    case kSubPhaseKnob:
      // Display knob's value with a text label
      DLPG_SET_LABEL_GENERIC(sKnobLabelString, DLPG_SUB_PHASE_LABEL_STR, kSubPhaseKnob, tSubPhaseLabel);
      UpdateWaveform();
      break;
    case kEnvelopeAttackKnob:
      // Display knob's value with a text label
      DLPG_SET_LABEL_GENERIC(sKnobLabelString, DLPG_ENVELOPE_ATTACK_LABEL_STR, kEnvelopeAttackKnob, tEnvelopeAttackLabel);
      UpdateWaveform();
      break;
    case kEnvelopeHoldKnob:
      // Display knob's value with a text label
      DLPG_SET_LABEL_GENERIC(sKnobLabelString, DLPG_ENVELOPE_HOLD_LABEL_STR, kEnvelopeHoldKnob, tEnvelopeHoldLabel);
      UpdateWaveform();
      break;
    case kEnvelopeReleaseKnob:
      // Display knob's value with a text label
      DLPG_SET_LABEL_GENERIC(sKnobLabelString, DLPG_ENVELOPE_RELEASE_LABEL_STR, kEnvelopeReleaseKnob, tEnvelopeReleaseLabel);
      UpdateWaveform();
      break;
    case kVolKnob:
      // Display knob's value with a text label
      DLPG_SET_LABEL_GENERIC(sKnobLabelString, DLPG_VOL_LABEL_STR, kVolKnob, tVolLabel);
      break;
    case kFlipSwitch:
      UpdateWaveform();
      break;
    case kBypassSwitch:
      bIsBypassed = GetParam(kBypassSwitch)->Bool();
      tScope->Hide(bIsBypassed);
    default:
      break;
  }
}
