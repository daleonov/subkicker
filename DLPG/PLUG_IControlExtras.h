
#ifndef _PLUG_ICONTROLEXTRAS_H
#define _PLUG_ICONTROLEXTRAS_H

#include "IControl.h"

#ifndef PLUG_DEFAULT_BG_ICOLOR
const IColor PLUG_DEFAULT_BG_ICOLOR(255, 40, 40, 40);
#endif

#ifndef PLUG_BAR_OVERLOAD_COLOR
const IColor PLUG_BAR_OVERLOAD_COLOR(255, 255, 0, 49);
#endif
const IColor METERING_BAR_DEFAULT_BG_ICOLOR(255, 61, 61, 61);
const IColor METERING_BAR_DEFAULT_FG_ICOLOR(255, 100, 255, 100);
const IColor METERING_BAR_DEFAULT_NOTCH_ICOLOR(255, 0, 96, 0);
#define METERING_BAR_ABOVE_NOTCH_ICOLOR PLUG_BAR_OVERLOAD_COLOR

#define METERING_BAR_DEFAULT_SIZE_IRECT IRECT(0, 0, 74, 492)
#define METERING_BAR_MIN_FG_HEIGHT 2
#define METERING_BAR_DEFAULT_NOTCH_VALUE -0.
#define METERING_BAR_MAX_NAME_SIZE 64
#define METERING_BAR_MAX_LABEL_SIZE 32

#ifndef PLUG_ALMOST_PLUS_0_DB
#define PLUG_ALMOST_PLUS_0_DB (1./1000)
#endif

// This define results in a background showing range to fNotchValue
#define METERING_BAR_NOTCH_1
// This define results in a small line corresponding to fNotchValue
//#define METERING_BAR_NOTCH_2
// Normally you would want to use only one of those two.

#ifdef METERING_BAR_NOTCH_1
#define METERING_BAR_DEFAULT_NOTCH_HEIGHT 0
#else
#define METERING_BAR_DEFAULT_NOTCH_HEIGHT 3
#endif

// Transparency of the bar in METERING_BAR_NOTCH_1 mode
#define PLUG_METERING_NOTCH_1_ALPHA 64
#define PLUG_METERING_SOLID_ALPHA 255
#define PLUG_METERING_NOTCH_1_NORMALIZED_ALPHA ((double)PLUG_METERING_NOTCH_1_ALPHA/PLUG_METERING_SOLID_ALPHA)

// If defined, the notch color is calculated as a solid color based on alpha value
// Saves resources. Remove this define to use actual alpha blending. 
#define PLUG_METERING_DONT_USE_ALPHA

// color = alpha * src + (1 - alpha) * dest, or
// color = alpha * (src - dest) + dest
#ifdef PLUG_METERING_DONT_USE_ALPHA
#define PLUG_METERING_CALC_COLOR(alpha, back, front) \
(alpha * (front-back) + back)
#endif

namespace Plug{
/*
@brief Vertical metering bar. Can be used for VU, loudness, gain reduction metering etc.
@param pPlug Plugin class pointer
@param x Bar's top left corner x
@param y Bar's top left corner y
@param pR IRECT representing the bar's size. Relative coordinates. 
@param paramIdx Unique ID of the control
*/
class ILevelMeteringBar : public IPanelControl
{
private:
	int mParamIdx;
	double fCurrentValue;
	double fNotchValue;
	IRECT mBarRect;
	int x;
	int y;
	bool bIsReversed;
	IColor *ptLevelBarColor;
	IColor *ptNotchColor;
	IColor *ptAboveNotchColor;

	/*
	@param Converts value in meter's units to respective vertical coordinate on the bar. 
	@retval Converted vertical coordinate in pixels relative to bottom y of the meter.
	*/
	int _CalculateRectHeight(double fValue);
public:
	ILevelMeteringBar(
		IPlugBase *pPlug,
		int x,
		int y,
		IRECT pR,
		int paramIdx,
		bool bIsReversed = false,
		const IColor *ptLevelBarColor = &METERING_BAR_DEFAULT_FG_ICOLOR,
		const IColor *ptNotchColor = &METERING_BAR_DEFAULT_NOTCH_ICOLOR,
		const IColor *ptAboveNotchColor = &METERING_BAR_ABOVE_NOTCH_ICOLOR
		);

	~ILevelMeteringBar();

	/*
	@param Graphic part. Sizes and colours are based on macros defined in the header. 
	*/
	bool Draw(IGraphics* pGraphics);

	/*
	@brief Store current value of the meter.
	@note That method doesn't redraw the meter - call Draw() to update graphics. 
	*/
	void SetValue(double fValue);

	/*
	@brief Value of the notch. Notch can be used to mark target, peak, zero level etc. 
	*/
	void SetNotchValue(double fValue);

};

} //namespace Plug

#endif //_PLUG_ICONTROLEXTRAS_H
