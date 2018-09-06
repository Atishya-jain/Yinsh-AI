#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <random>
#include <algorithm>
#include "struct.h"
using namespace std;

class player
{
private:
	// All your algos
	void minimax();
	void place_markers();
	int num_rings_placed;
	vector<pair<int,int>> ring_pos;
	int num_rings;
	pair<pair<int, int>, pair<int, int>> stretch;
	int id;
	int trail_length;
	int board_size;
public:
	player();
	player(int numr, int idd, int tl);
	void make_next_move(vector<vector<pos>>& board, vector<int>& moves);
	void place_rings(vector<vector<pos>>& board, vector<int>& moves);
	void get_valid_moves(vector<vector<pos>>& board, vector<pair<int,pair<int,int>>>& all_valid_moves, int ring_index);	
};

#endif