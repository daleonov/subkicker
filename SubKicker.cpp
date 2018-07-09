#include "DLPG_Version.h"
#include "DLPG_NoteNames.h"
#include "DLPG_NoteFrequencies.h"
#include "DLPG_Subdivisions.h"
#include "SubKicker.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

SubKicker::SubKicker(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(kNumParams, kNumPrograms, instanceInfo), fOutputGainLinear(1.)
{
  TRACE;
  IBitmap tBmp;
  char sEnumText[DLPG_ENUM_CONTROL_STRING_SIZE];

  //arguments are: name, defaultVal, minVal, maxVal, step, label
  GetParam(kTrigNoteKnob)->InitInt("Trigger (ext) | Midi note", DLPG_TRIG_ANY_NOTE, DLPG_TRIG_NOTE_RANGE, "");
  GetParam(kTrigChKnob)->InitInt("Trigger (ext) | Midi channel", DLPG_TRIG_ANY_CH, DLPG_TRIG_CH_RANGE, "");
  GetParam(kTrigHoldKnob)->InitDouble("Trigger (int) | Hold (ms)", DLPG_TRIG_HOLD_DEFAULT, DLPG_TRIG_HOLD_RANGE, 0.1, "ms");
  GetParam(kTrigThreshKnob)->InitDouble("Trigger (int) | Threshold", DLPG_TRIG_THRESH_DEFAULT, DLPG_TRIG_THRESH_RANGE, 0.1, "dB");

  GetParam(kSubFreqKnob)->InitDouble("Sub | Frequency", DLPG_SUB_FREQ_DEFAULT, DLPG_SUB_FREQ_RANGE, 0.1, "Hz");
  GetParam(kSubPhaseKnob)->InitDouble("Sub | Phase", DLPG_SUB_PHASE_DEFAULT, DLPG_SUB_PHASE_RANGE, 0.1, "Deg");
  GetParam(kSubNoteKnob)->InitEnum("Sub | Note", DLPG_DEFAULT_SUB_NOTE_STATE, DLPG_SUB_NOTE_STATES);
  GetParam(kTrigSubdivisionKnob)->InitEnum("Trigger (int) | Hold (subdivision)", DLPG_TRIG_SUBDIVISION_DEFAULT, DLPG_TRIG_SUBDIVISION_STATES);
  // Human readable labels for generic UI's
  for(int i=0; i<DLPG_SUB_NOTE_STATES; i++){
    sprintf(
      sEnumText,
      DLPG_SUB_NOTE_LABEL_STR,
      DLPG_SUB_NOTE_KNOB_VALUE_TO_NOTE_LABEL(i),
      DLPG_SUB_NOTE_KNOB_VALUE_TO_HZ(i)
      );
    GetParam(kSubNoteKnob)->SetDisplayText(i, sEnumText);
  }
  GetParam(kSubFreqKnob)->SetShape(DLPG_SUB_FREQ_KNOB_SHAPE);

  for(int i=0; i<DLPG_TRIG_SUBDIVISION_STATES; i++){
    sprintf(
      sEnumText,
      DLPG_TRIG_SUBDIVISION_LABEL_STR,
      DLPG_TRIG_SUBDIVISION_KNOB_VALUE_TO_LABEL(i),
      DLPG_TRIG_SUBDIVISION_KNOB_VALUE_TO_SECONDS(i, GetTempo()) * 1000.
      );
    GetParam(kTrigSubdivisionKnob)->SetDisplayText(i, sEnumText);
  }


  GetParam(kEnvelopeAttackKnob)->InitDouble("Envelope | Attack", DLPG_ENVELOPE_ATTACK_DEFAULT, DLPG_ENVELOPE_ATTACK_RANGE, 0.1, "ms");
  GetParam(kEnvelopeHoldKnob)->InitDouble("Envelope | Hold", DLPG_ENVELOPE_HOLD_DEFAULT, DLPG_ENVELOPE_HOLD_RANGE, 0.1, "ms");
  GetParam(kEnvelopeReleaseKnob)->InitDouble("Envelope | Release", DLPG_ENVELOPE_RELEASE_DEFAULT, DLPG_ENVELOPE_RELEASE_RANGE, 0.1, "ms");

  GetParam(kVolKnob)->InitDouble("Volume", DLPG_VOL_DEFAULT, DLPG_VOL_RANGE, 0.1, "dB");
  GetParam(kVolKnob)->SetShape(DLPG_VOL_KNOB_SHAPE);

  GetParam(kBypassSwitch)->InitEnum("Bypass", DLPG_DEFAULT_BYPASS_SWITCH_STATE, DLPG_SWITCH_STATES);
  GetParam(kBypassSwitch)->SetDisplayText(0, "Normal");
  GetParam(kBypassSwitch)->SetDisplayText(1, "Bypassed");
  GetParam(kTrigSwitch)->InitEnum("Triger (int) | Source", DLPG_DEFAULT_TRIG_SWITCH_STATE, DLPG_SWITCH_STATES);
  GetParam(kTrigSwitch)->SetDisplayText(0, "Internal");
  GetParam(kTrigSwitch)->SetDisplayText(1, "External");
  GetParam(kSnapSwitch)->InitEnum("Sub | Frequency snap", DLPG_DEFAULT_SNAP_SWITCH_STATE, DLPG_SWITCH_STATES);
  GetParam(kSnapSwitch)->SetDisplayText(0, "to Hz");
  GetParam(kSnapSwitch)->SetDisplayText(1, "to notes");
  GetParam(kFlipSwitch)->InitEnum("Sub | Phase flip", DLPG_DEFAULT_FLIP_SWITCH_STATE, DLPG_SWITCH_STATES);
  GetParam(kFlipSwitch)->SetDisplayText(0, "off");
  GetParam(kFlipSwitch)->SetDisplayText(1, "invert");
  // Sidechain triggering - not yet implemented
  /*
  GetParam(kTrigInputSwitch)->InitEnum("Trigger (int) | Input source", DLPG_DEFAULT_TRIG_INPUT_SWITCH_STATE, DLPG_SWITCH_STATES);
  GetParam(kTrigInputSwitch)->SetDisplayText(0, "ch. 1-2");
  GetParam(kTrigInputSwitch)->SetDisplayText(1, "ch. 3-4");
  */
  GetParam(kTrigInpMuteSwitch)->InitEnum("Input mute", DLPG_DEFAULT_TRIG_INPMUTE_SWITCH_STATE, DLPG_SWITCH_STATES);
  GetParam(kTrigInpMuteSwitch)->SetDisplayText(0, "don't mute");
  GetParam(kTrigInpMuteSwitch)->SetDisplayText(1, "mute");
  GetParam(kSubShapeSwitch)->InitEnum("Sub | Waveform", DLPG_DEFAULT_SUB_SHAPE_SWITCH_STATE, DLPG_SWITCH_STATES);
  GetParam(kSubShapeSwitch)->SetDisplayText(0, "Sine");
  GetParam(kSubShapeSwitch)->SetDisplayText(1, "Triangle");
  GetParam(kTrigHoldSnapSwitch)->InitEnum("Trig | Hold snap", DLPG_DEFAULT_TRIG_HOLD_SNAP_SWITCH_STATE, DLPG_SWITCH_STATES);
  GetParam(kTrigHoldSnapSwitch)->SetDisplayText(0, "Time");
  GetParam(kTrigHoldSnapSwitch)->SetDisplayText(1, "Subdivisions");

  GetParam(kOutputMeter)->InitDouble(
    "[Output level]",
    DLPG_OUTPUT_METER_RANGE_MIN,
    DLPG_OUTPUT_METER_RANGE_MIN,
    DLPG_OUTPUT_METER_RANGE_MAX,
    0.1,
    "dB"
    );

  // Background
  pGraphics = MakeGraphics(this, kWidth, kHeight);
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

  tBmp = pGraphics->LoadIBitmap(DLPG_TRIG_HOLD_KNOB_ID, DLPG_TRIG_HOLD_KNOB_FN, DLPG_STANDARD_KNOB_FRAMES);
  tTrigHoldKnob = new IKnobMultiControl(this, DLPG_KNOB_GRID(1, 2), kTrigHoldKnob, &tBmp);
  pGraphics->AttachControl(tTrigHoldKnob);

  tBmp = pGraphics->LoadIBitmap(DLPG_TRIG_SUBDIVISION_KNOB_ID, DLPG_TRIG_SUBDIVISION_KNOB_FN, DLPG_TRIG_SUBDIVISION_KNOB_FRAMES);
  tTrigSubdivisionKnob = new IKnobMultiControl(this, DLPG_KNOB_GRID(1, 2), kTrigSubdivisionKnob, &tBmp);
  pGraphics->AttachControl(tTrigSubdivisionKnob);
  // Hide all, the switch will take care of them later
  tTrigChKnob->Hide(true);
  tTrigNoteKnob->Hide(true);
  tTrigHoldKnob->Hide(true);
  tTrigSubdivisionKnob->Hide(true);
  tTrigThreshKnob->Hide(true);

  // Sub section
  tBmp = pGraphics->LoadIBitmap(DLPG_SUB_FREQ_KNOB_ID, DLPG_SUB_FREQ_KNOB_FN, DLPG_STANDARD_KNOB_FRAMES);
  tSubFreqKnob = new IKnobMultiControl(this, DLPG_KNOB_GRID(1, 3), kSubFreqKnob, &tBmp);
  pGraphics->AttachControl(tSubFreqKnob);

  tBmp = pGraphics->LoadIBitmap(DLPG_SUB_PHASE_KNOB_ID, DLPG_SUB_PHASE_KNOB_FN, DLPG_STANDARD_KNOB_FRAMES);
  tSubPhaseKnob = new IKnobMultiControl(this, DLPG_KNOB_GRID(1, 4), kSubPhaseKnob, &tBmp);
  pGraphics->AttachControl(tSubPhaseKnob);

  tBmp = pGraphics->LoadIBitmap(DLPG_SUB_NOTE_KNOB_ID, DLPG_SUB_NOTE_KNOB_FN, DLPG_SUB_NOTE_KNOB_FRAMES);
  tSubNoteKnob = new IKnobMultiControl(this, DLPG_KNOB_GRID(1, 3), kSubNoteKnob, &tBmp);
  pGraphics->AttachControl(tSubNoteKnob);

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
  // Sidechain triggering - not yet implemented
  /*
  tBmp = pGraphics->LoadIBitmap(DLPG_TRIG_INPUT_SWITCH_ID, DLPG_TRIG_INPUT_SWITCH_FN, DLPG_SWITCH_STATES);
  tTrigInputSwitch = new ISwitchControl(this, DLPG_SWITCH_GRID(2, 2), kTrigInputSwitch, &tBmp);
  pGraphics->AttachControl(tTrigInputSwitch);
  tTrigInputSwitch->GrayOut(true);
  tTrigInputSwitch->Hide(true);
  */
  tBmp = pGraphics->LoadIBitmap(DLPG_TRIG_INPMUTE_SWITCH_ID, DLPG_TRIG_INPMUTE_SWITCH_FN, DLPG_SWITCH_STATES);
  tTrigInpMuteSwitch = new ISwitchControl(this, DLPG_SWITCH_GRID(3, 2), kTrigInpMuteSwitch, &tBmp);
  pGraphics->AttachControl(tTrigInpMuteSwitch);
  tTrigInpMuteSwitch->GrayOut(true);
  tTrigInpMuteSwitch->Hide(true);
  tBmp = pGraphics->LoadIBitmap(DLPG_SUB_WAVEFORM_SHAPE_SWITCH_ID, DLPG_SUB_WAVEFORM_SHAPE_SWITCH_FN, DLPG_SWITCH_STATES);
  tSubShapeSwitch = new ISwitchControl(this, DLPG_SWITCH_GRID(2, 2), kSubShapeSwitch, &tBmp);
  pGraphics->AttachControl(tSubShapeSwitch);
  tBmp = pGraphics->LoadIBitmap(DLPG_TRIG_HOLD_SNAP_SWITCH_ID, DLPG_TRIG_HOLD_SNAP_SWITCH_FN, DLPG_SWITCH_STATES);
  tTrigHoldSnapSwitch = new ISwitchControl(this, DLPG_SWITCH_GRID(3, 2), kTrigHoldSnapSwitch, &tBmp);
  pGraphics->AttachControl(tTrigHoldSnapSwitch);
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

  tBmp = pGraphics->LoadIBitmap(DLPG_OUTPUT_METER_OVERLAY_ID, DLPG_OUTPUT_METER_OVERLAY_FN, 1);
  tOutputMeterOverlay = new IBitmapControl(this, kOutputMeterOverlayX, kOutputMeterOverlayY, &tBmp);
  pGraphics->AttachControl(tOutputMeterOverlay);

  // *** Knob labels - begin
  IText tKnobLabelCommon = IText(DLPG_KNOB_LABEL_STRING_SIZE);
  tKnobLabelCommon.mColor = tKnobLabelColor;
  tKnobLabelCommon.mSize = DLPG_KNOB_LABEL_FONT_SIZE;
  tKnobLabelCommon.mAlign = tKnobLabelCommon.DLPG_KNOB_LABEL_ALIGN;
  // Trig section
  tTrigNoteLabel = new ITextControl(this, DLPG_KNOB_LABEL_GRID_IRECT(1, 1), &tKnobLabelCommon, "Trig Note");
  tTrigChLabel = new ITextControl(this, DLPG_KNOB_LABEL_GRID_IRECT(1, 2), &tKnobLabelCommon, "Trig Ch");
  tTrigThreshLabel = new ITextControl(this, DLPG_KNOB_LABEL_GRID_IRECT(1, 1), &tKnobLabelCommon, "Trig Thresh");
  tTrigHoldLabel = new ITextControl(this, DLPG_KNOB_LABEL_GRID_IRECT(1, 2), &tKnobLabelCommon, "Trig Hold ms");
  tTrigSubdivisionLabel = new ITextControl(this, DLPG_KNOB_LABEL_GRID_IRECT(1, 2), &tKnobLabelCommon, "Trig Hold s/d");
  pGraphics->AttachControl(tTrigNoteLabel);
  pGraphics->AttachControl(tTrigChLabel);
  pGraphics->AttachControl(tTrigHoldLabel);
  pGraphics->AttachControl(tTrigThreshLabel);
  pGraphics->AttachControl(tTrigSubdivisionLabel);
  // Hide all, the trig switch will take care of them later
  tTrigHoldLabel->Hide(true);
  tTrigThreshLabel->Hide(true);
  tTrigNoteLabel->Hide(true);
  tTrigChLabel->Hide(true);
  tTrigSubdivisionLabel->Hide(true);
  // Sub section
  tSubFreqLabel = new ITextControl(this, DLPG_KNOB_LABEL_GRID_IRECT(1, 3), &tKnobLabelCommon, "Sub Freq");
  tSubNoteLabel = new ITextControl(this, DLPG_KNOB_LABEL_GRID_IRECT(1, 3), &tKnobLabelCommon, "Sub Note");
  tSubPhaseLabel = new ITextControl(this, DLPG_KNOB_LABEL_GRID_IRECT(1, 4), &tKnobLabelCommon, "Sub Phase");
  pGraphics->AttachControl(tSubFreqLabel);
  pGraphics->AttachControl(tSubNoteLabel);
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
  // Output meter
  IText tOutputMeterLabelIText = IText(DLPG_OUTPUT_METER_LABEL_STRING_SIZE);
  tOutputMeterLabelIText.mColor = tOutputMeterLabelIColor;
  tOutputMeterLabelIText.mSize = DLPG_OUTPUT_METER_LABEL_FONT_SIZE;
  tOutputMeterLabelIText.mAlign = tOutputMeterLabelIText.DLPG_OUTPUT_METER_LABEL_ALIGN;
  tOutputMeterLabel = new ITextControl(this, tOutputMeterLabelIrect, &tOutputMeterLabelIText, DLPG_OUTPUT_METER_LABEL_MINUS_INF_STR);
  tOutputMeterNotchLabel = new ITextControl(this, tOutputMeterNotchLabelIrect, &tOutputMeterLabelIText, "Notch");
  pGraphics->AttachControl(tOutputMeterLabel);
  pGraphics->AttachControl(tOutputMeterNotchLabel);
  
  // *** Knob labels - end

  // Edge trigger
  double fSampleRate = GetSampleRate();
  double fTrigHold = GetParam(kTrigHoldKnob)->Value() / 1000.;
  double fTrigThresh = DLPG_LOG_TO_LINEAR(GetParam(kTrigThreshKnob)->Value());
  tEdgeTrigger = new dlpg::EdgeTrigger(fSampleRate, fTrigHold, fTrigThresh);

  // Scope
  // Some text labels for generic UI's
  GetParam(kScope)->InitEnum("Preview waveform", 0, DLPG_SWITCH_STATES);
  GetParam(kScope)->SetDisplayText(0, "(Not clicked)");
  GetParam(kScope)->SetDisplayText(1, "(Clicked)");
  tScope = new dlpg::IWavScopeControl(this, PLUG_ScopeIrect, kScope, vSubkickWaveform);
  pGraphics->AttachControl(tScope);

  // Waveform stuff
  tWaveGenerator = new dlpg::WaveGenerator();
  tEnvelopeGenerator = new dlpg::EnvelopeGenerator();

  /*
  Those two knobs (and switches) are mutually exclusive, so we
  hide both of them and figure out later which one to show.
  */
  tSubFreqKnob->Hide(true);
  tSubNoteKnob->Hide(true);
  tSubFreqLabel->Hide(true);
  tSubNoteLabel->Hide(true);

  AttachGraphics(pGraphics);

  // Midi shenanigans
  memset(abKeyStatus, 0, DLPG_MIDI_NOTES_TOTAL * sizeof(bool));
  nNumKeysPressed = 0;

  //tScope->LoadWave(&vWaveform);
  //MakePreset("preset 1", ... );
  MakeDefaultPreset((char *) "-", kNumPrograms);
}

SubKicker::~SubKicker() {}

void SubKicker::ProcessMidiMsg(IMidiMsg* pMsg){

  int nStatusMsg = pMsg->StatusMsg();
  int nVelocity = pMsg->Velocity();

  switch (nStatusMsg)
  {
    case IMidiMsg::kNoteOn:
    case IMidiMsg::kNoteOff:
      // We care only about note on/off messages
      if (nStatusMsg == IMidiMsg::kNoteOn && nVelocity)
      {
        abKeyStatus[pMsg->NoteNumber()] = true;
        nNumKeysPressed += 1;
      }
      else
      {
        abKeyStatus[pMsg->NoteNumber()] = false;
        nNumKeysPressed -= 1;
      }
      break;
    default:
      // Ignore other MIDI messages
      return;
  }

  tMidiQueue.Add(pMsg);
}

void SubKicker::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
  // Mutex is already locked for us.
  double* pfOutL = outputs[0];
  double* pfOutR = outputs[1];
  double* pfInL = inputs[0];
  double* pfInR = inputs[1];
  int nTargetNote, nTargetCh;
  int nCurrentNote, nCurrentCh, nCurrentVelocity;
  static int nCurrentWaveformSample = 0;
  static bool bPlay = false;
  double fCurrentMeterPeakLinear = 0., fCurrentMeterPeakDb, fMeterGainLinear;
  static double fPreviousMeterPeakLinear = 0.;
  double fCurrentMeterLabelPeakDb, fCurrentMeterNotchDb;
  char sOutputMeterLabelString[DLPG_OUTPUT_METER_LABEL_STRING_SIZE];
  char sOutputMeterLabelMinusInfString[] = DLPG_OUTPUT_METER_LABEL_MINUS_INF_STR;
  char* psOutputMeterLabelString = NULL;
  bool bIsTriggeredInternally = GetParam(kTrigSwitch)->Bool();
  double fCurrentRectifiedSampleLinear;
  dlpg::TriggerState_t eTriggerState;
  IMidiMsg tMidiMsg;

  if(1){
    // Todo: Make sure the midi notes' offset is correct in the sample chunk
    for (int i = 0; i < nFrames; ++i, ++pfInR, ++pfInL){
      fCurrentRectifiedSampleLinear = (fabs(*pfInR) + fabs(*pfInL))/2;
      eTriggerState = tEdgeTrigger->ProcessMonoSampleLinear(fCurrentRectifiedSampleLinear);
      if(eTriggerState == dlpg::kTriggerRisingEdge){
        // Send "Note on"
        // Emulate pressing a midi key to play a waveform
        tMidiMsg.MakeNoteOnMsg(10, 100, 0, 1);
        tMidiQueue.Add(&tMidiMsg);

        #if DLPG_TRIG_INT_IMMEDIATE_NOTE_OFFS
        tMidiMsg.MakeNoteOffMsg(10, 0, 1);
        tMidiQueue.Add(&tMidiMsg);
        #endif
      }
      if(eTriggerState == dlpg::kTriggerFallingEdge){
        // Send "Note off"
        #if !DLPG_TRIG_INT_IMMEDIATE_NOTE_OFFS
        tMidiMsg.MakeNoteOffMsg(10, 0, 1);
        tMidiQueue.Add(&tMidiMsg);
        #endif
      }
    }
  }

  for (int nOffset = 0; nOffset < nFrames; ++nOffset, ++pfOutL, ++pfOutR){
    while (!tMidiQueue.Empty()){
      IMidiMsg* pMsg = tMidiQueue.Peek();
      if (pMsg->mOffset > nOffset) break;

      /* Kept this from Oli Larkin's example, but we
      don't need to run it standalone in Wndows anyway */
      #if !defined(OS_WIN) && !defined(SA_API)
      SendMidiMsg(pMsg);
      #endif

      int nStatusMsg = pMsg->StatusMsg();
      // We only care about 'note on' messages
      if (nStatusMsg == IMidiMsg::kNoteOn){
        nTargetCh = GetParam(kTrigChKnob)->Int();
        nTargetNote = GetParam(kTrigNoteKnob)->Int();
        nCurrentCh = DLPG_MIDI_IPLUG_INDEX_TO_CH(pMsg->Channel());
        nCurrentNote = pMsg->NoteNumber();
        nCurrentVelocity = pMsg->Velocity();
        // Play the waveform if MIDI input equals to what we're looking for
        if( \
          ((nCurrentCh == nTargetCh) || (nTargetCh == DLPG_TRIG_ANY_CH)) && \
          ((nCurrentNote == nTargetNote) || (nTargetNote == DLPG_TRIG_ANY_NOTE)) && \
          (nCurrentVelocity >= DLPG_REQUIRED_VELOCITY) \
          ){
          bPlay = true;
        }
      }

      tMidiQueue.Remove();
    }
    if((nCurrentWaveformSample < vSubkickWaveform.size()) && bPlay){
      // Start or resume playback
      *pfOutL = vSubkickWaveform[nCurrentWaveformSample] * fOutputGainLinear;
      *pfOutR = *pfOutL;
      if(nCurrentWaveformSample == 1)
        tScope->Highlight(true);
      nCurrentWaveformSample++;
      fCurrentMeterPeakLinear = IPMAX(fCurrentMeterPeakLinear, (*pfOutL + *pfOutR)/2);
    }
    else{
      // Stop playback
      nCurrentWaveformSample = 0;
      bPlay = false;
      tScope->Highlight(false);
    }
  }

  tMidiQueue.Flush(nFrames);

  // Applying a filter to meter's value
  fMeterGainLinear = (fCurrentMeterPeakLinear < fPreviousMeterPeakLinear) ? \
    DLPG_OUTPUT_METER_FILTER_DECAY : \
    DLPG_OUTPUT_METER_FILTER_ATTACK;
  fCurrentMeterPeakLinear = \
    fCurrentMeterPeakLinear * fMeterGainLinear + \
    fPreviousMeterPeakLinear * (1.0 - fMeterGainLinear);
  fCurrentMeterPeakDb = DLPG_LINEAR_TO_LOG(fCurrentMeterPeakLinear);
  tOutputMeter->SetValue(fCurrentMeterPeakDb);
  fPreviousMeterPeakLinear = fCurrentMeterPeakLinear;

  // Meter's notch - represents max output value
  fCurrentMeterLabelPeakDb = fCurrentMeterPeakDb;
  fCurrentMeterNotchDb = tOutputMeter->GetNotchValue();
  if(fCurrentMeterLabelPeakDb > fCurrentMeterNotchDb){
    // Apparently the filter above introduces some attenuation, so we have to compensate it
    fCurrentMeterNotchDb = fCurrentMeterLabelPeakDb + DLPG_OUTPUT_METER_FILTER_MAKEUP_GAIN_DB;
    tOutputMeter->SetNotchValue(fCurrentMeterNotchDb);
    sprintf(
      sOutputMeterLabelString,
      DLPG_OUTPUT_METER_LABEL_STR,
      fCurrentMeterNotchDb
      );
    tOutputMeterNotchLabel->SetTextFromPlug(sOutputMeterLabelString);
  }

  // Text reading for output meter
  /*
  Todo: When waveform is not being played, the label is still being
  spammed with "-oo dB" text all the time (each sample chunk), which
  doesn't look too efficient.
  */
  if(fCurrentMeterLabelPeakDb > DLPG_OUTPUT_METER_RANGE_MIN){
    sprintf(
      sOutputMeterLabelString,
      DLPG_OUTPUT_METER_LABEL_STR,
      fCurrentMeterLabelPeakDb
      );
    psOutputMeterLabelString = sOutputMeterLabelString;
  }
  else{
    psOutputMeterLabelString = sOutputMeterLabelMinusInfString;
  }
  tOutputMeterLabel->SetTextFromPlug(psOutputMeterLabelString); 
}

void SubKicker::Reset()
{
  TRACE;
  IMutexLock lock(this);
  double fSampleRateHz = GetSampleRate();

  // Update edge trigger
  // Todo: Probably reset it as well?
  tEdgeTrigger->SetSampleRate(fSampleRateHz);

  // tWaveGenerator and tEnvelopeGenerator's sample rate are updated in UpdateWaveform()
  UpdateWaveform();

  // Midi shenanigans
  tMidiQueue.Resize(GetBlockSize());
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
  double fPhaseFlip = GetParam(kFlipSwitch)->Bool() ? dlpg::fPi : 0.;
  double fPhase = DLPG_DEG_TO_RADIANS(GetParam(kSubPhaseKnob)->Value()) + fPhaseFlip;
  double fFrequency = GetParam(kSnapSwitch)->Bool() ? \
    DLPG_SUB_NOTE_KNOB_VALUE_TO_HZ(GetParam(kSubNoteKnob)->Int()) : \
    GetParam(kSubFreqKnob)->Value();
  dlpg::WaveForm_t eSubShape = GetParam(kSubShapeSwitch)->Bool() ? dlpg::kTriangle : dlpg::kSine;
  dlpg::EnvelopeShape_t eEnvelopeAttackShape = DLPG_DEFAULT_ENVELOPE_ATTACK_SHAPE;
  dlpg::EnvelopeShape_t eEnvelopeReleaseShape = DLPG_DEFAULT_ENVELOPE_RELEASE_SHAPE;

  // Delete old waveform
  vSubkickWaveform.clear();
  std::vector<double> vAttackEnvelope(0), vReleaseEnvelope(0);

  // Sample rate
  tWaveGenerator->SetSampleRate(fSampleRate);
  tEnvelopeGenerator->SetSampleRate(fSampleRate);

  // Generate wave
  tWaveGenerator->Generate(vSubkickWaveform, fDuration, fFrequency, fPhase, eSubShape);

  // Apply Attack and Release envelopes 
  tEnvelopeGenerator->Generate(vAttackEnvelope, fAttack, dlpg::kAttack, eEnvelopeAttackShape);
  tEnvelopeGenerator->Generate(vReleaseEnvelope, fRelease, dlpg::kRelease, eEnvelopeReleaseShape);
  std::vector<double>::size_type i, j;
  for(i = 0; i != vAttackEnvelope.size(); i++){
    vSubkickWaveform[i] *= vAttackEnvelope[i];
  }
  for(i = vSubkickWaveform.size()-vReleaseEnvelope.size(), j=0; i != vSubkickWaveform.size(); i++, j++){
    vSubkickWaveform[i] *= vReleaseEnvelope[j];
  }

  tScope->LoadWave(vSubkickWaveform);
  tScope->SetDirty(false);
  return true;
}

/*
@brief Finds closest note to an arbitrary frequency
@param fFrequency Frequency in Hz
@retval index of a "Sub Note" knob value
*/
inline int SubKicker::HzToSubNoteKnobValue(double fFrequency){
  double fDelta, fClosestDelta = 20000.;
  int nClosestIndex = -1;
  for(
    int i = 0;
    i < DLPG_SUB_NOTE_STATES;
    i++){
    fDelta = fabs(DLPG_SUB_NOTE_KNOB_VALUE_TO_HZ(i) - fFrequency);
    if(fDelta < fClosestDelta){
      fClosestDelta = fDelta;
      nClosestIndex = i;
    }
  }
  return nClosestIndex;
}

void SubKicker::OnParamChange(int paramIdx)
{
  IMutexLock lock(this);
  char sKnobLabelString[DLPG_KNOB_LABEL_STRING_SIZE];
  char sOutputMeterNotchLabelMinusInfString[] = DLPG_OUTPUT_METER_LABEL_MINUS_INF_STR;
  int nKnobValue;
  int nPreviewNote, nPreviewCh;
  IMidiMsg tMidiMsg;
  static bool bIsInit = true;
  bool bSwitchState, bExtraSwitchState;
  double fKnobValue, fNormalizedKnobValue;

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
    case kTrigThreshKnob:
      // Display knob's value with a text label
      DLPG_SET_LABEL_GENERIC(sKnobLabelString, DLPG_TRIG_THRESH_LABEL_STR, kTrigThreshKnob, tTrigThreshLabel);
      fKnobValue = GetParam(kTrigThreshKnob)->Value();
      tEdgeTrigger->SetThresholdLinear(DLPG_LOG_TO_LINEAR(fKnobValue));
      break;
    case kTrigHoldKnob:
      // Display knob's value with a text label
      DLPG_SET_LABEL_GENERIC(sKnobLabelString, DLPG_TRIG_HOLD_LABEL_STR, kTrigHoldKnob, tTrigHoldLabel);
      fKnobValue = GetParam(kTrigHoldKnob)->Value();
      tEdgeTrigger->SetHoldTime(fKnobValue / 1000.);
      break;
    case kTrigSubdivisionKnob:
      // Display knob's value with a text label ###
      nKnobValue = GetParam(kTrigSubdivisionKnob)->Int();
      fKnobValue = DLPG_TRIG_SUBDIVISION_KNOB_VALUE_TO_SECONDS(nKnobValue, GetTempo());
      sprintf(
        sKnobLabelString,
        DLPG_TRIG_SUBDIVISION_LABEL_STR,
        DLPG_TRIG_SUBDIVISION_KNOB_VALUE_TO_LABEL(nKnobValue),
        fKnobValue * 1000.
        );
      tTrigSubdivisionLabel->SetTextFromPlug(sKnobLabelString);
      tEdgeTrigger->SetHoldTime(fKnobValue);
      // 
      break;
    case kSubNoteKnob:
      // Display knob's value with a text label
      nKnobValue = GetParam(kSubNoteKnob)->Int();
      sprintf(
        sKnobLabelString,
        DLPG_SUB_NOTE_LABEL_STR,
        DLPG_SUB_NOTE_KNOB_VALUE_TO_NOTE_LABEL(nKnobValue),
        DLPG_SUB_NOTE_KNOB_VALUE_TO_HZ(nKnobValue)
        );
      tSubNoteLabel->SetTextFromPlug(sKnobLabelString);
      UpdateWaveform();
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
      /* Output gain is only being applied in DSP thread, so the
         scope wouldn't (and shouldn't) reflect the volume changes. */
      fKnobValue = GetParam(kVolKnob)->Value();
      this->fOutputGainLinear = DLPG_LOG_TO_LINEAR(fKnobValue);
      break;
    case kFlipSwitch:
      UpdateWaveform();
      break;
    case kSubShapeSwitch:
      UpdateWaveform();
      break;
    case kBypassSwitch:
      bIsBypassed = GetParam(kBypassSwitch)->Bool();
      tScope->Hide(bIsBypassed);
      break;
    case kSnapSwitch:
      bSwitchState = GetParam(kSnapSwitch)->Bool();
      // Convert value of Sub Freq knob to Sub Note or vice versa
      if(!bSwitchState){
        nKnobValue = GetParam(kSubNoteKnob)->Int();
        fKnobValue = DLPG_SUB_NOTE_KNOB_VALUE_TO_HZ(nKnobValue);
        fNormalizedKnobValue = DLPG_SUB_FREQ_KNOB_NORMALIZE(fKnobValue);
        GetGUI()->SetParameterFromPlug(kSubFreqKnob, fNormalizedKnobValue, true);
        InformHostOfParamChange(kSubFreqKnob, fNormalizedKnobValue);
        tSubFreqKnob->SetDirty(true);
      }
      else{
        fKnobValue = GetParam(kSubFreqKnob)->Value();
        nKnobValue = HzToSubNoteKnobValue(fKnobValue);
        fNormalizedKnobValue = DLPG_SUB_NOTE_KNOB_NORMALIZE((double)nKnobValue);
        GetGUI()->SetParameterFromPlug(kSubNoteKnob, fNormalizedKnobValue, true);
        InformHostOfParamChange(kSubNoteKnob, fNormalizedKnobValue);
        tSubNoteKnob->SetDirty(true);
      }
      // Display only relevant controls
      tSubFreqKnob->Hide(bSwitchState);
      tSubNoteKnob->Hide(!bSwitchState);
      tSubFreqLabel->Hide(bSwitchState);
      tSubNoteLabel->Hide(!bSwitchState);
      break;
    case kScope:
      // Do not send a note during startup
      if(bIsInit) break;
      // Clicking a scope triggers the waveform output (for preview purposes)
      nKnobValue = GetParam(kTrigChKnob)->Int();
      nPreviewCh = (nKnobValue == DLPG_TRIG_ANY_CH) ? \
        DLPG_MIDI_CH_TO_IPLUG_INDEX(DLPG_DEFAULT_PREVIEW_CH) : 
        DLPG_MIDI_CH_TO_IPLUG_INDEX(nKnobValue);
      nKnobValue = GetParam(kTrigNoteKnob)->Int();
      nPreviewNote = (nKnobValue == DLPG_TRIG_ANY_NOTE) ? \
        DLPG_DEFAULT_PREVIEW_NOTE : \
        nKnobValue;
      // Emulate pressing a midi key to play a waveform
      tMidiMsg.MakeNoteOnMsg(nPreviewNote, DLPG_PREVIEW_VELOCITY, 0, nPreviewCh);
      tMidiQueue.Add(&tMidiMsg);
      tMidiMsg.MakeNoteOffMsg(nPreviewNote, 0, nPreviewCh);
      tMidiQueue.Add(&tMidiMsg);
      break;
    case kOutputMeter:
      // Clicking on the meter resets the notch
      tOutputMeter->SetNotchValue(DLPG_OUTPUT_METER_NOTCH);
      tOutputMeterNotchLabel->SetTextFromPlug(sOutputMeterNotchLabelMinusInfString);
      break;
    case kTrigSwitch:
      bSwitchState = GetParam(kTrigSwitch)->Bool();
      bExtraSwitchState = GetParam(kTrigHoldSnapSwitch)->Bool();

      /*
      Trig sw: ext = 0, int = 1;
      Snap sw:  ms = 0, s/d = 1.
      | Trig sw | Snap sw || Hold knob | Subdiv knob | Ch knob |
      |    0    |    0    ||  hidden   |   hidden    | visible |
      |    0    |   [1]   ||  hidden   |   hidden    | visible |
      |    1    |    0    ||  visible  |   hidden    | hidden  |
      |    1    |    1    ||  hidden   |   visible   | hidden  |

      Ch knob =          !Trig
      Hold knob =        Trig && !snap
      Subdiv knob =      Trig && snap
      Ch knob hide =     Trig
      Hold knob hide =   !(Trig && !snap)
      Subdiv knob hide = !(Trig && snap)

      */
      tTrigChKnob->Hide(bSwitchState);
      tTrigNoteKnob->Hide(bSwitchState);
      tTrigHoldKnob->Hide(!(bSwitchState && !bExtraSwitchState));
      tTrigSubdivisionKnob->Hide(!(bSwitchState && bExtraSwitchState));
      tTrigThreshKnob->Hide(!bSwitchState);

      tTrigNoteLabel->Hide(bSwitchState);
      tTrigChLabel->Hide(bSwitchState);
      tTrigHoldLabel->Hide(!(bSwitchState && !bExtraSwitchState));
      tTrigSubdivisionLabel->Hide(!(bSwitchState && bExtraSwitchState));
      tTrigThreshLabel->Hide(!bSwitchState);

      tTrigHoldSnapSwitch->GrayOut(!bSwitchState);

      // Reset the trigger every time
      tEdgeTrigger->Reset();
    case kTrigHoldSnapSwitch:
      // Update controls 
      bSwitchState = GetParam(kTrigSwitch)->Bool();
      bExtraSwitchState = GetParam(kTrigHoldSnapSwitch)->Bool();
      tTrigHoldKnob->Hide(!(bSwitchState && !bExtraSwitchState));
      tTrigSubdivisionKnob->Hide(!(bSwitchState && bExtraSwitchState));
      tTrigHoldLabel->Hide(!(bSwitchState && !bExtraSwitchState));
      tTrigSubdivisionLabel->Hide(!(bSwitchState && bExtraSwitchState));
      // Update trigger's hold time
      // If the tempo of the song changes, this control is getting SetDirty() from DSP thread
      if(bExtraSwitchState){
        // Is snapped to subdivision
        nKnobValue = GetParam(kTrigSubdivisionKnob)->Int();
        fKnobValue = DLPG_TRIG_SUBDIVISION_KNOB_VALUE_TO_SECONDS(nKnobValue, GetTempo());
      }
      else{
        // If not snapped (milliseconds)
        fKnobValue = GetParam(kTrigHoldKnob)->Value() / 1000.;
      }
      tEdgeTrigger->SetHoldTime(fKnobValue);
    default:
      break;
  }
  /* All params are triggered during the startup, so the following
  flag is a workaround against 'false' triggering */
  bIsInit = false;
}
