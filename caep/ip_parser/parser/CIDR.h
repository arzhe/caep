#ifndef CAEP_CIDR_H
#define CAEP_CIDR_H

#include "./IP.h"
#include "./IPNet.h"

namespace caep {

class CIDR {
public:
    IP ip;
    IPNet net;
};

} // namespace caep 

#endif
