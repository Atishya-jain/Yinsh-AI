#include <iostream>
#include "Game.h"
#include <ctime>
using namespace std;

int main(int argc, char *argv[]){
    clock_t begin = clock();
	// game core(begin);

	// cerr << argv[1] << " "<< argv[2] << endl;
	string tmp = argv[2];
	tmp.erase(tmp.find_last_not_of(" \n\r\t")+1);

	game core(begin, argv[1], tmp);
	//cerr << "Enter initial_inputs\n";
	core.initial_input();	
	core.initialize_board();
	
	return 0;
}