#include "signal.h"
#include <string>

//std::string path = "/home/stud/Music/ERIKA.mp3"; // można dać jako argument do czegoś w rodzaju loop'a
//std::string path = "/home/stud/Music/02 - Ice Cube - Check Yo Self (feat. Das Efx)(Remix).mp3"; 
//const char* path = "/home/stud/Music/Cypress Hill - Lowrider (Official Video).mp3";
//std::string path = "/home/stud/Music/Fear of the Dark.wav";


int main(){
	std::cout << "Żeby przerwać program naciśnij # przy pytaniu o ścieżkę." << std::endl;

	std::string input;	
	int elems[2] = {0,0};

	while( true ){
		
		if(!std::getline(std::cin, input))
			throw("Nie spodziewany error przy wczytywaniu ścieżki");
		else if(input == "#")
			break;

		Signal utwor(input.c_str()); // konwersja std::string na char*
	
		int sajz = utwor.size();

		std::cout << "Wczytana próbka ma " << sajz << " elementów.\n";		

		std::cout << "Od którego elementu zapisać sygnał?" << std::endl;

		std::cin >> elems[0];		

		std::cout << "Do którego elementu zapisać sygnał?" << std::endl;

		std::cin >> elems[1];	

		if( elems[0] < elems[1] && elems[0] > 0 && elems[1] > 0 && elems[1] < sajz && elems[0] < sajz )
			utwor.saveSignal(elems);
		else{
			std::cout << "Coś poszło nie tak z zapisywaniem sygnału. Sprawdź czy dobrze wpisałeś liczby." << std::endl;
			continue;
		}
		utwor.DFT();

		std::cout << "Od którego elementu zapisać widmo fourierowski?" << std::endl;

		std::cin >> elems[0];		

		std::cout << "Do którego elementu zapisać widmo?" << std::endl;

		std::cin >> elems[1];	

		if( elems[0] < elems[1] && elems[0] > 0 && elems[1] > 0 && elems[1] < sajz && elems[0] < sajz )
			utwor.saveFourier(elems);
		else{
			std::cout << "Coś poszło nie tak z zapisywaniem sygnału. Sprawdź czy dobrze wpisałeś liczby." << std::endl;
			continue;
		}
		
		utwor.playSound();
	
	}
	return 0;
}

