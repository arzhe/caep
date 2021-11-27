#ifndef CAEP_MUTEX_LOCK_H
#define CAEP_MUTEX_LOCK_H

#include <pthread.h>

#include "./noncopyable.h"

namespace caep {

class MutexLock : noncopyable {
private:
    pthread_mutex_t mutex;

public:
    MutexLock() {
        pthread_mutex_init(&mutex, NULL);
    }

    ~MutexLock() {
        pthread_mutex_lock(&mutex);
        pthread_mutex_destroy(&mutex);
    }

    void lock() {
        pthread_mutex_lock(&mutex);
    }

    void unlock() {
        pthread_mutex_unlock(&mutex);
    }

    pthread_mutex_t* get() {
        return &mutex;
    }

private:
    friend class Condition;
};

class MutexLockGuard : noncopyable {
private:
    MutexLock& mutex;

public:
    explicit MutexLockGuard(MutexLock& mutex) : mutex(mutex) {
        this->mutex.lock();
    }

    ~MutexLockGuard() {
        this->mutex.unlock();
    }
};

}
#endif
