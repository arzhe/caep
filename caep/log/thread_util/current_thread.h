#ifndef CAEP_CURRENT_THREAD_H
#define CAEP_CURRENT_THREAD_H

namespace caep {

namespace CurrentThread {

extern __thread int t_cached_tid;
extern __thread char t_tid_string[32];
extern __thread int t_tid_string_length;
extern __thread const char* t_thread_name;

void CachedTid();

inline int Tid() {
    if(__builtin_expect(t_cached_tid == 0, 0)) {
        CachedTid();
    }
    return t_cached_tid;
}

inline const char* TidString() {
    return t_tid_string;
}

inline int TidStringLength() {
    return t_tid_string_length;
}

inline const char* name() {
    return t_thread_name;
}

} // namespace CurrentThread
} // namespace caep

#endif
