
#ifndef _DLPG_ICONTROLEXTRAS_H
#define _DLPG_ICONTROLEXTRAS_H

#include "IControl.h"
#include <vector>

// Set to true to fill waveform to zero
#define DLPG_SCOPE_FILL true

// Set to true to enable antialiasing
#define DLPG_SCOPE_ANTIALIAS false

// Distance between edges of scope and waveform
#define DLPG_SCOPE_V_PADDING 0
#define DLPG_SCOPE_H_PADDING 0

// Scope range is from -DLPG_SCOPE_V_RANGE to +DLPG_SCOPE_V_RANGE
#define DLPG_SCOPE_V_RANGE 1.
/* Positive signal has to go towards the negative side of Y axis,
   since Y axis for pixels is pointed down. */
#define DLPG_SCOPE_SCALE_SIGNAL(s, h) \
((-s+DLPG_SCOPE_V_RANGE)*((double)h - 2*DLPG_SCOPE_V_PADDING)/2)

const IColor DLPG_DEFAULT_SCOPE_BG_ICOLOR(255, 40, 40, 40);
const IColor DLPG_DEFAULT_SCOPE_OUTLINE_ICOLOR(255, 255, 255, 255);
#if (DLPG_SCOPE_FILL == true)
const IColor DLPG_DEFAULT_SCOPE_FILL_ICOLOR(255, 55, 55, 55);
#endif
const IColor DLPG_SCOPE_ONCLICK_ICOLOR(20, 255, 255, 255);

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
  bool Draw(IGraphics* pGraphics);
  void OnMouseDown(int x, int y, IMouseMod* pMod);
  void OnMouseUp(int x, int y, IMouseMod* pMod);
  bool LoadWave(std::vector<double> &pvBuffer);
  /*
  @brief Highlight (literally) the scope - for clicking etc
  @param bHighlightState true = highlighted, false = normal
  */
  bool Highlight(bool bHighlightState);

private:
  IRECT mScopeRect;
  std::vector<double> pvBuffer;
  bool bHighlightState;
};
} //namespace dlpg

#endif //_PLUG_ICONTROLEXTRAS_H
