/* $Header:  ~/code/GitRepositories/MyGit/caep   2.1.0   22 Aug 2019 19:00:00   ArZr        $ */
/***********************************************************************************************
 ***                  C O N F I D E N T I A L  ---  A R Z R  S T U D I O S                   ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Caep                                                         *
 *                                                                                             *
 *                    File Name : matcher.h                                                    *
 *                                                                                             *
 *                   Programmer : Guan Zhe                                                     *
 *                                                                                             *
 *                   Start Date : Aug 22, 2019                                                 *
 *                                                                                             *
 *                  Last Update : Oct 20, 2021   [ArZr]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Matcher::LoadMatcherFromModel -- Loads matcher functions' messages from Model.            *
 *   Matcher::LoadMatcherMap -- Initializes Matcher::matcher_map.                              *
 *   Matcher::AddMatcher -- Adds matcher function to current Matcher.                          *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifndef CAEP_MATCHER_H
#define CAEP_MATCHER_H

#include "./model.h"
#include "../util/caep_util.h"

namespace caep {

/*------------------------------------------------------------------------------------------------
 * @brief Type of Matcher Functions described in Caep.
 */
typedef bool (*MatcherFunc)(std::string, std::string);

class Matcher {
public:
    
    /*--------------------------------------------------------------------------------------------
     * @brief Stores Matcher Functions and their names.
     */
    std::unordered_map<std::string, MatcherFunc> matcher_map;

    /*
     * @brief Determines which Matcher Functions are on or off.
     */
    std::unordered_map<std::string, bool> matcher_switch;

    void LoadMatcherFromModel(Model* model);

    void LoadMatcherMap();

    bool AddMatcher(std::string matcher_name, MatcherFunc mf);

};

} // namespace caep 

#endif
