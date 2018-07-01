
#ifndef _DLPG_ICONTROLEXTRAS_H
#define _DLPG_ICONTROLEXTRAS_H

#include "IControl.h"

#ifndef DLPG_DEFAULT_SCOPE_BG_ICOLOR
const IColor DLPG_DEFAULT_SCOPE_BG_ICOLOR(255, 61, 61, 61);
#endif


namespace dlpg{
class IWavScopeControl: public IControl{

public:
  IWavScopeControl(
    IPlugBase *pPlug,
    IRECT pR,
    int paramIdx
    );

  ~IWavScopeControl();
  /*
  @param Graphic part. Sizes and colours are based on macros defined in the header. 
  */
  bool Draw(IGraphics* pGraphics);

private:
  IRECT mScopeRect;

};
} //namespace dlpg

#endif //_PLUG_ICONTROLEXTRAS_H
