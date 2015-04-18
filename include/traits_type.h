#ifndef __TRAITS_TYPE__H__
#define __TRAITS_TYPE__H__

namespace hu{

struct true_POD{};
struct false_POD{};

//默认所有类型都是false_POD
template<typename T>
struct	traits_type{
	typedef false_POD is_POD;
};


//偏特化版本
template<typename T>
struct	traits_type<T*>{
	typedef true_POD is_POD;
};


template<>
struct traits_type<char>{
	typedef true_POD is_POD;
};

template<>
struct traits_type<bool>{
	typedef true_POD is_POD;
};

template<>
struct traits_type<int>{
	typedef true_POD is_POD;
};

template<>
struct traits_type<long>{
	typedef true_POD is_POD;
};

template<>
struct traits_type<float>{
	typedef true_POD is_POD;
};

template<>
struct traits_type<double>{
	typedef true_POD is_POD;
};


/*TODO
 * unsigned int, unsigned char, ......
 */

}

#endif
