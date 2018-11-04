#include <iostream>
#include <vector>
using namespace std;

void foo(vector<int> x[3]){
	x[0][0]=-1;
	x[1][0]=-1;
	x[2][0]=-1;
}
int main() {
	// your code goes here
	vector<int> x[3];
	x[0].push_back(1);
	x[1].push_back(1);
	x[2].push_back(1);
	foo(x);
	cout<<x[0][0];
	return 0;
}