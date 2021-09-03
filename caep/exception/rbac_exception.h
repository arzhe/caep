#ifndef CAEP_RBAC_EXCEPTION_H
#define CAEP_RBAC_EXCEPTION_H

#include <stdexcept>

namespace caep {

class CaepRbacException : std::invalid_argument {
public:
    using std::invalid_argument::invalid_argument;
};


} // namespace caep 

#endif
