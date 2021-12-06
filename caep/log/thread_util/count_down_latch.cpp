#include "./count_down_latch.h"

namespace caep {

CountDownLatch::CountDownLatch(int count) :
    mutex(), condition(mutex), count(count) {}

void CountDownLatch::Wait() {
    MutexLockGuard lock(mutex);
    while(count > 0) {
        condition.Wait();
    }
}

void CountDownLatch::CountDown() {
    MutexLockGuard lock(mutex);
    --count;
    if(count == 0) {
        condition.NotifyAll();
    }
}

} // namespace caep
