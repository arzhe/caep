#ifndef CAEP_TAILOR_H
#define CAEP_TAILOR_H

#include <string>

#include "./util.h"

namespace caep {

std::string& LTailor(std::string& str, const std::string& tail) {
    str.erase(0, str.find_first_not_of(tail));
    return str;
}

std::string& RTailor(std::string& str, const std::string& tail) {
    str.erase(str.find_last_not_of(tail) + 1);
}

std::string& Tailor(std::string& str, const std::string& tail) {
    return LTailor(RTailor(str, tail), tail);
}

} // namespace caep 

#endif
