#ifndef __SUBKICKER__
#define __SUBKICKER__

#include "IPlug_include_in_plug_hdr.h"
#include "DLPG_IControlExtras.h"
#include "DLPG_WaveGenerator.h"

#define DLPG_SWITCH_STATES 2

#define DLPG_SWITCH_X_BASE 520
#define DLPG_SWITCH_Y_BASE 10
#define DLPG_SWITCH_Y_STEP 36

#define DLPG_DEFAULT_BYPASS_SWITCH_STATE 0
#define DLPG_DEFAULT_TRIG_SWITCH_STATE 0
#define DLPG_DEFAULT_SNAP_SWITCH_STATE 1
#define DLPG_DEFAULT_FLIP_SWITCH_STATE 0
#define DLPG_DEFAULT_FREEZE_SWITCH_STATE 0

#define DLPG_STANDARD_KNOB_FRAMES 128
#define DLPG_TRIG_CH_KNOB_FRAMES 17
#define DLPG_TRIG_NOTE_KNOB_FRAMES 129

#define DLPG_KNOB_X_BASE 20
#define DLPG_KNOB_X_STEP 165
#define DLPG_KNOB_Y_BASE 200
#define DLPG_KNOB_Y_STEP 175

#define DLPG_KNOB_GRID(row, col)\
(DLPG_KNOB_X_BASE + (col-1)*DLPG_KNOB_X_STEP),\
(DLPG_KNOB_Y_BASE + (row-1)*DLPG_KNOB_Y_STEP)

#define DLPG_VOL_KNOB_SHAPE 0.26

const int kNumPrograms = 1;

enum EParams
{
  kGain,
  kScope,
  kBypassSwitch,
  kTrigSwitch,
  kSnapSwitch,
  kFlipSwitch,
  kFreezeSwitch,
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

  kScopeX = 10,
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
};

const IRECT PLUG_ScopeIrect(
  kScopeX,
  kScopeY,
  kScopeX + DLPG_SCOPE_W_PX,
  kScopeY + DLPG_SCOPE_H_PX
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
  ISwitchControl *tFreezeSwitch;
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
