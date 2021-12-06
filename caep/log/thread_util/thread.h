#ifndef CAEP_THREAD_H
#define CAEP_THREAD_H

#include <string>
#include <functional>
#include <pthread.h>

#include "./count_down_latch.h"
#include "./noncopyable.h"

namespace caep {

class Thread : noncopyable {
public:
    typedef std::function<void()> ThreadFunc;

private:
    bool started;
    bool joined;
    pthread_t pthread_id;
    pid_t tid;
    ThreadFunc func;
    std::string name;
    
    CountDownLatch latch;

    void SetDefaultName();

public:
    explicit Thread(const ThreadFunc& func, const std::string& name = std::string());
    ~Thread();

    void Start();
    int Join();
    bool Started() const {
        return started;
    }
    pid_t Tid() const {
        return tid;
    }
    const std::string& Name() const {
        return name;
    }
};

} // namespace caep
#endif
