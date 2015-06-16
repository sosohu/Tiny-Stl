/*
 * hash_function.h
 *
 *  Created on: 2015年6月16日
 *      Author: sosohu
 */

#ifndef INCLUDE_HASH_FUNCTION_H_
#define INCLUDE_HASH_FUNCTION_H_

#include <cstring>

using std::string;

namespace hu{

template<typename Key>
struct hash_function{
	size_t operator()(const Key key)	const{
		return static_cast<size_t>(key);
	}
};

template<>
struct hash_function<string>{
	size_t operator()(const string str)	const{
		unsigned long res = 0;
		for(size_t i = 0; i < str.size(); i++)
			res = 5*res + str[i];
		return static_cast<size_t>(res);
	}
};

template<>
struct hash_function<const string>{
	size_t operator()(const string str)	const{
		unsigned long res = 0;
		for(size_t i = 0; i < str.size(); i++)
			res = 5*res + str[i];
		return static_cast<size_t>(res);
	}
};

template<>
struct hash_function<char *>{
	size_t operator()(char *s)	const{
		unsigned long res;
		while(s) res =  5*res + (*s++);
		return static_cast<size_t>(res);
	}
};

template<>
struct hash_function<const char *>{
	size_t operator()(const char *s)	const{
		unsigned long res;
		while(s) res =  5*res + (*s++);
		return static_cast<size_t>(res);
	}
};



template<typename Key>
struct equal_key{
	bool operator()(const Key lhs, const Key rhs)	const{
		return lhs == rhs;
	}
};

template<>
struct equal_key<string>{
	bool operator()(const string lhs, const string rhs)	const{
		return lhs.compare(rhs) == 0;
	}
};

template<>
struct equal_key<const string>{
	bool operator()(const string lhs, const string rhs){
		return lhs.compare(rhs) == 0;
	}
};


template<typename Key>
struct extract_key_single{
	Key operator()(const Key key)	const{
		return key;
	}
};

}



#endif /* INCLUDE_HASH_FUNCTION_H_ */
