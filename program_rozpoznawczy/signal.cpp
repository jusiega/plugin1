#include <mpg123.h>
#include <ao/ao.h>
#include <cstring>
#include <fstream>
#include "signal.h"


/////////////////////////////////////////// WCZYTANIE SYGNAŁU Z .mp3 LUB .WAV
Signal::Signal(const char* path){
	short len = strlen(path);
	std::string fileType = "";
	int i=0;
	title = "";	

	std::cout << "Path " << path << '.' << std::endl;
	
	
	for(i=3; i>0; i--)
		fileType += path[len-i];

	i = len;
	while(path[--i] != '/');

	for(i=i+1; path[i] != '.' ;i++)	
		title += path[i];


	if( fileType == "mp3" ){	

    	mpg123_init();  // otwarcie biblioteki
	
	    // inicjalizacje zmiennych mpg
    	mpg123_handle *handler = mpg123_new(NULL, NULL);
    	size_t done;
    	bufsize = mpg123_outblock(handler);
    	char* buffer = (char*) malloc(bufsize);

    	mpg123_open(handler, path); // otwarcie pliku przypisanie wartości do handlera
    	mpg123_getformat(handler, &rate, &channels, &encoding); // do odtwarzania

   		bits = mpg123_encsize(encoding) * 8; // musi być 8 dla 16 bitów nie działa
    	size_t totSize = 0; // w bajtach

    	while(mpg123_read(handler, buffer, bufsize, &done) == MPG123_OK){
    		totSize += done; // długość sygnału w bajtach.
		}

		mpg123_seek(handler, 0, SEEK_SET); // ustawia handler na 0 elemencie od początku pliku .mp3
		numElements = totSize/sizeof(short);
    	signal = new short[numElements];       // totSize to wielokrotność bufsize (tak wychodzi z eriki)
    
		int elemPerSmp = bufsize/sizeof(short);
    	for(int i=0;mpg123_read(handler, buffer, bufsize, &done) == MPG123_OK; i++){
        	short* temp = reinterpret_cast<short*> (buffer);        // nie wiem jak to bez tej linijki zrobić, możnaby to poprawić
        	for(int j=0; j<elemPerSmp; j++)
    			signal[i*elemPerSmp + j] = temp[j];
    	}
    	std::cout << "Wczytałem próbki dźwięku w formacie PCM." << std::endl;
		fourier = (fftw_complex*)fftw_malloc(numElements*sizeof(fftw_complex));

    	free(buffer);
    	mpg123_close(handler);
    	mpg123_delete(handler);
    	mpg123_exit();
	}
	else if( fileType == "wav"){
		std::cout << "Powinienem wczytać wava." << std::endl;
	}
	else
		std::cout << "Nie rozpoznaję typu pliku." << std::endl;
	
}

void Signal::saveSignal(int elems[]){
	std::string tempTitle = this->title + "Sygnał.txt";
	std::ofstream doPliku(tempTitle, std::ios::binary);
	if(!doPliku.is_open())
		throw("Coś poszło nie tak przy otwieraniu pliku.");

	for(int i=elems[0]; i<elems[1]; i++)
		doPliku << signal[i] << ' ';

	std::cout << "Elementy sygnału zapisano w pliku " << tempTitle << '.' << std::endl;
	doPliku.close();
}


void Signal::saveFourier(int elems[]){
	std::string tempTitle = this->title + "Fourier.txt";
	std::ofstream doPliku(tempTitle);
	if(!doPliku.is_open())
		throw("Coś poszło nie tak przy otwieraniu pliku Fourier.");

	for(int i=elems[0]; i<elems[1]; i++)
		doPliku << fourier[i] << ' ';
	
	std::cout << "Elementy po transformacie fouriera zapisano w pliku " << tempTitle << '.' << std::endl;
	doPliku.close();
}


////////////////////////////// ODTWARZANIE SYGNAŁU
void Signal::playSound(){
        ao_initialize();
        int driver = ao_default_driver_id();
        ao_sample_format format;

        format.bits = bits;
        format.rate = rate;
        format.channels = channels;
        format.byte_format = AO_FMT_NATIVE;
        format.matrix = 0;

        ao_device* device = ao_open_live(driver, &format, NULL);
        std::cout << "Leci piosenka." << std::endl;

        for(int j=0; j<int(numElements*sizeof(short)/bufsize); j++){								// jeszcze dodać stop, start
                char* temp = (char*) malloc(bufsize);
                temp = reinterpret_cast<char*> (&signal[j*bufsize/sizeof(short)]); 	// tylko dla char* działa ao_play
                ao_play(device, temp, bufsize);
		}

        ao_close(device);
        ao_shutdown();
}



Signal::~Signal(){
	delete [] signal;
	delete [] fourier;
}

