#ifndef CAEP_MISSING_REQUIRED_SECTIONS_H
#define CAEP_MISSING_REQUIRED_SECTIONS_H

#include <stdexcept>

namespace caep {

class MissingRequiredSections : std::domain_error {
public:
    using std::domain_error::domain_error;
};

} // namespace caep 

#endif
