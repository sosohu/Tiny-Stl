#include "../include/alloc.h"
#include "../include/vector.h"
#include "../include/algorithm.h"
#include <iostream>
#include <vector>
#include <algorithm>


int main(){
	int data[] = {1,9,9,1,1,0,0,9};
	hu::vector<int> vec(std::begin(data), std::end(data));
	vec.resize(5);
	vec.assign(std::begin(data), std::end(data));
	int n = 5;
	int val = 2;
	vec.assign(n,val);
	vec.push_back(1);
	vec.pop_back();
	unsigned int m = 5;
	hu::vector<int> vec1(m,3); //TODO
	vec.swap(vec1);
	//vec.clear();
	vec.insert(vec.begin(), std::begin(data), std::end(data));
	vec.insert(vec.end(), (unsigned int)n, val); //TODO
	vec.erase(vec.begin(), vec.end() - 3);
	for(auto iter = vec.begin(); iter != vec.end(); iter++)
		cout<<*iter<<endl;
}
