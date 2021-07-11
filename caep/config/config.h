#ifndef CAEP_CONFIG_H
#define CAEP_CONFIG_H

#include <unordered_map>
#include <mutex>
#include <memory>

#include "./config_interface.h"

namespace caep {

class Config : public ConfigInterface {
private:

    static const std::string DEFAULT_COMMENT;
    static const std::string DEFAULT_SECTION;

    static std::mutex mtx_lock;

    std::unordered_map<std::string, std::unordered_map<std::string, std::string> > data;

    bool AddConfigData(std::string section, std::string key, std::string value);

    void ParseFile(std::string f_name);

    void ParseBuffer(std::istream* buffer);

public:

    static std::shared_ptr<Config> NewConfigFromFile(std::string f_name);

    static std::shared_ptr<Config> NewConfigFromText(std::string text);

    std::string GetConfigValue(std::string section, std::string key);

};


} // namespace caep 

#endif
