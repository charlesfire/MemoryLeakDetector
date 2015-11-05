#include "Foo.hpp"

Foo::Foo()
{
    ptr = new int(6);
    ptr2 = new int[2]{1, 4};
}

Foo::~Foo()
{
    //dtor
}
