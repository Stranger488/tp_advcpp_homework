#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP


#include <semaphore.h>
#include <stdexcept>
#include <cstring>

namespace Shmem {

class Semaphore {
public:
    Semaphore() {
        sem_ = nullptr;
    }

    Semaphore(sem_t* ptr) {
        sem_ = ptr;

        if (::sem_init(sem_, 1, 1) < 0) {
            throw std::runtime_error(std::strerror(errno));
        }
    }
    ~Semaphore() noexcept {
        if (sem_) {
            ::sem_destroy(sem_);
        }
    }

    void lock() {
        if(::sem_wait(sem_) < 0) {
            throw std::runtime_error(std::strerror(errno));
        }
    }

    void unlock() {
        if (::sem_post(sem_) < 0) {
            throw std::runtime_error(std::strerror(errno));
        }
    }

    const sem_t* get() const {
        return sem_;
    }

private:
    sem_t* sem_{};
};

class SemLock {
public:
    SemLock(Semaphore& sem) : sem_lock_(sem) {
        sem_lock_.lock();
    }

    ~SemLock() {
        sem_lock_.unlock();
    }

private:
    Semaphore& sem_lock_;
};

} // namespace Shmem


#endif // SEMAPHORE_HPP
