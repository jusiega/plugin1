//------------------------------------------------------------------------
// Copyright(c) 2023 pjmw.
//------------------------------------------------------------------------

#pragma once

#include "public.sdk/source/vst/vstaudioeffect.h"
#include "fftw3.h"

#define ncoefs 256
namespace xD {

//------------------------------------------------------------------------
//  EQF1Processor
//------------------------------------------------------------------------
class EQF1Processor : public Steinberg::Vst::AudioEffect
{
public:
	EQF1Processor ();
	~EQF1Processor () SMTG_OVERRIDE;

    // Create function
	static Steinberg::FUnknown* createInstance (void* /*context*/) 
	{ 
		return (Steinberg::Vst::IAudioProcessor*)new EQF1Processor; 
	}
	void curva_grande(int type, fftw_complex* eq_kurve, double f0, double gejn, Steinberg::Vst::SampleRate SR);
	void lowshelf(fftw_complex* eq_kurve, double w0, double gejn, Steinberg::Vst::SampleRate SR);
	void hishelf(fftw_complex* eq_kurve, double w0, double gejn, Steinberg::Vst::SampleRate SR);
	void lowpass(fftw_complex* eq_kurve, double w0, Steinberg::Vst::SampleRate SR);
	void highpass(fftw_complex* eq_kurve, double w0, Steinberg::Vst::SampleRate SR);
	void band(fftw_complex* eq_kurve, double w0, double gejn, Steinberg::Vst::SampleRate SR);
	void shiftrealnorm(int type);
	void xxx(fftw_complex* IR);//makes transformated curve (from memx) into fir

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
	double lgain=0.5;
	double mgain=0.5;
	double hgain=0.5;
	
	double lf=201;
	double mf=751;
	double hf=3001;
	
	double hpf=1.0;
	double lpf=20001.0;
	
	bool tl=1;
	bool th=1;
	
	Steinberg::Vst::ParamValue master=1.0;
	Steinberg::Vst::SampleRate SR=44100;
	double dt1;
	//int32 numChannels=2;
	Steinberg::Vst::Sample32* mem;
	fftw_complex meml	[2*ncoefs+1];
	fftw_complex memm	[2*ncoefs+1];
	fftw_complex memh	[2*ncoefs+1];
	fftw_complex memhp	[2*ncoefs+1];
	fftw_complex memlp	[2*ncoefs+1];
	
	fftw_complex eq_kurve_l		[2*ncoefs+1];
	fftw_complex eq_kurve_m		[2*ncoefs+1];
	fftw_complex eq_kurve_h		[2*ncoefs+1];
	fftw_complex eq_kurve_lp	[2*ncoefs+1];
	fftw_complex eq_kurve_hp	[2*ncoefs+1];

	fftw_plan hipassplan;
	fftw_plan lowpassplan;
	fftw_plan lowplan;
	fftw_plan midplan;
	fftw_plan highplan;
	
	
	//long long readidx[10];
	Steinberg::int32 numChannels=2;
	long long writeidx[10];
	long long ML=SR; //maximum number of coefs
};

//------------------------------------------------------------------------
} // namespace xD
