
#ifndef _DLPG_ICONTROLEXTRAS_H
#define _DLPG_ICONTROLEXTRAS_H

#include "IControl.h"
#include <vector>

// Scope range is from -DLPG_SCOPE_V_RANGE to +DLPG_SCOPE_V_RANGE
#define DLPG_SCOPE_V_RANGE 1.
#define DLPG_SCOPE_SCALE_SIGNAL(s, h) ((s+DLPG_SCOPE_V_RANGE)*(double)h/2)

const IColor DLPG_DEFAULT_SCOPE_BG_ICOLOR(255, 61, 61, 61);
const IColor DLPG_DEFAULT_SCOPE_OUTLINE_ICOLOR(255, 247, 147, 30);
const IColor DLPG_DEFAULT_SCOPE_FILL_ICOLOR(255, 128, 76, 15);

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
