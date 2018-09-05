#ifndef GAME_H
#define GAME_H

#include <iostream>
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
	vector <vector <pos>> board;
	int num_rings;
	int remove_win_rings;
	int trail_length;
	int count_black;
	int count_white;
public:
	game();
	game(int n);
	void initialize_board();
	void play(bool train);
	void my_coord_to_yinsh(int x, int y);
	void yinsh_coord_to_my(int ring, int pos);
	void update_board(int action_on_ring, int initial_x, int initial_y, final_x, final_y);
	void flip_marker(int x, int y);
	void remove_markers(int start_x, int start_y, int end_x, int end_y);
	void initial_input();
	void input();
	void output();
}

#endif