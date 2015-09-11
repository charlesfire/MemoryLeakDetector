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
    m_output << std::endl << "Memory Leak Detector v.1.0" << std::endl;
    if (!m_memory.empty())
    {
        m_output << m_memory.size() << " memory leak detected." << std::endl;
        for (auto it(m_memory.begin()); it != m_memory.end(); it++)
        {
            m_output << "Memory leak detected in function " << it->second.m_func << ", in file " << it->second.m_file << " at line " << it->second.m_line << "." << std::endl;
            std::free(it->first);
        }
    }
    else
        m_output << "No memory leak detected." << std::endl;
}

void* LeakDetector::allocate(size_t size, bool isAnArray, const std::string& file, const std::string& func, const int line)
{
    m_output << std::endl << "Memory Leak Detector v.1.0" << std::endl;
    void* ptr = std::malloc(size);
    m_memory.emplace(ptr, MemoryBlock(isAnArray, file, func, line));
    m_output << "Memory allocated in function " << func << ", in file " << file << " at line " << line << "." << std::endl;
    return ptr;
}

void LeakDetector::free(void* ptr, bool isAnArray)
{
    m_output << std::endl << "Memory Leak Detector v.1.0" << std::endl;

    if (m_memory.find(ptr) != m_memory.end())
    {
        if (m_memory.at(ptr).m_isAnArray == isAnArray)
        {
            m_memory.erase(ptr);
            std::free(ptr);
        }
        else
        {
            if (isAnArray)
            {
                m_output << "Error : Trying to delete a pointer with the delete[] operator in function " << m_nextDeleteStack.top().m_func << ", in file "
                         << m_nextDeleteStack.top().m_file << " at line " << m_nextDeleteStack.top().m_line << "." << std::endl;
            }
            else
            {
                m_output << "Error : Trying to delete an array with the delete operator in function " << m_nextDeleteStack.top().m_func << ", in file "
                         << m_nextDeleteStack.top().m_file << " at line " << m_nextDeleteStack.top().m_line << "." << std::endl;
            }
            m_memory.erase(ptr);
            std::free(ptr);
        }
        m_nextDeleteStack.pop();
    }
    else
    {
        if (m_nextDeleteStack.empty())
            m_output << "Error : Trying to delete unallocated memory" << std::endl;
        else
            m_output << "Error : Trying to delete unallocated memory in function " << m_nextDeleteStack.top().m_func << ", in file "
                     << m_nextDeleteStack.top().m_file << " at line " << m_nextDeleteStack.top().m_line << "." << std::endl;
    }
}

LeakDetector& LeakDetector::getInstance()
{
    static LeakDetector instance;
    std::cout << "Leak Detector called " << &instance << std::endl;
    return instance;
}

void LeakDetector::nextDelete(const std::string& file, const std::string& func, const int line)
{
    m_nextDeleteStack.push(MemoryBlock(false, file, func, line));
}
