/* $Header:  ~/code/GitRepositories/MyGit/caep   2.1.0   22 Aug 2020 19:00:00   ArZr        $ */
/***********************************************************************************************
 ***                  C O N F I D E N T I A L  ---  A R Z R  S T U D I O S                   ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Caep                                                         *
 *                                                                                             *
 *                    File Name : config_interface.h                                           *
 *                                                                                             *
 *                   Programmer : Guan Zhe                                                     *
 *                                                                                             *
 *                   Start Date : Aug 22, 2020                                                 *
 *                                                                                             *
 *                  Last Update : Oct 19, 2021   [ArZr]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef CAEP_CONFIG_INTERFACE_H
#define CAEP_CONFIG_INTERFACE_H

#include <string>
#include <vector>

namespace caep {

class ConfigInterface {
public:
    /*-----------------------------------------------------------------------------------
     * @brief Constructor & Destructor.  
     */ 
    ConfigInterface() {}
    virtual ~ConfigInterface() {}

    /*-----------------------------------------------------------------------------------
     * @brief Some methods that you can handle messages from config file by using them. 
     */ 
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
