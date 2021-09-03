#ifndef CAEP_CONFIG_INTERFACE_H
#define CAEP_CONFIG_INTERFACE_H

#include <string>
#include <vector>

namespace caep {

class ConfigInterface {
public:
    virtual bool GetBool(std::string sec_key) = 0;
    virtual int GetInt(std::string sec_key) = 0;
    virtual float GetFloat(std::string sec_key) = 0;
    virtual std::string GetString(std::string sec_key) = 0;
    virtual std::vector<std::string> GetStrings(std::string sec_key) = 0;
    virtual void Set(std::string sec_key, std::string value) = 0;
    virtual std::string Get(std::string sec_key) = 0;
};

} // namespace caep 

#endif
