#include "DLPG_IControlExtras.h"
#include "IGraphics.h"

namespace dlpg{

IWavScopeControl::IWavScopeControl(
  IPlugBase *pPlug,
  IRECT pR,
  int paramIdx,
  std::vector<double> &vBuffer
  )
:IControl(pPlug, pR, mParamIdx){
  this->mPlug = pPlug;
  this->mParamIdx = paramIdx;
  this->pvBuffer = vBuffer;
  memcpy(&this->mScopeRect, &pR, sizeof(pR));
}


IWavScopeControl::~IWavScopeControl(){

}

bool IWavScopeControl::LoadWave(std::vector<double> &vBuffer){
  this->pvBuffer = vBuffer;
  SetDirty(false);
  Redraw();
  return true;
}

bool IWavScopeControl::Draw(IGraphics* pGraphics){
  pGraphics->FillIRect(&DLPG_DEFAULT_SCOPE_BG_ICOLOR, &this->mScopeRect);

  int nWaveX = 10;
  for(auto i = pvBuffer.begin(); i != pvBuffer.end(); ++i, nWaveX++){
    pGraphics->ForcePixel(&DLPG_DEFAULT_SCOPE_OUTLINE_ICOLOR, nWaveX, ((double)*i)*50+100);
  }

  return true;
}

} //namespace dlpg
