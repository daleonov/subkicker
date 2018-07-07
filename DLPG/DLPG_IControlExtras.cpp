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
  bHighlightState = false;
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
  const int nHeight = this->mScopeRect.B - this->mScopeRect.T;
  const int nWaveYBase = this->mScopeRect.T + DLPG_SCOPE_V_PADDING;
  IColor *pBgColor;

  pGraphics->FillIRect(&DLPG_DEFAULT_SCOPE_BG_ICOLOR, &this->mScopeRect);

  // Assuming the buffer has at least 2 samples
  if(pvBuffer.size()<2)
    return true;

  #if (DLPG_SCOPE_FILL == true)
  /*
  First fill, then outline. We have to draw them in separate loops,
  otherwise the waveform won't look clean.
  */
  nWaveX = this->mScopeRect.L + DLPG_SCOPE_H_PADDING;
  for(std::vector<double>::size_type i = 0; i != pvBuffer.size(); i++, nWaveX+=1/fScale){
    pGraphics->DrawLine(
      &DLPG_DEFAULT_SCOPE_FILL_ICOLOR,
      nWaveX,
      nWaveYBase + DLPG_SCOPE_SCALE_SIGNAL(0, nHeight),
      nWaveX,
      nWaveYBase + DLPG_SCOPE_SCALE_SIGNAL(pvBuffer[i], nHeight)
      );
  }
  #endif //DLPG_SCOPE_FILL
  nWaveX = this->mScopeRect.L + DLPG_SCOPE_H_PADDING;
  for(std::vector<double>::size_type i = 1; i != pvBuffer.size(); i++, nWaveX+=1/fScale){
    pGraphics->DrawLine(
      &DLPG_DEFAULT_SCOPE_OUTLINE_ICOLOR,
      nWaveX,
      nWaveYBase + DLPG_SCOPE_SCALE_SIGNAL(pvBuffer[i-1], nHeight),
      nWaveX + 1/fScale,
      nWaveYBase + DLPG_SCOPE_SCALE_SIGNAL(pvBuffer[i], nHeight),
      0,
      DLPG_SCOPE_ANTIALIAS
      );
    }

  if(bHighlightState){
    pGraphics->FillIRect(&DLPG_SCOPE_ONCLICK_ICOLOR, &this->mScopeRect);
  }
  //SetDirty(false);

  return true;
}

bool IWavScopeControl::Highlight(bool bHighlightState){
  this->bHighlightState = bHighlightState;
  SetDirty(false);
  Redraw();
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
  this->fScale = \
    fDuration * fSampleRate / (mScopeRect.R - mScopeRect.L - 2*DLPG_SCOPE_H_PADDING);
  //SetDirty(false);
  //Redraw();
  return true;
}

void IWavScopeControl::OnMouseDown(int x, int y, IMouseMod* pMod){
  SetDirty(true);
}

void IWavScopeControl::OnMouseUp(int x, int y, IMouseMod* pMod){
  SetDirty(false);
}

} //namespace dlpg
