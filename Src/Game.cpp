#include "Utils.h"
#include "Game.h"
#include "Player.h"
int max_lim = numeric_limits<int>::max();
int min_lim = numeric_limits<int>::min();

game::game(){

}

game::game(clock_t tm){
	start_time = tm;
	diff_time = clock_t() - tm;
}

game::game(int n){
	num_rings = n;
	remove_win_rings = 3;
	trail_length = 5;
}

void game::initialize_board(){
	// cout << "Let's Initialise board\n";
	int l1 = 2*num_rings + 1;
	int mid = num_rings;

	for(int i = 0; i<=mid; i++){
		vector<pos> temp;
		for(int j = 0; j<l1; j++){
			if(j > mid + i){
				// pos temp_pos(min_lim, max_lim, 2, false, false);
				pos temp_pos(2, 2, false);
				temp.push_back(temp_pos);
			}else{
				// pos temp_pos(j-mid, mid - i, 2, false, true);
				pos temp_pos(2, 2, true);
				temp.push_back(temp_pos);				
			}
		}
		board.push_back(temp);
	}

	for(int i = mid+1; i <l1; i++){
		vector<pos> temp;	
		for(int j = 0; j<l1; j++){
			if(j < i - mid){
				// pos temp_pos(j-mid, mid - i, 2, false, true);
				pos temp_pos(2, 2, false);
				temp.push_back(temp_pos);
			}else{
				pos temp_pos(2, 2, true);
				// pos temp_pos(max_lim, min_lim, 2, false, false);
				temp.push_back(temp_pos);				
			}
		}
		board.push_back(temp);	
	}
	board[num_rings][0].setInvalid();
	board[l1-1][l1-1].setInvalid();
	board[0][num_rings].setInvalid();
	board[l1-1][num_rings].setInvalid();
	board[0][0].setInvalid();
	board[num_rings][l1-1].setInvalid();

	my_player = player(num_rings, id, trail_length, remove_win_rings, start_time, time_left);
	int_to_move[0] = "P";
	int_to_move[1] = "S";
	int_to_move[2] = "M";
	int_to_move[3] = "RS";
	int_to_move[4] = "RE";
	int_to_move[5] = "X";
	// cout << "Initialise board Done\n";	
	if(id == 0){
		// cout << "Start player 1\n";	
		play();
	}else{
		// cout << "Waiting for input\n";	
		input();
	}
}

void game::play(){
	vector<pair<int,pair<pair<int,int>,pair<int,int>>>> moves;
	my_player.make_next_move(board, my_player.my_ring_pos, my_player.opp_ring_pos, my_player.my_trails, my_player.opp_trails, moves, diff_time);
	output(moves);
}
void game::my_coord_to_yinsh(pair<int, int>& ret_coord, int c, int v){
	int h,p,x,y;
	x = c-num_rings;
	y = v-num_rings;
	cerr << "x: " << x << endl;
	cerr << "y: " << y << endl;

	if(x*y<0){
		h = abs(x)+abs(y);
	}else{
		h = max(abs(x), abs(y));
	}

	if(x>=0 && y>=0)
		p = x - y + h;
	else if(y<=0 && x<=0)
		p = y - x + 4*h;
	else if(x>0 and y<0)
		p = 2*h - y;
	else
		p = 5*h + y;

	ret_coord.first = h;
	ret_coord.second = p;
}
void game::yinsh_coord_to_my(pair<int, int>& ret_coord, int ring, int pos){
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
	ret_coord.first = x+num_rings;
	ret_coord.second = y+num_rings;
}

void game::input(){
	//////////////////////////////////May need to clear the trail vector//////////////////
	// for(int i = 0; i< 3; i++){
	// 	my_player.my_trails[i].clear();
	// 	pair <pair<int,int>, pair<int,int>> temp;
	// 	my_player.my_trails[i].push_back(temp);
	// }
	////////////////////////////////////////////////////////////////////////////////////
	clock_t start_in = clock();

	string s;
	getline(cin, s);

	clock_t end = clock();
	diff_time += end-start_in;

	vector <string> splited;
	splitString(s," ",splited);
	int num_moves_in_input = splited.size()/3;
	// cerr << "num_moves_in_input: " << splited.size() << endl;
	cerr << "num_moves_in_input: " << num_moves_in_input << endl;
	cerr << "Input Move:**************** " << s << endl;
	string move_type;
	int x1, y1, x2, y2, x3, y3;
	int i = 0;
	pair <int, int> my_coord, my_coord2;
	while(i < num_moves_in_input){
		move_type = splited[3*i];
		if(move_type.compare("P") == 0){
			x1 = stoi(splited[3*i+1]);
			y1 = stoi(splited[3*i+2]);	
			yinsh_coord_to_my(my_coord, x1, y1);
			my_player.update_board(board, my_player.opp_ring_pos, my_player.opp_trails, my_player.my_trails, 0, my_coord.first, my_coord.second, max_lim, max_lim, false);
		}else if(move_type.compare("S") == 0){
			cerr << "Start1\n";
			x1 = stoi(splited[3*i+1]);
			y1 = stoi(splited[3*i+2]);	
			yinsh_coord_to_my(my_coord, x1, y1);
			cerr << "Start2\n";
			// move the ring
			// cout << "Let's move it\n";
			x2 = stoi(splited[3*i+4]);
			y2 = stoi(splited[3*i+5]);
			cerr  << "Conversion again starts\n";
			yinsh_coord_to_my(my_coord2, x2, y2);
			// cout << "Mine move coords: " << my_coord2.first << " " << my_coord2.second << endl;
			cerr << "Move: " << endl;
			my_player.update_board(board, my_player.opp_ring_pos, my_player.opp_trails, my_player.my_trails, 1, my_coord.first, my_coord.second, my_coord2.first, my_coord2.second, false);
			cerr << "Move Done\n";
			i++;
		}else{
			// remove markers
			x1 = stoi(splited[3*i+1]);
			y1 = stoi(splited[3*i+2]);
			yinsh_coord_to_my(my_coord, x1, y1);
			x2 = stoi(splited[3*i+4]);
			y2 = stoi(splited[3*i+5]);
			yinsh_coord_to_my(my_coord2, x2, y2);
			cerr << "Removal of markers start:\n";
			my_player.update_board(board, my_player.opp_ring_pos, my_player.opp_trails, my_player.my_trails, 2, my_coord.first, my_coord.second, my_coord2.first, my_coord2.second, false);
			pair<pair<int, int>, pair<int, int>> pp = make_pair(make_pair(my_coord.first,my_coord.second), make_pair(my_coord2.first,my_coord2.second));
			cerr << "Repeated removal starts: \n";
			if(my_coord.second == my_coord2.second){
				my_player.remove_repeated_trails(board, my_player.opp_ring_pos, my_player.opp_trails, pp, 0);
			}else if(my_coord.first == my_coord2.first){
				my_player.remove_repeated_trails(board, my_player.opp_ring_pos, my_player.opp_trails, pp, 1);
			}else{
				my_player.remove_repeated_trails(board, my_player.opp_ring_pos, my_player.opp_trails, pp, 2);
			}
			cerr << "Repeated also finishes!!!\n";
			// remove ring
			x1 = stoi(splited[3*i+7]);
			y1 = stoi(splited[3*i+8]);
			yinsh_coord_to_my(my_coord, x1, y1);

			my_player.update_board(board, my_player.opp_ring_pos, my_player.opp_trails, my_player.my_trails, 3, my_coord.first, my_coord.second, max_lim, max_lim, false);
			cerr << "Done this move\n";
			i = i+2;
		}	
		i++;
	}
	play();
}
void game::initial_input(){
	string s;
	getline(cin, s);
	vector<string> splited;
	
	// cout << "Split started\n";
	splitString(s," ", splited);
	// cout << "Split done\n";
	
	// cout << "Size of splited array: " << splited.size() << endl;
	id = stoi(splited[0]) - 1;
	// cout << id << endl;
	
	num_rings = stoi(splited[1]);
	// cout << num_rings << endl;
	
	time_left = stoi(splited[2]);
	// cout << time_left << endl;
	
	remove_win_rings = 3;
	trail_length = 5;
	// cout << "Initial Input done\n" << endl;
}
void game::output(vector<pair<int,pair<pair<int,int>,pair<int,int>>>>& v){
	string ans;
	int len = v.size();
	cerr << "Length of final output: " << len << endl;
	pair <int, int> coords;

	// my_coord_to_yinsh(coords, v[1], v[2]);
	ans = "";
	// ans = int_to_move[v[0]];
	// ans += " ";
	// ans += to_string(coords.first);
	// ans += " ";
	// ans += to_string(coords.second);
	// ans += " ";

	for(int i = 0; i < len; i++){
		if(v[i].first == 0){
			ans += "P";
			my_coord_to_yinsh(coords, ((v[i].second).first).first, ((v[i].second).first).second);
			ans += " ";
			ans += to_string(coords.first);
			ans += " ";
			ans += to_string(coords.second);
			ans += " ";
		}else if(v[i].first == 1){
			ans += "S";
			my_coord_to_yinsh(coords, ((v[i].second).first).first, ((v[i].second).first).second);
			ans += " ";
			ans += to_string(coords.first);
			ans += " ";
			ans += to_string(coords.second);
			ans += " ";
			ans += "M";
			my_coord_to_yinsh(coords, ((v[i].second).second).first, ((v[i].second).second).second);
			ans += " ";
			ans += to_string(coords.first);
			ans += " ";
			ans += to_string(coords.second);
			ans += " ";
		}else if(v[i].first == 2){
			ans += "RS";
			my_coord_to_yinsh(coords, ((v[i].second).first).first, ((v[i].second).first).second);
			ans += " ";
			ans += to_string(coords.first);
			ans += " ";
			ans += to_string(coords.second);
			ans += " ";
			ans += "RE";
			my_coord_to_yinsh(coords, ((v[i].second).second).first, ((v[i].second).second).second);
			ans += " ";
			ans += to_string(coords.first);
			ans += " ";
			ans += to_string(coords.second);
			ans += " ";			
		}else{
			ans += "X";
			my_coord_to_yinsh(coords, ((v[i].second).first).first, ((v[i].second).first).second);
			ans += " ";
			ans += to_string(coords.first);
			ans += " ";
			ans += to_string(coords.second);
			ans += " ";						
		}
		// ans += int_to_move[v[3*i]];
		// ans += " ";
		// ans += to_string(coords.first);
		// ans += " ";
		// ans += to_string(coords.second);
		// ans += " ";
	}
	cout << ans << endl;
	input();
}
