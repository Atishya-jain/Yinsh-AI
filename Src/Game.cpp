#include "Game.h"


random_device rd; // obtain a random number from hardware
mt19937 eng(rd()); // seed the generator
uniform_int_distribution<> distr(-5.9999999, 5.9999999); // define the range

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
	// mark == 0 for no marker, mark == id for me and 1-id for other player
	set(int mark, bool r){
		marker = mark;
		ring = r;
	}

	setInvalid(){
		valid = false;
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
	board[num_rings][0].setInvalid();
	board[l1-1][0].setInvalid();
	board[0][num_rings].setInvalid();
	board[l1-1][num_rings].setInvalid();
	board[0][l1-1].setInvalid();
	board[num_rings][l1-1].setInvalid();
	if(id == 0){
		play();
	}else{
		input();
	}
}

void game::play(){
	vector<int> moves;
	
	output(moves);
}
void game::my_coord_to_yinsh(int x, int y){

}
pair<int, int> game::yinsh_coord_to_my(int ring, int pos){

}

pair<int, int> game::my_coord_to_board(int x, int y){
	pair <int, int> coords;
	coords.make_pair(num_rings + x, num_rings - y);
	return coords;
}

void game::update_board(int action_on_ring, int initial_x, int initial_y, final_x, final_y){
	pair <int, int> coords = my_coord_to_board(initial_x, initial_y);
	pair <int, int> coords1 = my_coord_to_board(final_x, final_y);
	
	if(action_on_ring == 0){
		board[coords.first][coords.second].set(0, true);
	}else if(action_on_ring == 1){
		board[coords.first][coords.second].set(id, false);
		flip_markers(coords.first, coords.second, coords1.first, coords1.second);		
		board[coords1.first][coords1.second].set(0, true);
	}else if(action_on_ring == 2){
		remove_markers(coords.first, coords.second, coords1.first, coords1.second);
	}else{
		board[coords.first][coords.second].set(0, false);
	}
}

// Won't flip the marker at x2, y2 and x1,y1
void game::flip_markers(int x1, int y1, int x2, int y2){
	int startX,startY,endX,endY;

	if(x1 == x2){
		startY = min(coords.second, coords1.second);
		endY = max(coords.second, coords1.second);
		for(int i = startY+1; i< endY; i++){
			board[x1][i].set(1-board[x1][i].marker,false);
		}
	}else if(y1 == y2){
		startX = min(coords.first, coords1.first);
		endX = max(coords.first, coords1.first);
		for(int i = startX+1; i< endX; i++){
			board[i][y1].set(1-board[i][y1].marker,false);
		}		
	}else{
		startX = min(coords.first, coords1.first);
		startY = min(coords.second, coords1.second);
		endX = max(coords.first, coords1.first);
		endY = max(coords.second, coords1.second);
		for(int i = startX+1, j=startY+1; i< endX && j<endY ; i++,j++){
			board[i][j].set(1-board[i][j].marker,false);
		}
	}	
}
void game::remove_markers(int start_x, int start_y, int end_x, int end_y){
int startX,startY,endX,endY;

	if(x1 == x2){
		startY = min(coords.second, coords1.second);
		endY = max(coords.second, coords1.second);
		for(int i = startY+1; i< endY; i++){
			board[x1][i].set(0,false);
		}
	}else if(y1 == y2){
		startX = min(coords.first, coords1.first);
		endX = max(coords.first, coords1.first);
		for(int i = startX+1; i< endX; i++){
			board[i][y1].set(0,false);
		}		
	}else{
		startX = min(coords.first, coords1.first);
		startY = min(coords.second, coords1.second);
		endX = max(coords.first, coords1.first);
		endY = max(coords.second, coords1.second);
		for(int i = startX+1, j=startY+1; i< endX && j<endY ; i++,j++){
			board[i][j].set(0,false);
		}
	}
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
	id = stoi(splited[0]) - 1;
	num_rings = stoi(splited[1]);
	time_left = stoi(splited[2]);
}
void game::output(vector<int> v){
	string ans;
	if(v[0] == 0){   // PLace a ring 	 
		ans = "P";
		ans += " ";
		ans += to_string(v[1]);
		ans += " ";
		ans += to_string(v[2]);
	}else if(v[0] == 1){   // Just move and no remove
		ans = "S";
		ans += " ";
		ans += to_string(v[1]);
		ans += " ";
		ans += to_string(v[2]);
		ans += " ";
		ans += "M";
		ans += " ";
		ans += to_string(v[3]);
		ans += " ";
		ans += to_string(v[4]);		
	}else{
		ans = "S";
		ans += " ";
		ans += to_string(v[1]);
		ans += " ";
		ans += to_string(v[2]);
		ans += " ";
		ans += "M";
		ans += " ";
		ans += to_string(v[3]);
		ans += " ";
		ans += to_string(v[4]);				
		ans += " ";
		ans = "RS";
		ans += " ";
		ans += to_string(v[5]);
		ans += " ";
		ans += to_string(v[6]);
		ans += " ";
		ans += "RE";
		ans += " ";
		ans += to_string(v[7]);
		ans += " ";
		ans += to_string(v[8]);		
		ans += " ";
		ans += "X";
		ans += " ";
		ans += to_string(v[9]);
		ans += " ";
		ans += to_string(v[10]);		
	}
	cout << ans << endl;
	input();
}
