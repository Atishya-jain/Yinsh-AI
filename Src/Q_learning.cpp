#include "Utils.h"
#include "Player.h"

void player::read_wts(){
	// cout << "HIHI\n";
	wt.clear();
	if(rd_wt_filename != "null"){
		ifstream infile1(rd_wt_filename.c_str());
    	float wtss = 0;
    	while(infile1 >> wtss){
    		//infile1 >> wtss;
    		// if(wtss == EOF){
    		// 	break;
    		// }
    		// cout << "hi\n";
    		wt.push_back(wtss);
    	}
    	infile1.close();
	}
	// cout << "YO\n";
	// for(int i = 0; i<wt.size(); i++){
	// 	cout << wt[i] << " ";
	// }cout << endl;
}

void player::write_wts(){
	if(wt_filename != "null"){
		ofstream outfile1(wt_filename.c_str());
		// cerr<<wt_filename<<"********************"<<endl;
		if(!(outfile1).is_open()){
	        cout << "There is no such output file" << "\n";
	        exit(3);
    	}
    	for(int i = 0; i<wt.size(); i++){
    		outfile1 << wt[i] << endl;
    		cerr << wt[i] << endl;
    	}
    	outfile1.close();
	}
}

void player::reward(vector<vector<pos>>& board, vector<pair<pair<int, int>, pair<int, int>>> my_cur_trails[3], vector<pair<pair<int, int>, pair<int, int>>> opp_cur_trails[3],vector<pair<int,int>>& my_cur_rings, vector<pair<int,int>>& opp_cur_rings){
	int my_mark = 0, opp_mark = 0, my_ring = 0, opp_ring = 0;
	for(int i = 0; i<board.size(); i++){
		for(int j = 0; j<board.size(); j++){
			if(board[i][j].ring==id){
				my_ring++;
			}else if(board[i][j].ring==1-id){
				opp_ring++;
			}else if(board[i][j].marker==id){
				my_mark++;
			}else if(board[i][j].marker==1-id){
				opp_mark++;
			}
		}
	}
	float val = (num_rings - my_ring)*10 - (num_rings - opp_ring)*20 + my_mark*0.1 - opp_mark*0.2;
	if(prev_reward == -1){
		curr_reward = val;
		prev_reward = val;
	}else{
		curr_reward = val - prev_reward;
		prev_reward = val;
	}
	cerr << "Rewards calculated: " << curr_reward << endl;
}

void player::wt_update(vector <float> &fut_h, vector <float> &loc_h){
	float Q_fut, Q_loc;
	for(int i = 0; i<wt.size(); i++){
		Q_fut += (wt[i]*fut_h[i]);
		Q_loc += (wt[i]*loc_h[i]);
		cerr << wt[i] << " ";
	}cerr << endl;
	for(int i = 0;i < wt.size(); i++){
		wt[i] = wt[i] + ALP*(curr_reward + GAM*Q_fut - Q_loc)*loc_h[i];
	}
	cerr << "Let's write wts\n";

	write_wts();
	cerr << "Wts written\n";

	// get reward val for current state
	// For all children get Q values
}