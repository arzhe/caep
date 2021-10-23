#ifndef CAEP_PARSE_CIDR_H
#define CAEP_PARSE_CIDR_H

#include "./CIDR.h"
#include "./IP.h"
#include "./IPMask.h"
#include "./dtoi.h"
#include "./byte.h"
#include "./parseIPv4.h"
#include "./parseIPv6.h"
#include "./CIDRMask.h"
#include "../exception/parser_exception.h"

namespace caep {

CIDR parseCIDR(std::string s);

} // namespace caep

#endif
