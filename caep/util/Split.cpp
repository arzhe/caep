#ifndef CAEP_SPLIT_CPP
#define CAEP_SPLIT_CPP

#include "./util.h"

#define LARGE 2147483647

namespace caep {

std::vector<std::string> Split(std::string str, const std::string& delim, int limit) {
    std::vector<std::string> vs;

    if(limit <= 0)
        limit = LARGE;

    for(int i = 0; i < limit; ++i) {
        size_t pos = str.find(delim);
        if(pos != std::string::npos) {
            vs.push_back(str.substr(0, pos));
            str = str.substr(pos + delim.length());
        }
        else 
            break;
    }
    vs.push_back(str);
    return vs;
}

} // namespace caep 

#endif
