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
#ifndef CAEP_CONFIG_CPP
#define CAEP_CONFIG_CPP

#include <fstream>
#include <sstream>

#include "./config.h"
#include "../exception/io_exception.h"
#include "../exception/illegal_argument_exception.h"
#include "../util/caep_util.h"

namespace caep {

/*-----------------------------------------------------------------------------------------------------
* DEFAULT_COMMENT is the comment that Caep supports to be used in CONF & CSV files.
* DEFAULT_SECTION is the default section when there is empty in "[]" block, then it will be "default" string. 
*/  
const std::string Config::DEFAULT_COMMENT = "#";
const std::string Config::DEFAULT_SECTION = "default";

std::mutex Config::mtx_lock;


/***********************************************************************************************
 ***                                Config::AddConfig                                        ***
 ***********************************************************************************************
 * DESCRIPTION: Add config messages to the Config.                                             *
 *                                                                                             * 
 *                                                                                             * 
 * INPUT:   section -- Section that described in '[]'.                                         * 
 *          key     -- Key that described on the left side of '='.                             * 
 *          value   -- Value that described on the right side of '='.                          *   
 *                                                                                             * 
 * OUTPUT:   If there is {sec, key, value} in Config, then returns false,                      * 
 *           else returns true.                                                                *
 *                                                                                             * 
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *  
 *=============================================================================================*/
bool Config::AddConfig(const std::string& section, const std::string& key, const std::string& value) {
    std::string sec = section;
    if(!sec.compare(""))
        sec = DEFAULT_SECTION;

    bool found = data[sec].find(key) != data[sec].end();
    if(!found) {
        data[sec][key] = value;
        return true;
    }
    
    return false;
}

/***********************************************************************************************
 ***                                Config::ParseFile                                        ***
 ***********************************************************************************************
 * DESCRIPTION: Parse config messages from a CONF file.                                        *
 *                                                                                             * 
 *                                                                                             * 
 * INPUT:   f_name -- CONF file name.                                                          * 
 *                                                                                             * 
 * OUTPUT:  If there is exited {sec, key, value} in Config, then returns false,                * 
 *           else returns true                                                                 *
 *                                                                                             * 
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *  
 *=============================================================================================*/
void Config::ParseFile(std::string f_name) {
    mtx_lock.lock();
    std::ifstream ifile;
    try {
        ifile.open(f_name);
    } catch(const std::ifstream::failure e) {
        mtx_lock.unlock();
        throw IOException("Cannot open file!");
    }
    ParseBuffer(&ifile);
    mtx_lock.unlock();
    ifile.close();
}

/***********************************************************************************************
 ***                                Config::ParseBuffer                                      ***
 ***********************************************************************************************
 * DESCRIPTION: Parse config messages from a CONF buffer.                                      *
 *                                                                                             * 
 *                                                                                             * 
 * INPUT:   buffer -- CONF buffer, could be a file_stream or a text_stream.                    * 
 *                                                                                             * 
 * OUTPUT:  NONE                                                                               * 
 *                                                                                             * 
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *  
 *=============================================================================================*/
void Config::ParseBuffer(std::istream* buffer) {
    std::string section = "";
    int line_num = 0;
    std::string line;
    while(true) {
        ++line_num;
        if(getline(*buffer, line, '\n')) {
            if(!line.compare(""))
                continue;
        }
        else 
            break;
        line = CaepUtil::Trim(line);
        if(line.find(DEFAULT_COMMENT) == 0)
            continue;
        else if(line.find("[") == 0 && CaepUtil::EndWith(line, std::string("]")))
            section = line.substr(1, line.length() - 2);
        else {
            auto key_val = CaepUtil::Split(line, std::string("="), 2);

            if(key_val.size() != 2) {
                char* error = new char;
                sprintf(error, "parse the content error: line %d, %s = ? ", line_num, key_val[0].c_str());
                throw IllegalArgumentException(std::string(error));
            }
            std::string key = CaepUtil::Trim(key_val[0]);
            std::string value = CaepUtil::Trim(key_val[1]);
            AddConfig(section, key, value);
        }
    }
}

/***********************************************************************************************
 ***                                Config::NewConfigFromFile                                ***
 ***********************************************************************************************
 * DESCRIPTION: Get a shared_ptr<Config> from a CONF file.                                     *
 *                                                                                             * 
 *                                                                                             * 
 * INPUT:   f_name -- CONF file name.                                                          * 
 *                                                                                             * 
 * OUTPUT:  shared_ptr<Config>                                                                 * 
 *                                                                                             * 
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *  
 *=============================================================================================*/
std::shared_ptr<Config> Config::NewConfigFromFile(std::string f_name) {
    std::shared_ptr<Config> c(new Config);
    c->ParseFile(f_name);
    return c;
}

/***********************************************************************************************
 ***                                Config::NewConfigFromText                                ***
 ***********************************************************************************************
 * DESCRIPTION: Get a shared_ptr<Config> from a CONF text.                                     *
 *                                                                                             * 
 *                                                                                             * 
 * INPUT:   f_name -- CONF text.                                                               * 
 *                                                                                             * 
 * OUTPUT:  shared_ptr<Config>                                                                 * 
 *                                                                                             * 
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *  
 *=============================================================================================*/
std::shared_ptr<Config> Config::NewConfigFromText(std::string text) {
    std::shared_ptr<Config> c(new Config);
    std::stringstream textstream(text);
    c->ParseBuffer(&textstream);
    return c;
}

/***********************************************************************************************
 ***                                Config::GetBool                                          ***
 ***********************************************************************************************
 * DESCRIPTION: Get bool-Type value from the Config.                                           *
 *                                                                                             * 
 *                                                                                             * 
 * INPUT:   sec_key -- A string contatinated with '::'.                                        *
 *                     Section is on the left of '::', Key is on the right of '::'.            *
 *                     If there is no '::', OUTPUT would be {DEFAULT_SECTION, key}->value.     *
 *                                                                                             * 
 * OUTPUT:  bool-Type value.                                                                   * 
 *                                                                                             * 
 * WARNINGS:    If value of {sec, key} is not bool-Type, OUTPUT could be 'false'.              *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *  
 *=============================================================================================*/
bool Config::GetBool(std::string sec_key) {
    return Get(sec_key).compare("true") == 0;
}

/***********************************************************************************************
 ***                                Config::GetInt                                           ***
 ***********************************************************************************************
 * DESCRIPTION: Get int-Type value from the Config.                                            *
 *                                                                                             * 
 *                                                                                             * 
 * INPUT:   sec_key -- A string contatinated with '::'.                                        *
 *                     Section is on the left of '::', Key is on the right of '::'.            * 
 *                     If there is no '::', OUTPUT would be {DEFAULT_SECTION, key}->value.     *
 *                                                                                             *
 * OUTPUT:  int-Type value.                                                                    * 
 *                                                                                             * 
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *  
 *=============================================================================================*/
int Config::GetInt(std::string sec_key) {
    return atoi(Get(sec_key).c_str());
}

/***********************************************************************************************
 ***                                Config::GetFloat                                         ***
 ***********************************************************************************************
 * DESCRIPTION: Get float-Type value from the Config.                                          *
 *                                                                                             * 
 *                                                                                             * 
 * INPUT:   sec_key -- A string contatinated with '::'.                                        *
 *                     Section is on the left of '::', Key is on the right of '::'.            * 
 *                     If there is no '::', OUTPUT would be {DEFAULT_SECTION, key}->value.     *
 *                                                                                             * 
 * OUTPUT:  float-Type value.                                                                  * 
 *                                                                                             * 
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *  
 *=============================================================================================*/
float Config::GetFloat(std::string sec_key) {
    return float(atof(Get(sec_key).c_str()));
}

/***********************************************************************************************
 ***                                Config::GetString                                        ***
 ***********************************************************************************************
 * DESCRIPTION: Get string-Type value from the Config.                                         *
 *                                                                                             * 
 *                                                                                             * 
 * INPUT:   sec_key -- A string contatinated with '::'.                                        *
 *                     Section is on the left of '::', Key is on the right of '::'.            * 
 *                     If there is no '::', OUTPUT would be {DEFAULT_SECTION, key}->value.     *
 *                                                                                             * 
 * OUTPUT:  string-Type value.                                                                 * 
 *                                                                                             * 
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *  
 *=============================================================================================*/
std::string Config::GetString(std::string sec_key) {
    return Get(sec_key);
}

/***********************************************************************************************
 ***                                Config::GetStrings                                       ***
 ***********************************************************************************************
 * DESCRIPTION: Get vector<string>-Type value from the Config.                                 *
 *                                                                                             * 
 *                                                                                             * 
 * INPUT:   sec_key -- A string contatinated with '::'.                                        *
 *                     Section is on the left of '::', Key is on the right of '::'.            * 
 *                     If there is no '::', OUTPUT would be {DEFAULT_SECTION, key}->value.     *
 *                                                                                             * 
 * OUTPUT:  vector<string>-Type value.                                                         * 
 *                                                                                             * 
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *  
 *=============================================================================================*/
std::vector<std::string> Config:: GetStrings(std::string sec_key) {
    auto s = Get(sec_key);
    if(!s.compare("")) 
        return std::vector<std::string>();
    auto vs = CaepUtil::Split(s, std::string(","));
    for(auto& avs : vs)
        avs = CaepUtil::Trim(avs);
    return vs;
}

/***********************************************************************************************
 ***                                Config::Set                                              ***
 ***********************************************************************************************
 * DESCRIPTION: Add config messages to the Config.                                             *
 *                                                                                             * 
 *                                                                                             * 
 * INPUT:   sec_key -- A string contatinated with '::'.                                        *
 *                     Section is on the left of '::', Key is on the right of '::'.            * 
 *                     If there is no '::' or empty on the left of '::',                       *
 *                     DEFAULT_SECTION' will be added to Section,                              *
 *                     Sec_key will be added to Key.                                           * 
 *                                                                                             *
 *          value -- It will be added to {sec, key}->value in Config.                          *
 *                                                                                             * 
 * OUTPUT:  NONE                                                                               * 
 *                                                                                             * 
 * WARNINGS:    sec_key cannot be empty, otherwise an exception will be thrown.                *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *  
 *=============================================================================================*/
void Config::Set(std::string sec_key, std::string value) {
    mtx_lock.lock();
    if(sec_key.length() == 0) {
        mtx_lock.unlock();
        throw IllegalArgumentException("sec_key is empty");
    }

    std::string section;
    std::string key;

    transform(sec_key.begin(), sec_key.end(), sec_key.begin(), ::tolower);
    auto sec_keys = CaepUtil::Split(sec_key, std::string("::"));
    if(sec_keys.size() >= 2) {
        section = sec_keys[0];
        key = sec_keys[1];
    }
    else { 
        section = "";
        key = sec_keys[0];
    }
    AddConfig(section, key, value);
    mtx_lock.unlock();
}

/***********************************************************************************************
 ***                                Config::Get                                              ***
 ***********************************************************************************************
 * DESCRIPTION: Get config messages to the Config.                                             *
 *                                                                                             * 
 *                                                                                             * 
 * INPUT:   sec_key -- A string contatinated with '::'.                                        *
 *                     Section is on the left of '::', Key is on the right of '::'.            * 
 *                     If there is no '::' or empty on the left of '::',                       * 
 *                     OUTPUT would be {DEFAULT_SECTION, key}->value.                          *
 *                                                                                             * 
 * OUTPUT:  Returns {sec, key}->value in Config.                                               *
 *                                                                                             * 
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *  
 *=============================================================================================*/
std::string Config::Get(std::string sec_key) {
    std::string section;
    std::string key;

    transform(sec_key.begin(), sec_key.end(), sec_key.begin(), ::tolower);
    auto sec_keys = CaepUtil::Split(sec_key, std::string("::"));
    if(sec_keys.size() >= 2) {
        section = sec_keys[0];
        key = sec_keys[1];
    }
    else {
        section = DEFAULT_SECTION;
        key = sec_keys[0];
    }

    bool found = data.find(section) != data.end() && data[section].find(key) != data[section].end();
    if(found)
        return data[section][key];
    return "";
}

} // namespace caep 

#endif

