#ifndef __ALGORITHM__H__
#define __ALGORITHM__H__

#include "iterator.h"

namespace hu{

template<typename InputIterator>
typename traits_iterator<InputIterator>::difference_type
_distance(InputIterator iter1, InputIterator iter2,
		input_iterator_tag category){
	typedef typename traits_iterator<InputIterator>::difference_type difference_type;
	difference_type d = 0;
	while(iter1++ != iter2){
		d++;
	}
	return d;
}

//偏特化
template<typename InputIterator>
typename traits_iterator<InputIterator>::difference_type
_distance(InputIterator iter1, InputIterator iter2,
		random_access_iterator_tag category){
	return iter2 - iter1;
}

template<typename InputIterator>
typename traits_iterator<InputIterator>::difference_type
distance(InputIterator iter1, InputIterator iter2){
	typedef typename traits_iterator<InputIterator>::iterator_category category;
	return _distance(iter1, iter2, category());
}

}


#endif
