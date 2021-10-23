#ifndef CAEP_ADAPTER_EXCEPTION_H
#define CAEP_ADAPTER_EXCEPTION_H

#include <stdexcept>

namespace caep {

class AdapterException : std::logic_error {
public:
    using std::logic_error::logic_error;
};

} // namespace caep 

#endif
