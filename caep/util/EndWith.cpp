#ifndef CAEP_END_WITH_CPP
#define CAEP_END_WITH_CPP

#include "./util.h"

namespace caep {

bool EndWith(std::string str, std::string suffix) {
    auto pos = search(str.cbegin(), str.cend(),
                      suffix.cbegin(), suffix.cend(),
                      [] (char c1, char c2) {
                          return toupper(c1) == toupper(c2);
                      });

    auto suffix_length = suffix.length();
    if((pos + suffix_length) != str.cend())
        return false;
    return true;
}

} // namespace caep 

#endif
