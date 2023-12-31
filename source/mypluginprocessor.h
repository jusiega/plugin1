//------------------------------------------------------------------------
// Copyright(c) 2023 pjmw.
//------------------------------------------------------------------------

#pragma once

#include "myplugincids.h"
#include "public.sdk/source/vst/vstaudioeffect.h"

namespace xD {

//------------------------------------------------------------------------
//  plugin1Processor
//------------------------------------------------------------------------
class plugin1Processor : public Steinberg::Vst::AudioEffect
{
public:
	plugin1Processor ();
	~plugin1Processor () SMTG_OVERRIDE;
    // Create function
	static Steinberg::FUnknown* createInstance (void* /*context*/) 
	{ 
		return (Steinberg::Vst::IAudioProcessor*)new plugin1Processor; 
	}

	//--- ---------------------------------------------------------------------
	// AudioEffect overrides:
	//--- ---------------------------------------------------------------------
	/** Called at first after constructor */
	Steinberg::tresult PLUGIN_API initialize (Steinberg::FUnknown* context) SMTG_OVERRIDE;
	
	/** Called at the end before destructor */
	Steinberg::tresult PLUGIN_API terminate () SMTG_OVERRIDE;
	
	/** Switch the Plug-in on/off */
	Steinberg::tresult PLUGIN_API setActive (Steinberg::TBool state) SMTG_OVERRIDE;

	/** Will be called before any process call */
	Steinberg::tresult PLUGIN_API setupProcessing (Steinberg::Vst::ProcessSetup& newSetup) SMTG_OVERRIDE;
	
	/** Asks if a given sample size is supported see SymbolicSampleSizes. */
	Steinberg::tresult PLUGIN_API canProcessSampleSize (Steinberg::int32 symbolicSampleSize) SMTG_OVERRIDE;

	/** Here we go...the process call */
	Steinberg::tresult PLUGIN_API process (Steinberg::Vst::ProcessData& data) SMTG_OVERRIDE;
		
	/** For persistence */
	Steinberg::tresult PLUGIN_API setState (Steinberg::IBStream* state) SMTG_OVERRIDE;
	Steinberg::tresult PLUGIN_API getState (Steinberg::IBStream* state) SMTG_OVERRIDE;

//------------------------------------------------------------------------
protected:
	Steinberg::Vst::ParamValue ingain=1.0;
	Steinberg::Vst::ParamValue d1gain=1.0;
	Steinberg::Vst::ParamValue tdelay1=maxdelay*0.4;
	Steinberg::Vst::ParamValue d2gain=1.0;
	Steinberg::Vst::ParamValue tdelay2=maxdelay*0.5;
	Steinberg::Vst::ParamValue pan1=0.2;
	Steinberg::Vst::ParamValue pan2=0.8;
	Steinberg::Vst::ParamValue amplitude1=0.5;
	Steinberg::Vst::ParamValue period1=flangermax*0.5;
	Steinberg::Vst::ParamValue amplitude2=0.5;
	Steinberg::Vst::ParamValue period2=flangermax*0.5;
	Steinberg::Vst::ParamValue master=1.0;
	Steinberg::Vst::SampleRate SR=44100;
	double dt1;
	double dt2;
	//int32 numChannels=2;
	Steinberg::Vst::Sample32* mem;
	Steinberg::Vst::Sample32* F1;
	size_t F1idx;
	size_t diff;
	long long readidx[10];
	long long readidx2[10];
	Steinberg::int32 numChannels=2;
	long long writeidx[10];
	long long ML=SR*maxdelay*2;
	long long idx_OLD;
	size_t TT1=SR*flangermax;
	double systime;


};

//------------------------------------------------------------------------
} // namespace xD
