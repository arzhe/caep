#ifndef CAEP_UNSUPPORTED_OPERATION_EXCEPTION_H
#define CAEP_UNSUPPORTED_OPERATION_EXCEPTION_H

#include <stdexcept>

namespace caep {

class UnsupportedOperationException : std::logic_error {
public:
    using std::logic_error::logic_error;
};

} // namespace caep 

#endif
