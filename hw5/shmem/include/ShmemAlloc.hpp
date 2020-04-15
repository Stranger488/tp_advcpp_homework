#ifndef SHMEMALLOC_HPP
#define SHMEMALLOC_HPP


#include <new>
#include <cstring>

namespace Shmem {

struct AllocState {
    char *start;
    char *end;
};

template<typename T>
struct ShmemAlloc {
    explicit ShmemAlloc(AllocState* state) {
        state_ = state;
    }

    ShmemAlloc(const ShmemAlloc& other) noexcept {
        state_ = other.state_;
    }

    T* allocate(std::size_t n) {
        char* res = state_->start;

        if (res + sizeof(T) * n > state_->end) {
            throw std::bad_alloc{};
        }
        state_->start += sizeof(T) * n;

        return reinterpret_cast<T*>(res);
    }

    void deallocate(T* p, std::size_t n) noexcept {
        if (state_->start - n * sizeof(T) == reinterpret_cast<char*>(p)) {
            state_->start -= n * sizeof(T);
        }
    }

private:
    AllocState *state_;
};

} // namespace Shmem


#endif // SHMEMALLOC_HPP
