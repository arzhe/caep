#ifndef CAEP_ILLEGAL_ARGUMENT_EXCEPTION_H
#define CAEP_ILLEGAL_ARGUMENT_EXCEPTION_H

#include <stdexcept>

namespace caep {

class IllegalArgumentException : std::invalid_argument {
public:
    using std::invalid_argument::invalid_argument;
};

} // namespace caep 

#endif
