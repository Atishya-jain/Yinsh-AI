#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <random>
#include <cmath>
#include "Player.h"

using namespace std;

struct pos;

class game
{
private:
	vector <vector <pos>> board; // Board state
	int num_rings; //Total number of rings
	int remove_win_rings; // Number of rings to remove for winning
	int trail_length; // Trail_length of markers to remove a ring
	int count_black; // total black markers on board
	int count_white; // Total white markers on board
	int id; // Identify if first move or not
	double time_left; // Time left for our moves
	bool train; // Set to true if training is going on
	player my_player;
	string int_to_move[6];
	// 0 == P, 1 == S, 2 == M, 3 == RS, 4 == RE, 5 == X
public:
	// Constructors
	game();
	game(int n);
	
	// Initialize the board with no markers and rings
	void initialize_board();
	
	// Game logic
	void play();
	
	// Coordinate converters
	void my_coord_to_yinsh(pair<int, int>& ret_coord, int x, int y);
	void yinsh_coord_to_my(pair<int, int>& ret_coord, int ring, int pos);
		
	// Initial Input of variables
	void initial_input();
	
	// Input from stdin
	void input();

	// Output my move to stdout
	void output(vector<int>& v);
};

#endif