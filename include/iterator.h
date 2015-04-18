#ifndef __ITERATOR__H__
#define __ITERATOR__H__
/*
 * iterator常用型别定义以及萃取等实现
 */

#include <cstdlib>

namespace hu{

//声明五种迭代器型别
struct input_iterator_tag{};
struct output_iterator_tag{};
struct forward_iterator_tag : public input_iterator_tag{};
struct bidirectional_iterator_tag : public forward_iterator_tag{};
struct random_access_iterator_tag : public bidirectional_iterator_tag{};


//标准iterator,以后所有iterator都要继承它
template<typename Category, typename T,
		typename Pointer = T*, typename Reference = T&,
		typename Difference = ptrdiff_t>
struct iterator{
	typedef Category iterator_category;
	typedef T value_type;
	typedef Pointer pointer_type;
	typedef Reference reference_type;
	typedef Difference difference_type;
};

//类型萃取器
template<typename T>
class traits_iterator{
	typedef typename T::value_type value_type;
	typedef typename T::pointer_type pointer_type;
	typedef typename T::reference_type reference_type;
	typedef typename T::difference_type difference_type;
	typedef typename T::iterator_category iterator_category;
};

//偏特化普通指针T*
template<typename T>
struct traits_iterator<T*>{
	typedef T value_type;
	typedef T* pointer_type;
	typedef T& reference_type;
	typedef ptrdiff_t difference_type;
	typedef random_access_iterator_tag iterator_category;
};

//偏特化普通const指针 const T*
template<typename T>
class traits_iterator<const T*>{
	typedef const T value_type;
	typedef const T* pointer_type;
	typedef T& reference_type;
	typedef ptrdiff_t difference_type;
	typedef random_access_iterator_tag iterator_category;
};

/*求迭代器的value_type*,之所以用value_type*而不是value_type是因为这个函数
 * 通常用来确定迭代器维护的类的类型（比如unintialized_copy函数），所以具体类型的对象
 * 无所谓，传指针就可以满足这个需求，而且拷贝的字节数也只有４字节
 * */
template<typename Iter>
typename traits_iterator<Iter>::value_type* value_type(const Iter&){
	//函数模板的参数只作为确定参数，不参与实际计算
	return static_cast<typename traits_iterator<Iter>::value_type*>(0);
}

//求迭代器的iterator_category
template<typename Iter>
typename traits_iterator<Iter>::iterator_category
iterator_category(const Iter&){
	typedef typename traits_iterator<Iter>::iterator_category category;
	return category();
}




}


#endif
