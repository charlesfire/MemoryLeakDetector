#ifndef MLD_HPP
#define MLD_HPP

#ifdef __DEBUG__

    #include "LeakDetector.hpp"
    #include <iostream>

    inline void* operator new(size_t size, const std::string& file, const std::string& func, const int line)
    {
        return LeakDetector::getInstance().allocate(size, false, file, func, line);
    }

    inline void* operator new[](size_t size, const std::string& file, const std::string& func, const int line)
    {
        return LeakDetector::getInstance().allocate(size, true, file, func, line);
    }

    inline void operator delete(void* ptr) noexcept
    {
        LeakDetector::getInstance().free(ptr, false);
    }

    inline void operator delete[](void* ptr) noexcept
    {
        LeakDetector::getInstance().free(ptr, true);
    }

    #define new new(__FILE__, __func__, __LINE__)

    #define delete LeakDetector::getInstance().nextDelete(__FILE__, __func__, __LINE__), delete

#endif // __DEBUG__

#endif // MLD_HPP
