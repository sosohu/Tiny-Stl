#ifndef __VECTOR__H__
#define __VECTOR__H__

#include "alloc.h"
#include "memory.h"
#include <cstddef>

namespace hu{

template<typename T, typename Alloc = alloc>
class vector{
private:
	typedef simple_alloc<T, alloc> data_alloc;

public:
	typedef T value_type;
	typedef T* poniter;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;

	//基本的构造析构函数
	vector();
	vector(size_type n);
	vector(size_type n, const_reference x);
	vector(const vector<T>& rhs);
	~vector();

	//属性函数
	size_type size(){
		return finish - start;
	}

	//
	value_type operator[](size_type pos) const;

private:
	poniter start;
	poniter finish;
	poniter end_of_storage;
};

template<typename T, typename Alloc>
vector<T,Alloc>::vector(){
	start = data_alloc::allocate();
	finish = start;
	end_of_storage = start + 1;
}

template<typename T, typename Alloc>
vector<T,Alloc>::vector(size_type n){
	start = data_alloc::allocate(n);
	finish = start;
	end_of_storage = start + n;
}

template<typename T, typename Alloc>
vector<T, Alloc>::vector(size_type n, const_reference x){
	start = data_alloc::allocate(n);
	uninitialized_fill_n(start, n, x);
	finish = start + n;
	end_of_storage = start + n;
}

template<typename T, typename Alloc>
vector<T,Alloc>::~vector(){
	data_alloc::deallocate(start, finish - start);
}

template<typename T, typename Alloc>
typename vector<T,Alloc>::value_type vector<T,Alloc>::operator[](size_type pos) const{
	return *(start + pos);
}

}

#endif
