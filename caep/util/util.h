#ifndef CAEP_UTIL_H
#define CAEP_UTIL_H

#include <string>
#include <vector>

namespace caep {

std::vector<std::string> Split(std::string str, const std::string& delim, int limit = 0);

bool EndWith(std::string str, std::string suffix);

std::string& LTailor(std::string& str, const std::string& tail = "\t\n\r\v ");

std::string& RTailor(std::string& str, const std::string& tail = "\t\n\r\v ");

std::string& Tailor(std::string& str, const std::string& tail = "\t\n\r\v ");

} // namespace caep 

#endif
