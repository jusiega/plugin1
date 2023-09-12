//------------------------------------------------------------------------
// Copyright(c) 2023 pjmw.
//------------------------------------------------------------------------

#include "myplugincontroller.h"
#include "myplugincids.h"
#include "pluginterfaces/base/ustring.h"



using namespace Steinberg;

namespace xD {

//------------------------------------------------------------------------
// EQF1Controller Implementation
//------------------------------------------------------------------------
tresult PLUGIN_API EQF1Controller::initialize (FUnknown* context)
{
	// Here the Plug-in will be instantiated

	//---do not forget to call parent ------
	tresult result = EditControllerEx1::initialize (context);
	if (result != kResultOk)
	{
		return result;
	}

	// Here you could register some parameters
	parameters.addParameter (STR16 ("Hi Pass"), STR16 ("Hz"), 0, 0, Vst::ParameterInfo::kCanAutomate, EQParams::kParamHiPassId);
	parameters.addParameter (STR16 ("Low Pass"), STR16 ("Hz"), 0, 1, Vst::ParameterInfo::kCanAutomate, EQParams::kParamLowPassId);
	parameters.addParameter (STR16 ("Low Type"), STR16 ("Peaking/Shelving"), 1, 1, Vst::ParameterInfo::kCanAutomate, EQParams::kParamLowToggleId);
	parameters.addParameter (STR16 ("Hi Type"), STR16 ("Peaking/Shelving"), 1, 1, Vst::ParameterInfo::kCanAutomate, EQParams::kParamHiToggleId);
	parameters.addParameter (STR16 ("Low Freq."), STR16 ("Hz"), 0, 0.5, Vst::ParameterInfo::kCanAutomate, EQParams::kParamLowFreqId);
	parameters.addParameter (STR16 ("Mid Freq."), STR16 ("Hz"), 0, 0.5, Vst::ParameterInfo::kCanAutomate, EQParams::kParamMidFreqId);
	parameters.addParameter (STR16 ("Hi Freq."), STR16 ("Hz"), 0, 0.5, Vst::ParameterInfo::kCanAutomate, EQParams::kParamHiFreqId);
	parameters.addParameter (STR16 ("Hi Gain"), STR16 ("dB"), 0, 0.5, Vst::ParameterInfo::kCanAutomate, EQParams::kParamHiGainId);
	parameters.addParameter (STR16 ("Mid Gain"), STR16 ("dB"), 0, 0.5, Vst::ParameterInfo::kCanAutomate, EQParams::kParamMidGainId);
	parameters.addParameter (STR16 ("Low Gain"), STR16 ("dB"), 0, 0.5, Vst::ParameterInfo::kCanAutomate, EQParams::kParamLowGainId);

	parameters.addParameter (STR16 ("Master"), STR16 ("dB"), 0, 0.5, Vst::ParameterInfo::kCanAutomate, EQParams::kParamMasterId);

	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API EQF1Controller::terminate ()
{
	// Here the Plug-in will be de-instantiated, last possibility to remove some memory!

	//---do not forget to call parent ------
	return EditControllerEx1::terminate ();
}

//------------------------------------------------------------------------
tresult PLUGIN_API EQF1Controller::setComponentState (IBStream* state)
{
	// Here you get the state of the component (Processor part)
	if (!state)
		return kResultFalse;

	return kResultOk;
}

//------------------------------------------------------------------------
tresult PLUGIN_API EQF1Controller::setState (IBStream* state)
{
	// Here you get the state of the controller

	return kResultTrue;
}

//------------------------------------------------------------------------
tresult PLUGIN_API EQF1Controller::getState (IBStream* state)
{
	// Here you are asked to deliver the state of the controller (if needed)
	// Note: the real state of your plug-in is saved in the processor

	return kResultTrue;
}

//------------------------------------------------------------------------
IPlugView* PLUGIN_API EQF1Controller::createView (FIDString name)
{
	// Here the Host wants to open your editor (if you have one)
	if (FIDStringsEqual (name, Vst::ViewType::kEditor))
	{
		// create your editor here and return a IPlugView ptr of it
        return nullptr;
	}
	return nullptr;
}

//------------------------------------------------------------------------
tresult PLUGIN_API EQF1Controller::setParamNormalized (Vst::ParamID tag, Vst::ParamValue value)
{
	// called by host to update your parameters
	tresult result = EditControllerEx1::setParamNormalized (tag, value);
	return result;
}

//------------------------------------------------------------------------
tresult PLUGIN_API EQF1Controller::getParamStringByValue (Vst::ParamID tag, Vst::ParamValue valueNormalized, Vst::String128 string)
{
	// called by host to get a string for given normalized value of a specific parameter
	// (without having to set the value!)
	UString128 result;
	switch (tag)
	{
		case kParamLowGainId:
		{
			double gejn=20.0*log10(3.75*valueNormalized+0.25);
			result.printFloat(gejn);
			break;
			/*int ldel = (int32)(32766 * valueNormalized * valueNormalized);
			if (ldel<4) ldel=4;
			result.printInt (ldel * 1000. / sampleRate);
			break;*/
		}
		case kParamMidGainId:
		{
			double gejn=20.0*log10(3.75*valueNormalized+0.25);
			result.printFloat(gejn);
			break;
			/*int ldel = (int32)(32766 * valueNormalized * valueNormalized);
			if (ldel<4) ldel=4;
			result.printInt (ldel * 1000. / sampleRate);
			break;*/
		}
		case kParamHiGainId:
		{
			double gejn=20.0*log10(3.75*valueNormalized+0.25);
			result.printFloat(gejn);
			break;
			/*int ldel = (int32)(32766 * valueNormalized * valueNormalized);
			if (ldel<4) ldel=4;
			result.printInt (ldel * 1000. / sampleRate);
			break;*/
		}
		case kParamHiFreqId:
		{
			double f3db=19000*valueNormalized+1000;
			result.printFloat(f3db);
			break;
		}
		case kParamMidFreqId:
		{
			double f3db=4750*valueNormalized+250;
			result.printFloat(f3db);
			break;
		}
		case kParamLowFreqId:
		{
			double f3db=475*valueNormalized+25;
			result.printFloat(f3db);
			break;
		}
		case kParamLowToggleId:
		{
			const char* p="Peaking";
			const char* s="Shelving";
			if (valueNormalized==1)
			{
				result.fromAscii(s);
			}
			else
			{
				result.fromAscii(p);
			}
			break;
		}
		case kParamHiToggleId:
		{
			const char* p="Peaking";
			const char* s="Shelving";
			if (valueNormalized==1)
			{
				result.fromAscii(s);
			}
			else
			{
				result.fromAscii(p);
			}
			break;
		}
		case kParamHiPassId:
		{
			double f3db=480*valueNormalized+20;
			result.printFloat(f3db);
			break;
		}
		case kParamLowPassId:
		{
			double f3db=19000*valueNormalized+1000;
			result.printFloat(f3db);
			break;
		}
		case kParamMasterId:
		{
			double gejn=20.0*log10(2.0*valueNormalized);
			result.printFloat(gejn);
			break;
		}
		default:
			return EditControllerEx1::getParamStringByValue (tag, valueNormalized, string);
	}
	result.copyTo (string, 128);
	return kResultTrue;
	//return EditControllerEx1::getParamStringByValue (tag, valueNormalized, string);
}

//------------------------------------------------------------------------
tresult PLUGIN_API EQF1Controller::getParamValueByString (Vst::ParamID tag, Vst::TChar* string, Vst::ParamValue& valueNormalized)
{
	// called by host to get a normalized value from a string representation of a specific parameter
	// (without having to set the value!)
	return EditControllerEx1::getParamValueByString (tag, string, valueNormalized);
}

//------------------------------------------------------------------------
} // namespace xD
