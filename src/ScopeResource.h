#ifndef _SCOPE_RESOURCE_GUARD_H_
#define _SCOPE_RESOURCE_GUARD_H_

#include <memory>

template <class T>
struct Deleter
{
    void operator()(T *rhs) const{
        rhs->release();
        std::cerr<<"RElease\n";
    }
};

template <class T>
auto Protect(T &rhs) -> std::shared_ptr<T>
{
    return std::shared_ptr<T>(&rhs, Deleter<T>());
}



#endif