#ifndef MAP_HPP
#define MAP_HPP


#include <sys/mman.h>
#include <cstring>
#include <stdexcept>
#include <semaphore.h>
#include <map>

#include "Semaphore.hpp"
#include "ShmemAlloc.hpp"

namespace Shmem {

constexpr size_t MMAP_SIZE = 4096;

template <
        class Key,
        class T,
        class Compare = std::less<Key>,
        class Allocator = ShmemAlloc<std::pair<const Key, T>>
        >
class Map {
public:
    Map() {
        void* mmap = ::mmap(nullptr, MMAP_SIZE, PROT_WRITE | PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if (mmap == MAP_FAILED) {
            throw std::runtime_error(std::strerror(errno));
        }

        auto* state = static_cast<AllocState*>(mmap);
        state->start = static_cast<char*>(mmap) + sizeof(*state);
        state->end = static_cast<char*>(mmap) + MMAP_SIZE;

        auto* semaphore = reinterpret_cast<sem_t*>(state->start);
        sem_ = Semaphore(semaphore);

        using ShmemMapAlloc = ShmemAlloc<std::pair<const Key, T>>;

        auto* map = reinterpret_cast< std::map<Key, T, ShmemMapAlloc >* >(state->start);
        state->start = static_cast<char*>(state->start) + sizeof(std::map< Key, T, ShmemMapAlloc >);

        ShmemMapAlloc shmem_alloc(state);
        map_ = new (map) std::map<Key, T, ShmemMapAlloc>{shmem_alloc};
    }

    ~Map() {
        map_->~Map<T>();
    }

    auto insert(const std::pair<const Key, T>& value) {
        SemLock sem_lock = SemLock(sem_);
        return map_->insert(value);
    }

    std::size_t erase(const Key& key) {
        SemLock sem_lock = SemLock(sem_);
        return map_->erase(key);
    }

    T& at(const Key& key) {
        SemLock sem_lock = SemLock(sem_);
        return map_->at(key);
    }

    const T& at(const Key& key) const {
        SemLock sem_lock = SemLock(sem_);
        return map_->at(key);
    }

private:
    Semaphore sem_;
    std::map<Key, T, ShmemAlloc<T>>* map_;
};

} // namespace Shmem


#endif // MAP_HPP
