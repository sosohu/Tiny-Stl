#ifndef __VECTOR__H__
#define __VECTOR__H__
/*
 * 简单的vector实现
 */

#include "alloc.h"
#include "memory.h"
#include "uninitialized.h"
#include <cstddef>
#include <climits>

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

	typedef poniter iterator;

	//基本的构造析构函数
	explicit vector();
	explicit vector(size_type n);
	vector(size_type n, const_reference x);
	vector(const vector<T>& rhs);

	template<typename InputIterator>
	vector(InputIterator begin, InputIterator end);

	~vector();

	//迭代器函数
	iterator begin(){ return start;}
	iterator end(){ return finish; }

	//属性函数
	size_type size() const{ 	return finish - start; }
	size_type max_size() const{ 	return UINT_MAX / sizeof(T);	}
	size_type capacity() const{	return end_of_storage - start;	}
	bool empty() const{	return finish == start;	}
	void resize(size_type n){
		if(capacity() < n){
			reserve(n);
		}else{
			finish = begin + n;
		}
	}
	void reserve(size_type n){
		size_type old_sz = capacity();
		if(old_sz < n){
			iterator new_start = data_alloc::allocate(n);
			uninitialized_copy(start, finish, new_start);
			data_alloc::deallocate(start, old_sz);
			start = new_start;
			finish = start + old_sz;
			end_of_storage = start + n;
		}
	}

	//访问函数
	const_reference operator[](size_type pos) const;
	reference operator[](size_type pos);
	value_type at(size_type pos) const;
	value_type front() const;
	value_type back() const;

	//修改函数
	template<typename InputIterator>
	void assign(InputIterator begin, InputIterator end);

	void assign(size_type n, const_reference value);
	void push_back(const const_reference value);
	void pop_back();

	template<typename InputIterator>
	void insert(iterator pos, InputIterator begin, InputIterator end);

	iterator insert(iterator pos, const_reference value);
	void insert(iterator pos, size_type n, const_reference value);

	iterator erase(iterator pos);
	iterator erase(iterator begin, iterator end);

	void swap(vector<T, Alloc> rhs);
	void clear();


private:
	iterator start;
	iterator finish;
	iterator end_of_storage;
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
	end_of_storage = finish;
}

/*
 * Alloc负责为对象分配内存，uninitialized系列函数负责构造对象
 */
template<typename T, typename Alloc>
vector<T, Alloc>::vector(size_type n, const_reference x){
	start = data_alloc::allocate(n);
	uninitialized_fill_n(start, n, x);
	finish = start + n;
	end_of_storage = finish;
}

template<typename T, typename Alloc>
template<typename InputIterator>
vector<T,Alloc>::vector(InputIterator begin, InputIterator end){
	difference_type n = distance(begin, end);
	start = data_alloc::allocate(n);
	uninitialized_copy(begin, end, start);
	finish = start + n;
	end_of_storage = finish;
}

template<typename T, typename Alloc>
vector<T,Alloc>::~vector(){
	data_alloc::deallocate(start, capacity());
}

template<typename T, typename Alloc>
typename vector<T,Alloc>::reference vector<T,Alloc>::operator[](size_type pos) {
	return *(start + pos);
}

template<typename T, typename Alloc>
typename vector<T,Alloc>::const_reference vector<T,Alloc>::operator[](size_type pos) const{
	return (const_cast<vector<T, Alloc> >(*this))[pos];
}

template<typename T, typename Alloc>
typename vector<T,Alloc>::value_type vector<T,Alloc>::at(size_type pos) const{
	if(pos >= size())	THROW_OUT_OF_RANGE;
	return *(start + pos);
}

template<typename T, typename Alloc>
typename vector<T,Alloc>::value_type vector<T, Alloc>::front() const{
	if(empty())	THROW_OUT_OF_RANGE;
	return *start;
}

template<typename T, typename Alloc>
typename vector<T,Alloc>::value_type vector<T, Alloc>::back() const{
	if(empty())	THROW_OUT_OF_RANGE;
	return *(finish - 1);
}


}

#endif
