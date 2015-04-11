#include "../include/alloc.h"
#include <iostream>
#include <vector>

using namespace std;

int main(){
	vector<int, hu::allocator<int> > vec{0,1,2,3,4};
	for(auto ele : vec)
		cout<<ele<<endl;
}
