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
	if(id == 1){
		play();
	}else{
		input();
	}
}

void game::play(){
	output();
}
void game::my_coord_to_yinsh(int x, int y){

}
pair<int, int> game::yinsh_coord_to_my(int ring, int pos){
	
}
void game::update_board(int action_on_ring, int initial_x, int initial_y, final_x, final_y){

}
void game::flip_marker(int x, int y){

}
void game::remove_markers(int start_x, int start_y, int end_x, int end_y){

}
void game::input(){
	string s;
	cin >> s;
	String[] splited = str.split(" ");
	int num_moves_in_input = (sizeof(splited)/sizeof(splited[0]))/3;
	string move_type;
	int x1, y1, x2, y2, x3, y3;
	move_type = splited[0];
	x1 = stoi(splited[1]);
	y1 = stoi(splited[2]);
	pair <int, int> my_coord = yinsh_coord_to_my(x1, y1);
	if(move_type.compare("P") == 0){
		// Just place the ring in position
		update_board(0, my_coord.first, my_coord.second, max_lim, max_lim);
	}else if((move_type.compare("S")) && (num_moves_in_input == 2)){
		// move the ring
		x2 = stoi(splited[4]);
		y2 = stoi(splited[5]);
		pair <int, int> my_coord2 = yinsh_coord_to_my(x2, y2);
		update_board(1, my_coord.first, my_coord.second, my_coord2.first, my_coord2.second);
	}else{
		// Move the ring
		x2 = stoi(splited[4]);
		y2 = stoi(splited[5]);
		pair <int, int> my_coord2 = yinsh_coord_to_my(x2, y2);
		update_board(1, my_coord.first, my_coord.second, my_coord2.first, my_coord2.second);

		// remove markers
		x1 = stoi(splited[7]);
		y1 = stoi(splited[8]);
		my_coord = yinsh_coord_to_my(x1, y1);
		x2 = stoi(splited[10]);
		y2 = stoi(splited[11]);
		my_coord2 = yinsh_coord_to_my(x2, y2);
		update_board(2, my_coord.first, my_coord.second, my_coord2.first, my_coord2.second);
		
		// remove ring
		x1 = stoi(splited[13]);
		y1 = stoi(splited[14]);
		my_coord = yinsh_coord_to_my(x1, y1);
		update_board(3, my_coord.first, my_coord.second, max_lim, max_lim);
	}
	play();
}
void game::initial_input(){
	string s;
	cin >> s;
	String[] splited = str.split(" ");
	id = stoi(splited[0]);
	num_rings = stoi(splited[1]);
	time_left = stoi(splited[2]);
}
void game::output(){

}
