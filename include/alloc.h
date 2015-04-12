#ifndef __ALLOC__H__
#define __ALLOC__H__

/*
 * 本文件主要实现高效的空间适配器，类似SGI的空间适配器实现，采用一级空间适配器superior_allocator和
 * 二级空间适配器junior_allocator.
 */

#include "def.h"
#include <cstdlib>
#include <cstring>

namespace hu{

template<int inst>
class superior_allocator{
private:
	//allocate when out of memory
	static void* oom_allocate(size_t n);
	static void* oom_reallocate(void* ptr, size_t old_sz, size_t new_sz);
	// use defined handler when out of memory
	static void (*allocate_failed_handler)();

public:
	static void* allocate(size_t n);
	static void deallocate(void* ptr, size_t);
	static void* reallocate(void *ptr, size_t old_sz, size_t new_sz);

	static void set_allocate_failed_handler(void (*f)());
};

template<int inst>
void (* superior_allocator<inst>::allocate_failed_handler)() = 0;

template<int inst>
void* superior_allocator<inst>::allocate(size_t n){
	void* result = malloc(n);
	if(result == NULL){
		return oom_allocate(n);
	}
	return result;
}

template<int inst>
void* superior_allocator<inst>::oom_allocate(size_t n){
	if(allocate_failed_handler == 0)
		throw hu::THROW_BAD_ALLOC;
	void* result = 0;
	while(result == 0){
		allocate_failed_handler(); //调用处理例程，试图释放一些内存
		result = malloc(n);
	}
	return result;
}

template<int inst>
void superior_allocator<inst>::deallocate(void* ptr, size_t){
	free(ptr);
}

template<int inst>
void* superior_allocator<inst>::reallocate(void *ptr,
								size_t old_sz, size_t new_sz){
	void* result = realloc(ptr, new_sz);
	if(result == 0){
		return oom_reallocate(ptr, old_sz, new_sz);
	}
	return result;
}

template<int inst>
void* superior_allocator<inst>::oom_reallocate(void* ptr,
								size_t old_sz, size_t new_sz){
	if(allocate_failed_handler == 0)
		throw THROW_BAD_ALLOC;
	void *result = 0;
	while(result == 0){
		allocate_failed_handler();
		result = realloc(ptr, new_sz);
	}
	return result;
}


#ifdef NOJUNIOR
typedef superior_allocator<0> alloc;
#else
//typedef superior_allocator<0> alloc;
#endif

/*
 * simple_alloc是一个将alloc简单包装的上层类，它一般在容器中经常被使用
 */
template<typename T, typename Alloc = alloc>
class simple_alloc{
public:
	static T* allocate(size_t n){
		return n == 0? 0 : (T*)(Alloc::allocate(n*sizeof(T)));
	}
	static T* allocate(){
		return (T*)Alloc::allocate(sizeof(T));
	}
	static void deallocate(T* ptr, size_t n){
		Alloc::deallocate(ptr, n*sizeof(T));
	}
	static void deallocate(T* ptr){
		Alloc::deallocate(ptr, sizeof(T));
	}
};

}


#endif
