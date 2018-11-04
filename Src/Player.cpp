#include "Utils.h"
#include "Player.h"
int max_lim_p = numeric_limits<int>::max();
int min_lim_p = numeric_limits<int>::min();

random_device rd; // obtain a random number from hardware
mt19937 eng(rd()); // seed the generator
// Driver function to sort the vector elements by  
// first element of pair in descending order 
static bool sortinrev(const pair<float,vector<pair<int, pair<pair<int,int>,pair<int,int>>>>> &a, const pair<float,vector<pair<int, pair<pair<int,int>,pair<int,int>>>>> &b) { 
       return (a.first > b.first); 
} 

player::player(){

}
player::player(int numr, int idd, int tl, int win, clock_t tm, double ti){
	num_rings_placed = 0;
	num_rings_removed = 0;
	num_rings = numr;
	DEPTH_TO_CHECK = 1;
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
	move_number = 0;
	start_time = tm;
	full_time = ti;
	// w1, w2, w3, w4, w5, wt_ctg, w6;
	wt.push_back(100);
	wt.push_back(100);
	wt.push_back(100);
	wt.push_back(100);
	wt.push_back(100);
	wt.push_back(100);
	wt.push_back(100);
	prev_reward = -1;
	curr_reward = -1;
	
	// w1 = 20000000.0;
	// w2 = 100000000.0;
	// w4 = 0;
	// if(move_number<8){
	// 	w6 = 10;
	// 	w3 = 300;
	// 	w5 = 50;
	// 	wt_ctg = 300;
	// }else{
	// 	w6 = 0;
	// 	w3 = 200;
	// 	w5 = 0;
	// 	wt_ctg = 10;
	// }


	// w3 = 300.0;
	// w5 = 0;
	// w6 = 0;
	// wt_ctg = 300;

	// w1 = 100000;
	// w2 = 100000000;
	// w3 = 100.0;
	// w4 = 100;
	// w5 = 100;
	// w6 = 100;
	// wt_ctg = 100;

	//initialize last_board
	// for(int i=0;i<board_size;i++){
	// 	vector<pos> tmp;
	// 	for(int j=0;j<board_size;j++){
	// 		pos temp_pos(2, 2, true);
	// 		tmp.push_back(temp_pos);
	// 	}
	// 	last_board.push_back(tmp);
	// }

}

player::player(int numr, int idd, int tl, int win, clock_t tm, double ti, string filename, string filename2){
	rd_wt_filename = filename + ".txt";
	wt_filename = filename2 + ".txt";

	num_rings_placed = 0;
	num_rings_removed = 0;
	num_rings = numr;
	DEPTH_TO_CHECK = 1;
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
	move_number = 0;
	start_time = tm;
	full_time = ti;
	// w1, w2, w3, w4, w5, wt_ctg, w6;
	wt.push_back(100);
	wt.push_back(100);
	wt.push_back(100);
	wt.push_back(100);
	wt.push_back(100);
	wt.push_back(100);
	wt.push_back(100);
	prev_reward = -1;
	curr_reward = -1;
	
	// w1 = 20000000.0;
	// w2 = 100000000.0;
	// w4 = 0;
	// if(move_number<8){
	// 	w6 = 10;
	// 	w3 = 300;
	// 	w5 = 50;
	// 	wt_ctg = 300;
	// }else{
	// 	w6 = 0;
	// 	w3 = 200;
	// 	w5 = 0;
	// 	wt_ctg = 10;
	// }


	// w3 = 300.0;
	// w5 = 0;
	// w6 = 0;
	// wt_ctg = 300;

	// w1 = 100000;
	// w2 = 100000000;
	// w3 = 100.0;
	// w4 = 100;
	// w5 = 100;
	// w6 = 100;
	// wt_ctg = 100;

	//initialize last_board
	// for(int i=0;i<board_size;i++){
	// 	vector<pos> tmp;
	// 	for(int j=0;j<board_size;j++){
	// 		pos temp_pos(2, 2, true);
	// 		tmp.push_back(temp_pos);
	// 	}
	// 	last_board.push_back(tmp);
	// }

}

void player::get_all_removes(vector<vector<pos>>& local_board, vector<pair<int,pair<pair<int,int>,pair<int,int>>>>& one_remove, vector<vector<pair<int,pair<pair<int,int>,pair<int,int>>>>>& valid_removes, vector<pair<int,int>> local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<pair<int, int>, pair<int, int>>> non_local_trails[3], bool my_turn){
	////cerr << "In get all removes\n";
	bool flag = false;
	// //////cerr << "Local trails size: " << local_trails[i].size() << endl;;
	for(int i = 0; i<3; i++){
		for(int j = 0; j< local_trails[i].size(); j++){
			vector<pair<pair<int, int>, pair<int, int>>> temp_trails[3];
			copy(local_trails[0].begin(), local_trails[0].end(), back_inserter(temp_trails[0]));
			copy(local_trails[1].begin(), local_trails[1].end(), back_inserter(temp_trails[1]));
			copy(local_trails[2].begin(), local_trails[2].end(), back_inserter(temp_trails[2]));
			vector<pair<pair<int, int>, pair<int, int>>> non_temp_trails[3];
			copy(non_local_trails[0].begin(), non_local_trails[0].end(), back_inserter(non_temp_trails[0]));
			copy(non_local_trails[1].begin(), non_local_trails[1].end(), back_inserter(non_temp_trails[1]));
			copy(non_local_trails[2].begin(), non_local_trails[2].end(), back_inserter(non_temp_trails[2]));

			vector<pair<int, int>> temp_ring_pos;
			copy(local_ring_pos.begin(), local_ring_pos.end(), back_inserter(temp_ring_pos));
		


			// ////cerr << "Copy Done\n";
			pair<pair<int, int>, pair<int, int>> pp = make_pair(make_pair((temp_trails[i][j].first).first, (temp_trails[i][j].first).second), make_pair((temp_trails[i][j].second).first, (temp_trails[i][j].second).second));
			one_remove.push_back(make_pair(2,make_pair(make_pair((temp_trails[i][j].first).first, (temp_trails[i][j].first).second), make_pair((temp_trails[i][j].second).first, (temp_trails[i][j].second).second))));
			//////cerr << "one_remove_size: "<< one_remove.size() << endl;

			update_board(local_board, temp_ring_pos, temp_trails, non_temp_trails, 2, (temp_trails[i][j].first).first, (temp_trails[i][j].first).second, (temp_trails[i][j].second).first, (temp_trails[i][j].second).second, my_turn);

			//////cerr << "Markers removed\n";
			// temp_trails[i].erase(temp_trails[i].begin()+j);
			//////cerr << "Let's remove repeated trails\n";
			remove_repeated_trails(local_board, temp_ring_pos, temp_trails, pp, i);

			//////cerr << "repeated trails gone\n";
			for(int k = 0; k<temp_ring_pos.size(); k++){
				////cerr << "Let's see which ring we are talking about: " << temp_ring_pos[k].first << " " << temp_ring_pos[k].second << endl;
				// All 3 necessary copies...
				vector<pair<pair<int, int>, pair<int, int>>> temp_trails2[3];
				//////cerr << "Copy temp trails\n";
				copy(temp_trails[0].begin(), temp_trails[0].end(), back_inserter(temp_trails2[0]));
				copy(temp_trails[1].begin(), temp_trails[1].end(), back_inserter(temp_trails2[1]));
				copy(temp_trails[2].begin(), temp_trails[2].end(), back_inserter(temp_trails2[2]));

				vector<pair<pair<int, int>, pair<int, int>>> non_temp_trails2[3];
				copy(non_temp_trails[0].begin(), non_temp_trails[0].end(), back_inserter(non_temp_trails2[0]));
				copy(non_temp_trails[1].begin(), non_temp_trails[1].end(), back_inserter(non_temp_trails2[1]));
				copy(non_temp_trails[2].begin(), non_temp_trails[2].end(), back_inserter(non_temp_trails2[2]));

				vector<pair<int, int>> temp_ring_pos2;
				copy(temp_ring_pos.begin(), temp_ring_pos.end(), back_inserter(temp_ring_pos2));
		

				// temp_trails2[0] = temp_trails[0];
				// temp_trails2[1] = temp_trails[1];
				// temp_trails2[2] = temp_trails[2];
				//////cerr << "Let's remove rings\n";
				one_remove.push_back(make_pair(3,make_pair(make_pair((temp_ring_pos2[k]).first, (temp_ring_pos2[k]).second), make_pair(max_lim_p,max_lim_p))));
				update_board(local_board, temp_ring_pos2, temp_trails2, non_temp_trails2, 3, (temp_ring_pos2[k]).first, (temp_ring_pos2[k]).second, max_lim_p, max_lim_p, my_turn);
				//////cerr << "rings removed!!!\n";
				//////cerr << "size after rings removed!!!: " << one_remove.size() << endl;
				if(((temp_trails2[0].size() != 0) || (temp_trails2[1].size() != 0) || (temp_trails2[2].size() != 0)) && (temp_ring_pos2.size() > num_rings-to_win_remove)){
					////cerr << "Going into recursion\n";
					get_all_removes(local_board, one_remove, valid_removes, temp_ring_pos2, temp_trails2, non_temp_trails2, my_turn);
					//////cerr << "Go back to place markers\n";
					// update_board(local_board, temp_ring_pos2, temp_trails2, non_temp_trails2, 4, ((one_remove[one_remove.size()-1].second).first).first, ((one_remove[one_remove.size()-1].second).first).second, ((one_remove[one_remove.size()-1].second).second).first, ((one_remove[one_remove.size()-1].second).second).second, my_turn);
					// one_remove.erase(one_remove.size()-1);
					one_remove.erase(one_remove.end()-1);
					// break;
				}else{
					////cerr << "May go back now\n";
					vector<pair<int,pair<pair<int,int>,pair<int,int>>>> to_revert;
					to_revert.push_back(one_remove[one_remove.size()-1]);
					revert(local_board, temp_trails2, non_temp_trails2, temp_ring_pos2, to_revert, my_turn);
					//////cerr << "Size of one\n";
					valid_removes.push_back(one_remove);
					one_remove.erase(one_remove.end()-1);
				}
			}
			update_board(local_board, temp_ring_pos, temp_trails, non_temp_trails, 4, ((one_remove[one_remove.size()-1].second).first).first, ((one_remove[one_remove.size()-1].second).first).second, ((one_remove[one_remove.size()-1].second).second).first, ((one_remove[one_remove.size()-1].second).second).second, my_turn);		
			one_remove.erase(one_remove.end()-1);
		}
	}
}

void player::get_neighbours(bool ascending, vector<vector<pos>> local_board, vector<pair<int,int>> local_ring_pos, vector<pair<int,int>> non_local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<pair<int, int>, pair<int, int>>> non_local_trails[3], vector<pair<float, vector<pair<int, pair<pair<int,int>,pair<int,int>>>>>>& move, bool my_turn){
	////cerr << "In get neighbours\n";
	// Get all remove markers and rings
	// For all such removes go ahead and make all possible moves
	// vector<vector<pos>> local_board = board;
	vector<vector<pair<int,pair<pair<int,int>,pair<int,int>>>>> valid_removes;
	// vector<pair<int,pair<pair<int,int>,pair<int,int>>>> valid_step;
	pair<pair<int, int>, pair<int, int>> pp;
	
	// vector<pair<float, vector<int, pair<pair<int,int>,pair<int,int>>>>> move
	// int siz = local_trails[0].size() + local_trails[1].size() + local_trails[2].size();
	vector<pair<int,pair<pair<int,int>,pair<int,int>>>> one_remove;
	// //////cerr << "Inside Get neighbours: " << local_trails[0].size() << " " << ;
	get_all_removes(local_board, one_remove, valid_removes, local_ring_pos, local_trails, non_local_trails, my_turn);// Gets all possible remove sequences if exist

	////cerr << "Size of valid_removes:----------------------- " << valid_removes.size() << endl;
	if(valid_removes.size() > 0){
		//////cerr << "Why is it here?\n";
		for(int i = 0; i < valid_removes.size(); i++){
			////cerr << "in get neighbours in valide remove loop: " << valid_removes.size() << " " << i << endl;
			
			vector<pair<pair<int, int>, pair<int, int>>> temp_trails2[3];
			copy(local_trails[0].begin(), local_trails[0].end(), back_inserter(temp_trails2[0]));
			copy(local_trails[1].begin(), local_trails[1].end(), back_inserter(temp_trails2[1]));
			copy(local_trails[2].begin(), local_trails[2].end(), back_inserter(temp_trails2[2]));
			vector<pair<pair<int, int>, pair<int, int>>> non_temp_trails2[3];
			copy(non_local_trails[0].begin(), non_local_trails[0].end(), back_inserter(non_temp_trails2[0]));
			copy(non_local_trails[1].begin(), non_local_trails[1].end(), back_inserter(non_temp_trails2[1]));
			copy(non_local_trails[2].begin(), non_local_trails[2].end(), back_inserter(non_temp_trails2[2]));

			vector<pair<int, int>> temp_ring_pos2;
			copy(local_ring_pos.begin(), local_ring_pos.end(), back_inserter(temp_ring_pos2));

			play_move(local_board, valid_removes[i], temp_ring_pos2, temp_trails2, non_temp_trails2, my_turn);
			
			if(temp_ring_pos2.size() > num_rings-to_win_remove){
				for(int j = 0; j<temp_ring_pos2.size(); j++){
					vector<pair<int,pair<int,int>>> valid_moves;
					get_valid_moves(local_board, temp_ring_pos2, valid_moves, j, my_turn);
					////cerr << "Get Valid moves size: " << valid_moves.size() << " r_index: " << j << " coord: " << temp_ring_pos2[j].first << " " << temp_ring_pos2[j].second << endl; 
					// formulate_move(move, valid_removes, valid_moves);
					for(int k = 0; k < valid_moves.size(); k++){
						vector<pair<pair<int, int>, pair<int, int>>> temp_trails[3];
						copy(temp_trails2[0].begin(), temp_trails2[0].end(), back_inserter(temp_trails[0]));
						copy(temp_trails2[1].begin(), temp_trails2[1].end(), back_inserter(temp_trails[1]));
						copy(temp_trails2[2].begin(), temp_trails2[2].end(), back_inserter(temp_trails[2]));
						vector<pair<pair<int, int>, pair<int, int>>> non_temp_trails[3];
						copy(non_temp_trails2[0].begin(), non_temp_trails2[0].end(), back_inserter(non_temp_trails[0]));
						copy(non_temp_trails2[1].begin(), non_temp_trails2[1].end(), back_inserter(non_temp_trails[1]));
						copy(non_temp_trails2[2].begin(), non_temp_trails2[2].end(), back_inserter(non_temp_trails[2]));
						vector<pair<int, int>> temp_ring_pos;
						copy(temp_ring_pos2.begin(), temp_ring_pos2.end(), back_inserter(temp_ring_pos));

						// temp_trail[0] = local_trails[0];
						// temp_trail[1] = local_trails[1];
						// temp_trail[2] = local_trails[2];

						int num_moves = 0;
						play(local_board, temp_trails, non_temp_trails, temp_ring_pos, non_local_ring_pos, move, j, valid_moves[k], my_turn, num_moves);
						concatenate(valid_removes[i], move, num_moves);
						// revert(local_board, temp_trail, rem_rings, local_ring_pos, move[move.size()-1], valid_moves[k], my_turn);
					}
					// revert(local_board, temp_trail, rem_rings, local_ring_pos, move[move.size()-1], valid_moves[k], my_turn);
				}
			}else{
				float h; 
				if(temp_ring_pos2.size() <= (num_rings-to_win_remove)){
					h = max_lim_p;
				}else{
					if(my_turn){
						vector<float> fi;
						fi = heuristic(local_board, my_turn, local_trails, non_local_trails, local_ring_pos, non_local_ring_pos);
						h = wt[0]*fi[0]+wt[1]*fi[1]+wt[2]*fi[2]+wt[3]*fi[3]+wt[4]*fi[4]+wt[5]*fi[5]+wt[6]*fi[6];
					}
					else{
						vector<float> fi;
						fi = heuristic(local_board, my_turn, local_trails, non_local_trails, local_ring_pos, non_local_ring_pos);
						h = wt[0]*fi[0]+wt[1]*fi[1]+wt[2]*fi[2]+wt[3]*fi[3]+wt[4]*fi[4]+wt[5]*fi[5]+wt[6]*fi[6];
					}
				}
				move.push_back(make_pair(h, valid_removes[i]));	
				revert(local_board, temp_trails2, non_temp_trails2, temp_ring_pos2, valid_removes[i], my_turn);
				break;
			}		
			revert(local_board, temp_trails2, non_temp_trails2, temp_ring_pos2, valid_removes[i], my_turn);
		}
	}else{
		//////cerr << "No valid remove\n";
		for(int j = 0; j<local_ring_pos.size(); j++){
			vector<pair<int,pair<int,int>>> valid_moves;
			get_valid_moves(local_board, local_ring_pos, valid_moves, j, my_turn);
			////cerr << "Get Valid moves size: " << valid_moves.size() << " r_index: " << j << " coord: " << local_ring_pos[j].first << " " << local_ring_pos[j].second << endl; 		
			
			for(int k = 0; k < valid_moves.size(); k++){
				vector<pair<pair<int, int>, pair<int, int>>> temp_trails[3];
				
				////cerr << "Size of local_trails before play: " << local_trails[0].size() << " " << local_trails[1].size() << " " << local_trails[2].size() << " \n";
				
				copy(local_trails[0].begin(), local_trails[0].end(), back_inserter(temp_trails[0]));
				copy(local_trails[1].begin(), local_trails[1].end(), back_inserter(temp_trails[1]));
				copy(local_trails[2].begin(), local_trails[2].end(), back_inserter(temp_trails[2]));
				
				////cerr << "Size of temp_trails before play: " << temp_trails[0].size() << " " << temp_trails[1].size() << " " << temp_trails[2].size() << " \n";
				
				vector<pair<pair<int, int>, pair<int, int>>> non_temp_trails[3];
				copy(non_local_trails[0].begin(), non_local_trails[0].end(), back_inserter(non_temp_trails[0]));
				copy(non_local_trails[1].begin(), non_local_trails[1].end(), back_inserter(non_temp_trails[1]));
				copy(non_local_trails[2].begin(), non_local_trails[2].end(), back_inserter(non_temp_trails[2]));

				vector<pair<int, int>> temp_ring_pos;
				copy(local_ring_pos.begin(), local_ring_pos.end(), back_inserter(temp_ring_pos));

				// cout << "Size of temp_trails before play: " << local_trails[0].size << " " << local_trails[1].size << " " << local_trails[2].size << " \n";
				// temp_trail[0] = local_trails[0];
				// temp_trail[1] = local_trails[1];
				// temp_trail[2] = local_trails[2];
				int num_moves = 0;
				////cerr << "Let's play them: " << k << " valid_move: " << valid_moves[k].first << " " << valid_moves[k].second.first << " " << valid_moves[k].second.second << endl;
				play(local_board, temp_trails, non_temp_trails, temp_ring_pos, non_local_ring_pos, move, j, valid_moves[k], my_turn, num_moves);
				////cerr << "Size of temp_trails after play: " << temp_trails[0].size() << " " << temp_trails[1].size() << " " << temp_trails[2].size() << " \n";
				//////cerr << "Size of local_trails after play: " << local_trails[0].size() << " " << local_trails[1].size() << " " << local_trails[2].size() << " \n";
				//////cerr << "Play over\n";
				// concatenate(valid_removes[i], move, num_moves);
				// revert(local_board, temp_trail, local_ring_pos, move, j, valid_moves[k], my_turn);
			}
		}
	}
	if(ascending){
		sort(move.begin(),move.end());	
	}else{
		sort(move.begin(),move.end(),sortinrev);	
	}
}

void player::revert(vector<vector<pos>>& local_board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<pair<int, int>, pair<int, int>>> non_local_trails[3], vector<pair<int,int>>& local_ring_pos, vector<pair<int, pair<pair<int,int>,pair<int,int>>>>& moves, bool my_turn){
	////cerr << "In recert\n";
	int start = moves.size() - 1;
	int player;
	if(my_turn){
		player = id;
	}else{
		player = 1-id;
	}

	for(int i = start; i>= 0; i--){
		////cerr << "Which revert? " << moves[i].first << endl;
		// ////cerr << "Start: " << start << " i: "<<i<< endl;

		if(moves[i].first == 3){
			update_board(local_board, local_ring_pos, local_trails, non_local_trails, 0, ((moves[i].second).first).first, ((moves[i].second).first).second, max_lim_p, max_lim_p, my_turn);
		}else if(moves[i].first == 2){
			update_board(local_board, local_ring_pos, local_trails, non_local_trails, 4, ((moves[i].second).first).first, ((moves[i].second).first).second, ((moves[i].second).second).first, ((moves[i].second).second).second, my_turn);
		}else if(moves[i].first == 1){
			update_board(local_board, local_ring_pos, local_trails, non_local_trails, 5, ((moves[i].second).second).first, ((moves[i].second).second).second, ((moves[i].second).first).first, ((moves[i].second).first).second, my_turn);
			// local_board[((moves[i].second).second).first][((moves[i].second).second).second].set(2,2);
		}else if(moves[i].first == 0){
			update_board(local_board, local_ring_pos, local_trails, non_local_trails, 3, ((moves[i].second).first).first, ((moves[i].second).first).second, ((moves[i].second).second).first, ((moves[i].second).second).second, my_turn);
			// local_board[((moves[i].second).second).first][((moves[i].second).second).second].set(2,2);
		}
	}
}
void player::concatenate(vector<pair<int,pair<pair<int,int>,pair<int,int>>>>& valid_removes, vector<pair<float, vector<pair<int, pair<pair<int,int>,pair<int,int>>>>>>& moves, int& num_moves){
	////cerr << "In Concatenate\n";
	for(int i = 0; i<num_moves; i++){
		vector<pair<int, pair<pair<int,int>,pair<int,int>>>> temp;
		temp = valid_removes;
		////cerr << "Iter: " << i << endl;
		temp.insert(temp.end(), (moves[moves.size()-i-1].second).begin(), (moves[moves.size()-i-1].second).end());
		////cerr << "Iterrrrrrrrr: " << i << endl;
		// copy(temp.begin(), temp.end(), back_inserter(moves[moves.size()-i-1].second));
		moves[moves.size()-i-1].second = temp;				
	}
}
// void player::concatenate(vector<pair<int,pair<pair<int,int>,pair<int,int>>>>& valid_removes, vector<pair<float, vector<pair<int, pair<pair<int,int>,pair<int,int>>>>>>& moves, int& num_moves){
// 	//////cerr << "In Concatenate\n";
// 	for(int i = 0; i<num_moves; i++){
// 		vector<pair<int, pair<pair<int,int>,pair<int,int>>>> temp;
// 		// temp = valid_removes;
// 		copy(valid_removes.begin(), valid_removes.end(), back_inserter(temp));
// 		temp.insert(temp.end(), moves[moves.size()-i-1].second.begin(), moves[moves.size()-i-1].second.end());
// 			// temp2.insert(temp2.end(), valid_removes[i].begin(), valid_removes[i].end());

// 		// copy(moves[moves.size()-i-1].second.begin(), moves[moves.size()-i-1].second.end(), back_inserter(temp));
// 		for(int j = 0; j<moves[moves.size()-i-1].second.size(); j++){
// 			moves[moves.size()-i-1].second[j] = temp[j];
// 		}
// 		for(int j = moves[moves.size()-i-1].second.size(); j<temp.size(); j++){
// 			moves[moves.size()-i-1].second.push_back(temp[j]);
// 		}
// 		//////cerr << "Iter: " << i << endl;
// 		// temp.insert(temp.end(), (moves[moves.size()-i-1].second).begin(), (moves[moves.size()-i-1].second).end());
// 		//////cerr << "Iterrrrrrrrr: " << i << endl;
// 		// copy(temp.begin(), temp.end(), back_inserter(moves[moves.size()-i-1].second));
// 		// moves[moves.size()-i-1].second = temp;				
// 	}
// }

void player::play(vector<vector<pos>>& local_board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<pair<int, int>, pair<int, int>>> non_local_trails[3], vector<pair<int,int>>& local_ring_pos, vector<pair<int,int>>& non_local_ring_pos, vector<pair<float, vector<pair<int, pair<pair<int,int>,pair<int,int>>>>>>& move, int ring_index, pair<int,pair<int,int>> valid_moves, bool my_turn, int& num_moves){
	// vector<pair<pair<int, int>, pair<int, int>>> temp_trails2[3];
	// copy(local_trails[0].begin(), local_trails[0].end(), back_inserter(temp_trails2[0]));
	// copy(local_trails[1].begin(), local_trails[1].end(), back_inserter(temp_trails2[1]));
	// copy(local_trails[2].begin(), local_trails[2].end(), back_inserter(temp_trails2[2]));

	////cerr << "Inside play\n";
	//////cerr << "Coordinates: " << local_ring_pos[ring_index].first << " " << local_ring_pos[ring_index].second << " " << (valid_moves.second).first << " " << (valid_moves.second).second << endl;
	vector<pair<int, pair<pair<int,int>,pair<int,int>>>> temp;
	temp.push_back(make_pair(1, make_pair(make_pair(local_ring_pos[ring_index].first,local_ring_pos[ring_index].second), make_pair((valid_moves.second).first,(valid_moves.second).second))));
	update_board(local_board, local_ring_pos, local_trails, non_local_trails, 1, local_ring_pos[ring_index].first, local_ring_pos[ring_index].second, (valid_moves.second).first, (valid_moves.second).second, my_turn);

	vector<vector<pair<int,pair<pair<int,int>,pair<int,int>>>>> valid_removes;
	vector<pair<int,pair<pair<int,int>,pair<int,int>>>> one_remove;
	//////cerr << "After move remove?\n";
	////cerr << "local_trails size: " << local_trails[0].size() << " " << local_trails[1].size() << " " << local_trails[2].size() << " \n"; 
	get_all_removes(local_board, one_remove, valid_removes, local_ring_pos, local_trails, non_local_trails, my_turn);
	
	for(int i = 0; i<valid_removes.size(); i++){
		for(int j = 0; j<valid_removes[i].size(); j++){
			////cerr << "Valid remove move selected: " << valid_removes[i].size()  << " Type: " << valid_removes[i][j].first << " coords: " << valid_removes[i][j].second.first.first << " " << valid_removes[i][j].second.first.second << " " << valid_removes[i][j].second.second.first << " " << valid_removes[i][j].second.second.second << "  ";
		}////cerr << endl;
	}////cerr << endl;
	
	////cerr << "***************************************************************************************************************\n";
	// vector<pair<pair<int, int>, pair<int, int>>> temp_trails2[3];
	// copy(local_trails[0].begin(), local_trails[0].end(), back_inserter(temp_trails2[0]));
	// copy(local_trails[1].begin(), local_trails[1].end(), back_inserter(temp_trails2[1]));
	// copy(local_trails[2].begin(), local_trails[2].end(), back_inserter(temp_trails2[2]));
	if(valid_removes.size() > 0){
		////cerr << "yes some removes\n";
		num_moves = valid_removes.size();
		// int len = valid_moves.size();
		for(int i = 0; i<num_moves; i++){
			////cerr << "In some remove loop\n";
			vector<pair<pair<int, int>, pair<int, int>>> temp_trails[3];
			copy(local_trails[0].begin(), local_trails[0].end(), back_inserter(temp_trails[0]));
			copy(local_trails[1].begin(), local_trails[1].end(), back_inserter(temp_trails[1]));
			copy(local_trails[2].begin(), local_trails[2].end(), back_inserter(temp_trails[2]));
			vector<pair<pair<int, int>, pair<int, int>>> non_temp_trails[3];
			copy(non_local_trails[0].begin(), non_local_trails[0].end(), back_inserter(non_temp_trails[0]));
			copy(non_local_trails[1].begin(), non_local_trails[1].end(), back_inserter(non_temp_trails[1]));
			copy(non_local_trails[2].begin(), non_local_trails[2].end(), back_inserter(non_temp_trails[2]));

			vector<pair<int, int>> temp_ring_pos;
			copy(local_ring_pos.begin(), local_ring_pos.end(), back_inserter(temp_ring_pos));


			// temp_trails[0] = local_trails[0];
			// temp_trails[1] = local_trails[1];
			// temp_trails[2] = local_trails[2];
			////cerr << "Play one remove \n";
			play_move(local_board, valid_removes[i], temp_ring_pos, temp_trails, non_temp_trails, my_turn);
			vector<pair<int, pair<pair<int,int>,pair<int,int>>>> temp2;
			temp2 = temp;
			temp2.insert(temp2.end(), valid_removes[i].begin(), valid_removes[i].end());
			// for(int j = 0; j<valid_removes[i].size(); j=j++){	
			// 	update_board(local_board, local_ring_pos, temp_trails, valid_removes[i][j].first, ((valid_removes[i][j].second).first).first, ((valid_removes[i][j].second).first).second, ((valid_removes[i][j].second).second).first, ((valid_removes[i][j].second).second).second, my_turn);
			// }
			////cerr << "after updating board\n";
			// remove_trails_and_rings(board, rem_rings, local_trails, moves);
			float h; 
			if(temp_ring_pos.size() <= (num_rings-to_win_remove)){
				h = max_lim_p;
			}else{
				if(my_turn){
					vector<float> fi;
					fi = heuristic(local_board, my_turn, local_trails, non_local_trails, local_ring_pos, non_local_ring_pos);
					h = wt[0]*fi[0]+wt[1]*fi[1]+wt[2]*fi[2]+wt[3]*fi[3]+wt[4]*fi[4]+wt[5]*fi[5]+wt[6]*fi[6];
				}
				else{
					vector<float> fi;
					fi = heuristic(local_board, my_turn, local_trails, non_local_trails, local_ring_pos, non_local_ring_pos);
					h = wt[0]*fi[0]+wt[1]*fi[1]+wt[2]*fi[2]+wt[3]*fi[3]+wt[4]*fi[4]+wt[5]*fi[5]+wt[6]*fi[6];
				}
			}
			move.push_back(make_pair(h, temp2));
			revert(local_board, temp_trails, non_temp_trails, temp_ring_pos, valid_removes[i], my_turn);
		}
	}else{
		num_moves = 1;
		////cerr << "no removes\n";
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
				// h = heuristic(local_board, my_turn, local_trails, non_local_trails);
			// }
			if(my_turn){
					vector<float> fi;
					fi = heuristic(local_board, my_turn, local_trails, non_local_trails, local_ring_pos, non_local_ring_pos);
					h = wt[0]*fi[0]+wt[1]*fi[1]+wt[2]*fi[2]+wt[3]*fi[3]+wt[4]*fi[4]+wt[5]*fi[5]+wt[6]*fi[6];
			}
			else{
				vector<float> fi;
				fi = heuristic(local_board, my_turn, local_trails, non_local_trails, local_ring_pos, non_local_ring_pos);
				h = wt[0]*fi[0]+wt[1]*fi[1]+wt[2]*fi[2]+wt[3]*fi[3]+wt[4]*fi[4]+wt[5]*fi[5]+wt[6]*fi[6];
			}

			move.push_back(make_pair(h, temp));
	}
	////cerr << "After all jhanjhat\n";
	revert(local_board, local_trails, non_local_trails, local_ring_pos, temp, my_turn);
}

void player::play_move(vector<vector<pos>>& local_board, vector<pair<int,pair<pair<int,int>,pair<int,int>>>>& moves, vector<pair<int,int>>& local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<pair<int, int>, pair<int, int>>> non_local_trails[3], bool my_turn){
	////cerr << "inside play_move\n";
	int len = moves.size();
	////cerr << "length of move: " << len << endl;
	for(int i = 0; i<len; i++){
		////cerr << "Coordinates: " << moves[i].first << " " << ((moves[i].second).first).first << " " << ((moves[i].second).first).second << " " << ((moves[i].second).second).first << " " << ((moves[i].second).second).second << endl;
		update_board(local_board, local_ring_pos, local_trails, non_local_trails, (moves[i].first), ((moves[i].second).first).first, ((moves[i].second).first).second, ((moves[i].second).second).first, ((moves[i].second).second).second, my_turn);
	}
}
float player::check_dominance(vector<vector<pos>> board, int dir, int xc, int yc, float wt1, float wt2, float wt3, float wt4){
	int step_x, step_y, startX, startY;
	int num_my_1 = 0, num_my_2 = 0, num_my_3 = 0, num_my_4 = 0, my_num_var = 0;
	int num_opp_1 = 0, num_opp_2 = 0, num_opp_3 = 0, num_opp_4 = 0, opp_num_var = 0;
	bool my_trail = false;
	bool opp_trail = false;

	if(dir == 0){
		startX = 0; startY = yc;
		step_x = 1; step_y = 0;
	}else if(dir == 1){
		startX = xc; startY = 0;		
		step_x = 0; step_y = 1;
	}else{
		startX = xc-min(xc,yc); startY = yc-min(xc,yc);
		step_x = 1; step_y = 1;		
	}

	for(int i = startX, j = startY; (i <board_size)&&(j<board_size); i += step_x, j += step_y){
		if(board[i][j].marker == id){
			my_num_var++;
			if(!my_trail){
				my_trail = true;
				opp_trail = false;
				if(opp_num_var > 0){
					if(opp_num_var == 1){
						num_opp_1++;
					}else if(opp_num_var == 2){
						num_opp_2++;
					}else if(opp_num_var == 3){
						num_opp_3++;
					}else if(opp_num_var == 4){
						num_opp_4++;
					}
					opp_num_var = 0;
				}
			}
		}else if(board[i][j].marker == 1-id){
			opp_num_var++;
			if(!opp_trail){
				my_trail = false;
				opp_trail = true;
				if(my_num_var > 0){
					if(my_num_var == 1){
						num_my_1++;
					}else if(my_num_var == 2){
						num_my_2++;
					}else if(my_num_var == 3){
						num_my_3++;
					}else if(my_num_var == 4){
						num_my_4++;
					}
					my_num_var = 0;
				}
			}
		}else{
			if(opp_trail){
				if(opp_num_var > 0){
					if(opp_num_var == 1){
						num_opp_1++;
					}else if(opp_num_var == 2){
						num_opp_2++;
					}else if(opp_num_var == 3){
						num_opp_3++;
					}else if(opp_num_var == 4){
						num_opp_4++;
					}
					opp_num_var = 0;
				}
			}else if(my_trail){
				if(my_num_var > 0){
					if(my_num_var == 1){
						num_my_1++;
					}else if(my_num_var == 2){
						num_my_2++;
					}else if(my_num_var == 3){
						num_my_3++;
					}else if(my_num_var == 4){
						num_my_4++;
					}
					my_num_var = 0;
				}
			}
			opp_trail = false;
			my_trail = false;
		}
	}		
	if(opp_trail){
		if(opp_num_var > 0){
			if(opp_num_var == 1){
				num_opp_1++;
			}else if(opp_num_var == 2){
				num_opp_2++;
			}else if(opp_num_var == 3){
				num_opp_3++;
			}else if(opp_num_var == 4){
				num_opp_4++;
			}
			opp_num_var = 0;
		}
	}else if(my_trail){
		if(my_num_var > 0){
			if(my_num_var == 1){
				num_my_1++;
			}else if(my_num_var == 2){
				num_my_2++;
			}else if(my_num_var == 3){
				num_my_3++;
			}else if(my_num_var == 4){
				num_my_4++;
			}
			my_num_var = 0;
		}
	}
	return wt1*(num_my_1 - num_opp_1) + wt2*(num_my_2 - num_opp_2) + wt3*(num_my_3 - num_opp_3) + wt4*(num_my_4 - num_opp_4);
}
float player::check_ring_adjacent_trails(vector<vector<pos>>& board, vector<pair<int,int>>& cur_rings){
	float cur_trails=0;
	for(int i=0;i<cur_rings.size();i++){
		vector<int> step_x_set;
		vector<int> step_y_set;
		step_x_set.push_back(-1);
		step_x_set.push_back(0);
		step_x_set.push_back(1);
		step_y_set.push_back(-1);
		step_y_set.push_back(0);
		step_y_set.push_back(1);
		for(int j=0;j<step_x_set.size();j++){
			for(int k=0;k<step_y_set.size();k++){
				int last_marker;
				int ct_trail=0;
				if((step_x_set[j]==0 && step_y_set[k]==0) || (step_x_set[j]==-1 && step_y_set[k]==1) || (step_x_set[j]==1 && step_y_set[k]==-1) ) continue;
				int new_x = cur_rings[i].first;
				int new_y = cur_rings[i].second;
				while(true){
					new_x +=step_x_set[j];
					new_y +=step_y_set[k];

					if(new_x<0 || new_x>board_size-1) break;
					if(new_y<0 || new_y>board_size-1) break;

					if(board[new_x][new_y].valid){
						if(ct_trail==0){
							ct_trail++;
							last_marker = board[new_x][new_y].marker;
						}
						else if(ct_trail==4){
							cur_trails+=pow(10,ct_trail-1);
							break;
						}
						else{
							if(board[new_x][new_y].marker==last_marker){
								ct_trail++;
							}else{
								cur_trails+=pow(10,ct_trail-1);
								break;
							}
						}
					}else {
						if (ct_trail>0)
							cur_trails+=pow(10,ct_trail-1);
						else
							break;
					}
				}
			}
		}
	}
	return cur_trails;
}
int player::check_ring_adjacent_empty(vector<vector<pos>>& board, vector<pair<int,int>>& cur_rings){
	int moves_avail = 0;
	for(int i=0;i<cur_rings.size();i++){
		vector<int> step_x_set;
		vector<int> step_y_set;
		step_x_set.push_back(-1);
		step_x_set.push_back(0);
		step_x_set.push_back(1);
		step_y_set.push_back(-1);
		step_y_set.push_back(0);
		step_y_set.push_back(1);
		for(int j=0;j<step_x_set.size();j++){
			for(int k=0;k<step_y_set.size();k++){
				int last_marker;
				if((step_x_set[j]==0 && step_y_set[k]==0) || (step_x_set[j]==-1 && step_y_set[k]==1) || (step_x_set[j]==1 && step_y_set[k]==-1) ) continue;
				int new_x = cur_rings[i].first;
				int new_y = cur_rings[i].second;
				while(true){
					new_x += step_x_set[j];
					new_y += step_y_set[k];
					if(new_x<0 || new_x>board_size-1) break;
					if(new_y<0 || new_y>board_size-1) break;

					if(board[new_x][new_y].valid){
						if(board[new_x][new_y].marker==2 && board[new_x][new_y].ring!=2)
							moves_avail++;
						else break;
					}else break;
				}
			}
		}
	}
	return moves_avail;

}
vector<float> player::heuristic(vector<vector<pos>>& board, bool my_turn, vector<pair<pair<int, int>, pair<int, int>>> my_cur_trails[3], vector<pair<pair<int, int>, pair<int, int>>> opp_cur_trails[3],vector<pair<int,int>>& my_cur_rings, vector<pair<int,int>>& opp_cur_rings){
	int num_my_rings = 0;
	int num_opp_rings = 0;
	int num_my_markers = 0;
	int num_opp_markers = 0;
	int num_my_3len_trails = 0;
	int num_opp_3len_trails = 0;
	float num_my_ring_adjacent_trail=0;
	float num_opp_ring_adjacent_trail=0;
	float my_dominance = 0;
	int num_my_free_moves=0;
	int num_opp_free_moves=0;

	float val1_ctg, val2_ctg, val3_ctg, val4_ctg;
	val1_ctg = 0.001;
	val2_ctg = 0.01;
	val3_ctg = 0.1;
	val4_ctg = 1;



	// w1 = 2000000.0;
	// w2 = 100000000.0;
	// w3 = 300.0;
	// w4 = 0;
	// w5 = 0;
	// w6 = 300;
	// wt_ctg = 100;

	num_my_ring_adjacent_trail = check_ring_adjacent_trails(board, my_cur_rings);
	num_opp_ring_adjacent_trail = check_ring_adjacent_trails(board, opp_cur_rings);
	num_my_free_moves = check_ring_adjacent_empty(board, my_cur_rings);
	num_opp_free_moves = check_ring_adjacent_empty(board, opp_cur_rings);


	

	for(int i=0;i<board.size();i++){
		for(int j=0;j<board[i].size();j++){
			//checking for 3 length trails
			// int orig_tl = trail_length;
			// vector<pair<pair<int, int>, pair<int, int>>> tmp_trails[3];
			if(i == 0){
				my_dominance += check_dominance(board, 0, i, j, val1_ctg, val2_ctg, val3_ctg, val4_ctg);
			}
			if(j == 0){		
				my_dominance += check_dominance(board, 1, i, j, val1_ctg, val2_ctg, val3_ctg, val4_ctg);
			}
			if((i == 0) || (j == 0)){	
				my_dominance += check_dominance(board, 2, i, j, val1_ctg, val2_ctg, val3_ctg, val4_ctg);
			}
			// trail_length = orig_tl;

			if(board[i][j].ring==id){
				num_my_rings++;
			}else if(board[i][j].ring==1-id){
				num_opp_rings++;
			}else if(board[i][j].marker==id){
				num_my_markers++;
			}else if(board[i][j].marker==1-id){
				num_opp_markers++;
			}
		}
	}
	// //cerr<<"MY 3len trails"<<num_my_3len_trails<<endl;
	vector<float> to_return;
	to_return.push_back((num_opp_rings - opp_cur_trails[0].size()-opp_cur_trails[1].size()-opp_cur_trails[2].size())/3);
	to_return.push_back((num_rings - num_my_rings)/3);
	to_return.push_back((num_my_markers - num_opp_markers)/10);
	to_return.push_back(num_my_3len_trails - num_opp_3len_trails);
	to_return.push_back((num_my_ring_adjacent_trail - num_opp_ring_adjacent_trail)/10000);
	to_return.push_back(num_my_free_moves - num_opp_free_moves);
	to_return.push_back(my_dominance);

	return to_return;

	// return w6*(num_my_free_moves - num_opp_free_moves) + wt_ctg*my_dominance + w5*(num_my_ring_adjacent_trail - num_opp_ring_adjacent_trail) + w1*(num_opp_rings - opp_cur_trails[0].size()-opp_cur_trails[1].size()-opp_cur_trails[2].size()) + w2*(num_rings - num_my_rings) + w3*(num_my_markers - num_opp_markers) + w4*(num_my_3len_trails - num_opp_3len_trails);
}



//I think it should return <board, heuristic>
pair<int,float> player::MinVal(vector<vector<pos>>& board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<pair<int, int>, pair<int, int>>> non_local_trails[3], int current_depth, float alpha, float beta, int rings_placed){
	pair<int,float> child;
	pair<int,float> best_child;
	best_child.first = -1;
	best_child.second = max_lim_p;

	vector<pair<float, vector<pair<int, pair<pair<int,int>,pair<int,int>>>>>> move;
	if (rings_placed >= num_rings)
		get_neighbours(true, board, opp_ring_pos, my_ring_pos, local_trails, non_local_trails, move, false);
	else
// void player::place_rings(vector<vector<pos>>& board, vector<pair<int,int>>& local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<float, vector<pair<int, pair<pair<int,int>,pair<int,int>>>>>>& move){
		place_rings(board, opp_ring_pos, local_trails, move);


	for(int s=0;s<move.size();s++){
// void player::play_move(vector<vector<pos>>& local_board, vector<pair<int,pair<pair<int,int>,pair<int,int>>>>& moves, vector<pair<int,int>>& local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], bool my_turn){
		vector<pair<pair<int, int>, pair<int, int>>> temp_trails[3];
		copy(local_trails[0].begin(), local_trails[0].end(), back_inserter(temp_trails[0]));
		copy(local_trails[1].begin(), local_trails[1].end(), back_inserter(temp_trails[1]));
		copy(local_trails[2].begin(), local_trails[2].end(), back_inserter(temp_trails[2]));
		vector<pair<pair<int, int>, pair<int, int>>> non_temp_trails[3];
		copy(non_local_trails[0].begin(), non_local_trails[0].end(), back_inserter(non_temp_trails[0]));
		copy(non_local_trails[1].begin(), non_local_trails[1].end(), back_inserter(non_temp_trails[1]));
		copy(non_local_trails[2].begin(), non_local_trails[2].end(), back_inserter(non_temp_trails[2]));

		vector<pair<int, int>> temp_ring_pos;
		copy(opp_ring_pos.begin(), opp_ring_pos.end(), back_inserter(temp_ring_pos));
		

		play_move(board, move[s].second, temp_ring_pos, temp_trails, non_temp_trails, false);
	// for (int s = 0; s<10; s++s in children(state)){ //CHANGE_THIS
		if (current_depth == DEPTH_TO_CHECK-1)
			child = make_pair(s, move[s].first);
		else{
			int temp_rings_placed=rings_placed;
			if (id==0) temp_rings_placed++;
			child = MaxVal(board, non_temp_trails, temp_trails, current_depth+1,alpha,beta, temp_rings_placed);
		}
// void player::revert(vector<vector<pos>>& local_board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<int,int>>& local_ring_pos, vector<pair<int, pair<pair<int,int>,pair<int,int>>>>& moves, bool my_turn){

		revert(board, temp_trails, non_temp_trails, temp_ring_pos, move[s].second, false);

		beta = min(beta,child.second);
		if (alpha>=beta) return make_pair(s,child.second);
		if (child.second < best_child.second){
			best_child.first = s;
			best_child.second = child.second;
		}
	}
	return best_child; 
	// if (current_depth == DEPTH_TO_CHECK)

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

pair<int,float> player::MaxVal(vector<vector<pos>>& board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<pair<int, int>, pair<int, int>>> non_local_trails[3], int current_depth, float alpha, float beta, int rings_placed){
	// if (current_depth == DEPTH_TO_CHECK)

	// vector<vector<pos>> tmp;
	// pair<vector<vector<pos>>,float> best_child = make_pair(tmp,-1);
	pair<int,float> child;
	pair<int,float> best_child;
	best_child.first = -1;
	best_child.second = min_lim_p;

	vector<pair<float, vector<pair<int, pair<pair<int,int>,pair<int,int>>>>>> move;
	if (rings_placed >= num_rings)
		get_neighbours(false, board, my_ring_pos, opp_ring_pos, local_trails, non_local_trails, move, true);
	else
// void player::place_rings(vector<vector<pos>>& board, vector<pair<int,int>>& local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<float, vector<pair<int, pair<pair<int,int>,pair<int,int>>>>>>& move){
		place_rings(board, my_ring_pos, local_trails, move);


	for(int s=0;s<move.size();s++){
// void player::play_move(vector<vector<pos>>& local_board, vector<pair<int,pair<pair<int,int>,pair<int,int>>>>& moves, vector<pair<int,int>>& local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], bool my_turn){
		vector<pair<pair<int, int>, pair<int, int>>> temp_trails[3];
		copy(local_trails[0].begin(), local_trails[0].end(), back_inserter(temp_trails[0]));
		copy(local_trails[1].begin(), local_trails[1].end(), back_inserter(temp_trails[1]));
		copy(local_trails[2].begin(), local_trails[2].end(), back_inserter(temp_trails[2]));
		vector<pair<pair<int, int>, pair<int, int>>> non_temp_trails[3];
		copy(non_local_trails[0].begin(), non_local_trails[0].end(), back_inserter(non_temp_trails[0]));
		copy(non_local_trails[1].begin(), non_local_trails[1].end(), back_inserter(non_temp_trails[1]));
		copy(non_local_trails[2].begin(), non_local_trails[2].end(), back_inserter(non_temp_trails[2]));

		vector<pair<int, int>> temp_ring_pos;
		copy(my_ring_pos.begin(), my_ring_pos.end(), back_inserter(temp_ring_pos));
		


		////cerr<<"before play_move: "<<move[s].second.size()<<endl;
		play_move(board, move[s].second, temp_ring_pos, temp_trails, non_temp_trails, true);
		// for (int s = 0; s<10; s++s in children(state)){ //CHANGE_THIS
		if (current_depth == DEPTH_TO_CHECK-1 || move[s].first>=max_lim_p-1)
			child = make_pair(s, move[s].first);
		else{
			int temp_rings_placed=rings_placed;
			if (id==1) temp_rings_placed++;
			child = MinVal(board, non_temp_trails, temp_trails, current_depth+1,alpha,beta, temp_rings_placed);
		}
// void player::revert(vector<vector<pos>>& local_board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<int,int>>& local_ring_pos, vector<pair<int, pair<pair<int,int>,pair<int,int>>>>& moves, bool my_turn){
		////cerr<<"before revert: "<<move[s].second.size()<<endl;

		revert(board, temp_trails, non_temp_trails, temp_ring_pos, move[s].second, true);

		alpha = max(alpha,child.second);
		if (alpha>=beta) return make_pair(s,child.second);
		if (child.second > best_child.second){
			best_child.first = s;
			best_child.second = child.second;
		}
	}
	return best_child; 
}

// int ccc = 0;
void player::make_next_move(vector<vector<pos>>& board, vector<pair<int,int>>& local_ring_pos, vector<pair<int,int>>& non_local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<pair<int, int>, pair<int, int>>> non_local_trails[3], vector<pair<int,pair<pair<int,int>,pair<int,int>>>>& out, clock_t diff_time){
	
	//last_board creation
	// for(int i=0;i<board.size();i++)
	// 	for(int j=0;j<board.size();j++)
	// 		last_board[i][j] = board[i][j];
	// vector<float> old_fi = heuristic(last_board, true, local_trails, non_local_trails, local_ring_pos, non_local_ring_pos);
	// float old_heuristic = w1*old_fi[0]+w2*old_fi[1]+w3*old_fi[2]+w4*old_fi[3]+w5*old_fi[4]+w6*old_fi[5]+wt_ctg*old_fi[6];

	cerr<<"TRYING TO MAKE NEXT MOVE"<<endl;
	clock_t curr = clock();
	time_used_up = (curr - start_time - diff_time);
	double elapsed_secs = time_used_up/CLOCKS_PER_SEC;
	time_left = full_time - elapsed_secs;
	if(time_left < 50) DEPTH_TO_CHECK=2;
	if(time_left < 20) DEPTH_TO_CHECK=1;
	if(time_left < 5) DEPTH_TO_CHECK=0;
	bool play_started;
	// cerr << "Time left: " << time_left << endl;
	// //cerr << "curr: " << curr << endl;
	// //cerr << "used up: " << time_used_up << endl;
	// //cerr << "elapsed: " << elapsed_secs << endl;
	// //cerr << "full time: " << full_time << endl;
	//cerr << "Time Left: " << time_left << endl;
		
	vector<pair<float, vector<pair<int, pair<pair<int,int>,pair<int,int>>>>>> move;

	vector<float> loc_h, fut_h;
	if (num_rings_placed >= num_rings){
		get_neighbours(false, board, local_ring_pos, non_local_ring_pos, local_trails, non_local_trails, move, true);
		reward(board, local_trails, non_local_trails, local_ring_pos, non_local_ring_pos);
		loc_h = heuristic(board, true, local_trails, non_local_trails, local_ring_pos, non_local_ring_pos);
		play_started = true;
	}
	else{
		place_rings(board, local_ring_pos, local_trails, move);
		play_started = false;
	}

		// int best_move_index = 0;
		int best_move_index;
		// float new_heuristic = return_move.second;
		cerr << "AGAIN: " << num_rings_placed << " " << num_rings << " " << move.size() << endl;	
		if(move.size() > 0 && best_move_index>=0 && DEPTH_TO_CHECK > 0){
		cerr << "MAX_VAL\n";	
			pair<int, float> return_move = MaxVal(board, local_trails, non_local_trails, 0, min_lim_p, max_lim_p, num_rings_placed);
			best_move_index = return_move.first;

			play_move(board, move[best_move_index].second, local_ring_pos, local_trails, non_local_trails, true);
			
			if(play_started){
				cerr << "Hope it's in here\n";
				fut_h = heuristic(board, true, local_trails, non_local_trails, local_ring_pos, non_local_ring_pos);
				cerr << "Q val calculated\n";
				wt_update(fut_h, loc_h);
				cerr << "Hope it's out of here\n";

			}
			
			out = move[best_move_index].second;

			if (num_rings_placed < num_rings) num_rings_placed++;
			if (num_rings_placed == num_rings) DEPTH_TO_CHECK = 2;

			move_number++;
			if(move_number>18) DEPTH_TO_CHECK = 2;
			if(local_ring_pos.size()<=num_rings-2 && num_rings_placed>=num_rings) DEPTH_TO_CHECK =2;
		}else if(move.size() > 0 && best_move_index>=0 && DEPTH_TO_CHECK == 0){
			play_move(board, move[0].second, local_ring_pos, local_trails, non_local_trails, true);
			if(play_started){
				fut_h = heuristic(board, true, local_trails, non_local_trails, local_ring_pos, non_local_ring_pos);
				wt_update(fut_h, loc_h);
			}
			out = move[0].second;
			if (num_rings_placed < num_rings) num_rings_placed++;
		}else{
			while(true){
				;
			}
		}
	
	// float old_heuristic = w1*old_fi[0]+w2*old_fi[1]+w3*old_fi[2]+w4*old_fi[3]+w5*old_fi[4]+w6*old_fi[5]+wt_ctg*old_fi[6];
	// 		float to_change_by = 5000.0;
	// 		if(new_heuristic - old_heuristic){
	// 			if(old_fi[0]>0) w1 += w1/to_change_by; 
	// 			else w1 -= w1/to_change_by;
				
	// 			if(old_fi[1]>0) w2 += w2/to_change_by; 
	// 			else w2 -= w2/to_change_by;
				
	// 			if(old_fi[2]>0) w3 += w3/to_change_by; 
	// 			else w3 -= w3/to_change_by;
				
	// 			if(old_fi[3]>0) w4 += w4/to_change_by; 
	// 			else w4 -= w4/to_change_by;
				
	// 			if(old_fi[4]>0) w5 += w5/to_change_by; 
	// 			else w5 -= w5/to_change_by;
				
	// 			if(old_fi[5]>0) w6 += w6/to_change_by; 
	// 			else w6 -= w6/to_change_by;
				
	// 			if(old_fi[6]>0) wt_ctg += wt_ctg/to_change_by; 
	// 			else wt_ctg -= wt_ctg/to_change_by;
	// 		}else{
	// 			if(old_fi[0]<0) w1 += w1/to_change_by; 
	// 			else w1 -= w1/to_change_by;
				
	// 			if(old_fi[1]<0) w2 += w2/to_change_by; 
	// 			else w2 -= w2/to_change_by;
				
	// 			if(old_fi[2]<0) w3 += w3/to_change_by; 
	// 			else w3 -= w3/to_change_by;
				
	// 			if(old_fi[3]<0) w4 += w4/to_change_by; 
	// 			else w4 -= w4/to_change_by;
				
	// 			if(old_fi[4]<0) w5 += w5/to_change_by; 
	// 			else w5 -= w5/to_change_by;
				
	// 			if(old_fi[5]<0) w6 += w6/to_change_by; 
	// 			else w6 -= w6/to_change_by;
				
	// 			if(old_fi[6]<0) wt_ctg += wt_ctg/to_change_by; 
	// 			else wt_ctg -= wt_ctg/to_change_by;

	// 		}
			// cerr<<"w1: "<<w1<<endl;
			// cerr<<"w2: "<<w2<<endl;
			// cerr<<"w3: "<<w3<<endl;
			// cerr<<"w4: "<<w4<<endl;
			// cerr<<"w5: "<<w5<<endl;
			// cerr<<"w6: "<<w6<<endl;
			// cerr<<"wt_ctg: "<<wt_ctg<<endl;

			//else if time is less than THRESHOLD then depth = 1
		


}

// int count = 0;
void player::place_rings(vector<vector<pos>>& board, vector<pair<int,int>>& local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<float, vector<pair<int, pair<pair<int,int>,pair<int,int>>>>>>& move){
	uniform_int_distribution<> distr(0, board_size-1); // define the range
	float half_board = board_size/2.0;
	normal_distribution<> normal_distr(half_board,1);

	int x,y;
	bool err=true;
	bool same_line_error = false;
	int count=0;
	while(err){
		count++;
		same_line_error = false;
		if(count<100){
			x = normal_distr(eng);
			y = normal_distr(eng);
		}else{
			//if after many tries you still aren't 
			x = distr(eng);
			y = distr(eng);
		}
		//error_check
		for(int i=0;i<local_ring_pos.size();i++){
			int diff_x = x - local_ring_pos[i].first;
			int diff_y = y - local_ring_pos[i].second;
			if(diff_x==0 || diff_y==0 || (diff_y==diff_x)){
				same_line_error=true;
				break;
			}
		}
		if((!(same_line_error)) && (board[x][y].valid == true) && (board[x][y].ring == 2)){
			vector<pair<int, pair<pair<int,int>,pair<int,int>>>> tmp;
			tmp.push_back(make_pair(0,make_pair(make_pair(x,y),make_pair(max_lim_p,max_lim_p))));
			move.push_back(make_pair(1.1, tmp));
			err=false;
			//////cerr<<"CORRECT "<<count<<endl;
		}
	}
	
}

// void player::place_rings(vector<vector<pos>>& board, vector<pair<int,int>>& local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<float, vector<pair<int, pair<pair<int,int>,pair<int,int>>>>>>& move){
// 	uniform_int_distribution<> distr(0, board_size-1); // define the range
// 	int x = distr(eng);
// 	int y = distr(eng);
// 	// pair<int,int> coords = my_coord_to_board(x,y,num_rings);
// 	if((board[x][y].valid == true) && (board[x][y].ring == 2)){
// 		// board[x][y].set(2,true);
// 		// update_board(board, local_ring_pos, local_trails, 0, x, y, max_lim_p, max_lim_p, true);
// 		// mov movet, coord1, coord2;
// 		// movet.move_t = "P";
// 		// coord1.coord = x;
// 		// coord2.coord = y;
// 		// pair <int, pair<int,int>,pair<int,int>> p = make_pair(0,make_pair(make_pair(x,y),make_pair(max_lim_p,max_lim_p)));
// 		// vector<pair<float, vector<pair<int, pair<pair<int,int>,pair<int,int>>>>>>& move
// 		// move.push_back(make_pair(0,make_pair(make_pair(x,y),make_pair(max_lim_p,max_lim_p))));
// 		vector<pair<int, pair<pair<int,int>,pair<int,int>>>> tmp;
// 		tmp.push_back(make_pair(0,make_pair(make_pair(x,y),make_pair(max_lim_p,max_lim_p))));
// 		move.push_back(make_pair(1.1, tmp));
// 		// moves.push_back(0);
// 		// moves.push_back(x);
// 		// moves.push_back(y);
// 		// my_ring_pos.push_back(make_pair(x, y));
// 		// board[coords.first][coords.second].set(2, true);
// 	}else{
// 		place_rings(board, local_ring_pos, local_trails, move);
// 	}
// }

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


void player::update_board(vector<vector<pos>>& board, vector<pair<int,int>>& local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<pair<int, int>, pair<int, int>>> non_local_trails[3], int action_on_ring, int initial_x, int initial_y, int final_x, int final_y, bool my_turn){	
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
		//////cerr << "Flip Markers Start\n";
		//////cerr << "local_trails size before flip: " << local_trails[0].size() << " " << local_trails[1].size() << " " << local_trails[2].size() << " \n"; 
		flip_markers(board, local_trails, non_local_trails, initial_x, initial_y, final_x, final_y, my_turn);		
		//////cerr << "local_trails size after flip: " << local_trails[0].size() << " " << local_trails[1].size() << " " << local_trails[2].size() << " \n"; 
		//////cerr << "Flip end\n";

		board[final_x][final_y].set(2, player);

		int len = local_ring_pos.size();
		for(int i = 0; i<len; i++){
			if((local_ring_pos[i].first == initial_x) && (local_ring_pos[i].second == initial_y)){
				local_ring_pos[i].first = final_x;
				local_ring_pos[i].second = final_y;
				break;
			}
		}
	}else if(action_on_ring == 2){
		//////////////// Still a big doubt on remove_repeated trails
		pair<pair<int, int>, pair<int, int>> pp;
		pp.first.first = initial_x; pp.first.second = initial_y;
		pp.second.first = final_x; pp.second.second = final_y;

		remove_markers(board, local_trails, initial_x, initial_y, final_x, final_y, my_turn);
		if(initial_x == final_x){
			remove_repeated_trails(board, local_ring_pos, local_trails, pp, 1);
		}else if(initial_y == final_y){
			remove_repeated_trails(board, local_ring_pos, local_trails, pp, 0);
		}else{
			remove_repeated_trails(board, local_ring_pos, local_trails, pp, 2);
		}

	}else if(action_on_ring == 3){
		board[initial_x][initial_y].set(2, 2);
		// if(!my_turn){
			// int len = local_ring_pos.size();
			for(int i = 0; i<local_ring_pos.size(); i++){
				bool a1 = local_ring_pos[i].first == initial_x;
				bool a2 = local_ring_pos[i].second == initial_y;
				if(a1 && a2){
					local_ring_pos.erase(local_ring_pos.begin() + i);
					break;
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
	}else if(action_on_ring == 4){
		//////cerr << "placing them back\n";
		place_markers(board, local_trails, initial_x, initial_y, final_x, final_y, my_turn);
		//////cerr << "placed them back\n";
	}else{
		board[initial_x][initial_y].set(2, 2);
		// ////cerr << "Initial(coords): "<<initial_x<<" "<<initial_y<<endl;
		// ////cerr << "Flip Markers Start\n";
		//////cerr << "local_trails size before flip: " << local_trails[0].size() << " " << local_trails[1].size() << " " << local_trails[2].size() << " \n"; 
		flip_markers_revert(board, local_trails, initial_x, initial_y, final_x, final_y, my_turn);		
		//////cerr << "local_trails size after flip: " << local_trails[0].size() << " " << local_trails[1].size() << " " << local_trails[2].size() << " \n"; 
		//////cerr << "Flip end\n";
		// ////cerr << "Final(coords): "<<final_x<<" "<<final_y<<endl;

		board[final_x][final_y].set(2, player);

		int len = local_ring_pos.size();
		for(int i = 0; i<len; i++){
			if((local_ring_pos[i].first == initial_x) && (local_ring_pos[i].second == initial_y)){
				local_ring_pos[i].first = final_x;
				local_ring_pos[i].second = final_y;
				break;
			}
		}		
	}
}


// Won't flip the marker at x2, y2 and x1,y1
void player::flip_markers(vector<vector<pos>>& board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<pair<int, int>, pair<int, int>>> non_local_trails[3], int x1, int y1, int x2, int y2, bool my_turn){
	int startX,startY,endX,endY;

	if(x1 == x2){
		startY = min(y1, y2);
		endY = max(y1, y2);
		check_my_trail(board, local_trails, x1, y1, 0, my_turn);
		check_my_trail(board, local_trails, x1, y1, 2, my_turn);
		for(int i = startY+1; i< endY; i++){
			if(board[x1][i].marker != 2){
				board[x1][i].set(1-board[x1][i].marker,2);
				check_my_trail(board, non_local_trails, x1, i, 0, !my_turn);
				check_my_trail(board, non_local_trails, x1, i, 2, !my_turn);
				check_my_trail(board, local_trails, x1, i, 0, my_turn);
				check_my_trail(board, local_trails, x1, i, 2, my_turn);
			}
		}
		check_my_trail(board, local_trails, x1, y1, 1, my_turn);
		check_my_trail(board, non_local_trails, x1, y1, 1, !my_turn);
	
	}else if(y1 == y2){
		startX = min(x1, x2);
		endX = max(x1, x2);
		check_my_trail(board, local_trails, x1, y1, 1, my_turn);
		check_my_trail(board, local_trails, x1, y1, 2, my_turn);
		for(int i = startX+1; i< endX; i++){
			if(board[i][y1].marker != 2){
				board[i][y1].set(1-board[i][y1].marker,2);
				check_my_trail(board, non_local_trails, i, y1, 1, !my_turn);
				check_my_trail(board, non_local_trails, i, y1, 2, !my_turn);					
				check_my_trail(board, local_trails, i, y1, 1, my_turn);
				check_my_trail(board, local_trails, i, y1, 2, my_turn);
			}
		}		
		check_my_trail(board, non_local_trails, x1, y1, 0, !my_turn);
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
				check_my_trail(board, non_local_trails, i, j, 0, !my_turn);
				check_my_trail(board, non_local_trails, i, j, 1, !my_turn);
				check_my_trail(board, local_trails, i, j, 0, my_turn);
				check_my_trail(board, local_trails, i, j, 1, my_turn);
			}
		}
		check_my_trail(board, non_local_trails, x1, y1, 2, !my_turn);
		check_my_trail(board, local_trails, x1, y1, 2, my_turn);
	}	
}

// Won't flip the marker at x2, y2 and x1,y1
void player::flip_markers_revert(vector<vector<pos>>& board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], int x1, int y1, int x2, int y2, bool my_turn){
	int startX,startY,endX,endY;
	//////cerr << "In flip revert\n";

	if(x1 == x2){
		startY = min(y1, y2);
		endY = max(y1, y2);
		for(int i = startY+1; i< endY; i++){
			if(board[x1][i].marker != 2){
				board[x1][i].set(1-board[x1][i].marker,2);
			}
		}
	}else if(y1 == y2){
		startX = min(x1, x2);
		endX = max(x1, x2);
		for(int i = startX+1; i< endX; i++){
			if(board[i][y1].marker != 2){
				board[i][y1].set(1-board[i][y1].marker,2);
			}
		}		
	}else{
		startX = min(x1, x2);
		startY = min(y1, y2);
		endX = max(x1, x2);
		endY = max(y1, y2);
		for(int i = startX+1, j=startY+1; i< endX && j<endY ; i++,j++){
			if(board[i][j].marker != 2){
				board[i][j].set(1-board[i][j].marker,2);
			}
		}
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
				// remove_repeated_trails(board, local_ring_pos);
				break;
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
		// dir = 1;
	}else if(y1 == y2){
		startX = min(x1, x2);
		endX = max(x1, x2);
		for(int i = startX; i<= endX; i++){
			board[i][y1].set(player,2);
		}
		// dir = 0;		
	}else{
		startX = min(x1, x2);
		startY = min(y1, y2);
		endX = max(x1, x2);
		endY = max(y1, y2);
		for(int i = startX, j=startY; i<= endX && j<=endY ; i++,j++){
			board[i][j].set(player,2);
		}
		// dir = 2;
	}

	// if(my_turn){
		// pair<pair<int, int>, pair<int, int>> temp = make_pair(make_pair(x1,y1), make_pair(x2,y2));
		// local_trails[dir].push_back(temp);
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
	//////cerr << "In check_my_trail\n";
	int count = 0; int count2 = 0;
	int step_x, step_y, player, startX, startY;
	bool trail = false; bool tempMade = false;
	//////cerr << "My_turn: " << my_turn << endl;
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
				if((count == 0)&&(tempMade == false)){
					tempMade = false;
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
					break;
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
				count2 = count;
				tempMade = true;
				break;				
			}
		}
	}
	//////cerr << "Count in trail: " << count << endl;
	//////cerr << "Count2 in trail: " << count2 << endl;
	//////cerr << "tempMade in trail: " << tempMade << endl;
	//////cerr << "i in trail: " << i << endl;
	//////cerr << "j in trail: " << j << endl;
	if((tempMade == false) && (count >= trail_length)){
		(temp.second).first = i-step_x;
		(temp.second).second = j-step_y;
		count2 = count;		
	}

	if(count2 == trail_length){
		// if(my_turn){
		//////cerr << "Pushed in trails\n";
		// if((which_trails) ^ (whose_turn)){
			local_trails[dir].push_back(temp);
		// }else if((whose_turn) && (which_trails)){
		// 	me_to_opp[dir].push_back(temp);
		// }else{
		// 	opp_to_me[dir].push_back(temp);
		// }
		// }else{
			// local_trails[dir].push_back(temp);
		// }
	}else if(count2 > trail_length){
		//////cerr << "Pushed twice in trails\n";
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
		// if((which_trails) ^ (whose_turn)){
			local_trails[dir].push_back(temp);
			local_trails[dir].push_back(temp2);
		// }else if((whose_turn) && (which_trails)){
		// 	me_to_opp[dir].push_back(temp);
		// 	me_to_opp[dir].push_back(temp2);
		// }else{
		// 	opp_to_me[dir].push_back(temp);
		// 	opp_to_me[dir].push_back(temp2);
		// }
		// }		
	}
}

// void player::remove_trails_and_rings(vector<vector<pos>>& board, vector<pair<int,int>>& local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<int>& moves){
// 	// For now its random
// 	pair<pair<int, int>, pair<int, int>> pp;
// 	bool flag = false;
// 	for(int i = 0; i< 3; i++){
// 		int len = local_trails[i].size();
// 		//////cerr << "Size of " << i << " is: " << len << endl; 
// 		for(int j = 0; j< len; j++){
// 			//////cerr << "Inside: " << (local_trails[i][j].first).first << endl;
// 			(pp.first).first = (local_trails[i][j].first).first;
// 			(pp.first).second = (local_trails[i][j].first).second;
// 			(pp.second).first = (local_trails[i][j].second).first;
// 			(pp.second).second = (local_trails[i][j].second).second;

// 			moves.push_back(3); moves.push_back((pp.first).first); moves.push_back((pp.first).second);
// 			moves.push_back(4); moves.push_back((pp.second).first); moves.push_back((pp.second).second);
// 			update_board(board, local_ring_pos, local_trails, 2, (local_trails[i][j].first).first, (local_trails[i][j].first).second, (local_trails[i][j].second).first, (local_trails[i][j].second).second, true);
// 			remove_ring(board, local_trails, local_ring_pos, moves);
// 			remove_repeated_trails(board, local_ring_pos, local_trails, pp, i);
// 			flag = true;
// 			break;
// 		}
// 		if(flag){
// 			break;
// 		}
// 	}

// 	bool a1 = (local_trails[0].size() != 0);
// 	bool a2 = (local_trails[1].size() != 0);
// 	bool a3 = (local_trails[2].size() != 0);
// 	if((a1 || a2 || a3) && (local_ring_pos.size() > num_rings-to_win_remove)){
// 		remove_trails_and_rings(board, local_ring_pos, local_trails, moves);
// 	}
// }

// void player::remove_ring(vector<vector<pos>>& board, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], vector<pair<int,int>>& local_ring_pos, vector<int>& moves){
// 	moves.push_back(5); moves.push_back((local_ring_pos.back()).first); moves.push_back((local_ring_pos.back()).second);
// 	update_board(board, local_ring_pos, local_trails, 3, (local_ring_pos.back()).first, (local_ring_pos.back()).second, max_lim_p, max_lim_p, true);
// }

void player::remove_repeated_trails(vector<vector<pos>>& board, vector<pair<int,int>>& local_ring_pos, vector<pair<pair<int, int>, pair<int, int>>> local_trails[3], pair<pair<int, int>, pair<int, int>>& pp, int dir){
	////cerr << "In remove remove_repeated_trails\n";
	int x1, x2, y1, y2, len;
	x1 = min((pp.first).first,(pp.second).first);
	y1 = min((pp.first).second,(pp.second).second);
	x2 = max((pp.first).first,(pp.second).first);
	y2 = max((pp.first).second,(pp.second).second);
	

	vector<pair<int,int>> expanded_pp;
	//get pp size
	int size_pp = 1+ y2 - y1;
	if(size_pp==1){
		size_pp =  1+ x2 - x1;
	}
	//expand pp
	for(int i=0;i<size_pp;i++){
		if (dir==0) expanded_pp.push_back(make_pair(x1+i,y1));
		else if(dir==1) expanded_pp.push_back(make_pair(x1,y1+i));
		else expanded_pp.push_back(make_pair(x1+i,y1+i));
	}

	////cerr << "Local trails size jus before repeated trails: " << local_trails[0].size() << " " << local_trails[1].size() << " " << local_trails[2].size() << endl;
	for(int i=0;i<3;i++){
		// len = local_trails[i].size();
		for(int j=0;j<local_trails[i].size();j++){
			//get tmp size
			////cerr << "J loop: " << j << endl;
			int xx1 = min((local_trails[i][j].second).first, (local_trails[i][j].first).first);
			int yy1 = min((local_trails[i][j].second).second, (local_trails[i][j].first).second);
			int xx2 = max((local_trails[i][j].second).first, (local_trails[i][j].first).first);
			int yy2 = max((local_trails[i][j].second).second, (local_trails[i][j].first).second);
			
			int size_tmp =  1+ yy2 - yy1;
			if(size_tmp==1){
				size_tmp =  1+ xx2 - xx1;
			}
			//expand tmp
			vector<pair<int,int>> expanded_tmp;
			for(int k=0;k<size_tmp;k++){
				if (i==0) expanded_tmp.push_back(make_pair(xx1+k,yy1));
				else if (i==1) expanded_tmp.push_back(make_pair(xx1,yy1+k));
				else expanded_tmp.push_back(make_pair(xx1+k,yy1+k));
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
			if(flag) {
				local_trails[i].erase(local_trails[i].begin() + j);
				j--;
			}
		}
	}
	////cerr << "Local trails size jus after repeated trails: " << local_trails[0].size() << " " << local_trails[1].size() << " " << local_trails[2].size() << endl;









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