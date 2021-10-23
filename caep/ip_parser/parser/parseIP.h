#ifndef CAEP_PARSE_IP_H
#define CAEP_PARSE_IP_H

#include <string>

#include "./IP.h"
#include "./parseIPv4.h"
#include "./parseIPv6.h"

namespace caep {

IP parseIP(std::string s);

} // namespace caep

#endif
