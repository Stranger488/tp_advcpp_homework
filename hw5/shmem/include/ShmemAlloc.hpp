#ifndef SHMEMALLOC_HPP
#define SHMEMALLOC_HPP


#include <new>
#include <cstring>

namespace Shmem {

template<typename T>
struct Alloc {
    char* start;
    char* end;
};

template<typename T>
struct ShmemAlloc {
public:
    using value_type = T;

    template <class U>
    struct rebind { typedef ShmemAlloc<U> other; };

    ShmemAlloc(Alloc<value_type>* other) {
        alloc_ = other;
    }

    ShmemAlloc(const ShmemAlloc<value_type>& other) {
        alloc_ = other.alloc_;
    }

    template <typename U>
    ShmemAlloc(const ShmemAlloc<U>& alloc) {
        alloc_ = reinterpret_cast<Alloc<value_type>*>(alloc.alloc_);
    }

    value_type* allocate(std::size_t n) {
        if (alloc_->start + sizeof(value_type) * n > alloc_->end) {
            throw std::bad_alloc{};
        }
        alloc_->start += sizeof(value_type) * n;

        return reinterpret_cast<value_type*>(alloc_->start);
    }

    void deallocate(value_type* p, std::size_t n) noexcept {
        if (alloc_->start - n * sizeof(value_type) == reinterpret_cast<char*>(p)) {
            alloc_->start -= n * sizeof(value_type);
        }
    }


    Alloc<value_type>* alloc_{};
};

template <class T, class U>
bool operator==(const ShmemAlloc<T>& left, const ShmemAlloc<U>& right) {
    return left == right;
}

template <class T, class U>
bool operator!=(const ShmemAlloc<T>& left, const ShmemAlloc<U>& right)  {
    return left != right;
}

} // namespace Shmem


#endif // SHMEMALLOC_HPP
