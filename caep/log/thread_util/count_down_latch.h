/* $Header:  ~/code/GitRepositories/MyGit/caep   2.1.0   22 Aug 2019 19:00:00   ArZr        $ */
/***********************************************************************************************
 ***                  C O N F I D E N T I A L  ---  A R Z R  S T U D I O S                   ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Caep                                                         *
 *                                                                                             *
 *                    File Name : count_down_latch.h                                           *
 *                                                                                             *
 *                   Programmer : Guan Zhe                                                     *
 *                                                                                             *
 *                   Start Date : Aug 22, 2019                                                 *
 *                                                                                             *
 *                  Last Update : Dec 3, 2021   [ArZr]                                         *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   CountDownLatch::CountDownLatch -- Constructor for CountDownLatch.                         *
 *   CountDownLatch::Wait -- Current Thread blocks in wait queue until condition variable is   *
 *                   actived and some condition is satisfied.                                  *
 *   CountDownLatch::CountDown -- Changes the condition and if condition is satisfied, the     *
 *                   condition variable is actived so that the thread blocked in wait queue can*
 *                   be actived.                                                               *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef CAEP_COUNT_DOWN_LATCH_H
#define CAEP_COUNT_DOWN_LATCH_H

#include "./noncopyable.h"
#include "./mutex_lock.h"
#include "./condition.h"

namespace caep {

/*
 * @breif Class CountDownLatch's main purpose is to ensure that Function passed in Thread is
 * actually started, and then the outter start is returned.
 */
class CountDownLatch : noncopyable {
private:
    mutable MutexLock mutex;
    Condition condition;
    int count;

public:
    explicit CountDownLatch(int count);
    void Wait();
    void CountDown();
};

} // namespace caep

#endif
