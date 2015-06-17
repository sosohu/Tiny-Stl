/*
 * unordered_map.h
 *
 *  Created on: 2015年6月17日
 *      Author: sosohu
 */

#ifndef INCLUDE_UNORDERED_MAP_H_
#define INCLUDE_UNORDERED_MAP_H_

#include "def.h"
#include "hash_table.h"
#include "hash_function.h"

namespace hu{

template<typename Key, typename Mapped, typename HashFunc = hash_function<Key>,
		typename EqualKey = equal_key<Key>, typename Alloc = alloc>
class unordered_map{
public:
	typedef size_t size_type;
	typedef Key key_type;
	typedef Mapped mapped_type;
	typedef std::pair<Key, Mapped>	value_type;
	typedef unordered_map<Key, Mapped, HashFunc, EqualKey, Alloc>	unordered_map_type;
	typedef hash_table<Key, value_type, HashFunc, extract_key_pair<Key, value_type>, EqualKey, Alloc>	hash_table_type;
	typedef	typename hash_table_type::iterator	iterator;

	unordered_map() = default;
	template<typename InputIterator>
	unordered_map(InputIterator first, InputIterator last){
		while(first != last){
			insert(*first++);
		}
	}
	unordered_map(const unordered_map_type &rhs){
		ht.swap(rhs.ht);
	}
	~unordered_map() = default;

	bool empty()	{	return ht.empty();	}
	size_type size()	{	return ht.size();	}
	size_type max_size()	{	return ht.max_size();	}

	iterator begin()	{	return ht.begin();	}
	iterator end()	{	return ht.end();	}

	mapped_type& operator[](const key_type key){
		if(ht.find(key) == end()){
			insert(std::make_pair(key, mapped_type()));
		}
		return (ht.find(key))->second;
	}

	mapped_type at(const key_type key){
		if(ht.find(key) == end())	THROW_OUT_OF_RANGE;
		return (ht.find(key))->second;
	}

	size_type count(const key_type key)	{	return ht.count(key);	}
	iterator find(const key_type key)	{	return ht.find(key);	}

	/*
	 * attention: insert the value_type and erase the key_type
	 */
	void insert(const value_type key)	{	ht.insert_unique(key); }
	void erase(const key_type key)	{	ht.erase(key);	}
	void erase(iterator iter)	{	ht.erase(iter);	}
	void clear()	{	ht.clear();	}
	void swap(unordered_map_type &rhs)	{	ht.swap(rhs.ht);	}

	size_type bucket_count()	{	return ht.getBucketSize();	}
	size_type max_bucket_count()	{	return ht.maxBucketSize();	}
	size_type bucket_size(size_type n)	{	return ht.getBucket(n);	}
	size_type bucket(const key_type key)	{	return ht.getBucketId(key);	}

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



#endif /* INCLUDE_UNORDERED_MAP_H_ */
