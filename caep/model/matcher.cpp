/* $Header:  ~/code/GitRepositories/MyGit/caep   2.1.0   22 Aug 2019 19:00:00   ArZr        $ */
/***********************************************************************************************
 ***                  C O N F I D E N T I A L  ---  A R Z R  S T U D I O S                   ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Caep                                                         *
 *                                                                                             *
 *                    File Name : matcher.cpp                                                  *
 *                                                                                             *
 *                   Programmer : Guan Zhe                                                     *
 *                                                                                             *
 *                   Start Date : Aug 22, 2019                                                 *
 *                                                                                             *
 *                  Last Update : Oct 20, 2021   [ArZr]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Matcher::LoadMatcherFromModel -- Loads Matcher Functions' messages from Model.            *
 *   Matcher::LoadMatcherMap -- Initializes Matcher::matcher_map.                              *
 *   Matcher::AddMatcher -- Adds a Matcher Function and its name to current Matcher.           *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifndef CAEP_MATCHER_CPP
#define CAEP_MATCHER_CPP

#include "./matcher.h"
#include "../util/built_in_functions.h"
#include "../exception/caep_exception.h"

namespace caep {

/***********************************************************************************************
 ***                              Matcher::LoadMatcherFromModel                              ***
 ***********************************************************************************************
 * DESCRIPTION: Loads Matcher Functions' messages from Model, such as the pointer to Matcher   *
 *              Function, the name of Matcher Function and the option that controls whether    *
 *              the Matcher Function is on or off.                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   model -- Pointer to Model, Matcher loads the CONF section messages and PRM policy  *
 *                   rule of section 'm' from this pointer.                                    *
 *                                                                                             *
 * OUTPUT:   NONE                                                                              *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
void Matcher::LoadMatcherFromModel(Model* model) {
    LoadMatcherMap();

    auto matchers = CaepUtil::Split(model->m["m"].section_map["m"]->value, ",");

    auto matcher_option = model->m["m"].section_map["m"]->policy[0];
    
    auto matcher_count = matchers.size();
    auto option_count = matcher_option.size();
    if(option_count < matcher_count)
        throw IllegalArgumentException("The parameters of \"matcher\" section policy rules should be at least equal to the count of matcher listed in .INI file");

    for(size_t i = 0; i < matcher_count; ++i) {
        std::string matcher_name = CaepUtil::Trim(matchers[i]);
        bool op = matcher_option[i] == "on" ? true : false;
        matcher_switch[matcher_name] = op;
    }
}

/***********************************************************************************************
 ***                                Matcher::LoadMatcherMap                                  ***
 ***********************************************************************************************
 * DESCRIPTION: Initializes Matcher::matcher_map. Matcher::matcher_map stores Matcher Functions*
 *              and their names.                                                               *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   NONE                                                                               *
 *                                                                                             *
 * OUTPUT:   NONE                                                                              *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
void Matcher::LoadMatcherMap() {
    AddMatcher("DefaultMatcher", DefaultMatcher);
    AddMatcher("RegexMatcher",RegexMatcher);
    AddMatcher("IPMatcher", IPMatcher);
    AddMatcher("RoleMatch", nullptr);
    
}

/***********************************************************************************************
 ***                                Matcher::AddMatcher                                      ***
 ***********************************************************************************************
 * DESCRIPTION: Adds a Matcher Function and its name to current Matcher.                       *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   matcher_name -- Matcher Function's name.                                           *
 *                                                                                             *
 *          mf -- Object of the Matcher Function.                                              *
 *                                                                                             *
 * OUTPUT:   NONE                                                                              *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
bool Matcher::AddMatcher(std::string matcher_name, MatcherFunc mf) {
    bool found = matcher_map.find(matcher_name) != matcher_map.end();
    if(found)
        return false;
    matcher_map[matcher_name] = mf;
}

} // namespace caep

#endif
