#ifndef CAEP_BUILD_IN_FUNCTIONS_H
#define CAEP_BUILD_IN_FUNCTIONS_H

#include <string>

#include "../rbac/default_role_manager.h"

namespace caep {

/**
 * @breif DefaultMatcher determines whether str1 matches the pattern of str2(similar to RESTfull path), str2 can contain a *.
 * @breif eg: "foo/bar" matches "/foo/*".
 */ 
bool DefaultMatcher(std::string str1, std::string str2);


/**
 * @breif RegexMatch determines whether key1 matches the pattern of key2 in regular expression.
 */ 
bool RegexMatcher(std::string str1, std::string str2);

/**
 * @breif IPMatch determines whether IP address ip1 matches the pattern of IP address ip2, ip2 can be an IP address or a CIDR pattern.
 * @breif eg: "192.168.2.123" matches "192.168.2.0/24".
 */
bool IPMatcher(std::string ip1, std::string ip2);


} // namespace caep 

#endif
