/* $Header:  ~/code/GitRepositories/MyGit/caep   2.1.0   22 Aug 2019 19:00:00   ArZr        $ */
/***********************************************************************************************
 ***                  C O N F I D E N T I A L  ---  A R Z R  S T U D I O S                   ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Caep                                                         *
 *                                                                                             *
 *                    File Name : mutex_lock.h                                                 *
 *                                                                                             *
 *                   Programmer : Guan Zhe                                                     *
 *                                                                                             *
 *                   Start Date : Aug 22, 2019                                                 *
 *                                                                                             *
 *                  Last Update : Dec 1, 2021   [ArZr]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   MutexLock::MutexLock -- Constructor for MutexLock.                                        *
 *   MutexLock::~MutexLock -- Destructor for MutexLock.                                        *
 *   MutexLock::Lock -- Locks current mutex lock.                                              *
 *   MutexLock::Unlock -- Unlocks current mutex lock.                                          *
 *   MutexLock::Get -- Gets the pointer to current mutex lock.                                 *
 *                                                                                             *
 *   MutexLockGuard::MutexLockGuard -- Constructor for MutexLockGuard.                         *
 *   MutexLockGuard::~MutexLockGuard -- Destructor for MutexLockGuard.                         *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef CAEP_MUTEX_LOCK_H
#define CAEP_MUTEX_LOCK_H

#include <pthread.h>

#include "./noncopyable.h"

namespace caep {

/*
 * @breif Encapsulates the mutex, you can use it directly instead of pthread_mutex_t.
 * It slightly simplfies operations on mutex.
 */
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

/*
 * @brief Class MutexLockGuard implements MutexLock in RAII.
 * That is to say, Resource Acquisition Is Initialization.
 */
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
