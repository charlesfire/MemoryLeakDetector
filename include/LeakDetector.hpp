#ifndef LEAKDETECTOR_HPP
#define LEAKDETECTOR_HPP

#include <unordered_map>
#include <stack>
#include <string>
#include <ostream>
#include "MAllocator.hpp"

class LeakDetector
{
    private:
        struct MemoryBlock
        {
            MemoryBlock()=delete;
            MemoryBlock(bool isAnArray, const std::string& file, const std::string& func, const int line);

            bool m_isAnArray;
            std::string m_file;
            std::string m_func;
            int m_line;
        };

    public:
        virtual ~LeakDetector();
        void* allocate(size_t size, bool isAnArray, const std::string& file, const std::string& func, const int line);
        void free(void* ptr, bool isAnArray);
        static LeakDetector& getInstance();
        void nextDelete(const std::string& file, const std::string& func, const int line);
    private:
        LeakDetector();
        std::unordered_map<void*, MemoryBlock, std::hash<void*>, std::equal_to<void*>, MAllocator<std::pair<const void*, MemoryBlock>>> m_memory;
        std::ostream& m_output;
        std::stack<MemoryBlock, std::deque<MemoryBlock, MAllocator<MemoryBlock>>> m_nextDeleteStack;
};

#endif // LEAKDETECTOR_HPP
