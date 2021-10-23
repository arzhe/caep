#ifndef CAEP_IPV4_CPP
#define CAEP_IPV4_CPP

#include "./IPv4.h"

namespace caep {

IP IPv4(byte a, byte b, byte c, byte d) {
    IP p;
    std::vector <byte> newIP(IP::v4Inv6Prefix.begin(), IP::v4Inv6Prefix.end());
    p.ip = newIP;
    p.ip.push_back(a);
    p.ip.push_back(b);
    p.ip.push_back(c);
    p.ip.push_back(d);
    p.isLegal = true;
    return p;
}

} // namespace caep 

#endif
