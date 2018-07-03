#ifndef __SUBKICKER__
#define __SUBKICKER__

#include "IPlug_include_in_plug_hdr.h"
#include "DLPG_IControlExtras.h"
#include "DLPG_WaveGenerator.h"

// If this file is missing, run "git_version" script from project folder,
// or create an empty file if you don't use git.
#include "DLPG_Version.h"

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

#define DLPG_KNOB_X_BASE 120
#define DLPG_KNOB_X_STEP 167
#define DLPG_KNOB_Y_BASE 145
#define DLPG_KNOB_Y_STEP 198

#define DLPG_KNOB_GRID(row, col)\
(DLPG_KNOB_X_BASE + (col-1)*DLPG_KNOB_X_STEP),\
(DLPG_KNOB_Y_BASE + (row-1)*DLPG_KNOB_Y_STEP)

#define DLPG_VOL_KNOB_SHAPE 0.26

#define DLPG_SCOPE_W 440
#define DLPG_SCOPE_H 105

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

#ifdef _WIN32
#define DLPG_VERSION_TEXT_LABEL_FONT_SIZE 12
#elif defined(__APPLE__)
#define DLPG_VERSION_TEXT_LABEL_FONT_SIZE 13
#endif

const int kNumPrograms = 1;

enum EParams
{
  kGain,
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
  kEnvelopeAttackKnob,
  kEnvelopeHoldKnob,
  kEnvelopeReleaseKnob,
  kVolKnob,
  kNumParams
};

enum ELayout
{
  kWidth = GUI_WIDTH,
  kHeight = GUI_HEIGHT,

  kGainX = 100,
  kGainY = 300,
  kKnobFrames = 60,

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

  kTextVersion_X = 10,
  kTextVersion_Y = 448,
};

const IRECT tTextVersionIrect(
  kTextVersion_X,
  kTextVersion_Y,
  (kTextVersion_X + DLPG_VERSION_TEXT_LABEL_W),
  kTextVersion_Y + DLPG_VERSION_TEXT_LABEL_H
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
  double mGain;
  dlpg::IWavScopeControl* tScope;
  dlpg::WaveGenerator* tWaveGenerator;
  dlpg::EnvelopeGenerator* tEnvelopeGenerator;
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
  IKnobMultiControl *tSubPhaseKnob;
  IKnobMultiControl *tEnvelopeAttackKnob;
  IKnobMultiControl *tEnvelopeHoldKnob;
  IKnobMultiControl *tEnvelopeReleaseKnob;
  IKnobMultiControl *tVolKnob;
};

#endif
