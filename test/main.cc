#include "../include/alloc.h"
#include "../include/vector.h"
#include "../include/algorithm.h"
#include <iostream>
#include <vector>
#include <algorithm>


int main(){
	int data[] = {1,9,9,1,1,0,0,9};
	hu::vector<int> vec(std::begin(data), std::end(data));
	for(auto i = 0; i < vec.size(); i++)
		cout<<vec[i]<<endl;
}
