#include "signal.h"
#include <iostream>

const float pi = 3.14159265358979323846;

/*

NAUKA JAZDY Z ARGUMENTAMI:

FFTW_ESTIMATE - najszybsza opcja, ale bardzo mało dokładna (w ogóle nie dokładna lol)
FFTW_MEASURE - wolniejsza (dużo), ale bardziej dokładna (dla całej próbki jest stanowczo za wolna)
FFTW_PATEINT - tak wolna że nie przydatna praktycznie

*/


void Signal::DFT(){
	double* temp = new double[numElements];
	
	for(int i=0; i<numElements; i++)
		temp[i] = (double)signal[i];

	// DFT
	fftw_plan fftPlan = fftw_plan_dft_r2c_1d(numElements, temp, fourier, FFTW_ESTIMATE);

	fftw_execute(fftPlan);

	fftw_destroy_plan(fftPlan);

	delete [] temp;
}

void Signal::iDFT(){
	double* temp = new double[numElements];

	// IDFT
	fftw_plan ifftPlan = fftw_plan_dft_c2r_1d(numElements, fourier, temp, FFTW_ESTIMATE);

	fftw_execute(ifftPlan);

	fftw_destroy_plan(ifftPlan);

	for(int i=0; i<numElements; i++)
		signal[i] = short(std::round(temp[i]/numElements));

    delete [] temp;
}
