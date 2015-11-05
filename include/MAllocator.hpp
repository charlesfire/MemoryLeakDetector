#ifndef MALLOCATOR_HPP
#define MALLOCATOR_HPP

#include <cstdlib>
#include <limits>
#include <new>

template<class T>
struct MAllocator
{
    typedef T value_type;
    typedef value_type* pointer;
    typedef const value_type* const_pointer;
    typedef value_type& reference;
    typedef const value_type& const_reference;
    typedef std::size_t size_type;
    typedef std::ptrdiff_t difference_type;

    template<class U>
    struct rebind
    {
        typedef MAllocator<U> other;
    };

    inline MAllocator() {}
    inline ~MAllocator() {}
    inline MAllocator(const MAllocator&) {}
    template<class U>
    inline MAllocator(const MAllocator<U>&) {}

    inline pointer address(reference r)const { return &r; }
    inline const_pointer address(const_reference r)const { return &r; }

    inline pointer allocate(size_type cnt, const void* = nullptr)const
    {
        if (cnt == 0)
            return nullptr;
        pointer ptr = reinterpret_cast<pointer>(std::malloc(cnt * sizeof(T)));
        if (ptr == nullptr)
            throw std::bad_alloc();
        return ptr;
    }

    inline void deallocate(pointer p, size_type)const
    {
        std::free(p);
    }

    inline size_type max_size()const
    {
        return std::numeric_limits<size_type>::max() / sizeof(T);
    }

    inline void construct(pointer p, const T& t)const
    {
        new(p) T(t);
    }

    template<class ...Args>
    inline void construct(pointer p, Args... args)const
    {
        new(p) T(args...);
    }

    inline void destroy(pointer p)const { p->~T(); }

    inline bool operator==(MAllocator const&)const { return true; }
    inline bool operator!=(MAllocator const& a)const { return !operator==(a); }
};

#endif // MALLOCATOR_HPP
