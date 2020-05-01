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
    using ShStrAllocator = ShMemAlloc<char>;
    using ShStrType = std::basic_string<char, std::char_traits<char>, ShStrAllocator>;

    using MapKey = std::conditional_t<std::is_same_v<Key, std::string>,
            ShStrType, Key>;
    using MapT = std::conditional_t<std::is_same_v<T, std::string>,
            ShStrType, T>;

    using value_type = std::pair<const Key, T>;
    using map_value_type = std::pair<const MapKey, MapT>;

    using comparator = std::less<MapKey>;
    using ShMapAllocator = ShMemAlloc<map_value_type>;
    using ShMap = std::map<MapKey, MapT, comparator, ShMapAllocator>;
    using iterator = typename ShMap::iterator;

    Map(size_t mmap_number) {
        size_t n = sizeof(map_value_type) * mmap_number;
        mmap_ptr_ = make_shmem<char>(n);

        AllocState* alloc{};
        alloc = reinterpret_cast<AllocState*>(mmap_ptr_.get());
        alloc->start = reinterpret_cast<char*>(mmap_ptr_.get()) + sizeof(*alloc);
        alloc->end = reinterpret_cast<char*>(mmap_ptr_.get()) + n * sizeof(char);

        auto* sem = reinterpret_cast<sem_t*>(alloc->start);
        sem_ = new (sem) Semaphore();
        alloc->start = static_cast<char*>(alloc->start) + sizeof(*sem_);

        auto* map = reinterpret_cast<ShMap*>(alloc->start);
        alloc->start = static_cast<char*>(alloc->start) + sizeof(ShMap);

        ShMapAllocator shmem_alloc{alloc};
        map_ = new (map) ShMap{shmem_alloc};
    }

    ~Map() {

    }

    void destroy() {
        sem_->destroy();
        map_->~map();
    }

    auto convert(const ShStrType& shstr) {
        return std::string(shstr);
    }
    auto convert(const std::string& str) {
        return ShStrType{str, map_->get_allocator()};
    }
    template<typename P>
    auto convert(P p) {
        return p;
    }

    auto at(const Key& key) {
        auto sem_lock = SemLock(sem_);
        return map_->at(convert(key));
    }

    auto operator[](const Key& key) {
        auto sem_lock = SemLock(sem_);
        return map_->operator[](convert(key));
    }

    auto operator[](Key&& key) {
        auto sem_lock = SemLock(sem_);
        return map_->operator[](std::move(convert(key)));
    }

    auto insert(const value_type& value) {
        SemLock sem_lock = SemLock(sem_);
        return map_->insert({convert(value.first), convert(value.second)});
    }

    auto insert(typename ShMap::const_iterator hint, const value_type& value) {
        SemLock sem_lock = SemLock(sem_);
        return map_->insert(hint, {convert(value.first), convert(value.second)});
    }

    void clear() {
        SemLock sem_lock = SemLock(sem_);
        map_->clear();
    }
    auto erase(const Key& key) {
        SemLock sem_lock = SemLock(sem_);
        return map_->erase(convert(key));
    }
    auto erase(iterator position) {
        SemLock sem_lock = SemLock(sem_);
        return map_->erase(position);
    }

private:
    Semaphore* sem_;
    ShUniquePtr<char> mmap_ptr_;
    ShMap* map_;
};

} // namespace shmem


#endif // MAP_HPP
