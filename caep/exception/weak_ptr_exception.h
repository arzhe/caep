#ifndef CAEP_WEAK_PTR_EXCEPTION_H
#define CAEP_WEAK_PTR_EXCEPTION_H

#include <stdexcept>

namespace caep {

class WeakPtrException : std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

} // namespace caep 


#endif
