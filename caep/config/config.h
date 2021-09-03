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

    bool AddConfig(std::string section, std::string key, std::string value);

    void ParseFile(std::string f_name);

    void ParseBuffer(std::istream* buffer);

public:

    static std::shared_ptr<Config> NewConfigFromFile(std::string f_name);

    static std::shared_ptr<Config> NewConfigFromText(std::string text);

    /******************************
     * Following functions return types depending on files(eg: config.ini), 
     * which has "bool, int, float, string, vector<string>" data in [section] part.
     * eg: [debug_int]
     *     debug_int = 1024
     *
     *     [debug_bool]
     *     debug_bool = true
     *
     *     [debug_float]
     *     debug_float = 3.04
     *
     *     [debug_string]
     *     debug_string = debug
     *
     *     [debug_vector<string>]
     *     debug_vector<string> = vec1, vec2, vec3
     *
     * @param
     *  #sec_key : combines section and key, eg:debug_int::debug_int.
     ******************************/
    bool GetBool(std::string sec_key);
    
    int GetInt(std::string sec_key);

    float GetFloat(std::string sec_key);

    std::string GetString(std::string sec_key);

    std::vector<std::string> GetStrings(std::string sec_key);

    /******************************/

    // set Config::data to sec->key->value, in which sec_key
    // will be splited into sec and key by "::".
    void Set(std::string sec_key, std::string value);
           
    // get Config::data, according to sec_key, which will be
    // splited into sec and key by "::", Get then return 
    // value as a string.
    std::string Get(std::string sec_key);

};


} // namespace caep 

#endif
