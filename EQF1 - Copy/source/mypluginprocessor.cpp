//------------------------------------------------------------------------
// Copyright(c) 2023 pjmw.
//------------------------------------------------------------------------

#include "mypluginprocessor.h"
#include "myplugincids.h"
#include "pluginterfaces/vst/ivstprocesscontext.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include "public.sdk/source/vst/vstaudioprocessoralgo.h"

using namespace Steinberg;

namespace xD {

void EQF1Processor::xxx(fftw_complex* IR)
{
	for (int i=0; i<2*ncoefs; i++)
	{
		IR[i][1]=sqrt(IR[i][1]*IR[i][1]+IR[i][0]*IR[i][0]);
	}
	double N=1/(2.0*double(ncoefs));
	for(int i=0;i<2*ncoefs-1;i++)
	{
		if (i-(2*ncoefs-2)/2>=0)
			IR[i][0]=IR[i-(2*ncoefs-2)/2][1]*N;
		else
			IR[i][0]=IR[i+(2*ncoefs-2)/2][1]*N;
	}
	IR[2*ncoefs-1][0]=0.0;
}

void EQF1Processor::shiftrealnorm(int type)
{
	switch(type)//0 - hi pass, 1 - low, 2 - mid, 3 - high, 4 - lowpass
	{
		case 0:
		{
			fftw_execute(hipassplan);
			xxx(memhp);
			break;
		}
		case 1:
		{
			fftw_execute(lowplan);
			xxx(meml);
			break;
		}
		case 2:
		{
			fftw_execute(midplan);
			xxx(memm);
			break;
		}
		case 3:
		{
			fftw_execute(highplan);
			xxx(memh);
			break;
		}
		case 4:
		{
			fftw_execute(lowpassplan);
			xxx(memlp);
			break;
		}
		default:
		{
			break;
		}
	}
}

void EQF1Processor::highpass(fftw_complex* eq_kurve, double w0, Steinberg::Vst::SampleRate SR) //second order butterworth
{
	double w;
	w0=w0*2.0*3.141592653589793238462;
	for (int i=0;i<=ncoefs;i++)
	{
		w=double(i*SR/ncoefs*2.0*3.141592653589793238462);
		eq_kurve[i][0]=1/sqrt(2*(w0/w)*(w0/w)+(1+(w0/w)*(w0/w))*(1+(w0/w)*(w0/w)));
	}
}

void EQF1Processor::lowpass(fftw_complex* eq_kurve, double w0, Steinberg::Vst::SampleRate SR)
{
	double w;
	w0=w0*2.0*3.141592653589793238462;
	for (int i=0;i<=ncoefs;i++)
	{
		w=double(i*SR/ncoefs*2.0*3.141592653589793238462);
		eq_kurve[i][0]=1/sqrt((w/w0)*(w/w0)*(w/w0)*(w/w0)+1);
	}
}

void EQF1Processor::band(fftw_complex* eq_kurve, double w0, double gejn, Steinberg::Vst::SampleRate SR)
{
	double w;
	w0=w0*2.0*3.141592653589793238462;
	double wl=w0/sqrt(1.5);
	double wh=wl*1.5;
	gejn=0.25*exp(2.773*gejn)-1.0;
	for (int i=0;i<=ncoefs;i++)
	{
		w=double(i*SR/ncoefs*2.0*3.141592653589793238462);
		eq_kurve[i][0]=w*(wh-wl)/sqrt(w0*w0*w0*w0+w*w*w*w+w*w*(wh*wh+wl*wl)-4*w*w*w0*w0)*gejn+1;
	}
	
}

void EQF1Processor::hishelf(fftw_complex* eq_kurve, double w0, double gejn, Steinberg::Vst::SampleRate SR)
{
	double w;
	w0=w0*2.0*3.141592653589793238462;
	gejn=0.25*exp(2.773*gejn)-1.0;
	for (int i=0;i<=ncoefs;i++)
	{
		w=double(i*SR/ncoefs*2.0*3.141592653589793238462);
		eq_kurve[i][0]=1.0+gejn/sqrt((w0/w)*(w0/w)+1);
	}
	
}

void EQF1Processor::lowshelf(fftw_complex* eq_kurve, double w0, double gejn, Steinberg::Vst::SampleRate SR)
{
	double w;
	w0=w0*2.0*3.141592653589793238462;
	gejn=0.25*exp(2.773*gejn)-1.0;
	for (int i=0;i<=ncoefs;i++)
	{
		w=double(i*SR/ncoefs*2.0*3.141592653589793238462);
		eq_kurve[i][0]=(1+w0*gejn/sqrt(w*w+w0*w0));
	}
	
}


void EQF1Processor::curva_grande(int type, fftw_complex* eq_kurve, double f0, double gejn, Steinberg::Vst::SampleRate SR)//type 0 - high pass, 1 - low shelf, 2 - band, 3- hi shelf, 4 - low pass
{
	switch(type)
	{
		case 0:
		{
			highpass(eq_kurve,f0,SR);
			break;
		}
		case 1:
		{
			lowshelf(eq_kurve,f0,gejn,SR);
			break;
		}
		case 2:
		{
			band(eq_kurve,f0,gejn,SR);
			break;
		}
		case 3:
		{
			hishelf(eq_kurve,f0,gejn,SR);
			break;
		}
		case 4:
		{
			lowpass(eq_kurve,f0,SR);
			break;
		}
		default:
		{
			break;
		}
	}
	for (int i=ncoefs+1;i<2*ncoefs;i++)
	{
		eq_kurve[i][0]=eq_kurve[ncoefs-i][0];
	}
	for (int i=0;i<2*ncoefs;i++)
	{
		eq_kurve[i][1]=0;
	}
	
}


//------------------------------------------------------------------------
// EQF1Processor
//------------------------------------------------------------------------
EQF1Processor::EQF1Processor ()
{
	//--- set the wanted controller for our processor
	setControllerClass (kEQF1ControllerUID);
}

//------------------------------------------------------------------------
EQF1Processor::~EQF1Processor ()
{}

//------------------------------------------------------------------------
tresult PLUGIN_API EQF1Processor::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated
	
	//---always initialize the parent-------
	tresult result = AudioEffect::initialize (context);
	// if everything Ok, continue
	if (result != kResultOk)
	{
		return result;
	}

	//--- create Audio IO ------
	addAudioInput (STR16 ("Stereo In"), Steinberg::Vst::SpeakerArr::kStereo);
	addAudioOutput (STR16 ("Stereo Out"), Steinberg::Vst::SpeakerArr::kStereo);

	/* If you don't need an event bus, you can remove the next line */
	addEventInput (STR16 ("Event In"), 1);

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API EQF1Processor::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!
	
	//---do not forget to call parent ------
	return AudioEffect::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API EQF1Processor::setActive (TBool state)
{
	//--- called when the Plug-in is enable/disable (On/Off) -----
	processContextRequirements=getProcessContextRequirements();
	SR=processSetup.sampleRate;
	hipassplan=fftw_plan_dft_1d(2*ncoefs,eq_kurve_hp,memhp,FFTW_BACKWARD,FFTW_ESTIMATE);
	lowpassplan=fftw_plan_dft_1d(2*ncoefs,eq_kurve_lp,memlp,FFTW_BACKWARD,FFTW_ESTIMATE);
	highplan=fftw_plan_dft_1d(2*ncoefs,eq_kurve_h,memh,FFTW_BACKWARD,FFTW_ESTIMATE);
	midplan=fftw_plan_dft_1d(2*ncoefs,eq_kurve_m,memm,FFTW_BACKWARD,FFTW_ESTIMATE);
	lowplan=fftw_plan_dft_1d(2*ncoefs,eq_kurve_l,meml,FFTW_BACKWARD,FFTW_ESTIMATE);
	if (state)
	{
		mem=new Vst::Sample32[size_t(numChannels*ML+20)]();
		for (int i=0;i<numChannels;i++)
		{
			writeidx[i]=0+i*ML;
		}
		for (int i=0; i<2*ncoefs+1; i++)
		{
			meml		[i][0]=0;
			memm		[i][0]=0;
			memh		[i][0]=0;
			memhp		[i][0]=0;
			memlp		[i][0]=0;
			meml		[i][1]=0;
			memm		[i][1]=0;
			memh		[i][1]=0;
			memhp		[i][1]=0;
			memlp		[i][1]=0;
			eq_kurve_l	[i][0]=0;
			eq_kurve_m	[i][0]=0;
			eq_kurve_h	[i][0]=0;
			eq_kurve_lp	[i][0]=0;
			eq_kurve_hp	[i][0]=0;
			eq_kurve_l	[i][1]=0;
			eq_kurve_m	[i][1]=0;
			eq_kurve_h	[i][1]=0;
			eq_kurve_lp	[i][1]=0;
			eq_kurve_hp	[i][1]=0;
		}
		curva_grande(1,eq_kurve_l,lf,lgain,SR);
		curva_grande(2,eq_kurve_m,mf,mgain,SR);
		curva_grande(3,eq_kurve_h,hf,hgain,SR);
		curva_grande(4,eq_kurve_lp,lpf,0,SR);
		curva_grande(0,eq_kurve_hp,hpf,0,SR);

	}
	if (!state)
	{
		//memset(mem,0.0,size_t(numChannels*ML*sizeof(Vst::Sample32)));
		memset(mem,0.0,size_t(numChannels*ML*sizeof(Vst::Sample32)));
		delete[] mem;
		fftw_destroy_plan(hipassplan);
		fftw_destroy_plan(lowpassplan);
		fftw_destroy_plan(midplan);
		fftw_destroy_plan(highplan);
		fftw_destroy_plan(lowplan);
	}
	return AudioEffect::setActive (state);
}

//------------------------------------------------------------------------
tresult PLUGIN_API EQF1Processor::process (Vst::ProcessData& data)
{
	//--- First : Read inputs parameter changes-----------

    if (data.inputParameterChanges)
    {
        int32 numParamsChanged = data.inputParameterChanges->getParameterCount ();
        for (int32 index = 0; index < numParamsChanged; index++)
        {
            if (auto* paramQueue = data.inputParameterChanges->getParameterData (index))
            {
                Vst::ParamValue value;
                int32 sampleOffset;
                int32 numPoints = paramQueue->getPointCount ();
                switch (paramQueue->getParameterId())
                {
					case EQParams::kParamLowGainId:
                    {
						if (paramQueue->getPoint(numPoints-1,sampleOffset,value)==kResultTrue)
                        {
						    lgain=value;
							if(tl)
								curva_grande(1,eq_kurve_l,lf,lgain,SR);
							else
								curva_grande(2,eq_kurve_l,lf,lgain,SR);
							shiftrealnorm(1);
						}
						break;
					}
					case EQParams::kParamMidGainId:
                    {
						if (paramQueue->getPoint(numPoints-1,sampleOffset,value)==kResultTrue)
                        {
						    mgain=value;
								curva_grande(2,eq_kurve_m,mf,mgain,SR);
							shiftrealnorm(2);
						}	
						break;
					}
					case EQParams::kParamHiGainId:
                    {
						if (paramQueue->getPoint(numPoints-1,sampleOffset,value)==kResultTrue)
                        {
						    lgain=value;
							if(tl)
								curva_grande(3,eq_kurve_h,hf,hgain,SR);
							else
								curva_grande(2,eq_kurve_h,hf,hgain,SR);
							shiftrealnorm(3);
						}
						break;
					}
					case EQParams::kParamLowFreqId:
                    {
						if (paramQueue->getPoint(numPoints-1,sampleOffset,value)==kResultTrue)
                        {
						    lf=475*value+25;
							if(tl)
								curva_grande(1,eq_kurve_l,lf,lgain,SR);
							else
								curva_grande(2,eq_kurve_l,lf,lgain,SR);
							shiftrealnorm(1);
						}
						break;
					}
					case EQParams::kParamMidFreqId:
                    {
						if (paramQueue->getPoint(numPoints-1,sampleOffset,value)==kResultTrue)
                        {
						    lf=4750*value+250;
								curva_grande(2,eq_kurve_m,mf,mgain,SR);
							shiftrealnorm(2);
						}
						break;
					}
					case EQParams::kParamHiFreqId:
                    {
						if (paramQueue->getPoint(numPoints-1,sampleOffset,value)==kResultTrue)
                        {
						    lf=19000*value+1000;
							if(th)
								curva_grande(3,eq_kurve_h,hf,hgain,SR);
							else
								curva_grande(2,eq_kurve_h,hf,hgain,SR);
							shiftrealnorm(3);
						}
						break;
					}
					case EQParams::kParamLowPassId:
                    {
						if (paramQueue->getPoint(numPoints-1,sampleOffset,value)==kResultTrue)
                        {
						    lpf=19000*value+1000;
								curva_grande(4,eq_kurve_lp,lpf,0,SR);
							shiftrealnorm(4);
						}
						break;
					}
					case EQParams::kParamHiPassId:
                    {
						if (paramQueue->getPoint(numPoints-1,sampleOffset,value)==kResultTrue)
                        {
						    hpf=480*value+20;
								curva_grande(0,eq_kurve_hp,hpf,0,SR);
							shiftrealnorm(0);
						}
						break;
					}
					case kParamHiToggleId:
					{
						if (paramQueue->getPoint(numPoints-1,sampleOffset,value)==kResultTrue)
                        {
						    th=value;
							if(th)
								curva_grande(3,eq_kurve_h,hf,hgain,SR);
							else
								curva_grande(2,eq_kurve_h,hf,hgain,SR);
							shiftrealnorm(3);
						}
						break; 
					}	
					case kParamLowToggleId:
					{
						if (paramQueue->getPoint(numPoints-1,sampleOffset,value)==kResultTrue)
                        {
						    tl=value;
							if(tl)
								curva_grande(1,eq_kurve_l,lf,lgain,SR);
							else
								curva_grande(2,eq_kurve_l,lf,lgain,SR);
							shiftrealnorm(1);
						}
						break; 
					}					
					case EQParams::kParamMasterId:
                    {
						if (paramQueue->getPoint(numPoints-1,sampleOffset,value)==kResultTrue)
                        {
						    master=2*value;
						}
						break;
					}
					default:
					{
						break;
					}
				}
			}
		}
	}
	
	//--- Here you have to implement your processing
	uint32 sampleFramesSize=getSampleFramesSizeInBytes(processSetup,data.numSamples);
	void** in=getChannelBuffersPointer(processSetup,data.inputs[0]);
	void** out=getChannelBuffersPointer(processSetup,data.outputs[0]);

	if (data.numInputs == 0 || data.numSamples == 0)
    {
		return kResultOk;
	}

	if (data.inputs[0].silenceFlags != 0)
	{
		// mark output silence too
		data.outputs[0].silenceFlags = data.inputs[0].silenceFlags;
		memset(mem,0.0,size_t(numChannels*ML*sizeof(Vst::Sample32)));
		// the plug-in has to be sure that if it sets the flags silence that the output buffer are clear
		for (int32 i=0;i<numChannels;i++)
		{
			// do not need to be cleared if the buffers are the same (in this case input buffer are
			// already cleared by the host)
			if (in[i] != out[i])
			{
				memset(out[i], 0, sampleFramesSize);
			}
		}
		// nothing to do at this point
		return kResultOk;
	}
	
	//assert output
	data.outputs[0].silenceFlags = 0;

	//float gain=mGain;
	//float delay=mDelay;
	// for each channel
	//int32 del=2048;
	//int32 cunt0=0;
	//int32 cunt1=0;
	for (int32 i=0; i<numChannels; i++)
	{
		int32 samples = data.numSamples;
		Vst::Sample32* ptrIn = (Vst::Sample32*)in[i];//i=0=>LEFT CHANNNNEL
		//ptrIn++;
		Vst::Sample32* ptrOut = (Vst::Sample32*)out[i];
		//long long index2;
		//ptrOut++;
		//Vst::Sample32 tmp;
		//Vst::Sample32 tdel;
		// for each sample in this channel
		//while (--samples >= 0)
		for (int j=0; j<samples; j++)
		{
			mem[writeidx[i]]=ptrIn[j];
			
			ptrOut[j]=ptrIn[j]*memhp[0][0]*200;
			/*for (int k=2*ncoefs;k>=0;k--)
			{
				int l=2*ncoefs-k;
				if(writeidx[i]-k<i*ML)
					//index2=writeidx[i]-k+ML;
					mem[writeidx[i]]=mem[writeidx[i]-k+ML]*memhp[l];

				else
					mem[writeidx[i]]=mem[writeidx[i]-k]*memhp[l];
			
			}*/
			//ptrOut[j]=
			// apply delay
			//tmp = (*ptrIn++) * gain;
			//(*ptrOut++) = tmp;
			//ptrIn[j]=ptrIn[j]*ingain;
		
			

			//if (writeidx[i]<(i+1)*ML-1)
			//{
			//	writeidx[i]++;
			//}
			//else 
			//{
			//	writeidx[i]=long long(i*ML);
			//}
			
			//idx_OLD=readidx[i];
			//readidx[i]=writeidx[i]-dt1*SR;
			//if (readidx[i]<i*ML)
			//{
			//	readidx[i]+=ML;
			//}
			
			/*if (idx_OLD-readidx[i]==0)
			{
				//for (int k=0;k<4;k++)
				mem[readidx[i]]=mem[readidx[i]-1]+mem[readidx[i]]/2.0; //interpolejszyn for flanger
				//interpolejszyn worsens the distorszyn
			}*/

			
		}
	}
	
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API EQF1Processor::setupProcessing (Vst::ProcessSetup& newSetup)
{
	//--- called before any processing ----
	return AudioEffect::setupProcessing (newSetup);
}

//------------------------------------------------------------------------
tresult PLUGIN_API EQF1Processor::canProcessSampleSize (int32 symbolicSampleSize)
{
	// by default kSample32 is supported
	if (symbolicSampleSize == Vst::kSample32)
		return kResultTrue;

	// disable the following comment if your processing support kSample64
	/* if (symbolicSampleSize == Vst::kSample64)
		return kResultTrue; */

	return kResultFalse;
}

//------------------------------------------------------------------------
tresult PLUGIN_API EQF1Processor::setState (IBStream* state)
{
	// called when we load a preset, the model has to be reloaded
	IBStreamer streamer (state, kLittleEndian);
	
	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API EQF1Processor::getState (IBStream* state)
{
	// here we need to save the model
	IBStreamer streamer (state, kLittleEndian);

	return kResultOk;
}

//------------------------------------------------------------------------
} // namespace xD
