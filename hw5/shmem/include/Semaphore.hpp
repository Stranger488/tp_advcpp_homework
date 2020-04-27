#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP


#include <semaphore.h>
#include <stdexcept>
#include <cstring>

namespace shmem {

class Semaphore {
public:
    Semaphore() {
        if (::sem_init(&sem_, 1, 1) < 0) {
            throw std::runtime_error(std::strerror(errno));
        }
    }
    ~Semaphore() noexcept {

    }

    void destroy() {
        ::sem_destroy(&sem_);
    }

    void lock() {
        if(::sem_wait(&sem_) < 0) {
            throw std::runtime_error(std::strerror(errno));
        }
    }

    void unlock() {
        if (::sem_post(&sem_) < 0) {
            throw std::runtime_error(std::strerror(errno));
        }
    }

private:
    sem_t sem_;
};

class SemLock {
public:
    SemLock(Semaphore* sem) : sem_lock_(sem) {
        sem_lock_->lock();
    }

    ~SemLock() {
        sem_lock_->unlock();
    }

private:
    Semaphore* sem_lock_;
};

} // namespace shmem


#endif // SEMAPHORE_HPP
