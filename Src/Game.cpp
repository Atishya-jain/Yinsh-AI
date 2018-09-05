#include "Game.h"

struct pos{
	int x;
	int y;
	int marker;
	bool ring, valid;
	
	pos(int x1, int y1, int mark, bool r, bool val){
		x = x1;
		y = y1;
		marker = mark;
		ring = r;
		valid = val;
	}
	set(int x1, int y1, int mark, bool r, bool val){
		x = x1;
		y = y1;
		marker = mark;
		ring = r;
		valid = val;
	}
}
game::game(){

}

game::game(int n){
	num_rings = n;
	remove_win_rings = 3;
	trail_length = 5;
}

void game::initialize_board(){
	int l1 = 2*num_rings + 1;
	int mid = num_rings;

	for(int i = 0; i<=mid; i++){
		vector<pos> temp;
		for(int j = 0; j<l1; j++){
			if(j < mid - i){
				pos temp_pos(min_lim, max_lim, 0, false, false);
				temp.push_back(temp_pos);
			}else{
				pos temp_pos(j-mid, mid - i, 0, false, true);
				temp.push_back(temp_pos);				
			}
		}
		board.push_back(temp);
	}

	for(int i = mid+1; i <l1; i++){
		vector<pos> temp;	
		for(int j = 0; j<l1; j++){
			if(j < l1 - i + mid){
				pos temp_pos(j-mid, mid - i, 0, false, true);
				temp.push_back(temp_pos);
			}else{
				pos temp_pos(max_lim, min_lim, 0, false, false);
				temp.push_back(temp_pos);				
			}
		}
		board.push_back(temp);	
	}
}

void game::initialize_board(){

}
void game::play(bool train){

}
void game::my_coord_to_yinsh(int x, int y){

}
void game::yinsh_coord_to_my(int ring, int pos){

}
void game::update_board(int action_on_ring, int initial_x, int initial_y, final_x, final_y){

}
void game::flip_marker(int x, int y){

}
void game::remove_markers(int start_x, int start_y, int end_x, int end_y){

}
void game::input(){

}
void game::output(){

}
