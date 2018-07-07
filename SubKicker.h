#ifndef __SUBKICKER__
#define __SUBKICKER__

#include "IPlug_include_in_plug_hdr.h"
#include "IMidiQueue.h"
#include "DLPG_IControlExtras.h"
#include "PLUG_IControlExtras.h"
#include "DLPG_WaveGenerator.h"

#define DLPG_LOG_TO_LINEAR(v) (pow(10, v/20.))
#define DLPG_LINEAR_TO_LOG(v) (20.*log10(v))

#define DLPG_SWITCH_STATES 2

#define DLPG_SWITCH_X_BASE 565
#define DLPG_SWITCH_X_STEP 110
#define DLPG_SWITCH_Y_BASE 10
#define DLPG_SWITCH_Y_STEP 36

#define DLPG_SWITCH_GRID(row, col)\
(DLPG_SWITCH_X_BASE + (col-1)*DLPG_SWITCH_X_STEP),\
(DLPG_SWITCH_Y_BASE + (row-1)*DLPG_SWITCH_Y_STEP)

#define DLPG_DEFAULT_BYPASS_SWITCH_STATE 0
#define DLPG_DEFAULT_TRIG_SWITCH_STATE 0
#define DLPG_DEFAULT_SNAP_SWITCH_STATE 1
#define DLPG_DEFAULT_FLIP_SWITCH_STATE 0
#define DLPG_DEFAULT_TRIG_INPUT_SWITCH_STATE 0
#define DLPG_DEFAULT_TRIG_INPMUTE_SWITCH_STATE 0

#define DLPG_STANDARD_KNOB_FRAMES 128
#define DLPG_TRIG_CH_KNOB_FRAMES 17
#define DLPG_TRIG_NOTE_KNOB_FRAMES 129
#define DLPG_SUB_NOTE_KNOB_FRAMES 25

#define DLPG_VOL_KNOB_SHAPE 0.26
#define DLPG_SUB_FREQ_KNOB_SHAPE 1.45

// Should be outside of respective standard midi ranges
// 0 or 17
#define DLPG_TRIG_ANY_CH 17
#define DLPG_TRIG_ANY_CH_STR "Any"
// -1 or 128
#define DLPG_TRIG_ANY_NOTE 128
#define DLPG_TRIG_ANY_NOTE_STR "Any"

// Sub frequency knobs
// Lowest note and range:
// E-1 = 20.6 Hz = Note 16, range 2 octaves up
#define DLPG_MIDI_NOTE_NUMBER_TO_FREQ_INDEX(n) (n)
#define DLPG_MIDI_FREQ_INDEX_TO_NOTE_NUMBER(n) (n)
#define DLPG_SUB_LOWEST_NOTE_MIDI_NUMBER 16 
#define DLPG_SUB_NOTE_STATES 25
#define DLPG_DEFAULT_SUB_NOTE_STATE 12
#define DLPG_SUB_NOTE_KNOB_VALUE_TO_MIDI_NUMBER(v) (v + DLPG_SUB_LOWEST_NOTE_MIDI_NUMBER)
#define DLPG_SUB_NOTE_KNOB_VALUE_TO_HZ(v) (afNoteFrequencies[DLPG_MIDI_NOTE_NUMBER_TO_FREQ_INDEX(DLPG_SUB_NOTE_KNOB_VALUE_TO_MIDI_NUMBER(v))])
#define DLPG_SUB_NOTE_KNOB_VALUE_TO_NOTE_LABEL(v) (asNoteLabels[DLPG_MIDI_NOTE_NUMBER_TO_FREQ_INDEX(DLPG_SUB_NOTE_KNOB_VALUE_TO_MIDI_NUMBER(v))])
#define DLPG_SUB_FREQ_MIN DLPG_SUB_NOTE_KNOB_VALUE_TO_HZ(0)
#define DLPG_SUB_FREQ_MAX DLPG_SUB_NOTE_KNOB_VALUE_TO_HZ(DLPG_SUB_NOTE_STATES-1)
#define DLPG_SUB_FREQ_DEFAULT DLPG_SUB_NOTE_KNOB_VALUE_TO_HZ(DLPG_DEFAULT_SUB_NOTE_STATE)
#define DLPG_SUB_FREQ_RANGE DLPG_SUB_FREQ_MIN, DLPG_SUB_FREQ_MAX
#define DLPG_SUB_FREQ_KNOB_NORMALIZE(v) ToNormalizedParam(v, DLPG_SUB_FREQ_MIN, DLPG_SUB_FREQ_MAX, DLPG_SUB_FREQ_KNOB_SHAPE)
#define DLPG_SUB_NOTE_KNOB_NORMALIZE(v) ToNormalizedParam(v, 0, DLPG_SUB_NOTE_STATES - 1, 1.)
// For Hz to Note knob value conversion, use HzToSubNoteKnobValue()

// Other knobs
#define DLPG_TRIG_CH_RANGE 1, DLPG_TRIG_ANY_CH
#define DLPG_TRIG_CH_DEFAULT DLPG_TRIG_ANY_CH
#define DLPG_TRIG_NOTE_RANGE 0, DLPG_TRIG_ANY_NOTE
#define DLPG_TRIG_NOTE_DEFAULT DLPG_TRIG_ANY_NOTE
#define DLPG_TRIG_ATTACK_RANGE 0.1, 100.
#define DLPG_TRIG_ATTACK_DEFAULT 10.
#define DLPG_TRIG_THRESH_RANGE -60., 0.
#define DLPG_TRIG_THRESH_DEFAULT -6.
#define DLPG_SUB_PHASE_RANGE -180., 180.
#define DLPG_SUB_PHASE_DEFAULT 0.
#define DLPG_ENVELOPE_ATTACK_RANGE .1, 200.
#define DLPG_ENVELOPE_ATTACK_DEFAULT 25.
#define DLPG_ENVELOPE_HOLD_RANGE .1, 400.
#define DLPG_ENVELOPE_HOLD_DEFAULT 100.
#define DLPG_ENVELOPE_RELEASE_RANGE .1, 200.
#define DLPG_ENVELOPE_RELEASE_DEFAULT 100.
#define DLPG_VOL_RANGE -60., 12.
#define DLPG_VOL_DEFAULT -0.1

#define DLPG_KNOB_X_BASE 120
#define DLPG_KNOB_X_STEP 167
#define DLPG_KNOB_Y_BASE 145
#define DLPG_KNOB_Y_STEP 198

#define DLPG_KNOB_GRID(row, col)\
(DLPG_KNOB_X_BASE + (col-1)*DLPG_KNOB_X_STEP),\
(DLPG_KNOB_Y_BASE + (row-1)*DLPG_KNOB_Y_STEP)

#define DLPG_SCOPE_W 440
#define DLPG_SCOPE_H 105

#define DLPG_OUTPUT_METER_RANGE_MIN -60.
#define DLPG_OUTPUT_METER_RANGE_MAX +12.
#define DLPG_OUTPUT_METER_NOTCH DLPG_OUTPUT_METER_RANGE_MAX
#define DLPG_OUTPUT_METER_W 74
#define DLPG_OUTPUT_METER_H 490
const IColor tOutputMeterFgIcolor(255, 0, 184, 67);

#define DLPG_VERSION_TEXT_LABEL_STRING_SIZE 96
#define DLPG_VERSION_TEXT_LABEL_COLOR_MONO 90
#define DLPG_VERSION_TEXT_LABEL_W 160
#define DLPG_VERSION_TEXT_LABEL_H 64
const IColor tTextVersionColor(
  255,
  DLPG_VERSION_TEXT_LABEL_COLOR_MONO,
  DLPG_VERSION_TEXT_LABEL_COLOR_MONO,
  DLPG_VERSION_TEXT_LABEL_COLOR_MONO
  );

// Plug's info label
#ifdef _DLPG_VERSION_H
#define DLPG_VERSTION_TEXT \
"SubKicker v%s\n\
by Daniel Leonov Plugs\n\
danielleonovplugs.com\n\
(%s@%s)"
#else
#define DLPG_VERSTION_TEXT \
"SubKicker v%s\n\
by Daniel Leonov Plugs\n\
danielleonovplugs.com"
#endif

#define DLPG_BUGREPORT_LABEL_STRING_SIZE 16
#define DLPG_BUGREPORT_LABEL_COLOR_MONO 90
#define DLPG_BUGREPORT_LABEL_W 100
#define DLPG_BUGREPORT_LABEL_H 16
const IColor tBugreportLabelColor(
  255,
  DLPG_BUGREPORT_LABEL_COLOR_MONO,
  DLPG_BUGREPORT_LABEL_COLOR_MONO,
  DLPG_BUGREPORT_LABEL_COLOR_MONO
  );
#define DLPG_BUGREPORT_LABEL_TEXT "Report a bug"

#define DLPG_KNOB_LABEL_STRING_SIZE 28
#define DLPG_KNOB_LABEL_COLOR_MONO 196
#define DLPG_KNOB_LABEL_W 155
#define DLPG_KNOB_LABEL_H 16
#define DLPG_KNOB_LABEL_ALIGN kAlignCenter
const IColor tKnobLabelColor(
  255,
  DLPG_KNOB_LABEL_COLOR_MONO,
  DLPG_KNOB_LABEL_COLOR_MONO,
  DLPG_KNOB_LABEL_COLOR_MONO
  );

#define DLPG_KNOB_LABEL_X_BASE DLPG_KNOB_X_BASE
#define DLPG_KNOB_LABEL_X_STEP DLPG_KNOB_X_STEP
#define DLPG_KNOB_LABEL_Y_BASE 145+155
#define DLPG_KNOB_LABEL_Y_STEP DLPG_KNOB_Y_STEP

#define DLPG_KNOB_LABEL_GRID(row, col)\
(DLPG_KNOB_LABEL_X_BASE + (col-1)*DLPG_KNOB_LABEL_X_STEP),\
(DLPG_KNOB_LABEL_Y_BASE + (row-1)*DLPG_KNOB_LABEL_Y_STEP)

#define DLPG_KNOB_LABEL_GRID_IRECT(row, col) \
IRECT(\
(DLPG_KNOB_LABEL_X_BASE + (col-1)*DLPG_KNOB_LABEL_X_STEP),\
(DLPG_KNOB_LABEL_Y_BASE + (row-1)*DLPG_KNOB_LABEL_Y_STEP),\
(DLPG_KNOB_LABEL_X_BASE + (col-1)*DLPG_KNOB_LABEL_X_STEP) + DLPG_KNOB_LABEL_W,\
(DLPG_KNOB_LABEL_Y_BASE + (row-1)*DLPG_KNOB_LABEL_Y_STEP) + DLPG_KNOB_LABEL_H)

#define DLPG_ENVELOPE_GENERIC_STR "%0.1f ms"
#define DLPG_LEVEL_GENERIC_STR "%+0.1f dB"

#define DLPG_TRIG_NOTE_LABEL_STR "%d (%s%d)"
#define DLPG_TRIG_CH_LABEL_STR "%02d"
#define DLPG_TRIG_ATTACK_LABEL_STR DLPG_ENVELOPE_GENERIC_STR
#define DLPG_TRIG_THRESH_LABEL_STR DLPG_LEVEL_GENERIC_STR
#define DLPG_SUB_FREQ_LABEL_STR "%0.2f Hz"
#define DLPG_SUB_PHASE_LABEL_STR "%+0.1f\xB0"
#define DLPG_SUB_NOTE_LABEL_STR "%s (%0.2f Hz)"
#define DLPG_ENVELOPE_ATTACK_LABEL_STR DLPG_ENVELOPE_GENERIC_STR
#define DLPG_ENVELOPE_HOLD_LABEL_STR DLPG_ENVELOPE_GENERIC_STR
#define DLPG_ENVELOPE_RELEASE_LABEL_STR DLPG_ENVELOPE_GENERIC_STR
#define DLPG_VOL_LABEL_STR DLPG_LEVEL_GENERIC_STR

#define DLPG_SET_LABEL_GENERIC(buf, str, id, obj)\
sprintf(buf, str, GetParam(id)->Value());\
obj->SetTextFromPlug(buf)

#ifdef _WIN32
#define DLPG_KNOB_LABEL_FONT_SIZE 14
#define DLPG_VERSION_TEXT_LABEL_FONT_SIZE 12
#elif defined(__APPLE__)
#define DLPG_KNOB_LABEL_FONT_SIZE 15
#define DLPG_VERSION_TEXT_LABEL_FONT_SIZE 13
#endif
#define DLPG_BUGREPORT_LABEL_FONT_SIZE DLPG_VERSION_TEXT_LABEL_FONT_SIZE

// Choose between kLogarithmic or kLinear
#define DLPG_ENVELOPE_ATTACK_SHAPE kLinear
#define DLPG_ENVELOPE_RELEASE_SHAPE DLPG_ENVELOPE_ATTACK_SHAPE

#define DLPG_MIDI_NOTES_TOTAL 128

#define DLPG_DEFAULT_PREVIEW_NOTE 64
#define DLPG_DEFAULT_PREVIEW_CH 10
#define DLPG_PREVIEW_VELOCITY 100
#define DLPG_REQUIRED_VELOCITY 1

// Channel numbers start with 0 in IPlug, but they start with 1 in actual MIDI
#define DLPG_MIDI_CH_TO_IPLUG_INDEX(ch) (ch - 1)
#define DLPG_MIDI_IPLUG_INDEX_TO_CH(i) (i + 1)

#define DLPG_ENUM_CONTROL_STRING_SIZE 32

const int kNumPrograms = 1;

enum EParams
{
  kScope,
  kBypassSwitch,
  kTrigSwitch,
  kSnapSwitch,
  kFlipSwitch,
  kTrigInputSwitch,
  kTrigInpMuteSwitch,
  kTrigNoteKnob,
  kTrigChKnob,
  kTrigAttackKnob,
  kTrigThreshKnob,
  kSubFreqKnob,
  kSubPhaseKnob,
  kSubNoteKnob,
  kEnvelopeAttackKnob,
  kEnvelopeHoldKnob,
  kEnvelopeReleaseKnob,
  kVolKnob,
  kOutputMeter,
  kNumParams,
  kTrigNoteLabel,
  kTrigChLabel,
  kTrigAttackLabel,
  kTrigThreshLabel,
  kSubFreqLabel,
  kSubPhaseKLabel,
  kSubNoteLabel,
  kEnvelopeAttackLabel,
  kEnvelopeHoldLabel,
  kEnvelopeReleaseLabel,
  kVolLabel,
};

enum ELayout
{
  kWidth = GUI_WIDTH,
  kHeight = GUI_HEIGHT,

  kScopeX = 122,
  kScopeY = 10,

  kBypassSwitchX = DLPG_SWITCH_X_BASE,
  kBypassSwitchY = DLPG_SWITCH_Y_BASE,

  kTrigSwitchX = DLPG_SWITCH_X_BASE,
  kTrigSwitchY = DLPG_SWITCH_Y_BASE + DLPG_SWITCH_Y_STEP,

  kSnapSwitchX = DLPG_SWITCH_X_BASE,
  kSnapSwitchY = DLPG_SWITCH_Y_BASE + 2*DLPG_SWITCH_Y_STEP,

  kFlipSwitchX = DLPG_SWITCH_X_BASE,
  kFlipSwitchY = DLPG_SWITCH_Y_BASE + 3*DLPG_SWITCH_Y_STEP,

  kFreezeSwitchX = DLPG_SWITCH_X_BASE,
  kFreezeSwitchY = DLPG_SWITCH_Y_BASE + 4*DLPG_SWITCH_Y_STEP,

  kTextVersionX = 10,
  kTextVersionY = 448,

  kBugreporLabelX = 27,
  kBugreporLabelY = 501,

  kOutputMeterX = 785,
  kOutputMeterY = 10,
};

const IRECT tOutputMeterIrect(\
  kOutputMeterX, \
  kOutputMeterY, \
  kOutputMeterX + DLPG_OUTPUT_METER_W, \
  kOutputMeterY+DLPG_OUTPUT_METER_H);

const IRECT tTextVersionIrect(
  kTextVersionX,
  kTextVersionY,
  kTextVersionX + DLPG_VERSION_TEXT_LABEL_W,
  kTextVersionY + DLPG_VERSION_TEXT_LABEL_H
  );
const IRECT tBugreportLabelIrect(
  kBugreporLabelX,
  kBugreporLabelY,
  kBugreporLabelX + DLPG_BUGREPORT_LABEL_W,
  kBugreporLabelY + DLPG_BUGREPORT_LABEL_H
  );
const IRECT PLUG_ScopeIrect(
  kScopeX,
  kScopeY,
  kScopeX + DLPG_SCOPE_W,
  kScopeY + DLPG_SCOPE_H
);

class SubKicker : public IPlug
{
public:
  SubKicker(IPlugInstanceInfo instanceInfo);
  ~SubKicker();

  void Reset();
  void OnParamChange(int paramIdx);
  void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);

private:
  bool UpdateWaveform();
  std::vector<double> vSubkickWaveform;
  bool bIsBypassed;
  dlpg::IWavScopeControl* tScope;
  dlpg::WaveGenerator* tWaveGenerator;
  dlpg::EnvelopeGenerator* tEnvelopeGenerator;
  Plug::ILevelMeteringBar* tOutputMeter;
  ISwitchControl *tBypassSwitch;
  ISwitchControl *tTrigSwitch;
  ISwitchControl *tSnapSwitch;
  ISwitchControl *tFlipSwitch;
  ISwitchControl *tTrigInputSwitch;
  ISwitchControl *tTrigInpMuteSwitch;
  IKnobMultiControl *tTrigNoteKnob;
  IKnobMultiControl *tTrigChKnob;
  IKnobMultiControl *tTrigAttackKnob;
  IKnobMultiControl *tTrigThreshKnob;
  IKnobMultiControl *tSubFreqKnob;
  IKnobMultiControl *tSubNoteKnob;
  IKnobMultiControl *tSubPhaseKnob;
  IKnobMultiControl *tEnvelopeAttackKnob;
  IKnobMultiControl *tEnvelopeHoldKnob;
  IKnobMultiControl *tEnvelopeReleaseKnob;
  IKnobMultiControl *tVolKnob;
  ITextControl *tTrigNoteLabel;
  ITextControl *tTrigChLabel;
  ITextControl *tTrigAttackLabel;
  ITextControl *tTrigThreshLabel;
  ITextControl *tSubFreqLabel;
  ITextControl *tSubPhaseLabel;
  ITextControl *tSubNoteLabel;
  ITextControl *tEnvelopeAttackLabel;
  ITextControl *tEnvelopeHoldLabel;
  ITextControl *tEnvelopeReleaseLabel;
  ITextControl *tVolLabel;
  IGraphics* pGraphics;
  // Midi stuff
  void ProcessMidiMsg(IMidiMsg* pMsg);
  IMidiQueue tMidiQueue;
  bool abKeyStatus[DLPG_MIDI_NOTES_TOTAL];
  int nNumKeysPressed;
  // For switching from Hz mode to note snap mode of Sub Frequency knob
  inline int HzToSubNoteKnobValue(double fFrequency);
  double fOutputGainLinear;
};

#endif
