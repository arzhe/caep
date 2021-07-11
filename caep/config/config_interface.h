#ifndef CAEP_CONFIG_INTERFACE_H
#define CAEP_CONFIG_INTERFACE_H

#include <string>
#include <vector>

namespace caep {

class ConfigInterface {
public:
    
    virtual std::string GetConfigValue(std::string section, std::string key) = 0;

};

} // namespace caep 

#endif
