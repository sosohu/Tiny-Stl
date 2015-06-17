#include "../include/alloc.h"
#include "../include/vector.h"
#include "../include/unordered_set.h"
#include "../include/unordered_map.h"
#include "../include/algorithm.h"
#include <iostream>
#include <vector>
#include <algorithm>

using hu::vector;
using hu::unordered_set;
using hu::unordered_map;

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

void hash_set_test(){
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

vector<int> TwoSum(int values[], int size, int target){
	unordered_map<int, int> table;
	vector<int> res(2, -1);
	for(int i = 0; i < size; i++){
		if(table.count(target - values[i])){
			res[0] = table[target - values[i]];
			res[1] = i;
			return res;
		}
		//table.insert(std::make_pair(values[i], i));
		table[values[i]] = i;
	}
	return res;
}

void hash_map_test(){
	std::pair<int,int> data[] = {{1,9}, {9,2}, {0,2}, {2,8}};
	unordered_map<int, int> table(std::begin(data), std::end(data));
	table.erase(0);
	cout<<"table size: "<<table.size()<<endl;
	for(auto iter = table.begin(); iter != table.end(); iter++)
		cout<<iter->first<<","<<iter->second<<endl;
	cout<<table.load_factor()<<endl;
	int data1[] = {1,2,3,4,1,2,0,7};
	auto res = TwoSum(data1, sizeof(data1) / sizeof(int), 9);
	for(auto iter = res.begin(); iter != res.end(); iter++)
			cout<<*iter<<endl;
}

int main(){
	hash_map_test();
}
