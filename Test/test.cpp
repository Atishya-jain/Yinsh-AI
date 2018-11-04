#include <iostream>
using namespace std;

int main(){
	long long int a = 0;
	int z = 0;
	while(z < 2){
		for(long i=0;i<1000000000;i++){
			a++;
		}
		cout<<" . "<<endl;
		int x;
		cin>>x;
		cout<<x;
		z++;
	}
}