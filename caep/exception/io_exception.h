#ifndef CAEP_IO_EXCEPTION_H
#define CAEP_IO_EXCEPTION_H

#include <stdexcept>

namespace caep {

class IOException : std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

} // namespace caep 

#endif
