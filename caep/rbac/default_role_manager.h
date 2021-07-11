#ifndef CAEP_DEFAULT_ROLE_MANAGER_H
#define CAEP_DEFAULT_ROLE_MANAGER_H

#include <unordered_map>

#include "./role_manager.h"

namespace caep {

using MatchingFunc = bool (*)(std::string, std::string);

class Role {
private:
    std::vector<Role*> roles;
    
public:
    std::string name;

    static Role* NewRole(std::string name);

    void AddRole(Role* role);

    void DeleteRole(Role* role);

    bool HasRole(std::string name, int hierarchy_level);

    bool HadDirect(std::string name);

    std::string ToString();

    std::vector<std::string> GetRoles();
};

class DefaultRoleManager : public RoleManager {
private:
    std::unordered_map<std::string, Role*> all_roles;
    bool has_pattern;
    // the maximized allowd RBAC hirarchy level.
    int max_hierarchy_level;
    MatchingFunc mf;

    // determine whether there is a role named "name" in all_roles. 
    bool HasRole(std::string name);

    // create a new role or return an existing role depending on whether the role is in manager.
    Role* CreateRole(std::string name);

public:
    /*
     * DefaultRoleManager is the constructor for creating
     * an instance of the default RoleManager implementation.
     *
     * @para max_hierarchy_level the maximized allowed RBAC hirarchy level.
     */ 
    DefaultRoleManager(int max_hierarchy_level);
    
    /*
     * AddMatchingFunc allows RoleManager customizing role matching pattern.
     * And offers corresponding matching functions.
     */ 
    void AddMatchingFunc(MatchingFunc mf);

    /* clear all roles and users in RoleManager. 
     * Although there's no difference between "role" and "user" in caep.
     */
    void Clear();

    void AddLink(std::string name1, std::string name2);

    void DeleteLink(std::string name1, std::string name2);

    bool HasLink(std::string name1, std::string name2);

    std::vector<std::string> GetRoles(std::string name);

    std::vector<std::string> GetUsers(std::string name);

    void PrintRoles();
};

} // namespace caep 

#endif
