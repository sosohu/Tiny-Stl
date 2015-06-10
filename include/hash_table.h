#ifndef __HASH_TABLE__H__
#define __HASH_TABLE__H__

#include "memory.h"
#include "vector.h"

namespace hu{

template<typename Value>
struct hash_node{
	hash_node *next;
	Value val;
};

template<typename Key, typename Value, typename HashFunc,
			typename ExtractKey, typename Equal, typename Alloc = alloc>
struct hash_table_iterator{
public:
	typedef Key key_type;
	typedef Value value_type;
	typedef Value* pointer;
	typedef Value &reference;
	typedef hash_table_iterator<Key, Value, HashFunc, ExtractKey, Equal, Alloc> iterator;
	typedef const iterator const_iterator;
	typedef hash_table<Key, Value, HashFunc, ExtractKey, Equal, Alloc> hash_table_type;
	typedef hash_node<Value> hash_node_type;
	typedef forward_iterator_tag iterator_category;

public:
	hash_table_iterator() = default;
	hash_table_iterator(hash_node_type *cur, hash_table_type* ht) : cur(cur), ht(ht){}
	hash_table_iterator(const hash_table_iterator& rhs) = default;
	const hash_table_iterator& operator=(const hash_table_iterator& rhs) = default;

	reference operator*(){
		return cur->val;
	}

	pointer operator->(){
		return &(cur->val);
	}

	iterator operator++(){
		hash_node_type old = cur;
		cur = cur->next;
		if(cur == NULL){
			size_t id = ht->getBucketId(old->val);
			id++;
			while(!cur && id < ht->getBucketSize()){
				cur = ht->buckets[id];
			}
		}
		return *this;
	}
	iterator operator++(int){
		iterator tmp = *this;
		++*this;
		return tmp;
	}
	bool operator==(const_iterator &rhs) const{
		return	cur == rhs.cur;
	}
	bool operator!=(const_iterator &rhs) const{
		return cur != rhs.cur;
	}

public:
	hash_table_type *ht;
	hash_node_type	*cur;
};

/*
 * ExtractKey tell you how to get the key from the value. For example, Value type is pair<int, int> hash_map and
 * the ExtractKey is select the first one of the pair.
 */
template<typename Key, typename Value, typename HashFunc, typename ExtractKey, typename Equal, typename Alloc = alloc>
class hash_table{
public:
	typedef Key Key_type;
	typedef Value Value_type;
	typedef size_t size_type;
	typedef hash_table_iterator<Key, Value, HashFunc, ExtractKey, Equal, Alloc> iterator;

	hash_table(size_type n) : buckets(vector<hash_node*, Alloc>(n, NULL)){
		start.cur = finish.cur = NULL;
		num_element = 0;
	}

	~hash_table(){
		iterator iter = begin();
		while(iter++ != end()){
			//TODO
		}
	}

	size_type getBucketSize() const{ return buckets.size(); }
	size_type getBucketId(const Value_type &val){
		return hash_func(get_key(val)) % getBucketSize();
	}

	// iterator interface
	iterator begin()	const {	return start; }
	iterator end() const { return finish; }

	void resize(size_type new_size);
	size_type count(const Value_type &vale);
	void insert_equal(const Value_type &value);
	void insert_unique(const Value_type &value);
	void erase(iterator iter);
	void erase(const Value_type &value);
	void clear();

private:
	vector<hash_node*, Alloc> buckets;
	size_t num_element; // count how many keys have been contained
	ExtractKey get_key;
	HashFunc hash_func;
	Equal equal;
	iterator start;
	iterator finish;
};

}

#endif
