#include <iostream>
#include "Game.h"
#include <ctime>
using namespace std;

int main(int argc, char *argv[]){
    clock_t begin = clock();
	game core(begin);
	//cerr << "Enter initial_inputs\n";
	core.initial_input();	
	core.initialize_board();
	
	return 0;
}