/* $Header:  ~/code/GitRepositories/MyGit/caep   2.1.0   22 Aug 2019 19:00:00   ArZr        $ */
/***********************************************************************************************
 ***                  C O N F I D E N T I A L  ---  A R Z R  S T U D I O S                   ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Caep                                                         *
 *                                                                                             *
 *                    File Name : section.cpp                                                  *
 *                                                                                             *
 *                   Programmer : Guan Zhe                                                     *
 *                                                                                             *
 *                   Start Date : Aug 22, 2019                                                 *
 *                                                                                             *
 *                  Last Update : Oct 20, 2021   [ArZr]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Section::BuildIncrementalRoleLinks -- Adds or deletes inheritance links for all Roles.    *
 *   Section::BuildRoleLinks -- Adds inheritance links for all Roles.                          *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef CAEP_SECTION_CPP
#define CAEP_SECTION_CPP

#include <algorithm>

#include "./section.h"
#include "../exception/illegal_argument_exception.h"

namespace caep {

/***********************************************************************************************
 ***                            Section::BuildIncrementalRoleLinks                           ***
 ***********************************************************************************************
 * DESCRIPTION: Adds or deletes inheritance links for all Roles by using outter policy rules.  *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   rm -- RoleManager.                                                                 *
 *          op -- Determines whether to addd or delete inheritance links.                      *
 *          rules -- Policy rules described in CSV file.                                       *
 *                                                                                             *
 * OUTPUT:   NONE                                                                              *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
void Section::BuildIncrementalRoleLinks(std::shared_ptr<RoleManager> rm, policy_op op, std::vector<std::vector<std::string>> rules) {
    this->rm = rm;
    size_t role_count = count(this->value.begin(), this->value.end(), '$');

    if(role_count < 2)
        throw IllegalArgumentException("the number of \"$\" in role section should be at least 2");

    for(const auto& r : rules) {
        std::vector<std::string> rule = r;

        if(rule.size() < role_count)
            throw IllegalArgumentException("role policy elements do no meet role section");
        if(rule.size() > role_count)
            rule = std::vector<std::string>(rule.begin(), rule.begin() + role_count);

        std::vector<std::string> domain(rule.begin() + 2, rule.end());

        switch(op) {
            case policy_op::policy_add:
                this->rm->AddLink(rule[0], rule[1], domain);
                break;
            case policy_op::policy_remove:
                this->rm->DeleteLink(rule[0], rule[1], domain);
        }
    }
}

/***********************************************************************************************
 ***                              Section::BuildRoleLinks                                    ***
 ***********************************************************************************************
 * DESCRIPTION: Adds inheritance links for all Roles. What differs from                        *
 *              BuildIncrementalRoleLinks is that BuildRoleLinks uses inner policy rules to    *
 *              build its' inheritance links.                                                  *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   rm -- RoleManager.                                                                 *
 *                                                                                             *
 * OUTPUT:   NONE                                                                              *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
void Section::BuildRoleLinks(std::shared_ptr<RoleManager> rm) {
    this->rm = rm;
    size_t role_count = count(this->value.begin(), this->value.end(), '$');

    if(role_count < 2)
        throw IllegalArgumentException("the number of \"$\" in role section should be at least 2");
   
    for(const auto& p : policy) {
        std::vector<std::string> rule = p;

        if(rule.size() < role_count)
            throw IllegalArgumentException("role policy elements do not meet role section");
        if(rule.size() > role_count)
            rule = std::vector<std::string>(rule.begin(), rule.begin() + role_count);

        std::vector<std::string> domain(rule.begin() + 2, rule.end());
        this->rm->AddLink(rule[0], rule[1], domain);
    }
}


} // namespace caep 

#endif
