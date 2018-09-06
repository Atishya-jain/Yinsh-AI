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
public:
	// Constructors
	game();
	game(int n);
	
	// Initialize the board with no markers and rings
	void initialize_board();
	
	// Game logic
	void play();
	
	// Coordinate converters
	pair<int, int> my_coord_to_yinsh(int x, int y);
	pair<int, int> yinsh_coord_to_my(int ring, int pos);
	
	// Update the board state. action_on_ring == 0 for placing a ring, 1 for moving and 2 for removing markers, 3 for removing ring
	void update_board(int action_on_ring, int initial_x, int initial_y, int final_x, int final_y);
	void flip_markers(int x1, int y1, int x2, int y2);
	void remove_markers(int start_x, int start_y, int end_x, int end_y);
	
	// Initial Input of variables
	void initial_input();
	
	// Input from stdin
	void input();

	// Output my move to stdout
	void output(vector<int>& v);
};

#endif