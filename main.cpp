#include <iostream>
#include <vector>
#include "mld.hpp"
#include "Foo.hpp"

int main()
{
    try
    {
        int* ptr = new int;
        int* ptr2 = new int[3];
        int* ptr3 = ptr2;
        delete ptr;
        delete[] ptr3;


        std::vector<int*> test;
        for (int i(0); i < 3; i++)
        {
            test.push_back(new int(10));
        }

        int yolo = 3;
        ptr = &yolo;
        delete ptr;

        Foo* foo = new Foo();
    }
    catch (std::exception e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
