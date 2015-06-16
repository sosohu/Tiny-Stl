#include "../include/alloc.h"
#include "../include/vector.h"
#include "../include/unordered_set.h"
#include "../include/algorithm.h"
#include <iostream>
#include <vector>
#include <algorithm>

using hu::vector;
using hu::unordered_set;

void vector_test(){
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

bool hasSum(int values[], int size, int target){
	unordered_set<int>	table;
	for(int i = 0; i < size; i++){
		if(table.count(target - values[i]))	return true;
		table.insert(values[i]);
	}
	return false;
}

void hash_test(){
	int data[] = {1,2,3,4,1,2,0,7};
	unordered_set<int>	table(std::begin(data), std::end(data));
	table.rehash(10);
	cout<<"table size: "<<table.size()<<endl;
	for(auto iter = table.begin(); iter != table.end(); iter++)
		cout<<*iter<<endl;
	cout<<table.load_factor()<<endl;
	table.clear();
	cout<<table.size()<<endl;
	cout<<hasSum(data, sizeof(data) / sizeof(int), 14)<<endl;
}

int main(){
	hash_test();
}
