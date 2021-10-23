/* $Header:  ~/code/GitRepositories/MyGit/caep   2.1.0   22 Aug 2019 19:00:00   ArZr        $ */
/***********************************************************************************************
 ***                  C O N F I D E N T I A L  ---  A R Z R  S T U D I O S                   ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Caep                                                         *
 *                                                                                             *
 *                    File Name : role_manager.h                                               *
 *                                                                                             *
 *                   Programmer : Guan Zhe                                                     *
 *                                                                                             *
 *                   Start Date : Aug 22, 2019                                                 *
 *                                                                                             *
 *                  Last Update : Oct 20, 2021   [ArZr]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifndef CAEP_ROLE_MANAGER_H
#define CAEP_ROLE_MANAGER_H 

#include <string>
#include <vector>

namespace caep {

class RoleManager {
public:

    /*-------------------------------------------------
     * @brief Constructor & Destructor.
     */ 
    RoleManager() {}
    virtual ~RoleManager() {}

    /*-------------------------------------------------
     * @brief Clears all roles in RoleManager.
     */
    virtual void Clear() = 0;
    
    /*
     * @brief Builds an inheritance links between two roles.
     *
     * @param role1 child role.
     * @param role2 parent role.
     * @param domain prefix of the role, determines their domain.
     */
    virtual void AddLink(std::string role1, std::string role2, std::vector<std::string> domain = {}) = 0;

    /*
     * @brief Deletes an inheritance links between two roles.
     *
     * @param role1 child role.
     * @param role2 parent role.
     * @param domain prefix of the role, determines their domain.
     */
    virtual void DeleteLink(std::string role1, std::string role2, std::vector<std::string> domain = {}) = 0;

    /*
     * @brief Determines whether there is an inheritance link between two roles.
     *
     * @param role1 child role.
     * @param role2 parent role.
     * @param domain prefix of the role, determines their domain.
     */
    virtual bool HasLink(std::string role1, std::string role2, std::vector<std::string> domain = {}) = 0;

    /*
     * @brief Get all roles that a user owns.
     *
     * @param user user's name.
     * @param domain prefix of the user, determines user's domain.
     */
    virtual std::vector<std::string> GetRoles(std::string user, std::vector<std::string> domain = {}) = 0;

    /*
     * @brief Get all users that a role owns.
     *
     * @param role role's name.
     * @param domain prefix of the role, determines role's domain.
     */
    virtual std::vector<std::string> GetUsers(std::string role, std::vector<std::string> domain = {}) = 0;

    /*
     * @brief Prints all roles in the rolemanager.
     */
    virtual void PrintRoles() = 0;
};


} // namespace caep 

#endif
