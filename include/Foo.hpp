#ifndef FOO_HPP
#define FOO_HPP

#include "mld.hpp"

class Foo
{
    public:
        Foo();
        virtual ~Foo();
    private:
        int* ptr;
        int* ptr2;
        int* ptr3 = new int;
};

#endif // FOO_HPP
