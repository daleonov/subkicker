
#ifndef _DLPG_ICONTROLEXTRAS_H
#define _DLPG_ICONTROLEXTRAS_H

#include "IControl.h"
#include <vector>

// Set to true to fill waveform to zero
#define DLPG_SCOPE_FILL true

// Set to true to enable antialiasing
#define DLPG_SCOPE_ANTIALIAS false

// Distance between edges of scope and waveform
#define DLPG_SCOPE_V_PADDING 3
#define DLPG_SCOPE_H_PADDING 3

// Scope range is from -DLPG_SCOPE_V_RANGE to +DLPG_SCOPE_V_RANGE
#define DLPG_SCOPE_V_RANGE 1.
#define DLPG_SCOPE_SCALE_SIGNAL(s, h) \
((s+DLPG_SCOPE_V_RANGE)*((double)h - 2*DLPG_SCOPE_V_PADDING)/2)

const IColor DLPG_DEFAULT_SCOPE_BG_ICOLOR(255, 40, 40, 40);
const IColor DLPG_DEFAULT_SCOPE_OUTLINE_ICOLOR(255, 255, 255, 255);
#if (DLPG_SCOPE_FILL == true)
const IColor DLPG_DEFAULT_SCOPE_FILL_ICOLOR(255, 55, 55, 55);
#endif

namespace dlpg{
class IWavScopeControl: public IControl{

public:
  IWavScopeControl(
    IPlugBase *pPlug,
    IRECT pR,
    int paramIdx,
    std::vector<double> &vBuffer
    );

  ~IWavScopeControl();
  /*
  @param Graphic part. Sizes and colours are based on macros defined in the header. 
  */
  bool Draw(IGraphics* pGraphics);
  bool LoadWave(std::vector<double> &pvBuffer);
  bool UpdateScale(double fDuration, double fSampleRate);

private:
  IRECT mScopeRect;
  std::vector<double> pvBuffer;
  double fScale;

};
} //namespace dlpg

#endif //_PLUG_ICONTROLEXTRAS_H
