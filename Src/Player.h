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
	int num_rings;
	// pair<pair<int, int>, pair<int, int>> stretch;
	int id;
	int trail_length;
	int board_size;
	int num_rings_removed;
	int to_win_remove;

	int DEPTH_TO_CHECK;
public:
	vector<pair<int,int>> my_ring_pos;
	vector<pair<int,int>> opp_ring_pos;
	vector<pair<pair<int, int>, pair<int, int>>> my_trails[3];
	vector<pair<pair<int, int>, pair<int, int>>> opp_trails[3];
	// vector<pair<pair<int, int>, pair<int, int>>> me_to_opp[3];
	// vector<pair<pair<int, int>, pair<int, int>>> opp_to_me[3];

	player();
	player(int numr, int idd, int tl, int win);
	void make_next_move(vector<vector<pos>>& board, vector<pair<int,int>>& local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<pair<int, int>, pair<int, int>>> non_local_trails[3], vector<pair<int,pair<pair<int,int>,pair<int,int>>>>& moves);
	// void place_rings(vector<vector<pos>>& board, vector<pair<int,int>>& local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<int,pair<pair<int,int>,pair<int,int>>>>& moves);
	void place_rings(vector<vector<pos>>& board, vector<pair<int,int>>& local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<float, vector<pair<int, pair<pair<int,int>,pair<int,int>>>>>>& move);
	
	// void place_rings(vector<vector<pos>>& board, vector<int>& moves);
	void get_valid_moves(vector<vector<pos>>& board, vector<pair<int,int>>& local_ring_pos, vector<pair<int,pair<int,int>>>& all_valid_moves, int ring_index, bool my_turn);	

	// Update the board state. action_on_ring == 0 for placing a ring, 1 for moving and 2 for removing markers, 3 for removing ring
	void update_board(vector<vector<pos>>& board, vector<pair<int,int>>& local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<pair<int, int>, pair<int, int>>> non_local_trails[3], int action_on_ring, int initial_x, int initial_y, int final_x, int final_y, bool my_turn);
	void flip_markers(vector<vector<pos>>& board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<pair<int, int>, pair<int, int>>> non_local_trails[3], int x1, int y1, int x2, int y2, bool my_turn);
	void flip_markers_revert(vector<vector<pos>>& board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], int x1, int y1, int x2, int y2, bool my_turn);
	void remove_markers(vector<vector<pos>>& board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], int start_x, int start_y, int end_x, int end_y, bool my_turn);
	void place_markers(vector<vector<pos>>& board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], int x1, int y1, int x2, int y2, bool my_turn);

	// Direction == 0 for along x-axis, 1 for y axis and 2 for parallel to x=y line
	void check_my_trail(vector<vector<pos>>& board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], int x1, int y1, int direction, bool my_turn);
	void remove_trails_and_rings(vector<vector<pos>>& board, vector<pair<int,int>>& local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<int>& moves);

	void remove_ring(vector<vector<pos>>& board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<int,int>>& local_ring_pos, vector<int>& moves);
	void remove_repeated_trails(vector<vector<pos>>& board, vector<pair<int,int>>& local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], pair<pair<int, int>, pair<int, int>>& pp, int dir);

	float heuristic(vector<vector<pos>>& board);
	pair<int,float> MinVal(vector<vector<pos>>& board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<pair<int, int>, pair<int, int>>> non_local_trails[3], int current_depth, float alpha, float beta, int rings_placed);
	pair<int,float> MaxVal(vector<vector<pos>>& board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<pair<int, int>, pair<int, int>>> non_local_trails[3], int current_depth, float alpha, float beta, int rings_placed);

	void get_all_removes(vector<vector<pos>>& local_board, vector<pair<int,pair<pair<int,int>,pair<int,int>>>>& one_remove, vector<vector<pair<int,pair<pair<int,int>,pair<int,int>>>>>& valid_removes, vector<pair<int,int>> local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<pair<int, int>, pair<int, int>>> non_local_trails[3], bool my_turn);
	void get_neighbours(bool ascending, vector<vector<pos>> local_board, vector<pair<int,int>> local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<pair<int, int>, pair<int, int>>> non_local_trails[3], vector<pair<float, vector<pair<int, pair<pair<int,int>,pair<int,int>>>>>>& move, bool my_turn);
	void revert(vector<vector<pos>>& local_board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<pair<int, int>, pair<int, int>>> non_local_trails[3], vector<pair<int,int>>& local_ring_pos, vector<pair<int, pair<pair<int,int>,pair<int,int>>>>& moves, bool my_turn);
	void concatenate(vector<pair<int,pair<pair<int,int>,pair<int,int>>>>& valid_removes, vector<pair<float, vector<pair<int, pair<pair<int,int>,pair<int,int>>>>>>& moves, int& num_moves);
	void play(vector<vector<pos>>& local_board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<pair<int, int>, pair<int, int>>> non_local_trails[3], vector<pair<int,int>>& local_ring_pos, vector<pair<float, vector<pair<int, pair<pair<int,int>,pair<int,int>>>>>>& move, int ring_index, pair<int,pair<int,int>> valid_moves, bool my_turn, int& num_moves);
	void play_move(vector<vector<pos>>& local_board, vector<pair<int,pair<pair<int,int>,pair<int,int>>>>& moves, vector<pair<int,int>>& local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<pair<int, int>, pair<int, int>>> non_local_trails[3], bool my_turn);

};

#endif