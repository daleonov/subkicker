
#ifndef _DLPG_ICONTROLEXTRAS_H
#define _DLPG_ICONTROLEXTRAS_H

#include "IControl.h"
#include <vector>

const IColor DLPG_DEFAULT_SCOPE_BG_ICOLOR(255, 61, 61, 61);
const IColor DLPG_DEFAULT_SCOPE_OUTLINE_ICOLOR(255, 85, 85, 85);

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

private:
  IRECT mScopeRect;
  std::vector<double> pvBuffer;

};
} //namespace dlpg

#endif //_PLUG_ICONTROLEXTRAS_H
