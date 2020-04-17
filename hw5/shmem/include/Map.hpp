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

constexpr size_t mmap_number = 512;

template<typename U>
using ShUniquePtr = std::unique_ptr<U, std::function<void(U*)>>;

template<typename U>
ShUniquePtr<U> make_shmem(size_t n) {
    void* mmap = ::mmap(nullptr, sizeof(U) * n,
                        PROT_WRITE | PROT_READ,
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (mmap == MAP_FAILED)
        throw std::runtime_error(strerror(errno));

    return {reinterpret_cast<U*>(mmap), [n](U* t) { ::munmap(t, sizeof(U) * n); }};
}

template <
    class Key,
    class T
>
class Map {
public:
    using value_type = std::pair<const Key, T>;
    using comparator = std::less<Key>;
    using ShmemMapAllocator = ShmemAlloc<value_type>;
    using Shmap = std::map<Key, T, comparator, ShmemMapAllocator>;
    using iterator = typename Shmap::iterator;

    Map() {
        mmap_ptr_ = make_shmem<value_type>(mmap_number);

        /*
        void* mmap = ::mmap(nullptr, sizeof(value_type) * mmap_number,
                            PROT_WRITE | PROT_READ,
                            MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        if (mmap == MAP_FAILED) {
            throw std::runtime_error(strerror(errno));
        }

        mmap_ = mmap;

        alloc = static_cast<Alloc<value_type>*>(mmap);
        alloc->start = static_cast<char*>(mmap_) + sizeof(*alloc);
        alloc->end = static_cast<char*>(mmap_) + sizeof(value_type) * mmap_number;

        sem_ = Semaphore(reinterpret_cast<sem_t*>(alloc->start));
        alloc->start = static_cast<char*>(alloc->start) + sizeof(*(sem_.get()));

        Shmap* map = reinterpret_cast<Shmap*>(alloc->start);

        alloc->start = static_cast<char*>(alloc->start) + sizeof(Shmap);

        ShmemMapAllocator shalloc(alloc);
        map_ = new (map) Shmap{shalloc};
         */


        alloc = reinterpret_cast<Alloc<value_type>*>(mmap_ptr_.get());
        alloc->start = reinterpret_cast<char*>(mmap_ptr_.get()) + sizeof(*alloc);
        alloc->end = reinterpret_cast<char*>(mmap_ptr_.get()) + sizeof(value_type) * mmap_number;

        sem_ = Semaphore(reinterpret_cast<sem_t*>(alloc->start));
        alloc->start = static_cast<char*>(alloc->start) + sizeof(*sem_.get());

        auto* map = reinterpret_cast<Shmap*>(alloc->start);
        alloc->start = static_cast<char*>(alloc->start) + sizeof(Shmap);

        ShmemMapAllocator shmem_alloc(alloc);
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
    ShUniquePtr<value_type> mmap_ptr_;

//    void* mmap_;
    Alloc<value_type>* alloc;

    Shmap* map_;
};

} // namespace Shmem


#endif // MAP_HPP
