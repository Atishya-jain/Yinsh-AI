#include <iostream>
#include "Game.h"
#include <ctime>
using namespace std;

int main(int argc, char *argv[]){
    clock_t begin = clock();
	// game core(begin);

	// cerr << argv[1] << " "<< argv[2] << endl;
	game core(begin, argv[1], argv[2]);
	//cerr << "Enter initial_inputs\n";
	core.initial_input();	
	core.initialize_board();
	
	return 0;
}