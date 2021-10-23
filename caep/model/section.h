/* $Header:  ~/code/GitRepositories/MyGit/caep   2.1.0   22 Aug 2019 19:00:00   ArZr        $ */
/***********************************************************************************************
 ***                  C O N F I D E N T I A L  ---  A R Z R  S T U D I O S                   ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Caep                                                         *
 *                                                                                             *
 *                    File Name : section.h                                                    *
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
#ifndef CAEP_SECTION_H
#define CAEP_SECTION_H

#include <memory>

#include "../rbac/role_manager.h"

namespace caep {

/*------------------------------------------------------------------------------------------
 * @brief Options to determines whether to build or delete inheritance links.
 */
enum policy_op {
    policy_add,
    policy_remove
};

typedef enum policy_op policy_op;

/*------------------------------------------------------------------------------------------
 * @breif class Section represents an expression in CAEP model.
 *
 *                  eg: # model.ini
 *                  [applicability]
 *                  a = sub, res, act
 * 
 *                  eg: # policy.csv
 *                  a, Alice, data, read
 *                  a, Bob, data, write
 * 
 *  in this case:
 *  
 *  key -- "a"
 *  value -- "sub, res, act"
 *  key_val -- "sub", "res", "act"
 *  policy -- {{"Alice", "data", "read"},
 *            {"Bob", "data", "write"}}
 *  rm -- RoleManager 
 */
class Section {
public:
    std::string key;
    std::string value;
    std::vector<std::string> tokens;
    std::vector<std::vector<std::string>> policy;
    std::shared_ptr<RoleManager> rm;

    /*
     * @brief Uses outter rules to build or delete inheritance links for all roles, according to the 'op'.
     */
    void BuildIncrementalRoleLinks(std::shared_ptr<RoleManager> rm, policy_op op, std::vector<std::vector<std::string>> rules);
    
    /*
     * @brief Uses inner rules to build inheritance links for all roles.
     */
    void BuildRoleLinks(std::shared_ptr<RoleManager> rm);
};

} // namespace caep 

#endif
