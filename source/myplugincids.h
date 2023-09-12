//------------------------------------------------------------------------
// Copyright(c) 2023 pjmw.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace xD {
//------------------------------------------------------------------------
enum GainParams : Steinberg::Vst::ParamID
{
    kParamGainId=102, // should be a unique id...
    kParamDelayGainId=101,
    kParamDelay2GainId=111,
    kParamMasterId=100,
};
enum DelayParams : Steinberg::Vst::ParamID
{
    kParamDelayId=103,
    kParamDelayPanId=104,
    kParamDelay2Id=113,
    kParamDelay2PanId=114,
};
enum FlangerParams : Steinberg::Vst::ParamID
{
    kParamFlangerAmplitudeId=105,
    kParamFlangerPeriodId=106,
    kParamFlanger2AmplitudeId=115,
    kParamFlanger2PeriodId=116,
};

static const double maxdelay=1;
//static const double flangerlow=0;
static const double flangermax=40;

static const Steinberg::FUID kplugin1ProcessorUID (0x7A379F59, 0x443E5A6E, 0x9EDADFAB, 0xC31282BD);
static const Steinberg::FUID kplugin1ControllerUID (0x49AFF30A, 0x6A965AC4, 0xBF1BAD24, 0xCB9A594F);

#define plugin1VST3Category "Fx"

//------------------------------------------------------------------------
} // namespace xD
