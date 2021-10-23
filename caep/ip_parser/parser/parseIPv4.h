#ifndef CAEP_PARSE_IPV4_H
#define CAEP_PARSE_IPV4_H

#include <string>

#include "./IP.h"
#include "./dtoi.h"
#include "./byte.h"
#include "./IPv4.h"

namespace caep {

IP parseIPv4(std::string s);

} // namespace caep 

#endif
