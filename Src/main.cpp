#include <iostream>
#include "Game.h"
using namespace std;

int main(int argc, char *argv[]){
	game core;
	// cout << "Enter initial_inputs\n";
	core.initial_input();	
	core.initialize_board();
	
	return 0;
}