#ifndef __UNINITIALIZED__H__
#define __UNINITIALIZED__H__

/*
 * 本文件主要实现几个全局的初始化相关的函数
 * uninitialized_copy, uninitialized_fill
 * uninitialized_fill_n
 */

#include "iterator.h"
#include "traits_type.h"
#include "memory.h"
#include "out.h"
#include <algorithm>
#include <cstring>


namespace hu{

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------uninitialized_copy------------------------------------------
//-----------------------------------------------------------------------------------------------------
//true_POD
template<typename InputIterator, typename ForwardIterator>
ForwardIterator _uninitialized_copy_aux(InputIterator begin,
				InputIterator end, ForwardIterator result, true_POD){
	//快速拷贝 TODO
	std::copy(begin, end, result);
}

//false_POD
template<typename InputIterator, typename ForwardIterator>
ForwardIterator _uninitialized_copy_aux(InputIterator begin,
				InputIterator end, ForwardIterator result, false_POD){
	//result保持不变
	ForwardIterator cur = result;
	while(begin != end){
		_construct(&*cur, *begin);
		++begin;
		++cur;
	}
	return cur;
}

//T*仅仅用来萃取T的POD特性，根据是否是POD类型做出处理
template<typename InputIterator, typename ForwardIterator, typename T>
ForwardIterator _uninitialized_copy(InputIterator begin, InputIterator end,
					ForwardIterator result,T*){
	typedef typename traits_type<T>::is_POD isPOD;
	_uninitialized_copy_aux(begin, end, result, isPOD());
}


/*
 * [begin, end) 内容拷贝到[result, result + end - begin)中
 * result保持不变，返回result + end - begin
 */
template<typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator begin, InputIterator end,
					ForwardIterator result){
	return _uninitialized_copy(begin, end, result, value_type(result));
}

//char* 偏特化
template<>
char* uninitialized_copy(const char* begin, const char* end, char* result){
	//memmove当位置重叠时候保证拷贝正确，memcpy不保证
	memmove(result, begin, end - begin);
	return result + (end - begin);
}
//-----------------------------------------------------------------------------------------------------
//-----------------------------------------uninitialized_fill------------------------------------------
//-----------------------------------------------------------------------------------------------------
template<typename ForwardIterator, typename T>
void _uninitialized_fill_aux(ForwardIterator begin, ForwardIterator end,
									const T& value, true_POD){
	//TODO
	std::fill(begin,end, value);
}

template<typename ForwardIterator, typename T>
void _uninitialized_fill_aux(ForwardIterator begin, ForwardIterator end,
									const T& value, false_POD){
	ForwardIterator cur = begin;
	while(begin != end){
		_construct(&*cur, value);
	}
}

template<typename ForwardIterator, typename T, typename U>
void _uninitialized_fill(ForwardIterator begin, ForwardIterator end,
									const T& value, U*){
	typedef typename traits_type<U>::is_POD isPOD;
	_uninitialized_fill_aux(begin, end, value, isPOD());
}

/*
 *[begin, end)上用x值初始化
 * */
template<typename ForwardIterator, typename T>
void uninitialized_fill(ForwardIterator begin, ForwardIterator end,
									const T& value){
	_uninitialized_fill(begin, end, value, value_type(begin));
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------------------uninitialized_fill_n----------------------------------------
//-----------------------------------------------------------------------------------------------------
template<typename ForwardIterator, typename Size, typename T>
ForwardIterator _uninitialized_fill_n_aux(ForwardIterator begin, Size n,
									const T& value, true_POD){
	qDebug("std::fill_n");
	//TODO
	std::fill_n(begin, n, value);
}

template<typename ForwardIterator, typename Size, typename T>
ForwardIterator _uninitialized_fill_n_aux(ForwardIterator begin, Size n,
									const T& value, false_POD){
	ForwardIterator cur = begin;
	while(n--){
		_construct(&*cur, value);
		cur++;
	}
	return cur;
}

template<typename ForwardIterator, typename Size, typename T, typename U>
ForwardIterator _uninitialized_fill_n(ForwardIterator begin, Size n,
									const T& value, U*){
	typedef typename traits_type<U>::is_POD isPOD;
	return _uninitialized_fill_n_aux(begin, n, value, isPOD());
}


/*
 *[begin, begin + n)上用x值初始化
 * */
template<typename ForwardIterator, typename Size, typename T>
ForwardIterator uninitialized_fill_n(ForwardIterator begin, Size n,
									const T& value){
	return _uninitialized_fill_n(begin, n, value, value_type(begin));
}

/*
 * [begin, end) -> [result, result + end - begin)
 */
template<typename ForwardIterator>
void _copy_backward(ForwardIterator begin, ForwardIterator end, ForwardIterator result){
	ForwardIterator tail = result + distance(begin, end);
	while(tail != result){
		*--tail = *--end;
	}
}

/*
 * [begin, end) -> [result, result + end - begin)
 */
template<typename ForwardIterator>
void _copy_forward(ForwardIterator begin, ForwardIterator end, ForwardIterator result){
	while(begin != end){
		*result++ = *begin++;
	}
}

}
#endif
