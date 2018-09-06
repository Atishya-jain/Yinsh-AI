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
}

void player::make_next_move(vector<vector<pos>>& board, vector<int>& moves){
	if(num_rings_placed < num_rings){
		place_rings(board, moves);
		num_rings_placed++;
	}else{
		uniform_int_distribution<> distr2(0, num_rings+0.99999999); // define the range
		int ring = distr2(eng);
		// cout << "ring: " << ring << endl;
		pair<int,int> coords = board_to_my_coord(ring_pos[ring].first, ring_pos[ring].second, num_rings);
		vector<pair<int,pair<int,int>>> all_valid_moves;
		// cout << "Let's find all valid moves.\n";
		get_valid_moves(board, all_valid_moves, ring);
		// cout << "Valid moves done\n";
		if(all_valid_moves.size() > 0){
			uniform_int_distribution<> distr3(0, all_valid_moves.size()-0.00000001); // define the range
			int index = distr3(eng);
			if(all_valid_moves[index].first == 1){
				moves.push_back(1);
				moves.push_back(coords.first);
				moves.push_back(coords.second);
			
				coords = board_to_my_coord((all_valid_moves[index].second).first, (all_valid_moves[index].second).second, num_rings);
				moves.push_back(coords.first);
				moves.push_back(coords.second);
				ring_pos[ring].first = coords.first;
				ring_pos[ring].second = coords.second;
			}else if(all_valid_moves[index].first == 2){
				moves.push_back(2);
				moves.push_back(coords.first);
				moves.push_back(coords.second);

				coords = board_to_my_coord((all_valid_moves[index].second).first, (all_valid_moves[index].second).second, num_rings);
				moves.push_back(coords.first);
				moves.push_back(coords.second);
				ring_pos[ring].first = coords.first;
				ring_pos[ring].second = coords.second;

				coords = board_to_my_coord((stretch.first).first, (stretch.first).second, num_rings);
				moves.push_back(coords.first);
				moves.push_back(coords.second);

				coords = board_to_my_coord((stretch.second).first, (stretch.second).second, num_rings);
				moves.push_back(coords.first);
				moves.push_back(coords.second);

				coords = board_to_my_coord(ring_pos[ring_pos.size()-1].first, ring_pos[ring_pos.size()-1].second, num_rings);
				moves.push_back(coords.first);
				moves.push_back(coords.second);	

				// For now remove last ring always			
				ring_pos.erase(ring_pos.end()-1);
			}
		}else{
			make_next_move(board, moves);
		}
	}
}

void player::place_rings(vector<vector<pos>>& board, vector<int>& moves){
	uniform_int_distribution<> distr(-1*(num_rings+0.99999999), num_rings+0.99999999); // define the range
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
		place_rings(board, moves);
	}
}

void player::get_valid_moves(vector<vector<pos>>& board, vector<pair<int,pair<int,int>>>& all_valid_moves, int ring_index){
	int init_x = ring_pos[ring_index].first;
	int init_y = ring_pos[ring_index].second;
	bool trail = false; // Variable that is true at marker positions
	bool my_conti = false; // Variable that is true if continuously opponent's markers are coming
	bool stretched = false; // Variable to check if a continuous trail of 5 or more occurred
	int count_trail = 0;
	int step_x, step_y;

	// cout << "Start with for loop\n";
	for(int i = 0; i<6; i++){
		// cout << "step: " << i << endl;

		if(i == 0){
			step_x = 1; step_y = 0;
		}else if(i == 1){
			step_x = 1; step_y = 1;
		}else if(i == 2){
			step_x = 0; step_y = 1;
		}else if(i == 3){
			step_x = 0; step_y = -1;
		}else if(i == 4){
			step_x = -1; step_y = 0;
		}else if(i == 5){
			step_x = -1; step_y = -1;
		}
		trail = false;
		my_conti = false;
		count_trail = 0;
		init_x = ring_pos[ring_index].first;
		init_y = ring_pos[ring_index].second;
		stretched = false;
		init_x += step_x;
		init_y += step_y;
		while((init_x >= 0) && (init_x < board_size) && (init_y >= 0) && (init_y < board_size)){
			if(board[init_x][init_y].valid == false){
				break;
			}else if(board[init_x][init_y].marker != 2){
				trail = true;
				if(board[init_x][init_y].marker == 1-id){
					if(!my_conti){
						my_conti = true;
						(stretch.first).first = init_x;
						(stretch.first).second = init_y;
						(stretch.second).first = max_lim_p;
						(stretch.second).second = max_lim_p;						
					}
					count_trail++;
				}else{
					my_conti = false;
					if(count_trail == num_rings){
						(stretch.second).first = init_x;
						(stretch.second).second = init_y;
						stretched = true;					
					}else if(count_trail > num_rings){
						// Choose randomly but I am choosing first one always
						(stretch.first).first = min((stretch.first).first, init_x);
						(stretch.first).second = min((stretch.first).second, init_y);
						(stretch.second).first = (stretch.first).first + (trail_length-1);
						(stretch.second).second = (stretch.first).second + (trail_length-1);					
						stretched = true;
					}
					count_trail = 0;	
				}
			}else{
				if(trail){
					if(stretched){
						all_valid_moves.push_back(make_pair(2, make_pair(init_x, init_y)));
					}else if((my_conti == true) && (count_trail == num_rings)){
						all_valid_moves.push_back(make_pair(2, make_pair(init_x, init_y)));
						(stretch.second).first = init_x;
						(stretch.second).second = init_y;						
					}else if((my_conti == true) && (count_trail > num_rings)){
						all_valid_moves.push_back(make_pair(2, make_pair(init_x, init_y)));
						(stretch.first).first = min((stretch.first).first, init_x);
						(stretch.first).second = min((stretch.first).second, init_y);
						(stretch.second).first = (stretch.first).first + (trail_length-1);
						(stretch.second).second = (stretch.first).second + (trail_length-1);											
					}else{
						all_valid_moves.push_back(make_pair(1, make_pair(init_x, init_y)));						
					}
					my_conti = false;
					count_trail = 0;
					stretched = false;
					break;
				}else{
					all_valid_moves.push_back(make_pair(1,make_pair(init_x, init_y)));
				}
				trail = false;
			}
			init_x += step_x;
			init_y += step_y;
		}
	}
}
