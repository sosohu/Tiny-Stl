#ifndef __ALLOC__H__
#define __ALLOC__H__
/*
本文件主要实现allocator类，allocator类即是空间配置器，为所有STL的容器分配空间
*/

#include "memory.h"
#include <climits>

namespace hu{
	
template<typename T>
class allocator{

public:
//常用类型别名
	typedef T value_type;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef T& reference;
	typedef const T& const_reference;
	typedef unsigned int size_type;
	typedef int difference_type;

	template<typename U>
	struct rebind{
		typedef allocator<U> other;
	};

//常用函数
	allocator() = default;
	allocator(const allocator &rhs) = default;
	~allocator() = default;

	pointer allocate(size_type n, const void* hint= 0);
	void deallocate(pointer p, size_type n);

	size_type max_size() const;

	void construct(pointer p, const T& x);
	void destroy(pointer p);

	pointer address(const_reference x) const;

};

template<typename T>
typename allocator<T>::pointer allocator<T>::allocate(size_type n, const void* hint){
	return _allocate(n, (pointer)NULL);
}

template<typename T>
void allocator<T>::deallocate(pointer p, size_type n){
	_deallocate(p);
}

template<typename T>
typename allocator<T>::size_type allocator<T>::max_size() const{
	return UINT_MAX/sizeof(T);
}

template<typename T>
void allocator<T>::construct(pointer ptr, const T& value){
	_construct(ptr, value);
}

template<typename T>
void allocator<T>::destroy(pointer ptr){
	_destroy(ptr);
}

template<typename T>
typename allocator<T>::pointer allocator<T>::address(const_reference x) const{
	return (pointer)&x;
}

}


#endif
