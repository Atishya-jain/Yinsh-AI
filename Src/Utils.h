#ifndef UTIL_H
#define	UTIL_H

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

pair<int, int> game::my_coord_to_board(int &x, int &y, int &num_rings){
    pair <int, int> coords;
    coords.make_pair(num_rings + x, num_rings - y);
    return coords;
}

pair<int, int> game::board_to_my_coord(int &x, int &y, int &num_rings){
    pair <int, int> coords;
    coords.make_pair(x - num_rings, num_rings - y);
    return coords;
}

#endif

