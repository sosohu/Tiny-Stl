#include "../include/alloc.h"
#include "../include/vector.h"
#include "../include/algorithm.h"
#include <iostream>
#include <vector>
#include <algorithm>

using hu::vector;

int main(){
	int data[] = {1,9,9,1,1,0,0,9};
	vector<int> vec(std::begin(data), std::end(data));
	vec.resize(5);
	vec.assign(std::begin(data), std::end(data));
	vec.assign(5,2);
	vec.push_back(1);
	vec.pop_back();
	hu::vector<int> vec1(5,3);
	vec.swap(vec1);
	//vec.clear();
	vec.insert(vec.begin(), std::begin(data), std::end(data));
	vec.insert(vec.end(), 5, 2);
	vec.erase(vec.begin(), vec.end() - 3);
	for(auto iter = vec.begin(); iter != vec.end(); iter++)
		cout<<*iter<<endl;
}
