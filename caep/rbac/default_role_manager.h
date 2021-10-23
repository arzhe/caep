/* $Header:  ~/code/GitRepositories/MyGit/caep   2.1.0   22 Aug 2019 19:00:00   ArZr        $ */
/***********************************************************************************************
 ***                  C O N F I D E N T I A L  ---  A R Z R  S T U D I O S                   ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Caep                                                         *
 *                                                                                             *
 *                    File Name : default_role_manager.h                                       *
 *                                                                                             *
 *                   Programmer : Guan Zhe                                                     *
 *                                                                                             *
 *                   Start Date : Aug 22, 2019                                                 *
 *                                                                                             *
 *                  Last Update : Oct 20, 2021   [ArZr]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Role::Role -- Constructor.                                                                *
 *   Role::NewRole -- Gets a shared_ptr<Role>.                                                 *
 *   Role::AddRole -- Adds a Role to Role::all_roles.                                          *
 *   Role::DeleteRole -- Deletes a Role from Role::all_roles.                                  *
 *   Role::HasRole -- Determines if current Role::all_roles has a Role recursively.            *
 *   Role::HasDirectRole -- Determines if current Role::all_roles has a Role directly.         *
 *   Role::ToString -- Prints all Role's name in Role::all_roles.                              *
 *   Role::GetRoles -- Gets all Role's name in Role::all_roles.                                *
 *                                                                                             *
 *   DefaultRoleManager::HasRole -- Determines if RoleManager has a Role directly.             *
 *   DefaultRoleManager::CreateRole -- Gets a new Role or an existed Role.                     *
 *   DefaultRoleManager::DefaultRoleManager -- Constructor and specifies a hierarchy_level.    *
 *   DefaultRoleManager::AddMatchingFunc -- Adds a match function to RoleManager.              *
 *   DefaultRoleManager::Clear -- Clears all Roles.                                            *
 *   DefaultRoleManager::AddLink -- Builds a hieritance link between two Roles.                *
 *   DefaultRoleManager::DeleteLink -- Deletes a hieritance link between two Roles.            *
 *   DefaultRoleManager::HasLink -- Determines if there is a hieritance link between two Roles.*
 *   DefaultRoleManager::GetRoles -- Gets all Roles that a user owns.                          *
 *   DefaultRoleManager::GetUsers -- Gets all Users that a Role owns.                          *
 *   DefaultRoleManager::PrintRoles -- Prints all Roles in Rolemanager.                        *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#ifndef CAEP_DEFAULT_ROLE_MANAGER_H
#define CAEP_DEFAULT_ROLE_MANAGER_H

#include <unordered_map>
#include <iostream>

#include "./role_manager.h"

namespace caep {

/*-------------------------------------------------------------------------------------------
 * @brief Forward declaration.
 */
class Role;

typedef std::shared_ptr<Role> SRptr;
typedef std::weak_ptr<Role> WRptr;

/*-------------------------------------------------------------------------------------------
 * @brief These match function are for matching two Roles in customed pattern.
 */
using MatchingFunc = bool (*)(std::string, std::string);

/*-------------------------------------------------------------------------------------------
 * @brief This class records all messages that a Role has. Such as, a Role's name, it could
 * be a user's name or a role's name, because Caep do not distinguish user and role, all of
 * them are regarded as string-Type. In order to distinguish user and role, you can add suffix
 * to them, eg: User::Alice, Role::group. Also, Role class stores the inheritance tree that a 
 * Role owns, which is the bottom node of the inheritance tree.
 */
class Role {
private:
    /*
     * @brief Role's name.
     */
    std::string name;

    /*
     * @brief Describes a Role's inheritance tree, which is the bottom node of the inheritance tree.
     */
    std::vector<WRptr> roles;
public:
    
    explicit Role(std::string name) : name(name) {};    

    static SRptr NewRole(std::string name);
    void AddRole(WRptr role);
    void DeleteRole(WRptr role);
    bool HasRole(std::string name, int hierarchy_level);
    bool HasDirectRole(std::string name);

    std::string ToString();

    std::vector<std::string> GetRoles();
};

class DefaultRoleManager : public RoleManager {
private:
    std::unordered_map<std::string, SRptr> all_roles;
    bool has_pattern;
    MatchingFunc mf;
    int max_hierarchy_level;

    bool HasRole(std::string name);

    SRptr CreateRole(std::string name);

public:
    DefaultRoleManager(int max_hierarchy_level);
    
    void AddMatchingFunc(MatchingFunc mf);

    void Clear();
    void AddLink(std::string name1, std::string name2, std::vector<std::string> domain = {});
    void DeleteLink(std::string name1, std::string name2, std::vector<std::string> domain = {});
    bool HasLink(std::string name1, std::string name2, std::vector<std::string> domain = {});

    std::vector<std::string> GetRoles(std::string name, std::vector<std::string> domain = {});
    std::vector<std::string> GetUsers(std::string name, std::vector<std::string> domain = {});

    void PrintRoles();
};

} // namespace caep 

#endif
