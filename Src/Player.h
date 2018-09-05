#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include "Game.h"
using namespace std;

class player
{
private:
	// All your algos
	void minimax();
	void place_markers();
public:
	vector<int> make_next_move(vector<vector<pos>> board);

}

#endif