#include "Utils.h"
#include "Player.h"
int max_lim_p = numeric_limits<int>::max();
int min_lim_p = numeric_limits<int>::min();

random_device rd; // obtain a random number from hardware
mt19937 eng(rd()); // seed the generator

player::player(){

}

player::player(int numr, int idd, int tl){
	num_rings_placed = 0;
	num_rings = numr;
	id = idd;
	trail_length = tl;
	board_size = 2*numr+1;
	my_trails[0].clear();
	my_trails[1].clear();
	my_trails[2].clear();
	opp_trails[0].clear();
	opp_trails[1].clear();
	opp_trails[2].clear();
}

void player::make_next_move(vector<vector<pos>>& board, vector<int>& moves){
	if(num_rings_placed < num_rings){
		place_rings(board, moves);
		num_rings_placed++;
	}else{		
		// See if you may need to remove your ring as opponent makes your trail
		if((my_trails[0].size() != 0) || (my_trails[1].size() != 0) || (my_trails[2].size() != 0)){
			cerr << "Shouldn't be here\n";
			remove_trails_and_rings(board, moves);
		}

		uniform_int_distribution<> distr2(0, my_ring_pos.size()-1);
		int ring = distr2(eng);
		cerr << "Ring Selected: " << ring << endl;

		int initial_x = my_ring_pos[ring].first;
		int initial_y = my_ring_pos[ring].second;
		vector<pair<int,pair<int,int>>> all_valid_moves;

		cerr << "Let's get valid moves\n";
		// Asking for my valid moves
		get_valid_moves(board, all_valid_moves, ring, true);
		cerr << "Initial Moves done\n";
		// cerr << "Valid moves done\n";
		if(all_valid_moves.size() > 0){
			uniform_int_distribution<> distr3(0, all_valid_moves.size()-1);
			int index = distr3(eng);
			cerr << "Let's update board\n";
			cerr << "moves: " << (all_valid_moves[index].second).first << " " << (all_valid_moves[index].second).second << endl;
			update_board(board, 1, initial_x, initial_y, (all_valid_moves[index].second).first, (all_valid_moves[index].second).second, true);
			cerr << "update done\n";
			// mov movet1, movet2, coord1, coord2, coord3, coord4;
			// movet1.move_t = "S"; movet2.move_t = "M";
			// coord1.coord = initial_x; coord2.coord = initial_y;
			// coord3.coord = (all_valid_moves[index].second).first; coord4.coord = (all_valid_moves[index].second).second;

			moves.push_back(1);
			moves.push_back(initial_x);
			moves.push_back(initial_y);
			moves.push_back(2);
			moves.push_back((all_valid_moves[index].second).first);
			moves.push_back((all_valid_moves[index].second).second);
			my_ring_pos[ring].first = (all_valid_moves[index].second).first;
			my_ring_pos[ring].second = (all_valid_moves[index].second).second;
			
			cerr << "Let's remove trails\n";
			remove_trails_and_rings(board, moves);
			cerr << "Trails removed\n";
		}
		else{
			make_next_move(board, moves);
		}
	}
}

void player::place_rings(vector<vector<pos>>& board, vector<int>& moves){
	uniform_int_distribution<> distr(0, board_size-1); // define the range
	int x = distr(eng);
	int y = distr(eng);
	// pair<int,int> coords = my_coord_to_board(x,y,num_rings);
	if((board[x][y].valid == true) && (board[x][y].ring == 2)){
		// board[x][y].set(2,true);
		update_board(board, 0, x, y, max_lim_p, max_lim_p, true);
		// mov movet, coord1, coord2;
		// movet.move_t = "P";
		// coord1.coord = x;
		// coord2.coord = y;
		
		moves.push_back(0);
		moves.push_back(x);
		moves.push_back(y);
		// my_ring_pos.push_back(make_pair(x, y));
		// board[coords.first][coords.second].set(2, true);
	}else{
		place_rings(board, moves);
	}
}

void player::get_valid_moves(vector<vector<pos>>& board, vector<pair<int,pair<int,int>>>& all_valid_moves, int ring_index, bool my_turn){
	// all_valid_moves.clear();
	int player, dir, init_x, init_y, step_x, step_y;
	bool trail; // To check if we are going over a series of markers
	if(my_turn){
		player = id;
	}else{
		player = 1-id;
	}

	for(int i = 0; i<6; i++){
		if(i == 0){
			step_x = 1; step_y = 0;
			// dir = 0;
		}else if(i == 1){
			step_x = 1; step_y = 1;
			// dir = 2;
		}else if(i == 2){
			step_x = 0; step_y = 1;
			// dir = 1;
		}else if(i == 3){
			step_x = 0; step_y = -1;
			// dir = 1;
		}else if(i == 4){
			step_x = -1; step_y = 0;
			// dir = 0;
		}else if(i == 5){
			step_x = -1; step_y = -1;
			// dir = 2;
		}
		trail = false;
		if(my_turn){
			init_x = my_ring_pos[ring_index].first;
			init_y = my_ring_pos[ring_index].second;
		}else{
			init_x = opp_ring_pos[ring_index].first;
			init_y = opp_ring_pos[ring_index].second;
		}
		// check_my_trail(board, init_x, init_y, (dir+1)%3, true);
		// check_my_trail(board, init_x, init_y, (dir+2)%3, true);

		init_x += step_x;
		init_y += step_y;

		while((init_x >= 0) && (init_x < board_size) && (init_y >= 0) && (init_y < board_size)){
			if((board[init_x][init_y].valid == false) || (board[init_x][init_y].ring != 2)){
				break;
			}else if(board[init_x][init_y].marker != 2){
				trail = true;
				// check_my_trail(board, init_x, init_y, (dir+1)%3);
				// check_my_trail(board, init_x, init_y, (dir+2)%3);
			}else{
				if(trail){
					all_valid_moves.push_back(make_pair(player,make_pair(init_x, init_y)));
					break;
				}else{
					all_valid_moves.push_back(make_pair(player,make_pair(init_x, init_y)));
				}
				trail = false;
			}
			init_x += step_x;
			init_y += step_y;
		}
	}
}


void player::update_board(vector<vector<pos>>& board, int action_on_ring, int initial_x, int initial_y, int final_x, int final_y, bool my_turn){	
	int player;
	if(my_turn){
		player = id;
	}else{
		player = 1-id;
	}

	if(action_on_ring == 0){
		pair<int,int> temp;
		board[initial_x][initial_y].set(2, player);
		if(!my_turn){
			temp.first = initial_x;
			temp.second = initial_y;
			opp_ring_pos.push_back(temp);
		}else{
			temp.first = initial_x;
			temp.second = initial_y;
			my_ring_pos.push_back(temp);
		}		
	}else if(action_on_ring == 1){
		board[initial_x][initial_y].set(player, 2);
		cerr << "Flip Markers Start\n";
		flip_markers(board, initial_x, initial_y, final_x, final_y, my_turn);		
		cerr << "Flip end\n";
		board[final_x][final_y].set(2, player);
	}else if(action_on_ring == 2){
		remove_markers(board, initial_x, initial_y, final_x, final_y, my_turn);
	}else{
		board[initial_x][initial_y].set(2, 2);
		if(!my_turn){
			int len = opp_ring_pos.size();
			for(int i = 0; i<len; i++){
				bool a1 = opp_ring_pos[i].first == initial_x;
				bool a2 = opp_ring_pos[i].second == initial_y;
				if(a1 && a2){
					opp_ring_pos.erase(opp_ring_pos.begin() + i);
				}
			}
		}else{
			int len = my_ring_pos.size();
			for(int i = 0; i<len; i++){
				bool a1 = my_ring_pos[i].first == initial_x;
				bool a2 = my_ring_pos[i].second == initial_y;
				if(a1 && a2){
					my_ring_pos.erase(my_ring_pos.begin() + i);
				}
			}
		}
	}
}


// Won't flip the marker at x2, y2 and x1,y1
void player::flip_markers(vector<vector<pos>>& board, int x1, int y1, int x2, int y2, bool my_turn){
	int startX,startY,endX,endY;

	if(x1 == x2){
		startY = min(y1, y2);
		endY = max(y1, y2);
		check_my_trail(board, x1, y1, 0, my_turn);
		check_my_trail(board, x1, y1, 2, my_turn);
		for(int i = startY+1; i< endY; i++){
			if(board[x1][i].marker != 2){
				board[x1][i].set(1-board[x1][i].marker,2);
				check_my_trail(board, x1, i, 0, !my_turn);
				check_my_trail(board, x1, i, 2, !my_turn);
				check_my_trail(board, x1, i, 0, my_turn);
				check_my_trail(board, x1, i, 2, my_turn);
			}
		}
		check_my_trail(board, x1, y1, 1, my_turn);
		check_my_trail(board, x1, y1, 1, !my_turn);
	}else if(y1 == y2){
		startX = min(x1, x2);
		endX = max(x1, x2);
		check_my_trail(board, x1, y1, 1, my_turn);
		check_my_trail(board, x1, y1, 2, my_turn);
		for(int i = startX+1; i< endX; i++){
			if(board[i][y1].marker != 2){
				board[i][y1].set(1-board[i][y1].marker,2);
				check_my_trail(board, i, y1, 1, !my_turn);
				check_my_trail(board, i, y1, 2, !my_turn);
				check_my_trail(board, i, y1, 1, my_turn);
				check_my_trail(board, i, y1, 2, my_turn);
			}
		}		
		check_my_trail(board, x1, y1, 0, !my_turn);
		check_my_trail(board, x1, y1, 0, my_turn);
	}else{
		startX = min(x1, x2);
		startY = min(y1, y2);
		endX = max(x1, x2);
		endY = max(y1, y2);
		check_my_trail(board, x1, y1, 0, my_turn);
		check_my_trail(board, x1, y1, 1, my_turn);
		for(int i = startX+1, j=startY+1; i< endX && j<endY ; i++,j++){
			if(board[i][j].marker != 2){
				board[i][j].set(1-board[i][j].marker,2);
				check_my_trail(board, i, j, 0, !my_turn);
				check_my_trail(board, i, j, 1, !my_turn);
				check_my_trail(board, i, j, 0, my_turn);
				check_my_trail(board, i, j, 1, my_turn);
			}
		}
		check_my_trail(board, x1, y1, 2, !my_turn);
		check_my_trail(board, x1, y1, 2, my_turn);
	}	
}
void player::remove_markers(vector<vector<pos>>& board, int x1, int y1, int x2, int y2, bool my_turn){
int startX,startY,endX,endY, dir;

	if(x1 == x2){
		startY = min(y1, y2);
		endY = max(y1, y2);
		for(int i = startY; i<= endY; i++){
			board[x1][i].set(2,2);
		}
		dir = 1;
	}else if(y1 == y2){
		startX = min(x1, x2);
		endX = max(x1, x2);
		for(int i = startX; i<= endX; i++){
			board[i][y1].set(2,2);
		}
		dir = 0;		
	}else{
		startX = min(x1, x2);
		startY = min(y1, y2);
		endX = max(x1, x2);
		endY = max(y1, y2);
		for(int i = startX, j=startY; i<= endX && j<=endY ; i++,j++){
			board[i][j].set(2,2);
		}
		dir = 2;
	}

	if(my_turn){
		int len = my_trails[dir].size();
		for(int i = 0; i<len;i++){
			bool a1 = (my_trails[dir][i].first).first == x1;
			bool a2 = (my_trails[dir][i].first).second == y1;
			bool a3 = (my_trails[dir][i].second).first == x2;
			bool a4 = (my_trails[dir][i].second).second == y2;
			if(a1 && a2 && a3 && a4){
				my_trails[dir].erase(my_trails[dir].begin()+i);
			}
		}
	}else{
		int len = opp_trails[dir].size();
		for(int i = 0; i<len;i++){
			bool a1 = (opp_trails[dir][i].first).first == x1;
			bool a2 = (opp_trails[dir][i].first).second == y1;
			bool a3 = (opp_trails[dir][i].second).first == x2;
			bool a4 = (opp_trails[dir][i].second).second == y2;
			if(a1 && a2 && a3 && a4){
				opp_trails[dir].erase(opp_trails[dir].begin()+i);
			}
		}		
	}
}

void player::check_my_trail(vector<vector<pos>>& board, int x1, int y1, int dir, bool my_turn){
	int count = 0;
	int step_x, step_y, player, startX, startY;
	bool trail = false; bool tempMade = false;
	if(my_turn){
		player = id;
	}else{
		player = 1-id;
	} 

	if(dir == 0){
		step_x = 1; step_y = 0;
		startX = 0; startY = y1;
	}else if(dir == 1){
		step_x = 0; step_y = 1;
		startX = x1; startY = 0;
	}else{
		step_x = 1; step_y = 1;
		startX = x1 - min(x1,y1); startY = y1 - min(x1, y1);
	}
	pair <pair<int,int>, pair<int,int>> temp, temp2;
	int i,j;
	for(i = startX, j = startY; (i< board_size) && (j < board_size); i=i+step_x, j=j+step_y){
		if(board[i][j].valid == true){
			if(board[i][j].marker == player){
				trail = true;
				tempMade = false;
				if(count == 0){
					(temp.first).first = i;
					(temp.first).second = j;
				}
				count++;	
			}else if(board[i][j].marker == (1-player)){
				trail = true;
				if(count >= trail_length){
					tempMade = true;
					(temp.second).first = i-step_x;
					(temp.second).second = j-step_y;
					break;				
				}
				count = 0;
			}else{			
				if(count >= trail_length){
					tempMade = true;
					(temp.second).first = i-step_x;
					(temp.second).second = j-step_y;
				}
				// if(trail){
				// 	trail = false;
				// 	break;
				// }
				count = 0;
				trail = false;
				// break;
			}
		}else{
			if((count >= trail_length) && (tempMade == false)){
				(temp.second).first = i-step_x;
				(temp.second).second = j-step_y;
				tempMade = true;
				break;				
			}
		}
	}
	if((tempMade == false) && (count >= trail_length)){
		(temp.second).first = i-step_x;
		(temp.second).second = j-step_y;		
	}

	if(count == trail_length){
		if(my_turn){
			my_trails[dir].push_back(temp);
		}else{
			opp_trails[dir].push_back(temp);
		}
	}else if(count > trail_length){
		(temp2.second).first = (temp.second).first;
		(temp2.second).second = (temp.second).second;
		(temp2.first).first = (temp2.second).first - step_x*(trail_length-1);
		(temp2.first).second = (temp2.second).second - step_y*(trail_length-1);

		(temp.second).first = (temp.first).first + step_x*(trail_length-1);
		(temp.second).second = (temp.first).second + step_y*(trail_length-1);
		if(my_turn){
			my_trails[dir].push_back(temp);
			my_trails[dir].push_back(temp2);
		}else{
			opp_trails[dir].push_back(temp);
			opp_trails[dir].push_back(temp2);
		}		
	}
}

void player::remove_trails_and_rings(vector<vector<pos>>& board, vector<int>& moves){
	// For now its random
	pair<pair<int, int>, pair<int, int>> pp;
	bool flag = false;
	for(int i = 0; i< 3; i++){
		int len = my_trails[i].size();
		cerr << "Size of " << i << " is: " << len << endl; 
		for(int j = 0; j< len; j++){
			cerr << "Inside: " << (my_trails[i][j].first).first << endl;
			(pp.first).first = (my_trails[i][j].first).first;
			(pp.first).second = (my_trails[i][j].first).second;
			(pp.second).first = (my_trails[i][j].second).first;
			(pp.second).second = (my_trails[i][j].second).second;

			moves.push_back(3); moves.push_back((pp.first).first); moves.push_back((pp.first).second);
			moves.push_back(4); moves.push_back((pp.second).first); moves.push_back((pp.second).second);
			update_board(board, 2, (my_trails[i][j].first).first, (my_trails[i][j].first).second, (my_trails[i][j].second).first, (my_trails[i][j].second).second, true);
			remove_ring(board, moves);
			remove_repeated_trails(board, pp, i);
			flag = true;
			break;
		}
		if(flag){
			break;
		}
	}

	bool a1 = (my_trails[0].size() != 0);
	bool a2 = (my_trails[1].size() != 0);
	bool a3 = (my_trails[2].size() != 0);
	if(a1 || a2 || a3){
		remove_trails_and_rings(board, moves);
	}
}

void player::remove_ring(vector<vector<pos>>& board, vector<int>& moves){
	moves.push_back(5); moves.push_back((my_ring_pos.back()).first); moves.push_back((my_ring_pos.back()).second);
	update_board(board, 3, (my_ring_pos.back()).first, (my_ring_pos.back()).second, max_lim_p, max_lim_p, true);
}

void player::remove_repeated_trails(vector<vector<pos>>& board, pair<pair<int, int>, pair<int, int>>& pp, int dir){
	int x1, x2, y1, y2, len;
	x1 = (pp.first).first;
	y1 = (pp.first).second;
	x2 = (pp.second).first;
	y2 = (pp.second).second;
	
	if(dir == 0){
		len = my_trails[0].size();
		for(int i = 0; i< len; i++){
			bool a1 = ((pp.first).first >= (my_trails[0][i].first).first) && ((pp.first).first <= (my_trails[0][i].second).first);
			bool a2 = ((pp.second).first >= (my_trails[0][i].first).first) && ((pp.second).first <= (my_trails[0][i].second).first);
			if(a1 || a2){
				my_trails[0].erase(my_trails[0].begin() + i);
			}
		}
		len = my_trails[1].size();
		for(int i = 0; i< len; i++){
			bool a1 = ((pp.first).second >= (my_trails[1][i].second).first) && ((pp.first).second <= (my_trails[1][i].second).second);
			if(a1){
				my_trails[1].erase(my_trails[1].begin() + i);
			}
		}	

		///////////////////////////////// Get eq of line and find if both coordinates lie on opposite side

		// len = my_trails[2].size();
		// for(int i = 0; i< len; i++){
		// 	bool a1 = ((pp.first).second >= (my_trails[i].second).first) && ((pp.first).second <= (my_trails[i].second).second);
		// 	if(a1){
		// 		my_trails[2].erase(my_trails[2].begin() + i);
		// 	}
		// }
	}else if(dir == 1){
		len = my_trails[1].size();
		for(int i = 0; i< len; i++){
			bool a1 = ((pp.first).second >= (my_trails[1][i].first).second) && ((pp.first).second <= (my_trails[1][i].second).second);
			bool a2 = ((pp.second).second >= (my_trails[1][i].first).second) && ((pp.second).second <= (my_trails[1][i].second).second);
			if(a1 || a2){
				my_trails[1].erase(my_trails[1].begin() + i);
			}
		}
		len = my_trails[0].size();
		for(int i = 0; i< len; i++){
			bool a1 = ((pp.second).second >= (my_trails[0][i].second).second) && ((pp.first).second <= (my_trails[0][i].second).second);
			if(a1){
				my_trails[0].erase(my_trails[0].begin() + i);
			}
		}	

		///////////////////////////////// Get eq of line and find if both coordinates lie on opposite side

		// len = my_trails[2].size();
		// for(int i = 0; i< len; i++){
		// 	bool a1 = ((pp.first).second >= (my_trails[i].second).first) && ((pp.first).second <= (my_trails[i].second).second);
		// 	if(a1){
		// 		my_trails[2].erase(my_trails[2].begin() + i);
		// 	}
		// }
	}
	// else {

	// }
}