#include <iostream>
#include "Tableau.hpp"
#include "mld.hpp"

using namespace std;

int main()
{
    try
    {
        std::cout << "test" << std::endl;
        int* ptr = new int;
        std::cout << "test" << std::endl;
        int* ptr2 = new int;
        std::cout << "test" << std::endl;
        //LeakDetector::getInstance().nextDelete(__FILE__, __func__, __LINE__), delete ptr;
    }
    catch (std::exception e)
    {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
