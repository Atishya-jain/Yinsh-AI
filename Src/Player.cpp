#include "Player.h"
#include "Utils.h"

random_device rd; // obtain a random number from hardware
mt19937 eng(rd()); // seed the generator
uniform_int_distribution<> distr(-1*(num_rings+0.99999999), num_rings+0.99999999); // define the range
uniform_int_distribution<> distr2(0, num_rings+0.99999999); // define the range

player::player(){

}

player::player(int numr){
	num_rings_placed = 0;
	num_rings = numr;
}

void player::make_next_move(vector<vector<pair<int,int>>> board, vector<int> moves){
	if(num_rings_placed < num_rings){
		place_rings(board, moves, num_rings);
		num_rings_placed++;
	}else{
		int ring = distr2(eng);
		pair<int,int> coords = board_to_my_coord(ring_pos[ring].first, ring_pos[ring].second, num_rings);
		vector<pair<int,pair<int,int>>> all_valid_moves;
		get_valid_moves(board, all_valid_moves);
		if(all_valid_moves.size() > 0){
			uniform_int_distribution<> distr3(0, all_valid_moves.size()-0.00000001); // define the range
			int index = distr3(eng);
			if(all_valid_moves[index].first == 1){
				moves.push_back(1);
				moves.push_back(coords.first);
				moves.push_back(coords.second);
				coords = board_to_my_coord((all_valid_moves[index].second).first, (ring_pos[ring].second).second, num_rings);
				moves.push_back(coords.first);
				moves.push_back(coords.second);
			}else if(all_valid_moves[index].first == 2){
				moves.push_back(2);
				moves.push_back(coords.first);
				moves.push_back(coords.second);
				coords = board_to_my_coord((all_valid_moves[index].second).first, (ring_pos[ring].second).second, num_rings);
				moves.push_back(coords.first);
				moves.push_back(coords.second);
				moves.push_back((stretch.first).first);
				moves.push_back((stretch.first).second);
				moves.push_back((stretch.second).first);
				moves.push_back((stretch.second).second);

			}
		}else{
			make_next_move(board, moves);
		}
	}
}

void player::place_rings(vector<vector<pair<int,int>>> board, vector<int> moves){
	int x = distr(eng);
	int y = distr(eng);
	pair<int,int> coords = my_coord_to_board(x,y,num_rings);
	if((board[coords.first][coords.second].valid == true) && (board[coords.first][coords.second].ring == false)){
		board[coords.first][coords.second].set(0,true);
		moves.push_back(0);
		moves.push_back(x);
		moves.push_back(y);
		ring_pos.push_back(make_pair(coords.first, coords.second));
	}else{
		place_rings(board, moves, num_rings);
	}
}
