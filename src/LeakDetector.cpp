#include "LeakDetector.hpp"
#include <cstdlib>
#include <iostream>

LeakDetector::LeakDetector() : m_memory(), m_output(std::cerr), m_nextDeleteStack()
{
    //ctor
}

LeakDetector::MemoryBlock::MemoryBlock(bool isAnArray, const std::string& file, const std::string& func, int line) : m_isAnArray(isAnArray), m_file(file), m_func(func), m_line(line)
{

}

LeakDetector::~LeakDetector()
{
    m_output << std::endl << std::endl << "------- Memory Leak Detector v.1.0 -------" << std::endl;
    if (!m_memory.empty())
    {
        std::size_t totalMemoryLeakSize = 0;
        for (auto memoryBlock : m_memory)
            totalMemoryLeakSize += sizeof(memoryBlock.first);
        m_output << m_memory.size() << " memory leaks detected totalizing " << totalMemoryLeakSize << " bytes." << std::endl << std::endl;
        for (auto memoryBlock : m_memory)
        {
            m_output << "Memory leak of " << sizeof(memoryBlock.first) << " bytes detected." << std::endl;
            m_output << "\tIn file " << memoryBlock.second.m_file << std::endl;
            if (memoryBlock.second.m_file != "")
                m_output << "\tIn function " << memoryBlock.second.m_func << std::endl;
            m_output << "\tAt line " << memoryBlock.second.m_line << std::endl << std::endl;
            std::free(memoryBlock.first);
        }
        m_output << m_memory.size() << " memory leaks detected totalizing " << totalMemoryLeakSize << " bytes.";
    }
    else
        m_output << "No memory leak detected." << std::endl;

    m_output << std::endl << std::endl << "------------------------------------------" << std::endl;
}

void* LeakDetector::allocate(size_t size, bool isAnArray, const std::string& file, const std::string& func, const int line)
{
    void* ptr = std::malloc(size);
    m_memory.emplace(ptr, MemoryBlock(isAnArray, file, func, line));
    return ptr;
}

void LeakDetector::free(void* ptr, bool isAnArray)
{
    if (m_memory.find(ptr) != m_memory.end())
    {
        if (m_memory.at(ptr).m_isAnArray == isAnArray)
        {
            m_memory.erase(ptr);
            std::free(ptr);
        }
        else
        {
            m_output << std::endl << std::endl << "------- Memory Leak Detector v.1.0 -------" << std::endl;
            if (isAnArray)
            {
                m_output << "Error : Trying to delete single element pointer with delete[] operator in file " << m_nextDeleteStack.top().m_file << ", ";
                if (m_nextDeleteStack.top().m_func != "")
                    m_output << "in function " << m_nextDeleteStack.top().m_func << ", ";
                m_output << "at line " << m_nextDeleteStack.top().m_line << "." << std::endl;
            }
            else
            {
                m_output << "Error : Trying to delete array pointer with delete operator in file " << m_nextDeleteStack.top().m_file << ", ";
                if (m_nextDeleteStack.top().m_func != "")
                    m_output << "in function " << m_nextDeleteStack.top().m_func << ", ";
                m_output << "at line " << m_nextDeleteStack.top().m_line << "." << std::endl;
            }
            m_memory.erase(ptr);
            std::free(ptr);
        }
        m_nextDeleteStack.pop();
    }
    else
    {
        if (!m_nextDeleteStack.empty())
        {
            m_output << std::endl << std::endl << "------- Memory Leak Detector v.1.0 -------" << std::endl;
            m_output << "Error : Trying to delete non dynamic memory in file " << m_nextDeleteStack.top().m_file << ", ";
            if (m_nextDeleteStack.top().m_func != "")
                m_output << "in function " << m_nextDeleteStack.top().m_func << ", ";
            m_output << "at line " << m_nextDeleteStack.top().m_line << "." << std::endl;
        }
    }
}

LeakDetector& LeakDetector::getInstance()
{
    static LeakDetector instance;
    return instance;
}

void LeakDetector::nextDelete(const std::string& file, const std::string& func, const int line)
{
    m_nextDeleteStack.push(MemoryBlock(false, file, func, line));
}
