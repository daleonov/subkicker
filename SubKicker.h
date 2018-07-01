#ifndef __SUBKICKER__
#define __SUBKICKER__

#include "IPlug_include_in_plug_hdr.h"

class SubKicker : public IPlug
{
public:
  SubKicker(IPlugInstanceInfo instanceInfo);
  ~SubKicker();

  void Reset();
  void OnParamChange(int paramIdx);
  void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);

private:
  double mGain;
};

#endif
