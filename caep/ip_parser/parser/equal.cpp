#ifndef CAEP_EQUAL_CPP
#define CAEP_EQUAL_CPP

#include "./equal.h"

namespace caep {

bool equal(IPMask m1, IPMask m2) {
    if(m1.size() != m2.size())
        return false;

    for(int i = 0; i < m1.size(); ++i) {
        if(m1[i] != m2[i])
            return false;
    }

    return true;
}

} // namespace caep 


#endif
