#ifndef CAEP_ALLFF_CPP
#define CAEP_ALLFF_CPP

#include "./allFF.h"

namespace caep {

bool allFF(std::vector<byte> b) {
    for(auto& bi : b) {
        if(bi != 0xFF) {
            return false;
        }
    }
    return true;
}

} // namespace caep 

#endif
