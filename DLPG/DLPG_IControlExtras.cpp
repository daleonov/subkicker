#include "DLPG_IControlExtras.h"

namespace dlpg{

IWavScopeControl::IWavScopeControl(
  IPlugBase *pPlug,
  IRECT pR,
  int paramIdx
  )
:IControl(pPlug, pR, mParamIdx){
  this->mPlug = pPlug;
  this->mParamIdx = paramIdx;
  memcpy(&this->mScopeRect, &pR, sizeof(pR));
}


IWavScopeControl::~IWavScopeControl(){

}

bool IWavScopeControl::Draw(IGraphics* pGraphics){
  pGraphics->FillIRect(&DLPG_DEFAULT_SCOPE_BG_ICOLOR, &this->mScopeRect);
  return true;
}

} //namespace dlpg
