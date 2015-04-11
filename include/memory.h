#ifndef __MEMORY__H__
#define __MEMORY__H__
/*
本文件主要实现负责最底层的memory相关工作的函数实现，比如_allocate, _deallocate, uninitialized_copy等等
*/

#include "ctype.h"
#include "out.h"
#include <cstddef>

namespace hu{

template<typename T>
inline T* _allocate(ptrdiff_t size, T*){
	T* tmp = (T*)(::operator new((size_t)(size*sizeof(T)))); // ::operator new 最底层的内存分配，类似malloc
	if(tmp == 0){
		qError("out of memory, allocate failed.");
	}
	qDebug("_allocate");
	return tmp;
}

template<typename T>
inline void _deallocate(T* p){
	::operator delete(p);
	qDebug("_deallocate");
}

template<typename T1, typename T2>
inline void _construct(T1* p, const T2& value){
	new(p) T1(value); //new 指定内存位置的用法
	qDebug("_construct");
}

template<typename T>
inline void _destroy(T* p){
	p->~T();
	qDebug("_destroy");
}

}

#endif
