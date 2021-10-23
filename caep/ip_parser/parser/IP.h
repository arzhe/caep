#ifndef CAEP_IP_H
#define CAEP_IP_H

#include <vector>
#include <sstream>

#include "./allFF.h"
#include "./IPMask.h"
#include "./equal.h"
#include "./byte.h"

namespace caep {

class IP {
public:
    std::vector<byte> ip;
    bool isLegal;
    static byte IPv4len;
    static byte IPv6len;
    static IPMask v4Inv6Prefix;

    IP();

    IP Mask(IPMask mask);

    bool Equal(IP x);

    std::string toString();

    /**
     * @breif To4 converts the IPv4 address ip to a 4-byte representation.
     * @breif If ip is not an IPv4 address, To4 returns nil.
     */
    IP To4();

    static bool isZeros(IP p);
    
};

} // namespace caep 

#endif
