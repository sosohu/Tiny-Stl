/*
 * unordered_set.h
 *
 *  Created on: 2015年6月16日
 *      Author: sosohu
 */

#ifndef INCLUDE_UNORDERED_SET_H_
#define INCLUDE_UNORDERED_SET_H_

#include "hash_function.h"
#include "hash_table.h"

namespace hu{

template<typename Key, typename HashFunc = hash_function<Key>, typename EqualKey = equal_key<Key>, typename Alloc = alloc>
class unordered_set{
public:
	typedef Key Key_type;
	typedef Key Value_type;
	typedef Key* pointer;
	typedef Key& reference;
	typedef unordered_set<Key, HashFunc, EqualKey, Alloc> unordered_set_type;
	typedef size_t size_type;
	typedef hash_table<Key, Key, HashFunc, extract_key_single<Key>, EqualKey, Alloc> hash_table_type;
	typedef typename hash_table_type::iterator iterator;

	unordered_set() = default;
	template<typename InputIterator>
	unordered_set(InputIterator first, InputIterator last){
		while(first != last){
			insert(*first++);
		}
	}
	unordered_set(const unordered_set_type &rhs){
		ht.swap(rhs.ht);
	}
	~unordered_set()	= default;

	bool empty()	{	return ht.empty();	}
	size_type size()	{	return ht.size();	}
	size_type max_size()	{	return ht.max_size();	}

	iterator begin()	{	return ht.begin();	}
	iterator end()	{	return ht.end();	}

	size_type count(const Key_type key)	{	return ht.count(key);	}
	iterator find(const Key_type key){	return ht.find(key);	}

	void insert(const Value_type key)	{	ht.insert_unique(key); }
	void erase(const Key_type key)	{	ht.erase(key);	}
	void erase(iterator iter)	{	ht.erase(iter);	}
	void clear()	{	ht.clear();	}
	void swap(unordered_set_type &rhs)	{	ht.swap(rhs.ht);	}

	size_type bucket_count()	{	return ht.getBucketSize();	}
	size_type max_bucket_count()	{	return ht.maxBucketSize();	}
	size_type bucket_size(size_type n)	{	return ht.getBucket(n);	}
	size_type bucket(const Key_type key)	{	return ht.getBucketId(key);	}

	double load_factor()	{	return ht.load_factor();	}
	double max_load_factor()	{	return ht.max_load_factor();	}
	void max_load_factor(double factor)	{	ht.max_load_factor(factor);	}
	 // n is the new buckets num
	void rehash(size_type n){
		ht.resize(n);
	}
	// n is the num of element
	void reserve(size_type n){
		if(n >	bucket_count() * load_factor()){
			rehash(n / max_load_factor());
		}
	}

private:
	hash_table_type ht;
};


}



#endif /* INCLUDE_UNORDERED_SET_H_ */
