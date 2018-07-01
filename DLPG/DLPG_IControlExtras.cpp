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
  fScale = 1.;
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

  double nWaveX = 10;
  /*
  for(auto i = pvBuffer.begin(); i != pvBuffer.end(); ++i, nWaveX++){
    pGraphics->ForcePixel(&DLPG_DEFAULT_SCOPE_OUTLINE_ICOLOR, nWaveX, ((double)*i)*50+100);
  }*/
  for(std::vector<double>::size_type i = 0; i != pvBuffer.size(); i++, nWaveX+=1/fScale){
    pGraphics->ForcePixel(&DLPG_DEFAULT_SCOPE_OUTLINE_ICOLOR, nWaveX, (pvBuffer[i])*50+100);
  }

  return true;
}

bool IWavScopeControl::UpdateScale(double fDuration, double fSampleRate){
  /*
  Quick maths.
  User changeable params:
  - F (frequency, Hz),
  - Ds (duration, seconds);
  System parameters:
  - Sr (sample rate, Hz),
  - W (scope width, pixels);
  Variable parameters:
  - Dn (duration, samples)
  - Scl (scope scale, samples/pixel)

  Dn[samples] = Ds[seconds] * Sr[Hz]
  Scl[samples/pixel] = Dn/W = Ds * Sr / W
  */
  this->fScale = fDuration * fSampleRate / DLPG_SCOPE_W_PX;
  SetDirty(false);
  Redraw();
  return true;
}

} //namespace dlpg
