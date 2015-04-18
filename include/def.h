#ifndef __DEF__H__
#define __DEF__H__

#include <new>
#include <exception>
#include <stdexcept>

namespace hu{

std::bad_alloc BAD_ALLOC;
std::out_of_range OUT_OF_RANGE("out of range");
#define THROW_BAD_ALLOC throw BAD_ALLOC
#define THROW_OUT_OF_RANGE throw OUT_OF_RANGE;

}
#endif
