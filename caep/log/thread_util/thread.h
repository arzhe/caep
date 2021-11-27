#ifndef CAEP_THREAD_H
#define CAEP_THREAD_H

#include <string>
#include <functional>
#include <pthread.h>
#include "noncopyable.h"

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

};

}
#endif
