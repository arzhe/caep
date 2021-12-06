/* $Header:  ~/code/GitRepositories/MyGit/caep   2.1.0   22 Aug 2019 19:00:00   ArZr        $ */
/***********************************************************************************************
 ***                  C O N F I D E N T I A L  ---  A R Z R  S T U D I O S                   ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Caep                                                         *
 *                                                                                             *
 *                    File Name : condition.h                                                  *
 *                                                                                             *
 *                   Programmer : Guan Zhe                                                     *
 *                                                                                             *
 *                   Start Date : Aug 22, 2019                                                 *
 *                                                                                             *
 *                  Last Update : Dec 1, 2021   [ArZr]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Condition::Condition -- Constructor for Condition.                                        *
 *   Condition::~Condition -- Destructor for Condition.                                        *
 *   Condition::Wait -- Current thread blocks in wait queue until conditional variable is activ*
 *   Condition::Notify -- Sends a signal to another thread that is in the blocking wait state  *
 *              to get out of the blocking state.                                              *
 *   Condition::NotifyAll -- Sends signals to all threads that are in the blocking wait state  *
 *              to get out of the blocking state.                                              *
 *   Condition::WaitForSeconds -- If the condition is not met before a given time, true is     *
 *              returned(ETIMEDOUT is returned), else false is returned.                       *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef CAEP_CONDITION_H
#define CAEP_CONDITION_H

#include <pthread.h>
#include <errno.h>
#include <time.h>
#include <cstdint>

#include "./mutex_lock.h"
#include "./noncopyable.h"

namespace caep {

/*
 * @breif Class Condition capsulates operations on conditional variable.
 */
class Condition : noncopyable {
private:
    MutexLock& mutex;
    pthread_cond_t cond;

public:
    explicit Condition(MutexLock& mutex) : mutex(mutex) {
        pthread_cond_init(&cond, NULL);
    }

    ~Condition() {
        pthread_cond_destroy(&cond);
    }

    /*
     * @breif Current thread blocks in wait queue until conditional variable is activated.
     * Firstly, a mutex must be locked before Wait is called. When Wait is called, current thread 
     * will be put in the wait queue by OS, and then mutex is unlocked. There is an important
     * reason for doing this, releasing current resources to be available for other threads, so that
     * the thread can determine whether there is a change in the condition. When is returns, the mutex is again blocked.
     */
    void Wait() {
        pthread_cond_wait(&cond, mutex.get());
    }

    /*
     * @breif Send a signal to another thread that is in the blocking wait state to 
     * disengage from the blocking state and continue execution.
     */
    void Notify() {
        pthread_cond_signal(&cond);
    }

    /*
     * @breif Send signals to all thread that are in the blocking wait state to 
     * disengage from the blocking state and continue execution.
     */
    void NotifyAll() {
        pthread_cond_broadcast(&cond);
    }

    /* @breif If the condition is not met before a given time, true is
     * returned(ETIMEDOUT is returned), else false is returned.
     */
    bool WaitForSeconds(int seconds) {
        struct timespec abstime;
        clock_gettime(CLOCK_REALTIME, &abstime);
        abstime.tv_sec += static_cast<time_t>(seconds);
        return ETIMEDOUT == pthread_cond_timedwait(&cond, mutex.get(), &abstime);
    }
};

}

#endif
