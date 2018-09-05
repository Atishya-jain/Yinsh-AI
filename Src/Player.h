#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
using namespace std;

class player
{
private:
	// All your algos
	void minimax();
public:
	vector<int> make_next_move(vector<vector<pair<int,int>>> board);

}

#endif