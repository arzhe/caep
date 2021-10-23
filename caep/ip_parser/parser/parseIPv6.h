#ifndef CAEP_PARSE_IPV6_H
#define CAEP_PARSE_IPV6_H

#include <string>

#include "./IP.h"
#include "./xtoi.h"
#include "./parseIPv4.h"

namespace caep {

IP parseIPv6(std::string s);

} // namespace caep

#endif
