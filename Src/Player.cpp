#include "Utils.h"
#include "Player.h"
int max_lim_p = numeric_limits<int>::max();
int min_lim_p = numeric_limits<int>::min();

random_device rd; // obtain a random number from hardware
mt19937 eng(rd()); // seed the generator

player::player(){

}

player::player(int numr, int idd, int tl, int win){
	num_rings_placed = 0;
	num_rings_removed = 0;
	num_rings = numr;
	DEPTH_TO_CHECK = 3;
	id = idd;
	trail_length = tl;
	to_win_remove = win;
	board_size = 2*numr+1;
	my_trails[0].clear();
	my_trails[1].clear();
	my_trails[2].clear();
	opp_trails[0].clear();
	opp_trails[1].clear();
	opp_trails[2].clear();
}

void player::get_all_removes(vector<vector<pos>>& local_board, vector<pair<int,pair<pair<int,int>,pair<int,int>>>>& one_remove, vector<vector<pair<int,pair<pair<int,int>,pair<int,int>>>>>& valid_removes, vector<pair<int,int>> local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], bool my_turn){
	bool flag = false;
	// cerr << "Local trails size: " << local_trails[i].size() << endl;;
	for(int i = 0; i<3; i++){
		for(int j = 0; j< local_trails[i].size(); j++){
			vector<pair<pair<int, int>, pair<int, int>>> temp_trails[3];
			// std::vector<> v; 
			// temp_trails[0] = local_trails[0];
			// temp_trails[1] = local_trails[1];
			// temp_trails[2] = local_trails[2];
			copy(local_trails[0].begin(), local_trails[0].end(), back_inserter(temp_trails[0]));
			copy(local_trails[1].begin(), local_trails[1].end(), back_inserter(temp_trails[1]));
			copy(local_trails[2].begin(), local_trails[2].end(), back_inserter(temp_trails[2]));

			cerr << "Copy Done\n";
			update_board(local_board, local_ring_pos, temp_trails, 2, (temp_trails[i][j].first).first, (temp_trails[i][j].first).second, (temp_trails[i][j].second).first, (temp_trails[i][j].second).second, my_turn);
			cerr << "Markers removed\n";
			one_remove.push_back(make_pair(2,make_pair(make_pair((temp_trails[i][j].first).first, (temp_trails[i][j].first).second), make_pair((temp_trails[i][j].second).first, (temp_trails[i][j].second).second))));
			cerr << "one_remove_size: "<< one_remove.size() << endl;
			temp_trails[i].erase(temp_trails[i].begin()+j);
			pair<pair<int, int>, pair<int, int>> pp = make_pair(make_pair((temp_trails[i][j].first).first, (temp_trails[i][j].first).second), make_pair((temp_trails[i][j].second).first, (temp_trails[i][j].second).second));
			cerr << "Let's remove repeated trails\n";
			remove_repeated_trails(local_board, local_ring_pos, temp_trails, pp, i);
			cerr << "repeated trails gone\n";
			for(int k = 0; k<local_ring_pos.size(); k++){
				cerr << "Declare temp trails\n";
				vector<pair<pair<int, int>, pair<int, int>>> temp_trails2[3];
				cerr << "Copy temp trails\n";
				copy(temp_trails[0].begin(), temp_trails[0].end(), back_inserter(temp_trails2[0]));
				copy(temp_trails[1].begin(), temp_trails[1].end(), back_inserter(temp_trails2[1]));
				copy(temp_trails[2].begin(), temp_trails[2].end(), back_inserter(temp_trails2[2]));
				// temp_trails2[0] = temp_trails[0];
				// temp_trails2[1] = temp_trails[1];
				// temp_trails2[2] = temp_trails[2];
				cerr << "Let's remove rings\n";
				update_board(local_board, local_ring_pos, temp_trails2, 3, (local_ring_pos[i]).first, (local_ring_pos[i]).second, max_lim_p, max_lim_p, true);
				cerr << "rings removed!!!\n";
				one_remove.push_back(make_pair(3,make_pair(make_pair((local_ring_pos[i]).first, (local_ring_pos[i]).second), make_pair(max_lim_p,max_lim_p))));
				cerr << "size after rings removed!!!: " << one_remove.size() << endl;
				if(((temp_trails2[0].size() != 0) || (temp_trails2[1].size() != 0) || (temp_trails2[2].size() != 0)) && (local_ring_pos.size() > num_rings-to_win_remove)){
					cerr << "Going into recursion\n";
					get_all_removes(local_board, one_remove, valid_removes, local_ring_pos, temp_trails2, my_turn);
					cerr << "Go back to place markers\n";
					update_board(local_board, local_ring_pos, temp_trails2, 4, ((one_remove[one_remove.size()-1].second).first).first, ((one_remove[one_remove.size()-1].second).first).second, ((one_remove[one_remove.size()-1].second).second).first, ((one_remove[one_remove.size()-1].second).second).second, my_turn);
					// one_remove.erase(one_remove.size()-1);
					one_remove.erase(one_remove.end()-1);
					// break;
				}else{
					cerr << "May go back now\n";
					vector<pair<int,pair<pair<int,int>,pair<int,int>>>> to_revert;
					to_revert.push_back(one_remove[one_remove.size()-1]);
					revert(local_board, temp_trails2, local_ring_pos, to_revert, my_turn);
					cerr << "Size of one\n";
					valid_removes.push_back(one_remove);
					one_remove.erase(one_remove.end()-1);
				}
			}
		}
	}
}

void player::get_neighbours(vector<vector<pos>> local_board, vector<pair<int,int>> local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<float, vector<pair<int, pair<pair<int,int>,pair<int,int>>>>>>& move, bool my_turn){
	// Get all remove markers and rings
	// For all such removes go ahead and make all possible moves
	// vector<vector<pos>> local_board = board;
	vector<vector<pair<int,pair<pair<int,int>,pair<int,int>>>>> valid_removes;
	// vector<pair<int,pair<pair<int,int>,pair<int,int>>>> valid_step;
	pair<pair<int, int>, pair<int, int>> pp;
	
	// vector<pair<float, vector<int, pair<pair<int,int>,pair<int,int>>>>> move
	// int siz = local_trails[0].size() + local_trails[1].size() + local_trails[2].size();
	vector<pair<int,pair<pair<int,int>,pair<int,int>>>> one_remove;
	cerr << "Inside Get neighbours\n";
	get_all_removes(local_board, one_remove, valid_removes, local_ring_pos, local_trails, my_turn);	
	cerr << "Size of valid_removes: " << valid_removes.size() << endl;
	if(valid_removes.size() > 0){
		cerr << "Why is it here?\n";
		for(int i = 0; valid_removes.size(); i++){
			vector<pair<pair<int, int>, pair<int, int>>> temp_trails2[3];
			copy(local_trails[0].begin(), local_trails[0].end(), back_inserter(temp_trails2[0]));
			copy(local_trails[1].begin(), local_trails[1].end(), back_inserter(temp_trails2[1]));
			copy(local_trails[2].begin(), local_trails[2].end(), back_inserter(temp_trails2[2]));

			play_move(local_board, valid_removes[i], local_ring_pos, local_trails, my_turn);
			for(int j = 0; j<local_ring_pos.size(); j++){
				vector<pair<int,pair<int,int>>> valid_moves;
				get_valid_moves(local_board, local_ring_pos, valid_moves, j, my_turn);
				// formulate_move(move, valid_removes, valid_moves);
				for(int k = 0; k < valid_moves.size(); k++){
					vector<pair<pair<int, int>, pair<int, int>>> temp_trails[3];
					copy(temp_trails2[0].begin(), temp_trails2[0].end(), back_inserter(temp_trails[0]));
					copy(temp_trails2[1].begin(), temp_trails2[1].end(), back_inserter(temp_trails[1]));
					copy(temp_trails2[2].begin(), temp_trails2[2].end(), back_inserter(temp_trails[2]));
					// temp_trail[0] = local_trails[0];
					// temp_trail[1] = local_trails[1];
					// temp_trail[2] = local_trails[2];

					int num_moves = 0;
					play(local_board, temp_trails, local_ring_pos, move, j, valid_moves[k], my_turn, num_moves);
					concatenate(valid_removes[i], move, num_moves);
					// revert(local_board, temp_trail, rem_rings, local_ring_pos, move[move.size()-1], valid_moves[k], my_turn);
				}
			}
		}
	}else{
		cerr << "No valid remove\n";
		for(int j = 0; j<local_ring_pos.size(); j++){
			vector<pair<int,pair<int,int>>> valid_moves;
			get_valid_moves(local_board, local_ring_pos, valid_moves, j, my_turn);
			for(int k = 0; k < valid_moves.size(); k++){
				vector<pair<pair<int, int>, pair<int, int>>> temp_trails[3];
				cerr << "Size of local_trails before play: " << local_trails[0].size() << " " << local_trails[1].size() << " " << local_trails[2].size() << " \n";
				copy(local_trails[0].begin(), local_trails[0].end(), back_inserter(temp_trails[0]));
				copy(local_trails[1].begin(), local_trails[1].end(), back_inserter(temp_trails[1]));
				copy(local_trails[2].begin(), local_trails[2].end(), back_inserter(temp_trails[2]));
				cerr << "Size of temp_trails before play: " << temp_trails[0].size() << " " << temp_trails[1].size() << " " << temp_trails[2].size() << " \n";
				// cout << "Size of temp_trails before play: " << local_trails[0].size << " " << local_trails[1].size << " " << local_trails[2].size << " \n";
				// temp_trail[0] = local_trails[0];
				// temp_trail[1] = local_trails[1];
				// temp_trail[2] = local_trails[2];
				int num_moves = 0;
				cerr << "Let's play them\n";
				play(local_board, temp_trails, local_ring_pos, move, j, valid_moves[k], my_turn, num_moves);
				cerr << "Size of temp_trails after play: " << temp_trails[0].size() << " " << temp_trails[1].size() << " " << temp_trails[2].size() << " \n";
				cerr << "Size of local_trails after play: " << local_trails[0].size() << " " << local_trails[1].size() << " " << local_trails[2].size() << " \n";
				cerr << "Play over\n";
				// concatenate(valid_removes[i], move, num_moves);
				// revert(local_board, temp_trail, local_ring_pos, move, j, valid_moves[k], my_turn);
			}
		}
	}	
}

void player::revert(vector<vector<pos>>& local_board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<int,int>>& local_ring_pos, vector<pair<int, pair<pair<int,int>,pair<int,int>>>>& moves, bool my_turn){
	int start = moves.size() - 1;
	int player;
	if(my_turn){
		player = id;
	}else{
		player = 1-id;
	}

	for(int i = start; i>= 0; i--){
		if(moves[i].first == 3){
			update_board(local_board, local_ring_pos, local_trails, 0, ((moves[i].second).first).first, ((moves[i].second).first).second, max_lim_p, max_lim_p, my_turn);
		}else if(moves[i].first == 2){
			update_board(local_board, local_ring_pos, local_trails, 4, ((moves[i].second).first).first, ((moves[i].second).first).second, ((moves[i].second).second).first, ((moves[i].second).second).second, my_turn);
		}else if(moves[i].first == 1){
			update_board(local_board, local_ring_pos, local_trails, 1, ((moves[i].second).second).first, ((moves[i].second).second).second, ((moves[i].second).first).first, ((moves[i].second).first).second, my_turn);
			local_board[((moves[i].second).second).first][((moves[i].second).second).second].set(2,2);
		}
	}
}

void player::concatenate(vector<pair<int,pair<pair<int,int>,pair<int,int>>>>& valid_removes, vector<pair<float, vector<pair<int, pair<pair<int,int>,pair<int,int>>>>>>& moves, int& num_moves){
	for(int i = 0; i<num_moves; i++){
		vector<pair<int, pair<pair<int,int>,pair<int,int>>>> temp;
		temp = valid_removes;
		temp.insert(temp.end(), (moves[i].second).begin(), (moves[i].second).end());
		moves[moves.size()-i].second = temp;				
	}
}

void player::play(vector<vector<pos>>& local_board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<int,int>>& local_ring_pos, vector<pair<float, vector<pair<int, pair<pair<int,int>,pair<int,int>>>>>>& move, int ring_index, pair<int,pair<int,int>> valid_moves, bool my_turn, int& num_moves){
	cerr << "Inside play\n";
	cerr << "Coordinates: " << local_ring_pos[ring_index].first << " " << local_ring_pos[ring_index].second << " " << (valid_moves.second).first << " " << (valid_moves.second).second << endl;
	vector<pair<int, pair<pair<int,int>,pair<int,int>>>> temp;
	temp.push_back(make_pair(1, make_pair(make_pair(local_ring_pos[ring_index].first,local_ring_pos[ring_index].second), make_pair((valid_moves.second).first,(valid_moves.second).second))));
	update_board(local_board, local_ring_pos, local_trails, 1, local_ring_pos[ring_index].first, local_ring_pos[ring_index].second, (valid_moves.second).first, (valid_moves.second).second, my_turn);

	vector<vector<pair<int,pair<pair<int,int>,pair<int,int>>>>> valid_removes;
	vector<pair<int,pair<pair<int,int>,pair<int,int>>>> one_remove;
	cerr << "After move remove?\n";
	cerr << "local_trails size: " << local_trails[0].size() << " " << local_trails[1].size() << " " << local_trails[2].size() << " \n"; 
	get_all_removes(local_board, one_remove, valid_removes, local_ring_pos, local_trails, my_turn);
	// vector<pair<pair<int, int>, pair<int, int>>> temp_trails2[3];
	// copy(local_trails[0].begin(), local_trails[0].end(), back_inserter(temp_trails2[0]));
	// copy(local_trails[1].begin(), local_trails[1].end(), back_inserter(temp_trails2[1]));
	// copy(local_trails[2].begin(), local_trails[2].end(), back_inserter(temp_trails2[2]));
	if(valid_removes.size() > 0){
		cerr << "yes some removes\n";
		num_moves = valid_removes.size();
		// int len = valid_moves.size();
		for(int i = 0; i<num_moves; i++){
			cout << "In some remove loop\n";
			vector<pair<pair<int, int>, pair<int, int>>> temp_trails[3];
			copy(local_trails[0].begin(), local_trails[0].end(), back_inserter(temp_trails[0]));
			copy(local_trails[1].begin(), local_trails[1].end(), back_inserter(temp_trails[1]));
			copy(local_trails[2].begin(), local_trails[2].end(), back_inserter(temp_trails[2]));

			// temp_trails[0] = local_trails[0];
			// temp_trails[1] = local_trails[1];
			// temp_trails[2] = local_trails[2];
			cerr << "Play one remove \n";
			play_move(local_board, valid_removes[i], local_ring_pos, temp_trails, my_turn);
			vector<pair<int, pair<pair<int,int>,pair<int,int>>>> temp2;
			temp2 = temp;
			temp2.insert(temp2.end(), valid_removes[i].begin(), valid_removes[i].end());
			// for(int j = 0; j<valid_removes[i].size(); j=j++){	
			// 	update_board(local_board, local_ring_pos, temp_trails, valid_removes[i][j].first, ((valid_removes[i][j].second).first).first, ((valid_removes[i][j].second).first).second, ((valid_removes[i][j].second).second).first, ((valid_removes[i][j].second).second).second, my_turn);
			// }
			cerr << "after updating board\n";
			// remove_trails_and_rings(board, rem_rings, local_trails, moves);
			float h; 
			if(local_ring_pos.size() <= (num_rings-to_win_remove)){
				h = max_lim_p;
			}else{
				h = heuristic(local_board);
			}
			move.push_back(make_pair(h, temp2));
			revert(local_board, temp_trails, local_ring_pos, valid_removes[i], my_turn);
		}
	}else{
		cerr << "no removes\n";
			// vector<pair<int, pair<pair<int,int>,pair<int,int>>>> temp2;
			// temp2 = temp;
			// temp2.insert(temp2.end(), valid_removes[i].begin(), valid_removes[i].end());
			// for(int j = 0; j<valid_removes[i].size(); j=j++){	
			// 	update_board(local_board, local_ring_pos, local_trails, valid_removes[i][j].first, ((valid_removes[i][j].second).first).first, ((valid_removes[i][j].second).first).second, ((valid_removes[i][j].second).second).first, ((valid_removes[i][j].second).second).second, my_turn);
			// }
			// // remove_trails_and_rings(board, rem_rings, local_trails, moves);
			float h; 
			// if(local_ring_pos.size() <= (num_rings-to_win_remove)){
				// h = max_lim_p;
			// }else{
				h = heuristic(local_board);
			// }
			move.push_back(make_pair(h, temp));
	}
	cerr << "After all jhanjhat\n";
	revert(local_board, local_trails, local_ring_pos, temp, my_turn);
}

void player::play_move(vector<vector<pos>>& local_board, vector<pair<int,pair<pair<int,int>,pair<int,int>>>>& moves, vector<pair<int,int>>& local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], bool my_turn){
	int len = moves.size();
	for(int i = 0; i<len; i++){
		// cerr << "Coordinates: " << moves[i].first << " " << ((moves[i].second).first).first << " " << ((moves[i].second).first).second << " " << ((moves[i].second).second).first << " " << ((moves[i].second).second).second << endl;
		update_board(local_board, local_ring_pos, local_trails, (moves[i].first), ((moves[i].second).first).first, ((moves[i].second).first).second, ((moves[i].second).second).first, ((moves[i].second).second).second, my_turn);
	}
}

float player::heuristic(vector<vector<pos>>& board){
	return 1.1;
}

//I think it should return <board, heuristic>
pair<int,float> player::MinVal(vector<vector<pos>>& board, int current_depth, float alpha, float beta){
	// if (current_depth == DEPTH_TO_CHECK)
	// 	return make_pair(board,heuristic(board));

	// vector<vector<pos>> tmp;
	// pair<vector<vector<pos>>,float> best_child = make_pair(tmp,99999999);
	// pair<vector<vector<pos>>,float> child;

	// for (int s=0; s<10;s++ /*in children(state)*/){ //CHANGE_THIS
	// 	child = MaxVal(board,current_depth+1,alpha,beta);
	// 	beta = min(beta,child.second);
	// 	if (alpha>=beta) return child;
	// 	if (child.second < best_child.score){
	// 		best_child.first = child.first;
	// 		best_child.second = child.second;
	// 	}
	// }
	// return best_child; 
}

pair<int,float> player::MaxVal(vector<vector<pos>>& board, int current_dept, float alpha, float beta){
	// if (current_depth == DEPTH_TO_CHECK)
		// return make_pair(0,heuristic(board));

	// vector<vector<pos>> tmp;
	// pair<vector<vector<pos>>,float> best_child = make_pair(tmp,-1);
	// pair<vector<vector<pos>>,float> child;

	// for (int s = 0; s<10; s++/*s in children(state)*/){ //CHANGE_THIS
	// 	child = MinVal(board,current_depth+1,alpha,beta);
	// 	alpha = max(alpha,child.second);
	// 	if (alpha>=beta) return child;
	// 	if (child.second > best_child.score){
	// 		best_child.first = child.first;
	// 		best_child.second = child.second;
	// 	}
	// }
	// return best_child; 
}


void player::make_next_move(vector<vector<pos>>& board, vector<pair<int,int>>& local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<int,pair<pair<int,int>,pair<int,int>>>>& out){
	if(num_rings_placed < num_rings){
		place_rings(board, local_ring_pos, local_trails, out);
		num_rings_placed++;
		cerr << "DDOONNEE\n";
		// break;
	}else{	
		vector<pair<float, vector<pair<int, pair<pair<int,int>,pair<int,int>>>>>> move;
		
		// int len = local_ring_pos.size();
		get_neighbours(board, local_ring_pos, local_trails, move, true);
		cerr << "Finally Neighbours received and num valid moves: " << move.size() << endl;

		// cerr << "Final Coordinates: " << move[0].second
		play_move(board, move[0].second, local_ring_pos, local_trails, true);
		cerr << "Finally move done\n";
		out = move[0].second;
		


		// move_to_output();
	//	// See if you may need to remove your ring as opponent makes your trail
	// 	if((my_trails[0].size() != 0) || (my_trails[1].size() != 0) || (my_trails[2].size() != 0)){
	// 		cerr << "Shouldn't be here\n";
	// 		remove_trails_and_rings(board, my_trails, moves);
	// 	}
	// 	if(num_rings_removed < to_win_remove){
	// 		uniform_int_distribution<> distr2(0, my_ring_pos.size()-1);
	// 		int ring = distr2(eng);
	// 		cerr << "Ring Selected: " << ring << endl;

	// 		int initial_x = my_ring_pos[ring].first;
	// 		int initial_y = my_ring_pos[ring].second;
	// 		vector<pair<int,pair<int,int>>> all_valid_moves;

	// 		cerr << "Let's get valid moves\n";
	// 		// Asking for my valid moves
	// 		get_valid_moves(board, my_ring_pos, all_valid_moves, ring, true);
	// 		cerr << "Initial Moves done\n";
	// 		// cerr << "Valid moves done\n";
	// 		if(all_valid_moves.size() > 0){
	// 			uniform_int_distribution<> distr3(0, all_valid_moves.size()-1);
	// 			int index = distr3(eng);
	// 			cerr << "Let's update board\n";
	// 			cerr << "moves: " << (all_valid_moves[index].second).first << " " << (all_valid_moves[index].second).second << endl;
	// 			update_board(board, local_trails, 1, initial_x, initial_y, (all_valid_moves[index].second).first, (all_valid_moves[index].second).second, true);
	// 			cerr << "update done\n";
	// 			// mov movet1, movet2, coord1, coord2, coord3, coord4;
	// 			// movet1.move_t = "S"; movet2.move_t = "M";
	// 			// coord1.coord = initial_x; coord2.coord = initial_y;
	// 			// coord3.coord = (all_valid_moves[index].second).first; coord4.coord = (all_valid_moves[index].second).second;

	// 			moves.push_back(1);
	// 			moves.push_back(initial_x);
	// 			moves.push_back(initial_y);
	// 			moves.push_back(2);
	// 			moves.push_back((all_valid_moves[index].second).first);
	// 			moves.push_back((all_valid_moves[index].second).second);
	// 			my_ring_pos[ring].first = (all_valid_moves[index].second).first;
	// 			my_ring_pos[ring].second = (all_valid_moves[index].second).second;
				
	// 			cerr << "Let's remove trails\n";
	// 			remove_trails_and_rings(board, my_trails, moves);
	// 			cerr << "Trails removed\n";
	// 		}else{
	// 			make_next_move(board, moves);
	// 		}
	// 	}
	}
}

void player::place_rings(vector<vector<pos>>& board, vector<pair<int,int>>& local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<int,pair<pair<int,int>,pair<int,int>>>>& moves){
	uniform_int_distribution<> distr(0, board_size-1); // define the range
	int x = distr(eng);
	int y = distr(eng);
	// pair<int,int> coords = my_coord_to_board(x,y,num_rings);
	if((board[x][y].valid == true) && (board[x][y].ring == 2)){
		// board[x][y].set(2,true);
		update_board(board, local_ring_pos, local_trails, 0, x, y, max_lim_p, max_lim_p, true);
		// mov movet, coord1, coord2;
		// movet.move_t = "P";
		// coord1.coord = x;
		// coord2.coord = y;
		// pair <int, pair<int,int>,pair<int,int>> p = make_pair(0,make_pair(make_pair(x,y),make_pair(max_lim_p,max_lim_p)));
		moves.push_back(make_pair(0,make_pair(make_pair(x,y),make_pair(max_lim_p,max_lim_p))));
		// moves.push_back(0);
		// moves.push_back(x);
		// moves.push_back(y);
		// my_ring_pos.push_back(make_pair(x, y));
		// board[coords.first][coords.second].set(2, true);
	}else{
		place_rings(board, local_ring_pos, local_trails, moves);
	}
}

void player::get_valid_moves(vector<vector<pos>>& board, vector<pair<int,int>>& local_ring_pos, vector<pair<int,pair<int,int>>>& all_valid_moves, int ring_index, bool my_turn){
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
		// if(my_turn){
		init_x = local_ring_pos[ring_index].first;
		init_y = local_ring_pos[ring_index].second;
		// }else{
		// 	init_x = opp_ring_pos[ring_index].first;
		// 	init_y = opp_ring_pos[ring_index].second;
		// }
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


void player::update_board(vector<vector<pos>>& board, vector<pair<int,int>>& local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], int action_on_ring, int initial_x, int initial_y, int final_x, int final_y, bool my_turn){	
	int player;
	if(my_turn){
		player = id;
	}else{
		player = 1-id;
	}

	if(action_on_ring == 0){
		pair<int,int> temp;
		board[initial_x][initial_y].set(2, player);
		// if(!my_turn){
			temp.first = initial_x;
			temp.second = initial_y;
			local_ring_pos.push_back(temp);
		// }else{
			// temp.first = initial_x;
			// temp.second = initial_y;
			// my_ring_pos.push_back(temp);
		// }		
	}else if(action_on_ring == 1){
		board[initial_x][initial_y].set(player, 2);
		cerr << "Flip Markers Start\n";
		cerr << "local_trails size before flip: " << local_trails[0].size() << " " << local_trails[1].size() << " " << local_trails[2].size() << " \n"; 
		flip_markers(board, local_trails, initial_x, initial_y, final_x, final_y, my_turn);		
		cerr << "local_trails size after flip: " << local_trails[0].size() << " " << local_trails[1].size() << " " << local_trails[2].size() << " \n"; 
		cerr << "Flip end\n";

		board[final_x][final_y].set(2, player);

		for(int i = 0; i<num_rings; i++){
			if((local_ring_pos[i].first == initial_x) || (local_ring_pos[i].second == initial_y)){
				local_ring_pos[i].first = final_x;
				local_ring_pos[i].second = final_y;
				break;
			}
		}
	}else if(action_on_ring == 2){
		remove_markers(board, local_trails, initial_x, initial_y, final_x, final_y, my_turn);
	}else if(action_on_ring == 3){
		board[initial_x][initial_y].set(2, 2);
		// if(!my_turn){
			int len = opp_ring_pos.size();
			for(int i = 0; i<len; i++){
				bool a1 = local_ring_pos[i].first == initial_x;
				bool a2 = local_ring_pos[i].second == initial_y;
				if(a1 && a2){
					local_ring_pos.erase(local_ring_pos.begin() + i);
				}
			}
		// }else{
		// 	int len = my_ring_pos.size();
		// 	for(int i = 0; i<len; i++){
		// 		bool a1 = my_ring_pos[i].first == initial_x;
		// 		bool a2 = my_ring_pos[i].second == initial_y;
		// 		if(a1 && a2){
		// 			my_ring_pos.erase(my_ring_pos.begin() + i);
		// 		}
		// 	}
		// }
	}else{
		place_markers(board, local_trails, initial_x, initial_y, final_x, final_y, my_turn);
	}
}


// Won't flip the marker at x2, y2 and x1,y1
void player::flip_markers(vector<vector<pos>>& board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], int x1, int y1, int x2, int y2, bool my_turn){
	int startX,startY,endX,endY;

	if(x1 == x2){
		startY = min(y1, y2);
		endY = max(y1, y2);
		check_my_trail(board, local_trails, x1, y1, 0, my_turn);
		check_my_trail(board, local_trails, x1, y1, 2, my_turn);
		for(int i = startY+1; i< endY; i++){
			if(board[x1][i].marker != 2){
				board[x1][i].set(1-board[x1][i].marker,2);
				check_my_trail(board, local_trails, x1, i, 0, !my_turn);
				check_my_trail(board, local_trails, x1, i, 2, !my_turn);
				check_my_trail(board, local_trails, x1, i, 0, my_turn);
				check_my_trail(board, local_trails, x1, i, 2, my_turn);
			}
		}
		check_my_trail(board, local_trails, x1, y1, 1, my_turn);
		check_my_trail(board, local_trails, x1, y1, 1, !my_turn);
	}else if(y1 == y2){
		startX = min(x1, x2);
		endX = max(x1, x2);
		check_my_trail(board, local_trails, x1, y1, 1, my_turn);
		check_my_trail(board, local_trails, x1, y1, 2, my_turn);
		for(int i = startX+1; i< endX; i++){
			if(board[i][y1].marker != 2){
				board[i][y1].set(1-board[i][y1].marker,2);
				check_my_trail(board, local_trails, i, y1, 1, !my_turn);
				check_my_trail(board, local_trails, i, y1, 2, !my_turn);
				check_my_trail(board, local_trails, i, y1, 1, my_turn);
				check_my_trail(board, local_trails, i, y1, 2, my_turn);
			}
		}		
		check_my_trail(board, local_trails, x1, y1, 0, !my_turn);
		check_my_trail(board, local_trails, x1, y1, 0, my_turn);
	}else{
		startX = min(x1, x2);
		startY = min(y1, y2);
		endX = max(x1, x2);
		endY = max(y1, y2);
		check_my_trail(board, local_trails, x1, y1, 0, my_turn);
		check_my_trail(board, local_trails, x1, y1, 1, my_turn);
		for(int i = startX+1, j=startY+1; i< endX && j<endY ; i++,j++){
			if(board[i][j].marker != 2){
				board[i][j].set(1-board[i][j].marker,2);
				check_my_trail(board, local_trails, i, j, 0, !my_turn);
				check_my_trail(board, local_trails, i, j, 1, !my_turn);
				check_my_trail(board, local_trails, i, j, 0, my_turn);
				check_my_trail(board, local_trails, i, j, 1, my_turn);
			}
		}
		check_my_trail(board, local_trails, x1, y1, 2, !my_turn);
		check_my_trail(board, local_trails, x1, y1, 2, my_turn);
	}	
}
void player::remove_markers(vector<vector<pos>>& board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], int x1, int y1, int x2, int y2, bool my_turn){
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

	// if(my_turn){
		int len = local_trails[dir].size();
		for(int i = 0; i<len;i++){
			bool a1 = (local_trails[dir][i].first).first == x1;
			bool a2 = (local_trails[dir][i].first).second == y1;
			bool a3 = (local_trails[dir][i].second).first == x2;
			bool a4 = (local_trails[dir][i].second).second == y2;
			if(a1 && a2 && a3 && a4){
				local_trails[dir].erase(local_trails[dir].begin()+i);
			}
		}
	// }else{
	// 	int len = opp_trails[dir].size();
	// 	for(int i = 0; i<len;i++){
	// 		bool a1 = (opp_trails[dir][i].first).first == x1;
	// 		bool a2 = (opp_trails[dir][i].first).second == y1;
	// 		bool a3 = (opp_trails[dir][i].second).first == x2;
	// 		bool a4 = (opp_trails[dir][i].second).second == y2;
	// 		if(a1 && a2 && a3 && a4){
	// 			opp_trails[dir].erase(opp_trails[dir].begin()+i);
	// 		}
	// 	}		
	// }
}

void player::place_markers(vector<vector<pos>>& board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], int x1, int y1, int x2, int y2, bool my_turn){
int startX,startY,endX,endY, dir, player;
	if(my_turn){
		player = id;
	}else{
		player = 1-id;
	}

	if(x1 == x2){
		startY = min(y1, y2);
		endY = max(y1, y2);
		for(int i = startY; i<= endY; i++){
			board[x1][i].set(player,2);
		}
		dir = 1;
	}else if(y1 == y2){
		startX = min(x1, x2);
		endX = max(x1, x2);
		for(int i = startX; i<= endX; i++){
			board[i][y1].set(player,2);
		}
		dir = 0;		
	}else{
		startX = min(x1, x2);
		startY = min(y1, y2);
		endX = max(x1, x2);
		endY = max(y1, y2);
		for(int i = startX, j=startY; i<= endX && j<=endY ; i++,j++){
			board[i][j].set(player,2);
		}
		dir = 2;
	}

	// if(my_turn){
		pair<pair<int, int>, pair<int, int>> temp = make_pair(make_pair(x1,y1), make_pair(x2,y2));
		local_trails[dir].push_back(temp);
		// int len = local_trails[dir].size();
		// for(int i = 0; i<len;i++){
		// 	bool a1 = (local_trails[dir][i].first).first == x1;
		// 	bool a2 = (local_trails[dir][i].first).second == y1;
		// 	bool a3 = (local_trails[dir][i].second).first == x2;
		// 	bool a4 = (local_trails[dir][i].second).second == y2;
		// 	if(a1 && a2 && a3 && a4){
		// 		local_trails[dir].erase(local_trails[dir].begin()+i);
		// 	}
		// }
	// }else{
	// 	int len = opp_trails[dir].size();
	// 	for(int i = 0; i<len;i++){
	// 		bool a1 = (opp_trails[dir][i].first).first == x1;
	// 		bool a2 = (opp_trails[dir][i].first).second == y1;
	// 		bool a3 = (opp_trails[dir][i].second).first == x2;
	// 		bool a4 = (opp_trails[dir][i].second).second == y2;
	// 		if(a1 && a2 && a3 && a4){
	// 			opp_trails[dir].erase(opp_trails[dir].begin()+i);
	// 		}
	// 	}		
	// }
}

void player::check_my_trail(vector<vector<pos>>& board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], int x1, int y1, int dir, bool my_turn){
	int count = 0; int count2 = 0;
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
					count2 = count;
					break;				
				}
				count = 0;
			}else{			
				if(count >= trail_length){
					tempMade = true;
					(temp.second).first = i-step_x;
					(temp.second).second = j-step_y;
					count2 = count;
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

	if(count2 == trail_length){
		// if(my_turn){
			local_trails[dir].push_back(temp);
		// }else{
			// local_trails[dir].push_back(temp);
		// }
	}else if(count2 > trail_length){
		(temp2.second).first = (temp.second).first;
		(temp2.second).second = (temp.second).second;
		(temp2.first).first = (temp2.second).first - step_x*(trail_length-1);
		(temp2.first).second = (temp2.second).second - step_y*(trail_length-1);

		(temp.second).first = (temp.first).first + step_x*(trail_length-1);
		(temp.second).second = (temp.first).second + step_y*(trail_length-1);
		// if(my_turn){
			// my_trails[dir].push_back(temp);
			// my_trails[dir].push_back(temp2);
		// }else{
			local_trails[dir].push_back(temp);
			local_trails[dir].push_back(temp2);
		// }		
	}
}

void player::remove_trails_and_rings(vector<vector<pos>>& board, vector<pair<int,int>>& local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<int>& moves){
	// For now its random
	pair<pair<int, int>, pair<int, int>> pp;
	bool flag = false;
	for(int i = 0; i< 3; i++){
		int len = local_trails[i].size();
		cerr << "Size of " << i << " is: " << len << endl; 
		for(int j = 0; j< len; j++){
			cerr << "Inside: " << (local_trails[i][j].first).first << endl;
			(pp.first).first = (local_trails[i][j].first).first;
			(pp.first).second = (local_trails[i][j].first).second;
			(pp.second).first = (local_trails[i][j].second).first;
			(pp.second).second = (local_trails[i][j].second).second;

			moves.push_back(3); moves.push_back((pp.first).first); moves.push_back((pp.first).second);
			moves.push_back(4); moves.push_back((pp.second).first); moves.push_back((pp.second).second);
			update_board(board, local_ring_pos, local_trails, 2, (local_trails[i][j].first).first, (local_trails[i][j].first).second, (local_trails[i][j].second).first, (local_trails[i][j].second).second, true);
			remove_ring(board, local_trails, local_ring_pos, moves);
			remove_repeated_trails(board, local_ring_pos, local_trails, pp, i);
			flag = true;
			break;
		}
		if(flag){
			break;
		}
	}

	bool a1 = (local_trails[0].size() != 0);
	bool a2 = (local_trails[1].size() != 0);
	bool a3 = (local_trails[2].size() != 0);
	if((a1 || a2 || a3) && (local_ring_pos.size() > num_rings-to_win_remove)){
		remove_trails_and_rings(board, local_ring_pos, local_trails, moves);
	}
}

void player::remove_ring(vector<vector<pos>>& board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<int,int>>& local_ring_pos, vector<int>& moves){
	moves.push_back(5); moves.push_back((local_ring_pos.back()).first); moves.push_back((local_ring_pos.back()).second);
	update_board(board, local_ring_pos, local_trails, 3, (local_ring_pos.back()).first, (local_ring_pos.back()).second, max_lim_p, max_lim_p, true);
}

void player::remove_repeated_trails(vector<vector<pos>>& board, vector<pair<int,int>>& local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], pair<pair<int, int>, pair<int, int>>& pp, int dir){
	int x1, x2, y1, y2, len;
	x1 = (pp.first).first;
	y1 = (pp.first).second;
	x2 = (pp.second).first;
	y2 = (pp.second).second;
	

	vector<pair<int,int>> expanded_pp;
	//get pp size
	int size_pp = 1+ (pp.second).second - (pp.first).second;
	if(size_pp==1){
		size_pp =  1+ (pp.second).first - (pp.first).first;
	}
	//expand pp
	for(int i=0;i<size_pp;i++){
		if (dir==0) expanded_pp.push_back(make_pair((pp.first).first+i,(pp.first).second));
		else if(dir==1) expanded_pp.push_back(make_pair((pp.first).first,(pp.first).second+i));
		else expanded_pp.push_back(make_pair((pp.first).first+i,(pp.first).second+i));
	}

	for(int i=0;i<3;i++){
		len = local_trails[i].size();
		for(int j=0;j<len;j++){
			//get tmp size
			int size_tmp =  1+ (local_trails[i][j].second).second - (local_trails[i][j].first).second;
			if(size_tmp==1){
				size_tmp =  1+ (local_trails[i][j].second).first - (local_trails[i][j].first).first;
			}
			//expand tmp
			vector<pair<int,int>> expanded_tmp;
			for(int k=0;i<size_tmp;i++){
				if (i==0) expanded_tmp.push_back(make_pair((local_trails[i][j].first).first+k,(local_trails[i][j].first).second));
				else if (i==1) expanded_tmp.push_back(make_pair((local_trails[i][j].first).first,(local_trails[i][j].first).second+k));
				else expanded_tmp.push_back(make_pair((local_trails[i][j].first).first+k,(local_trails[i][j].first).second+k));
			}
			//check for matches
			bool flag = false;
			for(int k=0;k<size_pp;k++){
				for(int l=0;l<size_tmp;l++){
					if(expanded_pp[k].first==expanded_tmp[l].first && expanded_pp[k].second==expanded_tmp[l].second){
						flag = true;
						break;
					}
				}
				if(flag) break;
			}
			if(flag) local_trails[i].erase(local_trails[i].begin() + j);
		}
	}








	// if(dir == 0){
	// 	len = my_trails[0].size();
	// 	for(int i = 0; i< len; i++){
	// 		bool a1 = ((pp.first).second == (my_trails[0][i].first).second) && ((pp.first).first >= (my_trails[0][i].first).first) && ((pp.first).first <= (my_trails[0][i].second).first);
	// 		bool a2 = ((pp.first).second == (my_trails[0][i].first).second) && ((pp.second).first >= (my_trails[0][i].first).first) && ((pp.second).first <= (my_trails[0][i].second).first);
	// 		if(a1 || a2){
	// 			my_trails[0].erase(my_trails[0].begin() + i);
	// 		}
	// 	}
	// 	len = my_trails[1].size();
	// 	for(int i = 0; i< len; i++){
	// 		bool a1 = ((pp.first).second >= (my_trails[1][i].second).second) && ((pp.first).second <= (my_trails[1][i].second).second);
	// 		bool a1 = ((pp.first).second 
	// 		if(a1){
	// 			my_trails[1].erase(my_trails[1].begin() + i);
	// 		}
	// 	}	

	// 	///////////////////////////////// Get eq of line and find if both coordinates lie on opposite side

	// 	// len = my_trails[2].size();
	// 	// for(int i = 0; i< len; i++){
	// 	// 	bool a1 = ((pp.first).second >= (my_trails[i].second).first) && ((pp.first).second <= (my_trails[i].second).second);
	// 	// 	if(a1){
	// 	// 		my_trails[2].erase(my_trails[2].begin() + i);
	// 	// 	}
	// 	// }
	// }else if(dir == 1){
	// 	len = my_trails[1].size();
	// 	for(int i = 0; i< len; i++){
	// 		bool a1 = ((pp.first).second >= (my_trails[1][i].first).second) && ((pp.first).second <= (my_trails[1][i].second).second);
	// 		bool a2 = ((pp.second).second >= (my_trails[1][i].first).second) && ((pp.second).second <= (my_trails[1][i].second).second);
	// 		if(a1 || a2){
	// 			my_trails[1].erase(my_trails[1].begin() + i);
	// 		}
	// 	}
	// 	len = my_trails[0].size();
	// 	for(int i = 0; i< len; i++){
	// 		bool a1 = ((pp.second).second >= (my_trails[0][i].second).second) && ((pp.first).second <= (my_trails[0][i].second).second);
	// 		if(a1){
	// 			my_trails[0].erase(my_trails[0].begin() + i);
	// 		}
	// 	}	

	// 	///////////////////////////////// Get eq of line and find if both coordinates lie on opposite side

	// 	// len = my_trails[2].size();
	// 	// for(int i = 0; i< len; i++){
	// 	// 	bool a1 = ((pp.first).second >= (my_trails[i].second).first) && ((pp.first).second <= (my_trails[i].second).second);
	// 	// 	if(a1){
	// 	// 		my_trails[2].erase(my_trails[2].begin() + i);
	// 	// 	}
	// 	// }
	// }
	// else {

	// }
}