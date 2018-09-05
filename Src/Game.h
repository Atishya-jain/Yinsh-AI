#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include <pair>
using namespace std;

class game
{
private:
	vector <vector <pair<int,int>>> board;
	int count_black;
	int count_white;
public:
	void InitializeBoard();
	void play(bool train);
	void my_coord_to_Yinsh(int x, int y);
	void Yinsh_coord_to_my(int ring, int pos);
	void update_board(int action_on_ring, int initial_x, int initial_y, final_x, final_y);
	void flip_marker(int x, int y);
	void remove_markers(int start_x, int start_y, int end_x, int end_y);
}

#endif