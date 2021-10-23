/* $Header:  ~/code/GitRepositories/MyGit/caep   2.1.0   22 Aug 2019 19:00:00   ArZr        $ */
/***********************************************************************************************
 ***                  C O N F I D E N T I A L  ---  A R Z R  S T U D I O S                   ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Caep                                                         *
 *                                                                                             *
 *                    File Name : config.h                                                     *
 *                                                                                             *
 *                   Programmer : Guan Zhe                                                     *
 *                                                                                             *
 *                   Start Date : Aug 22, 2019                                                 *
 *                                                                                             *
 *                  Last Update : Oct 19, 2021   [ArZr]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Config::AddConfig -- Add config messages to the Config.                                   *
 *   Config::ParseFile -- Parse config messages from a CONF file.                              *
 *   Config::ParseBuffer -- Parse config messages from a CONF buffer.                          *
 *                                                                                             *
 *   Config::NewConfigFromFile -- Get a shared_ptr<Config> from a CONF file.                   *
 *   Config::NewConfigFromText -- Get a shared_ptr<Config> from a CONF text.                   *
 *   Config::GetBool -- Get bool-Type value from the Config.                                   *
 *   Config::GetInt -- Get int-Type value from the Config.                                     *
 *   Config::GetFloat -- Get float-Type value from the Config.                                 *
 *   Config::GetString -- Get string-Type value from the Config.                               *
 *   Config::GetStrings -- Get vector<string>-Type from the Config.                            *
 *   Config::Set -- Add config messages to the Config.                                         *
 *   Config::Get -- Get config messages from the Config.                                       *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef CAEP_CONFIG_H
#define CAEP_CONFIG_H

#include <unordered_map>
#include <mutex>
#include <memory>

#include "./config_interface.h"

namespace caep {

class Config : public ConfigInterface {
private:
    
    /*-----------------------------------------------------------------------------------------------------
     * @brief DEFAULT_COMMENT is the comment that Caep supports to be used in CONF & CSV files.
     * @brief DEFAULT_SECTION is the default section when there is empty in "[]" block, then it will be "default" string. 
     */  
    static const std::string DEFAULT_COMMENT;
    static const std::string DEFAULT_SECTION;

    static std::mutex mtx_lock;

    /*-----------------------------------------------------------------------------------------------------
     * @breif data stores triples, for example:
     *
     *           [applicability]
     *           a = sub, res, act
     *
     * then triples will be {"applicability", "a", "sub, res, act"}.
     */ 
    std::unordered_map<std::string, std::unordered_map<std::string, std::string> > data;

    /*-----------------------------------------------------------------------------------------------------
     * @brief Adds triples {section, key, value} to the Config.
     */
    bool AddConfig(const std::string& section, const std::string& key, const std::string& value);

    void ParseFile(std::string f_name);

    void ParseBuffer(std::istream* buffer);

public:
    
    static std::shared_ptr<Config> NewConfigFromFile(std::string f_name);

    static std::shared_ptr<Config> NewConfigFromText(std::string text);

    /*******************************************************************************************************
     * @breif Following functions return types depending on files(eg: config.ini),                         *
     * which has "bool, int, float, string, vector<string>" data in [section] part. For example:           *
     *                                                                                                     *
     *                          [debug_int]                                                                *                          
     *                          debug_int = 1024                                                           *
     *                                                                                                     * 
     *                          [debug_bool]                                                               *
     *                          debug_bool = true                                                          * 
     *                                                                                                     *
     *                          [debug_float]                                                              *
     *                          debug_float = 3.04                                                         *
     *                                                                                                     *
     *                          [debug_string]                                                             *
     *                          debug_string = debug                                                       * 
     *                                                                                                     *
     *                          [debug_vector<string>]                                                     * 
     *                          debug_vector<string> = vec1, vec2, vec3                                    *
     *                                                                                                     * 
     * @param sec_key combines section and key, eg: debug_int::debug_int.                                  * 
     *-----------------------------------------------------------------------------------------------------*/ 
    bool GetBool(std::string sec_key);
    int GetInt(std::string sec_key);
    float GetFloat(std::string sec_key);
    std::string GetString(std::string sec_key);
    std::vector<std::string> GetStrings(std::string sec_key);

    /*-----------------------------------------------------------------------------------
     * @brief Set & Get service for above functions.
     */

    /*
     * @brief Set sets Config::data to {section, key, value}, in which sec_key
     * will be splited into sec and key by "::".
     */ 
    void Set(std::string sec_key, std::string value);
 
    /*
     * @brief Get gets Config::data value according to {sec, key}, which is combined with "::".
     */ 
    std::string Get(std::string sec_key);

};


} // namespace caep 

#endif
