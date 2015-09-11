#include <iostream>
#include "Tableau.hpp"
#include "mld.hpp"

using namespace std;

int main()
{
    try
    {
        std::cout << "test*****************" << std::endl;
        int* ptr = new int;
        std::cout << "test-----------------" << std::endl;
        int* ptr2 = new int;
        std::cout << "test+++++++++++++++++" << std::endl;
        delete ptr;
    }
    catch (std::exception e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
