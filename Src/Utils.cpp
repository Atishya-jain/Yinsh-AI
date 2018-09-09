#include "Utils.h"
using namespace std;

// pair<int, int> my_coord_to_board(int &x, int &y, int &num_rings){
//     return make_pair(num_rings + x, num_rings - y);
// }

// pair<int, int> board_to_my_coord(int &x, int &y, int &num_rings){
//     return make_pair(x - num_rings, num_rings - y);
// }

void splitString(string message, string delimiter, vector <string>& result) {
    int pos = 0, length = 0, temp;
    temp = message.find ( delimiter.c_str ( ), pos );
    while ( temp != -1 )
    {
        length = temp - pos;
        result.push_back(message.substr ( pos, length ));
        pos = temp + delimiter.size ( );
        temp = message.find ( delimiter.c_str ( ), pos );
    }
    result.push_back(message.substr ( pos ));
}

