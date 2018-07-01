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
  double nWaveX;

  pGraphics->FillIRect(&DLPG_DEFAULT_SCOPE_BG_ICOLOR, &this->mScopeRect);

  // Assuming the buffer has at least 2 samples
  if(pvBuffer.size()<2)
    return true;

  /*
  First fill, then outline. We have to draw them in separate loops,
  otherwise the waveform won't look clean.
  */
  nWaveX = this->mScopeRect.L;
  for(std::vector<double>::size_type i = 0; i != pvBuffer.size(); i++, nWaveX+=1/fScale){
    pGraphics->DrawLine(
      &DLPG_DEFAULT_SCOPE_FILL_ICOLOR,
      nWaveX,
      this->mScopeRect.T + DLPG_SCOPE_SCALE_SIGNAL(0),
      nWaveX,
      this->mScopeRect.T + DLPG_SCOPE_SCALE_SIGNAL(pvBuffer[i])
      );
  }
  nWaveX = this->mScopeRect.L;
  for(std::vector<double>::size_type i = 1; i != pvBuffer.size(); i++, nWaveX+=1/fScale){
    pGraphics->DrawLine(
      &DLPG_DEFAULT_SCOPE_OUTLINE_ICOLOR,
      nWaveX,
      this->mScopeRect.T + DLPG_SCOPE_SCALE_SIGNAL(pvBuffer[i-1]),
      nWaveX + 1/fScale,
      this->mScopeRect.T + DLPG_SCOPE_SCALE_SIGNAL(pvBuffer[i]),
      0,
      true
      );
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
