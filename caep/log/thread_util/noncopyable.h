#ifndef CAEP_NONCOPYABLE_H
#define CAEP_NONCOPYABLE_H

namespace caep {

class noncopyable {
protected:
    noncopyable() {}
    ~noncopyable() {}

private:
    noncopyable(const noncopyable&);
    const noncopyable& operator=(const noncopyable&);
};

}
#endif
