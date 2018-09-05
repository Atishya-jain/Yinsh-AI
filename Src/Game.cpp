#include "Game.h"
#include "Player.h"
#include "Utils.h"

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
				pos temp_pos(min_lim, max_lim, 2, false, false);
				temp.push_back(temp_pos);
			}else{
				pos temp_pos(j-mid, mid - i, 2, false, true);
				temp.push_back(temp_pos);				
			}
		}
		board.push_back(temp);
	}

	for(int i = mid+1; i <l1; i++){
		vector<pos> temp;	
		for(int j = 0; j<l1; j++){
			if(j < l1 - i + mid){
				pos temp_pos(j-mid, mid - i, 2, false, true);
				temp.push_back(temp_pos);
			}else{
				pos temp_pos(max_lim, min_lim, 2, false, false);
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

	my_player = player(num_rings, id, trail_length);
	if(id == 0){
		play();
	}else{
		input();
	}
}

void game::play(){
	vector<int> moves;
	my_player.make_next_move(board, moves);
	if(moves[0] == 0){
		update_board(0, moves[1], moves[2], max_lim, max_lim);
	}else if(moves[0] == 1){
		update_board(1, moves[1], moves[2], moves[3], moves[4]);
	}else{
		update_board(1, moves[1], moves[2], moves[3], moves[4]);
		update_board(2, moves[5], moves[6], moves[7], moves[8]);
		update_board(3, moves[9], moves[10], max_lim, max_lim);
	}
	output(moves);
}
pair<int, int> game::my_coord_to_yinsh(int x, int y){
	int h,p;
	if(x*y<0){
		h = abs(x)+abs(y);
	}else{
		if (x>y){
			h = x;
		}else{
			h = y;
		}
	}

	if(x>=0 && y>=0)
		p = x - y + h;
	else if(y<=0 && x<=0)
		p = y - x + 4*h;
	else if(x>0 and y<0)
		p = 2*h - y;
	else
		p = 5*h + y;

	return make_pair(h,p);

}
pair<int, int> game::yinsh_coord_to_my(int ring, int pos){
	int x,y;
	if(pos>=0 && pos<=ring)
		y = ring;
	else if(pos>=3*ring && pos<=4*ring)
		y = -1*ring;
	else if(pos>ring && pos<3*ring)
		y = 2*ring - pos;
	else
		y = pos - 5*ring;

	if(pos>=ring && pos<=2*ring)
		x = ring;
	else if(pos>=4*ring && pos<=5*ring)
		x = -1*ring;
	else if(pos>2*ring && pos<4*ring)
		x = 3*ring - pos;
	else{
		if (pos<ring)
			x = pos;
		else
			x = pos - 6*ring;
	}
	return make_pair(x,y);

}

void game::update_board(int action_on_ring, int initial_x, int initial_y, int final_x, int final_y){
	pair <int, int> coords = my_coord_to_board(initial_x, initial_y, num_rings);
	pair <int, int> coords1 = my_coord_to_board(final_x, final_y, num_rings);
	
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
		startY = min(y1, y2);
		endY = max(y1, y2);
		for(int i = startY+1; i< endY; i++){
			if(board[x1][i].marker != 2){
				board[x1][i].set(1-board[x1][i].marker,false);
			}
		}
	}else if(y1 == y2){
		startX = min(x1, x2);
		endX = max(x1, x2);
		for(int i = startX+1; i< endX; i++){
			if(board[i][y1].marker != 2){
				board[i][y1].set(1-board[i][y1].marker,false);
			}
		}		
	}else{
		startX = min(x1, x2);
		startY = min(y1, y2);
		endX = max(x1, x2);
		endY = max(y1, y2);
		for(int i = startX+1, j=startY+1; i< endX && j<endY ; i++,j++){
			if(board[i][j].marker != 2){
				board[i][j].set(1-board[i][j].marker,false);
			}
		}
	}	
}
void game::remove_markers(int x1, int y1, int x2, int y2){
int startX,startY,endX,endY;

	if(x1 == x2){
		startY = min(y1, y2);
		endY = max(y1, y2);
		for(int i = startY+1; i< endY; i++){
			board[x1][i].set(0,false);
		}
	}else if(y1 == y2){
		startX = min(x1, x2);
		endX = max(x1, x2);
		for(int i = startX+1; i< endX; i++){
			board[i][y1].set(0,false);
		}		
	}else{
		startX = min(x1, x2);
		startY = min(y1, y2);
		endX = max(x1, x2);
		endY = max(y1, y2);
		for(int i = startX+1, j=startY+1; i< endX && j<endY ; i++,j++){
			board[i][j].set(0,false);
		}
	}
}
void game::input(){
	string s;
	getline(cin, s);
	// vector <string> splited;
	string splited[] = split(s," ");
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
	getline(cin, s);
	string splited[] = split(s," ");
	id = stoi(splited[0]) - 1;
	num_rings = stoi(splited[1]);
	time_left = stoi(splited[2]);
	remove_win_rings = 5;
	trail_length = 5;
}
void game::output(vector<int> v){
	string ans;
	pair <int, int> coords = my_coord_to_yinsh(v[1], v[2]);
	if(v[0] == 0){   // PLace a ring 	 
		pair <int, int> coords = my_coord_to_yinsh(v[1], v[2]);
		ans = "P";
		ans += " ";
		ans += to_string(coords.first);
		ans += " ";
		ans += to_string(coords.second);
	}else if(v[0] == 1){   // Just move and no remove
		pair <int, int> coords = my_coord_to_yinsh(v[1], v[2]);
		pair <int, int> coords1 = my_coord_to_yinsh(v[3], v[4]);
		ans = "S";
		ans += " ";
		ans += to_string(coords.first);
		ans += " ";
		ans += to_string(coords.second);
		ans += " ";
		ans += "M";
		ans += " ";
		ans += to_string(coords1.first);
		ans += " ";
		ans += to_string(coords1.second);		
	}else{
		pair <int, int> coords = my_coord_to_yinsh(v[1], v[2]);
		pair <int, int> coords1 = my_coord_to_yinsh(v[3], v[4]);
		pair <int, int> coords2 = my_coord_to_yinsh(v[5], v[6]);
		pair <int, int> coords3 = my_coord_to_yinsh(v[7], v[8]);
		pair <int, int> coords4 = my_coord_to_yinsh(v[9], v[10]);
		ans = "S";
		ans += " ";
		ans += to_string(coords.first);
		ans += " ";
		ans += to_string(coords.second);
		ans += " ";
		ans += "M";
		ans += " ";
		ans += to_string(coords1.first);
		ans += " ";
		ans += to_string(coords1.second);				
		ans += " ";
		ans = "RS";
		ans += " ";
		ans += to_string(coords2.first);
		ans += " ";
		ans += to_string(coords2.second);
		ans += " ";
		ans += "RE";
		ans += " ";
		ans += to_string(coords3.first);
		ans += " ";
		ans += to_string(coords3.second);		
		ans += " ";
		ans += "X";
		ans += " ";
		ans += to_string(coords4.first);
		ans += " ";
		ans += to_string(coords4.second);		
	}
	cout << ans << endl;
	input();
}
