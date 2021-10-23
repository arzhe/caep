#ifndef CAEP_IPNET_H
#define CAEP_IPNET_H

#include <utility>
#include <sstream>

#include "./IP.h"
#include "./IPMask.h"

namespace caep {

class IPNet {
    public:
        IP net_ip;
        IPMask mask;

        std::string NETIP_toString();

        std::string IPMask_toString();

        // Contains reports whether the network includes ip.
        bool contains(IP ipNew);

        static std::pair<IP, IPMask> networkNumberAndMask(IPNet n);

};

} // namespace caep 
#endif
