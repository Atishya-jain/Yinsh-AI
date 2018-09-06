#ifndef STRUCT_H
#define STRUCT_H

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

#endif