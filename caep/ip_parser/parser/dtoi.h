#ifndef CAEP_DTOI_H
#define CAEP_DTOI_H

#include <string>
#include <utility>

#include "./byte.h"

namespace caep {

// Decimal to integer.
// Returns number, characters consumed, success.
std::pair<int, int> dtoi(std::string s);

} // namespace caep 

#endif
