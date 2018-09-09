#ifndef STRUCT_H
#define STRUCT_H

struct pos{
    int marker, ring;
    bool valid;
    
    pos(int mark, int r, bool val){
        marker = mark;
        ring = r;
        valid = val;
    }
    // mark == 2 for no marker, mark == id for me and 1-id for other player
    void set(int mark, int r){
        marker = mark;
        ring = r;
    }

    void setInvalid(){
        valid = false;
    }
};

union mov{
    string move_t;
    int coord;
};
#endif