#include "../include/alloc.h"
#include "../include/vector.h"
#include "../include/algorithm.h"
#include <iostream>
#include <vector>


int main(){
	hu::vector<int> vec(5,1);
	for(auto i = 0; i < vec.size(); i++)
		cout<<vec[i]<<endl;
	int data[] = {1,2,3,4,5};
	cout<<"distance: "<<hu::distance(&data[0], &data[4])<<endl;
}
