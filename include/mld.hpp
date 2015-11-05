#ifndef MLD_HPP
#define MLD_HPP

#if (defined(__DEBUG__) || (_DEBUG)) && !defined(NO_MLD) && !defined(_MSC_VER)

    #ifdef _MSC_VER
        #define FUNCTION_NAME __FUNCSIG__
    #elif defined(__GNUC__)
        #define FUNCTION_NAME __PRETTY_FUNCTION__
    #else
        #define FUNCTION_NAME ""
    #endif

    #include <string>
    #include "LeakDetector.hpp"

    inline void* operator new(size_t size, const std::string& file, const std::string& func, const int line)
    {
        return LeakDetector::getInstance().allocate(size, false, file, func, line);
    }

    inline void* operator new[](size_t size, const std::string& file, const std::string& func, const int line)
    {
        return LeakDetector::getInstance().allocate(size, true, file, func, line);
    }

    inline void operator delete(void* ptr)
    {
        LeakDetector::getInstance().free(ptr, false);
    }

    inline void operator delete[](void* ptr)
    {
        LeakDetector::getInstance().free(ptr, true);
    }

    #ifndef new
        #define new new(__FILE__, FUNCTION_NAME, __LINE__)

        #define delete LeakDetector::getInstance().nextDelete(__FILE__, FUNCTION_NAME, __LINE__), delete
    #endif // new

#endif // (defined(__DEBUG__) || (_DEBUG)) && !defined(NO_MLD)

#endif // MLD_HPP
