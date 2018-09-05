#ifndef UTIL_H
#define	UTIL_H

#include <limits>
#include <vector>
#include <utility>

using namespace std;
int max_lim = numeric_limits<int>::max();
int min_lim = numeric_limits<int>::min();

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
    void set(int mark, bool r){
        marker = mark;
        ring = r;
    }

    void setInvalid(){
        valid = false;
    }
};

pair<int, int> my_coord_to_board(int &x, int &y, int &num_rings){
    return make_pair(num_rings + x, num_rings - y);
}

pair<int, int> board_to_my_coord(int &x, int &y, int &num_rings){
    return make_pair(x - num_rings, num_rings - y);
}

#endif

