#ifndef CAEP_CAEP_ENFORCER_EXCEPTION_H
#define CAEP_CAEP_ENFORCER_EXCEPTION_H

#include <stdexcept>

namespace caep {

class CaepEnforcerException : std::runtime_error {
public:
    using std::runtime_error::runtime_error;
};

} // namespace caep 

#endif
