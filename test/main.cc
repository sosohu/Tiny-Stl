#include "../include/alloc.h"
#include "../include/vector.h"
#include <iostream>
#include <vector>


int main(){
	hu::vector<int> vec(5,1);
	for(auto i = 0; i < vec.size(); i++)
		cout<<vec[i]<<endl;
}
