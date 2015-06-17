#ifndef __HASH_TABLE__H__
#define __HASH_TABLE__H__

#include "memory.h"
#include "vector.h"
#include "hash_function.h"

namespace hu{

template<typename Value>
struct hash_node{
	hash_node *next;
	Value val;
	hash_node(hash_node *next, Value val) : next(next), val(val){}
	hash_node() = default;
};

template<typename Key, typename Value, typename HashFunc,
		typename ExtractKey, typename EqualKey, typename Alloc>
class hash_table;

template<typename Key, typename Value, typename HashFunc,
			typename ExtractKey, typename EqualKey, typename Alloc = alloc>
struct hash_table_iterator{
public:
	typedef Key key_type;
	typedef Value value_type;
	typedef Value* pointer;
	typedef Value &reference;
	typedef hash_table_iterator<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc> iterator;
	typedef const iterator const_iterator;
	typedef hash_table<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc> hash_table_type;
	typedef hash_node<Value> hash_node_type;
	typedef forward_iterator_tag iterator_category;

public:
	hash_table_iterator() = default;
	hash_table_iterator(hash_node_type *cur, hash_table_type* ht) : cur(cur), ht(ht){}

	reference operator*(){
		return cur->val;
	}

	pointer operator->(){
		return &(cur->val);
	}

	iterator operator++(){
		hash_node_type *old = cur;
		cur = cur->next;
		if(cur == NULL){
			size_t id = ht->getBucketId(old->val);
			id++;
			while(id < ht->getBucketSize()){
				cur = ht->buckets[id++];
				if(cur)	break;
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

static const int prime_table_size = 28;
static const unsigned int prime_table[prime_table_size] = {
		53, 97, 193, 389, 769,
		1543, 3079, 6151, 12289, 24593,
		49157, 98317, 196613, 393241, 786433,
		1572869, 3145739, 6291469, 12582917, 25165843,
		50331653, 100663319, 201326611, 402653189, 805306457,
		1610612741, 3221225473ul, 4294967291ul
};

/*
 * ExtractKey tells you how to get the key from the value. For example, Value type is pair<int, int> hash_map and
 * the ExtractKey is select the first one of the pair.
 */
template<typename Key, typename Value, typename HashFunc = hash_function<Key>,
		typename ExtractKey = extract_key_single<Key>,
		typename EqualKey = equal_key<Key>, typename Alloc = alloc>
class hash_table{
	template<typename K, typename V, typename H, typename Ex, typename Eq, typename Al>
	friend  struct hash_table_iterator;
	template<typename Ku, typename Hu, typename Eu, typename Au>
	friend class unordered_set;
public:
	typedef Key Key_type;
	typedef Value Value_type;
	typedef size_t size_type;
	typedef hash_table<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc>	hash_table_type;
	typedef hash_table_iterator<Key, Value, HashFunc, ExtractKey, EqualKey, Alloc> iterator;
	typedef hash_node<Value> hash_node_type;

	hash_table(size_type n = prime_table[0]) : buckets(vector<hash_node_type*, Alloc>(n, NULL)),
			num_element(0), prime_pos(0), load_factor_gate(1.0), finish(NULL, this){}

	~hash_table(){
		clear();
	}

	size_type getBucketSize() const	{	return buckets.size(); }
	size_type maxBucketSize()	const	{	return UINT_MAX;	}
	size_type getBucketId(const Value_type val)	const{
		return hash_func(get_key(val)) % getBucketSize();
	}
	size_type getBucket(const size_type n)	const{
		hash_node<Value_type> *pCur = buckets[n];
		size_type num = 0;
		while(pCur){
			num++;
			pCur = pCur->next;
		}
		return num;
	}
	bool empty()	{	return num_element == 0;	}
	size_type size()	{	return num_element;	}
	size_type max_size()	{	return UINT_MAX;	}
	double load_factor()	{	return num_element * 1.0 / getBucketSize(); }
	double	max_load_factor()	{	return load_factor_gate;	}
	void max_load_factor(double lfg)	{	load_factor_gate = lfg;	}

	// iterator interface
	iterator begin(){
		iterator start = iterator(NULL, this);
		for(size_type i = 0; i < buckets.size(); i++)
			if(buckets[i]){
				start = iterator(buckets[i], this);
				break;
			}
		return start;
	}
	iterator end() const { return finish; }

	size_type count(const Key_type &vale);
	iterator find(const Key_type &vale);
	void insert_equal(const Value_type value);
	void insert_unique(const Value_type value);
	void erase(iterator iter);
	void erase(const Key_type key);
	void clear();
	void swap(hash_table_type &rhs);

private:
	void resize(const size_type sz);

private:
	vector<hash_node_type*, Alloc> buckets;
	size_t num_element; // count how many keys have been contained
	size_t prime_pos;
	double	load_factor_gate;
	ExtractKey get_key;
	HashFunc hash_func;
	EqualKey equal;
	iterator finish;
};

template<typename Key, typename Value, typename HashFunc, typename ExtractKey, typename Equal, typename Alloc>
typename hash_table<Key, Value, HashFunc, ExtractKey, Equal, Alloc>::size_type
hash_table<Key, Value, HashFunc, ExtractKey, Equal, Alloc>::count(const Key_type &key){
	hash_node_type *pCur = buckets[hash_func(key) % getBucketSize()];
	size_type num = 0;
	while(pCur){
		if(equal(get_key(pCur->val), key))	num++;
		pCur = pCur->next;
	}
	return num;
}

template<typename Key, typename Value, typename HashFunc, typename ExtractKey, typename Equal, typename Alloc>
typename hash_table<Key, Value, HashFunc, ExtractKey, Equal, Alloc>::iterator
hash_table<Key, Value, HashFunc, ExtractKey, Equal, Alloc>::find(const Key_type &val){
	hash_node_type *pCur = buckets[hash_func(val) % getBucketSize()];
	while(pCur){
		if(equal(get_key(pCur->val), val )){
			return iterator(pCur, this);
		}
		pCur = pCur->next;
	}
	return finish;
}

template<typename Key, typename Value, typename HashFunc, typename ExtractKey, typename Equal, typename Alloc>
void hash_table<Key, Value, HashFunc, ExtractKey, Equal, Alloc>::clear(){
	for(size_type i = 0; i < buckets.size(); i++){
		hash_node_type *pHead = buckets[i], *pNext = NULL;
		while(pHead){
			pNext = pHead->next;
			_destroy(pHead);
			pHead = pNext;
			--num_element;
		}
	}
}

template<typename Key, typename Value, typename HashFunc, typename ExtractKey, typename Equal, typename Alloc>
void hash_table<Key, Value, HashFunc, ExtractKey, Equal, Alloc>::resize(const size_type sz){
	vector<hash_node_type*, Alloc> old_buckets(sz, NULL);
	buckets.swap(old_buckets);
	num_element = 0;
	prime_pos = 0;
	while(prime_table[prime_pos+1] < sz)	prime_pos++;
	for(size_type i = 0; i < old_buckets.size(); i++){
		hash_node_type *pHead = old_buckets[i], *pNext = NULL;
		while(pHead){
			pNext = pHead->next;
			insert_equal(pHead->val);
			_destroy(pHead);
			pHead = pNext;
		}
	}
}

template<typename Key, typename Value, typename HashFunc, typename ExtractKey, typename Equal, typename Alloc>
void hash_table<Key, Value, HashFunc, ExtractKey, Equal, Alloc>::insert_unique(const Value_type value){
	if(count(get_key(value))){
		//remove the older value
		*find(get_key(value)) = value;
		return;
	}
	insert_equal(value);
}

template<typename Key, typename Value, typename HashFunc, typename ExtractKey, typename Equal, typename Alloc>
void hash_table<Key, Value, HashFunc, ExtractKey, Equal, Alloc>::insert_equal(const Value_type value){
	if(num_element >= load_factor_gate * getBucketSize()){
		prime_pos++;
		resize(prime_table[prime_pos]);
	}
	hash_node_type *pCur = buckets[hash_func(get_key(value)) % getBucketSize()];
	hash_node_type *pNode = new hash_node_type(NULL, value);
	if(!pCur)	buckets[hash_func(get_key(value)) % getBucketSize()] = pNode;
	else{
		while(pCur->next)	pCur = pCur->next;
		pCur->next = pNode;
	}
	num_element++;
}

// erase all the same Value
template<typename Key, typename Value, typename HashFunc, typename ExtractKey, typename Equal, typename Alloc>
void hash_table<Key, Value, HashFunc, ExtractKey, Equal, Alloc>::erase(const Key_type key){
	hash_node_type *pHead = buckets[hash_func(key) % getBucketSize()];
	hash_node_type *pCur = pHead, *pNext = NULL;
	if(!pCur)	return;
	while(pCur->next){
		if(equal(get_key(pCur->next->val), key)){
			pNext = pCur->next;
			pCur->next = pNext->next;
			_destroy(pNext);
			num_element--;
		}
	}
	if(equal(get_key(pHead->val), key)){
		buckets[hash_func(key) % getBucketSize()] = pHead->next;
		_destroy(pHead);
		num_element--;
	}
}

// only erase the one that iterator point
template<typename Key, typename Value, typename HashFunc, typename ExtractKey, typename Equal, typename Alloc>
void hash_table<Key, Value, HashFunc, ExtractKey, Equal, Alloc>::erase(iterator iter){
	hash_node<Value_type> *pCur = buckets[hash_func(get_key(iter->cur->val)) % getBucketSize()];
	if(pCur != iter)
		while(pCur->next != iter->cur)	pCur = pCur->next;
	pCur->next = iter->cur->next;
	_destroy(iter->cur);
	num_element--;
}

template<typename Key, typename Value, typename HashFunc, typename ExtractKey, typename Equal, typename Alloc>
void hash_table<Key, Value, HashFunc, ExtractKey, Equal, Alloc>::swap(hash_table_type &rhs){
	buckets.swap(rhs.buckets);
	std::swap(num_element, rhs.num_element);
	std::swap(prime_pos, rhs.prime_pos);
	std::swap(load_factor_gate, rhs.load_factor_gate);
}

}

#endif
