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
#include "ShMemAlloc.hpp"

namespace shmem {

constexpr size_t MMAP_NUMBER = 1024;

template<typename U>
using ShUniquePtr = std::unique_ptr<U, std::function<void(U*)>>;

template<typename U>
ShUniquePtr<U> make_shmem(size_t n) {
    void* mmap = ::mmap(nullptr, sizeof(U) * n,
                        PROT_WRITE | PROT_READ,
                        MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (mmap == MAP_FAILED)
        throw std::runtime_error(strerror(errno));

    return {reinterpret_cast<U*>(mmap), [n](U* u) { ::munmap(u, sizeof(U) * n); }};
}

template <class Key, class T>
class Map {
public:
    using value_type = std::pair<const Key, T>;
    using comparator = std::less<Key>;
    using ShMapAllocator = ShMemAlloc<value_type>;
    using ShMap = std::map<Key, T, comparator, ShMapAllocator>;
    using iterator = typename ShMap::iterator;

    Map() {
        mmap_ptr_ = make_shmem<value_type>(MMAP_NUMBER);

        AllocState<value_type>* alloc{};
        alloc = reinterpret_cast<AllocState<value_type>*>(mmap_ptr_.get());
        alloc->start = reinterpret_cast<char*>(mmap_ptr_.get()) + sizeof(*alloc);
        alloc->end = reinterpret_cast<char*>(mmap_ptr_.get()) + sizeof(value_type) * MMAP_NUMBER;

        sem_ = Semaphore(reinterpret_cast<sem_t*>(alloc->start));
        alloc->start = static_cast<char*>(alloc->start) + sizeof(*sem_.get());

        auto* map = reinterpret_cast<ShMap*>(alloc->start);
        alloc->start = static_cast<char*>(alloc->start) + sizeof(ShMap);

        ShMapAllocator shmem_alloc{alloc};
        map_ = new (map) ShMap{shmem_alloc};
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

    auto insert(const value_type& value) {
        SemLock sem_lock = SemLock(sem_);
        return map_->insert(value);
    }
    auto insert(typename ShMap::const_iterator hint, const value_type& value) {
        SemLock sem_lock = SemLock(sem_);
        return map_->insert(hint, value);
    }

    void clear() {
        SemLock sem_lock = SemLock(sem_);
        map_->clear();
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
    ShMap* map_;
};

} // namespace shmem


#endif // MAP_HPP
