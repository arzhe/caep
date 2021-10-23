#ifndef CAEP_PARSE_IP_CPP
#define CAEP_PARSE_IP_CPP

#include "./parseIP.h"

namespace caep {

IP parseIP(std::string s) {
    for(int i = 0 ; i < s.length() ; i++) {
        switch(s[i]) {
        case '.':
            return parseIPv4(s);
        case ':':
            return parseIPv6(s);
        }
    }
    IP p;
    p.isLegal = false;
    return p;
}

} // namespace caep

#endif
