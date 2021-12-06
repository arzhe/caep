#include <assert.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/prctl.h>

#include "./current_thread.h"
#include "./thread.h"

namespace caep {

namespace CurrentThread {
    __thread int t_cached_tid = 0;
    __thread char t_tid_string[32];
    __thread int t_tid_string_length = 6;
    __thread const char* t_thread_name = "default";
}

pid_t Gettid() {
    return static_cast<pid_t>(::syscall(SYS_gettid));
}

void CurrentThread::CachedTid() {
    if(t_cached_tid == 0) {
        t_cached_tid = Gettid();
        t_tid_string_length =
            snprintf(t_tid_string, sizeof t_tid_string, "%5d ", t_cached_tid);
    }
}

struct ThreadData {
    typedef Thread::ThreadFunc ThreadFunc;
    ThreadFunc func;
    std::string name;
    pid_t* tid;
    CountDownLatch* latch;

    ThreadData(const ThreadFunc& func, const std::string& name, pid_t* tid,
               CountDownLatch* latch)
        : func(func), name(name), tid(tid), latch(latch) {}

    void RunInThread() {
        *tid = CurrentThread::Tid();
        tid = nullptr;
        latch->CountDown();
        latch = nullptr;

        CurrentThread::t_thread_name = name.empty() ? "Thread" : name.c_str();
        prctl(PR_SET_NAME, CurrentThread::t_thread_name);

        func();
        CurrentThread::t_thread_name = "finished";
    }
};

void* StartThread(void* obj) {
    ThreadData* data = static_cast<ThreadData*>(obj);
    data->RunInThread();
    delete data;
    return nullptr;
}

Thread::Thread(const ThreadFunc& func, const std::string& n)
    : started(false),
      joined(false),
      pthread_id(0),
      tid(0),
      func(func),
      name(n),
      latch(1) {
    SetDefaultName();
}

Thread::~Thread() {
    if(started && !joined) {
        pthread_detach(pthread_id);
    }
}

void Thread::SetDefaultName() {
    if(name.empty()) {
        char buff[32];
        snprintf(buff, sizeof buff, "Thread");
        name = buff;
    }
}

void Thread::Start() {
    assert(!started);
    started = true;
    ThreadData* data = new ThreadData(func, name, &tid, &latch);
    if(pthread_create(&pthread_id, nullptr, &StartThread, data)) {
        started = false;
        delete data;
    }
    else {
        latch.Wait();
        assert(tid > 0);
    }
}

int Thread::Join() {
    assert(started);
    assert(!joined);
    joined = true;
    return pthread_join(pthread_id, nullptr);
}

} // namespace caep
