#ifndef SHMEMALLOC_HPP
#define SHMEMALLOC_HPP


#include <new>
#include <cstring>

namespace shmem {

struct AllocState {
    char* start;
    char* end;
};

template<typename T>
struct ShMemAlloc {
public:
    using value_type = T;

    ShMemAlloc(AllocState* other) {
        alloc_ = other;
    }

    template <typename U>
    ShMemAlloc(const ShMemAlloc<U>& other) {
        alloc_ = other.alloc_;
    }

    value_type* allocate(std::size_t n) {
        if (alloc_->start + sizeof(value_type) * n > alloc_->end) {
            throw std::bad_alloc();
        }
        alloc_->start += sizeof(value_type) * n;

        return reinterpret_cast<value_type*>(alloc_->start);
    }

    void deallocate(value_type* p, std::size_t n) noexcept {
        if (alloc_->start - n * sizeof(value_type) == reinterpret_cast<char*>(p)) {
            alloc_->start -= n * sizeof(value_type);
        }
    }

public:
    AllocState* alloc_{};
};

template <class T, class U>
bool operator==(const ShMemAlloc<T>& left, const ShMemAlloc<U>& right) {
    return left.alloc_ == right.alloc_;
}

template <class T, class U>
bool operator!=(const ShMemAlloc<T>& left, const ShMemAlloc<U>& right)  {
    return left.alloc_ != right.alloc_;
}

} // namespace shmem


#endif // SHMEMALLOC_HPP
