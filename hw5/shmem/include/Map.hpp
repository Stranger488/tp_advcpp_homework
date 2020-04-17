#ifndef MAP_HPP
#define MAP_HPP


#include <sys/mman.h>
#include <cstring>
#include <stdexcept>
#include <semaphore.h>
#include <map>
#include <functional>
#include <memory>

#include "Semaphore.hpp"
#include "ShmemAlloc.hpp"

namespace Shmem {

constexpr size_t mmap_number = 4;

template<typename T>
using ShUniquePtr = std::unique_ptr<T, std::function<void(T*)>>;

template<typename T>
ShUniquePtr<T> make_shmem(size_t n) {
    void* mmap = ::mmap(nullptr, sizeof(T) * n,
                        PROT_WRITE | PROT_READ,
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (mmap == MAP_FAILED)
        throw std::runtime_error(strerror(errno));

    return {reinterpret_cast<T*>(mmap), [n](T* t) { ::munmap(t, sizeof(T) * n); }};
}

template <class Key, class T>
class Map {
public:
    using value_type = std::pair<const Key, T>;
    using comparator = std::less<Key>;
    using ShmemMapAllocator = ShmemAlloc<value_type>;
    using Shmap = std::map<Key, T, comparator, ShmemMapAllocator>;
    using iterator = typename Shmap::iterator;

    Map() {
        ShUniquePtr<value_type> mmap = make_shmem<value_type>();

        auto* state = static_cast<AllocState*>(mmap.get());
        state->start = static_cast<char*>(mmap) + sizeof(*state);
        state->end = static_cast<char*>(mmap) + sizeof(T) * mmap_number;

        auto* semaphore = reinterpret_cast<sem_t*>(state->start);
        sem_ = Semaphore(semaphore);

        auto* map = reinterpret_cast<Shmap*>(state->start);
        state->start = static_cast<char*>(state->start) + sizeof(Shmap);

        ShmemMapAllocator shmem_alloc(state);
        map_ = new (map) Shmap{shmem_alloc};
    }

    ~Map() {
        map_->~map();
    }

    T& at(const Key& key) {
        SemLock sem_lock = SemLock(sem_);
        return map_->at(key);
    }

    const T& at(const Key& key) const {
        SemLock sem_lock = SemLock(sem_);
        return map_->at(key);
    }

    T& operator[](const Key& key) {
        SemLock sem_lock = SemLock(sem_);
        return map_->operator[](key);
    }

    T& operator[](Key&& key) {
        SemLock sem_lock = SemLock(sem_);
        return map_->operator[](std::move(key));
    }

    auto begin() {
        return map_->begin();
    }

    auto cbegin() const {
        return map_->cbegin();
    }

    auto end() {
        return map_->end();
    }

    auto cend() const {
        return map_->cend();
    }

    void clear() {
        map_->clear();
    }

    auto insert(const value_type & value) {
        SemLock sem_lock = SemLock(sem_);
        return map_->insert(value);
    }

    auto erase(const Key& key) {
        SemLock sem_lock = SemLock(sem_);
        return map_->erase(key);
    }

    auto erase(iterator position) {
        SemLock sem_lock = SemLock(sem_);
        return map_->erase(position);
    }

private:
    Semaphore sem_;
    std::map<Key, T, ShmemMapAllocator>* map_;
};

} // namespace Shmem


#endif // MAP_HPP
