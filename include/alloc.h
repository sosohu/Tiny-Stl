#ifndef __ALLOC__H__
#define __ALLOC__H__

/*
 * 本文件主要实现高效的空间适配器，类似SGI的空间适配器实现，采用一级空间适配器superior_allocator和
 * 二级空间适配器junior_allocator.
 */

#include "def.h"
#include <cstdlib>
#include <cstring>
#include <cassert>

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

const size_t LARGE_SIZE = 128; //超过128B就由superior_allocator分配
const size_t SLICE_SIZE = 8; //递增片大小
const size_t FREE_COUNT = LARGE_SIZE/SLICE_SIZE; //free list个数，8,16,...128
const size_t CHUNK_NUM = 20; //默认每个尺寸的chunk一次分配的个数

template<int inst>
class junior_allocator{
private:
	union obj{
		obj* next;
		char data[];
	};
	static obj* free_list[FREE_COUNT];
	static char* start; //当前可分配内存池的开始位置
	static char* finish; //内存池结束位置
	static size_t heap_size; //当前堆大小

	static size_t march_node(size_t size);
	static size_t node_size(size_t node_id);

	//分配n个node_id块
	static char* chunk_alloc(size_t node_id, size_t& n);
	static void* refill(size_t node_id);
public:

	static void* allocate(size_t n);
	static void* reallocte(void* ptr, size_t old_zs, size_t new_sz);
	static void deallocate(void* ptr, size_t);

};

template<int inst>
char* junior_allocator<inst>::start = 0;

template<int inst>
char* junior_allocator<inst>::finish = 0;

template<int inst>
size_t junior_allocator<inst>::heap_size = 0;

template<int inst>
typename junior_allocator<inst>::obj* junior_allocator<inst>::free_list[FREE_COUNT]
					= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

template<int inst>
size_t junior_allocator<inst>::march_node(size_t size){
	return (size-1) / SLICE_SIZE;
}

template<int inst>
size_t junior_allocator<inst>::node_size(size_t node_id){
	return (node_id + 1) * SLICE_SIZE;
}

template<int inst>
char* junior_allocator<inst>::chunk_alloc(size_t node_id, size_t &n){
	size_t requre = n*node_size(node_id);
	size_t size_left = finish - start;
	char* result;
	if(size_left < requre){//剩余空间不够
		if(node_size(node_id) < size_left){
			//还够分配一个node
			n = 1;
			result = start;
			start = start + node_size(node_id);
			return result;
		}else{//一个都不够分配了
			if(size_left > 0){
				//把剩余空间分配给相应free_list
				/*
				 * 由于每次分配堆大小都是８的整数倍，所以剩余的肯定属于8,16,24,...必然有一个
				 * 与它完全适配
				 */
				size_t node_march = march_node(size_left);
				assert(size_left == node_size(node_march));
				obj* cur = (obj*)start;
				cur->next = free_list[node_march];
				free_list[node_march] = cur;
				start = finish;
			}
			size_t add_size = requre + (heap_size >> 4);
			start = (char*)(::operator new(add_size));
			if(start == 0){
				//分配内存失败
				//TODO 应该从合适的free_list中回收一定的内存


				//实在不行调用一级空间适配器以调用用户定义的处理历程
				start = (char*)(superior_allocator<inst>::allocate(add_size));
			}
			finish = start + add_size;
		}
	}
	result = start;
	start = start + requre;
	return result;
}

template<int inst>
void* junior_allocator<inst>::refill(size_t node_id){
	 size_t chunk_num = CHUNK_NUM;
	 void* result;
	 result = chunk_alloc(node_id, chunk_num);
	 //chunk_num的值为实际分配到的块数目
	 obj* pos = (obj*)result;
	 for(auto i = 1; i < chunk_num; i++){
		 obj* next = (obj*)((char*)pos + node_size(node_id));
		 pos->next = next;
		 pos = pos->next;
	 }
	 pos->next = 0;
	 return result;
}

template<int inst>
void* junior_allocator<inst>::allocate(size_t n){
	if(n > LARGE_SIZE){
		//超过LARGE_SIZE交给superior_allocator分配
		return superior_allocator<inst>::allocate(n);
	}else{
		size_t node_id = march_node(n);
		/*
		 * free_list[node_id]是一个union, 其中next表示下一个可用的块位置，data表示可用块的位置
		 */
		if(free_list[node_id] == 0){
			//已经没有资源了
			free_list[node_id] = (obj*)refill(node_id);
		}
		void* result = free_list[node_id]->data;
		free_list[node_id] = free_list[node_id]->next;
		return result;
	}
}

template<int inst>
void* junior_allocator<inst>::reallocte(void* ptr, size_t old_sz, size_t new_sz){
	void* new_ptr = allocate(new_sz);
	size_t copy_sz = old_sz < new_sz? old_sz : new_sz;
	memcpy(new_ptr, ptr, copy_sz);
	deallocate(ptr, old_sz);
	return new_ptr;
}

template<int inst>
void junior_allocator<inst>::deallocate(void* ptr, size_t sz){
	if(sz > LARGE_SIZE)
		superior_allocator<inst>::deallocate(ptr, sz);
	size_t node_march = march_node(sz);
	obj* pos = (obj*)ptr;
	pos->next = free_list[node_march];
	free_list[node_march] = pos;
}


#ifdef NOJUNIOR
typedef superior_allocator<0> alloc;
#else
typedef junior_allocator<0> alloc;
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
