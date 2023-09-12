//------------------------------------------------------------------------
// Copyright(c) 2023 pjmw.
//------------------------------------------------------------------------

#pragma once

#include "pluginterfaces/base/funknown.h"
#include "pluginterfaces/vst/vsttypes.h"

namespace xD {
//------------------------------------------------------------------------
enum EQParams : Steinberg::Vst::ParamID
{
    kParamHiPassId=100, // should be a unique id...
    kParamLowToggleId=101,
    kParamLowFreqId=102,
    kParamLowGainId=103,
    kParamMidFreqId=104,
    kParamMidGainId=105,
    kParamHiToggleId=106,
    kParamHiFreqId=107,
    kParamHiGainId=108,
    kParamLowPassId=109,
    
    kParamMasterId=110,
};

static const Steinberg::FUID kEQF1ProcessorUID (0x309B6227, 0x5D1350D7, 0x9FAB2403, 0x573B6AFD);
static const Steinberg::FUID kEQF1ControllerUID (0x6C7B1066, 0xCEC75081, 0x879DACC2, 0x4FD1EB7C);

#define EQF1VST3Category "Fx"

//------------------------------------------------------------------------
} // namespace xD
