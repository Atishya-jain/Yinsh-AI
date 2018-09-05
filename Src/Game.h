#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <string>
#include <vector>
#include <pair>
#include <limits>
using namespace std;

int max_lim = numeric_limits<int>::max();
int min_lim = numeric_limits<int>::min();
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
public:
	game();
	game(int n);
	void initialize_board();
	void play();
	void my_coord_to_yinsh(int x, int y);
	pair<int, int> yinsh_coord_to_my(int ring, int pos);
	// action_on_ring == 0 for placing a ring, 1 for moving and 2 for removing markers, 3 for removing ring
	void update_board(int action_on_ring, int initial_x, int initial_y, final_x, final_y);
	void flip_marker(int x, int y);
	void remove_markers(int start_x, int start_y, int end_x, int end_y);
	void initial_input();
	void input();
	void output();
}

#endif