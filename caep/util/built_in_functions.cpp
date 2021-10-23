#ifndef CAEP_BUILD_IN_FUNTIONS_CPP
#define CAEP_BUILD_IN_FUNTIONS_CPP

#include <regex>

#include "./built_in_functions.h"
#include "../rbac/role_manager.h"
#include "./util.h"
#include "../exception/illegal_argument_exception.h"
#include "../ip_parser/parser/CIDR.h"
#include "../ip_parser/parser/IP.h"
#include "../ip_parser/parser/parseCIDR.h"
#include "../ip_parser/parser/parseIP.h"

namespace caep {

bool DefaultMatcher(std::string str1, std::string str2) {
    auto pos = str2.find("*");

    if(pos == std::string::npos)
        return str1 == str2;

    if(str1.length() > pos)
        return str1.substr(0, pos) == str2.substr(0, pos);

    return str1 == str2.substr(0, pos);
}

bool RegexMatcher(std::string str1, std::string str2) {
    std::regex regex_s(str2);
    return regex_match(str1, regex_s);
}

bool IPMatcher(std::string ip1, std::string ip2) {
    IP objIP1 = parseIP(ip1);
    if (objIP1.isLegal == false)
        throw IllegalArgumentException("invalid argument: ip1 in IPMatch() function is not an IP address.");

    CIDR objCIDR = parseCIDR(ip2);
    if (objCIDR.ip.isLegal == false) {
        IP objIP2 = parseIP(ip2);
        if (objIP2.isLegal == false)
            throw IllegalArgumentException("invalid argument: ip1 in IPMatch() function is not an IP address.");

        return objIP1.Equal(objIP2);
    }

    return objCIDR.net.contains(objIP1);
}

} // namespace caep 

#endif
