#ifndef CAEP_DEFAULT_ROLE_MANAGER_H
#define CAEP_DEFAULT_ROLE_MANAGER_H

#include <unordered_map>
#include <iostream>

#include "./role_manager.h"

namespace caep {

class Role;
typedef std::shared_ptr<Role> SRptr;
typedef std::weak_ptr<Role> WRptr;

using MatchingFunc = bool (*)(std::string, std::string);

class Role {
private:
    std::string name;

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
    // determine whether there is a role named "name" in all_roles. 
    bool HasRole(std::string name);

    // create a new role or return an existing role depending on whether the role is in manager.
    SRptr CreateRole(std::string name);

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

    // name is User's name.
    // According to the User_name, GetRoles finds roles a user inherits.
    std::vector<std::string> GetRoles(std::string name);

    // name is Role's name.
    // According to the Role_name, GetUsers finds users a role owns.
    std::vector<std::string> GetUsers(std::string name);

    void PrintRoles();
};

} // namespace caep 

#endif
