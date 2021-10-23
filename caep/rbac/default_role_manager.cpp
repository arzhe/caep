/* $Header:  ~/code/GitRepositories/MyGit/caep   2.1.0   22 Aug 2019 19:00:00   ArZr        $ */
/***********************************************************************************************
 ***                  C O N F I D E N T I A L  ---  A R Z R  S T U D I O S                   ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Caep                                                         *
 *                                                                                             *
 *                    File Name : default_role_manager.cpp                                     *
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

#ifndef CAEP_DEFAULT_ROLE_MANAGER_CPP
#define CAEP_DEFAULT_ROLE_MANAGER_CPP

#include "./default_role_manager.h"
#include "../exception/rbac_exception.h"
#include "../exception/weak_ptr_exception.h"

namespace caep {

/***********************************************************************************************
 ***                                Role::NewRole                                            ***
 ***********************************************************************************************
 * DESCRIPTION: Gets a shared_ptr<Role>.                                                       *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   name -- It is used to initialize Role::name.                                       *
 *                                                                                             *
 * OUTPUT:   Returns smart pointer to Role.                                                    *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
SRptr Role::NewRole(std::string name) {
    SRptr role(new Role(name));
    return role;
}

/***********************************************************************************************
 ***                                Role::AddRole                                            ***
 ***********************************************************************************************
 * DESCRIPTION: Adds a Role to Role::all_roles, in other words, a parent Role is added to      *
 *              the current Role inheritance tree. A Role Tree like:                           *
 *                                      admin                                                  *
 *                                     /     \                                                 *
 *                                  group1 group2                                              *
 *                                   /   \ /   \                                               *
 *                                Alice  Bob Charlse                                           *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   role -- A weak_ptr<Role>, which is to avoid self-referencing smart pointer in Role.*
 *                                                                                             *
 * OUTPUT:   NONE                                                                              *
 *                                                                                             *
 * WARNINGS:    Better to check if INPUT's raw pointer is not nullptr or released.             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
void Role::AddRole(WRptr role) {
    SRptr sr_ptr1, sr_ptr2;

    try{
        sr_ptr1 = role.lock();
    }
    catch(std::bad_weak_ptr bwp) {
        throw WeakPtrException("Added Role has been expired!");
    }

    for(const auto& it : this->roles) {
        try{
            sr_ptr2 = it.lock();
        }
        catch(std::bad_weak_ptr bwp) {
            throw WeakPtrException("There exits expired role in role tree.");
        }
        if(!(sr_ptr1->name).compare(sr_ptr2->name))
            return;
    }

    this->roles.push_back(role);
}

/***********************************************************************************************
 ***                                Role::DeleteRole                                         ***
 ***********************************************************************************************
 * DESCRIPTION: Deletes a Role from Role::all_roles, in other words, a parent Role is delete   *
 *              from the current Role inheritance tree.                                        *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   role -- A weak_ptr<Role>, which is to avoid self-referencing smart pointer in Role.*
 *                                                                                             *
 * OUTPUT:   NONE                                                                              *
 *                                                                                             *
 * WARNINGS:    Better to check if INPUT's raw pointer is not nullptr or released.             *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
void Role::DeleteRole(WRptr role) {
    SRptr sr_ptr1, sr_ptr2;

    try{
        sr_ptr1 = role.lock();
    }
    catch(std::bad_weak_ptr bwp) {
        throw WeakPtrException("Deleted Role has been expired!");
    }

    for(auto it = this->roles.begin(); it != this->roles.end();) {
        try{
            sr_ptr2 = it->lock();
        }
        catch(std::bad_weak_ptr bwp) {
            throw WeakPtrException("There exits expired role in role tree.");
        }
        if(!(sr_ptr1->name).compare(sr_ptr2->name)) {
            this->roles.erase(it);
            --it;
        }
        ++it;
    }
    return;
}

/***********************************************************************************************
 ***                                Role::HasRole                                            ***
 ***********************************************************************************************
 * DESCRIPTION: Determines if current Role has a specified parent Role. Moreover,              *
 *              this function will search for the Role recursively. That is to say, not only   *
 *              it searches for its parent Role, but also its parent Role's parent Role,       *
 *              and so on, for each of the Roles.                                              *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   name -- The Role to search for.                                                    *
 *          hierarchy_level -- Maximum search depth.                                           *
 *                                                                                             *
 * OUTPUT:   Returns true if the specified Role is found, else returns false.                  *
 *                                                                                             *
 * WARNINGS:    If hierarchy_level <= 0, HasRole won't search for the Role.                    *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
bool Role::HasRole(std::string name, int hierarchy_level) {
    if(!this->name.compare(name))
        return true;

    if(hierarchy_level <= 0)
        return false;

    SRptr sr_ptr;
    for(const auto& r : roles) {
        try{
            sr_ptr = r.lock();
        }
        catch(std::bad_weak_ptr bwp) {
            throw WeakPtrException("There exits expired role in role tree.");
        }
        if(sr_ptr->HasRole(name, --hierarchy_level))
            return true;
    }
    return false;
}

/***********************************************************************************************
 ***                                Role::HasDirectRole                                      ***
 ***********************************************************************************************
 * DESCRIPTION: Determines if current Role has a specified parent Role, but it only            *
 *              searches for adjacent parent Roles.                                            *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   name -- The Role to search for.                                                    *
 *                                                                                             *
 * OUTPUT:   Returns true if the specified Role is found, else returns false.                  *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
bool Role::HasDirectRole(std::string name) {
    SRptr sr_ptr;
    for(const auto& r : roles) {
        try{
            sr_ptr = r.lock();
        }
        catch(std::bad_weak_ptr bwp) {
            throw WeakPtrException("There exits expired role in role tree.");
        }
        if(!(sr_ptr->name).compare(name))
            return true;
        return false;
    }
}

/***********************************************************************************************
 ***                                Role::ToString                                           ***
 ***********************************************************************************************
 * DESCRIPTION: Returns all Roles' name as a string.                                           *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   NONE                                                                               *
 *                                                                                             *
 * OUTPUT:   Formatted string.                                                                 *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
std::string Role::ToString() {
    if(this->roles.size() == 0)
        return name;

    std::string names = "";
    if(this->roles.size() != 1) 
        names +=  "(";

    SRptr sr_ptr;
    for(size_t i = 0; i < roles.size(); ++i) {
        auto r = roles[i];
        try{
            sr_ptr = r.lock();
        }
        catch(std::bad_weak_ptr bwp) {
            throw WeakPtrException("There exits expired role in role tree.");
        }
        if(i == 0)
            names += sr_ptr->name;
        else
            names += ", " + sr_ptr->name;
    }

    if(this->roles.size() != 1)
        names += ")";

    return name + " < " + names;
}

/***********************************************************************************************
 ***                                Role::GetRoles                                           ***
 ***********************************************************************************************
 * DESCRIPTION: Returns all Roles' name as an array.                                           *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   NONE                                                                               *
 *                                                                                             *
 * OUTPUT:   A vector<string>, which stores all Roles' name.                                   *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
std::vector<std::string> Role::GetRoles() {
    std::vector<std::string> names;
    SRptr sr_ptr;
    for(const auto& r : roles) {
        try{
            auto sr_ptr = r.lock();
        }
        catch(std::bad_weak_ptr bwp) {
            throw WeakPtrException("There exits expired role in role tree.");
        }
        names.push_back(sr_ptr->name);
    }
    return names;
}

/***********************************************************************************************
 ***                            DefaultRoleManager::HasRole                                  ***
 ***********************************************************************************************
 * DESCRIPTION: Determines if there is a specified Role in RoleManager. Moreover, you can      *
 *              also search for the Role using the specified matching function.                *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   name -- Name of the Role to search for.                                            *
 *                                                                                             *
 * OUTPUT:   Returns true if the Role is found, else returns false.                            *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
bool DefaultRoleManager::HasRole(std::string name) {
    bool ok = false;
    if(this->has_pattern) {
        for(const auto& r : all_roles) {
            if(this->mf(name, r.first))
                ok = true;
        }
    }
    ok = ok || this->all_roles.find(name) != this->all_roles.end();

    return ok;
}

/***********************************************************************************************
 ***                            DefaultRoleManager::CreateRole                               ***
 ***********************************************************************************************
 * DESCRIPTION: Creates a new Role in RoleManager if the specified Role is not in RoleManager. *
 *              Or, if there is a specified Role in RoleManager, the existed Role would be     *
 *              returned. Moreover, if matching function is specified in RoleManager, it would *
 *              be applied to searching for the all Roles in RoleManager. All Roles that meet  *
 *              matching function would be added to Role Tree of the new Role or the existed   *
 *              Role.                                                                          *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   name -- Name of the Role to create or return.                                      *
 *                                                                                             *
 * OUTPUT:   Returns a new Role or existed Role.                                               *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
SRptr DefaultRoleManager::CreateRole(std::string name) {
    SRptr role;
    bool found = all_roles.find(name) != all_roles.end();
    if(!found) {
        all_roles[name] = Role::NewRole(name);
        role = all_roles[name];
    }
    else 
        role = all_roles[name];

    if(this->has_pattern) {
        for(const auto& r : all_roles) {
            if(this->mf(name, r.first) && name != r.first) {
                SRptr role1;
                bool found1 = this->all_roles.find(r.first) != this->all_roles.end();
                if(!found1) {
                    all_roles[r.first] = Role::NewRole(r.first);
                    role1 = all_roles[r.first];
                }
                else 
                    role1 = all_roles[r.first];
                role->AddRole(role1);
            }
        }
    }
    return role;
}

/***********************************************************************************************
 ***                        DefaultRoleManager::DefaultRoleManager                           ***
 ***********************************************************************************************
 * DESCRIPTION: Constructor for DefaultRoleManager, manually initialize the                    *
 *              max_hierarchy_level and has_pattern.                                           *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   max_hierarchy_level -- Set the maximum search depth for Role Tree.                 *
 *                                                                                             *
 * OUTPUT:   NONE                                                                              *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
DefaultRoleManager::DefaultRoleManager(int max_hierarchy_level) {
    this->max_hierarchy_level = max_hierarchy_level;
    this->has_pattern = false;
}

/***********************************************************************************************
 ***                        DefaultRoleManager::AddMatchingFunc                              ***
 ***********************************************************************************************
 * DESCRIPTION: Adds specified matching function to the RoleManager.                           *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   mf -- The matching function, which is used to match two Roles.                     *
 *                                                                                             *
 * OUTPUT:   NONE                                                                              *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
void DefaultRoleManager::AddMatchingFunc(MatchingFunc mf) {
    this->has_pattern = true;
    this->mf = mf;
}

/***********************************************************************************************
 ***                        DefaultRoleManager::Clear                                        ***
 ***********************************************************************************************
 * DESCRIPTION: Clears all Roles in RoleManager. It's the only way to erase the Role           *
 *              in RoleManager.                                                                *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   NONE                                                                               *
 *                                                                                             *
 * OUTPUT:   NONE                                                                              *
 *                                                                                             *
 * WARNINGS:    Caep do not support erasing single or multiple Roles in the RoleManager,       *
 *              but only can clears all Roles. That is to say, clear all of them,              *
 *              or none of them.                                                               *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
void DefaultRoleManager::Clear() {
    std::unordered_map<std::string, SRptr>().swap(this->all_roles);
}

/***********************************************************************************************
 ***                        DefaultRoleManager::AddLink                                      ***
 ***********************************************************************************************
 * DESCRIPTION: Builds an inheritance link between two Roles. If the Role is not in RoleManager*
 *              it would be created in RoleManager. Therefore, you can use this function       *
 *              directly without creating Roles first.                                         *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   name1 -- Child Role's name.                                                        *
 *          name2 -- Parent Role's name.                                                       *
 *          domain -- Describes the domain in which Roles is located.                          *
 *                                                                                             *
 * OUTPUT:   NONE                                                                              *
 *                                                                                             *
 * WARNINGS:    Size of domain should not exceed over 1, otherwise, an exception will be thrown*
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
void DefaultRoleManager::AddLink(std::string name1, std::string name2, std::vector<std::string> domain) {
    if(domain.size() == 1) {
        name1 = domain[0] + "::" + name1;
        name2 = domain[0] + "::" + name2;
    } else if(domain.size() > 1)
        throw CaepRbacException("error: domain should be 1 parameter");

    auto role1 = this->CreateRole(name1);
    auto role2 = this->CreateRole(name2);
    role1->AddRole(role2);
}

/***********************************************************************************************
 ***                        DefaultRoleManager::DeleteLink                                   ***
 ***********************************************************************************************
 * DESCRIPTION: Deletes an inheritance link between two Roles. DeleteLink won't erase Roles in *
 *              RoleManager, but only erase Roles in Role Tree. Note: the Role in RoleManager  *
 *              persists util the DefaultRoleManager::Clear function is called.                *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   name1 -- Child Role's name.                                                        *
 *          name2 -- Parent Role's name.                                                       *
 *          domain -- Describes the domain in which Roles is located.                          *
 *                                                                                             *
 * OUTPUT:   NONE                                                                              *
 *                                                                                             *
 * WARNINGS:    Size of domain should not exceed over 1, otherwise, an exception will be thrown*
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
void DefaultRoleManager::DeleteLink(std::string name1, std::string name2, std::vector<std::string> domain) {
    if(domain.size() == 1) {
        name1 = domain[0] + "::" + name1;
        name2 = domain[0] + "::" + name2;
    } else if(domain.size() > 1)
        throw CaepRbacException("error: domain should be 1 parameter");

    if(!HasRole(name1) || !HasRole(name2))
        throw CaepRbacException("error: name1 or name2 does not exist");

    auto role1 = this->CreateRole(name1);
    auto role2 = this->CreateRole(name2);
    role1->DeleteRole(role2);
}

/***********************************************************************************************
 ***                        DefaultRoleManager::HasLink                                      ***
 ***********************************************************************************************
 * DESCRIPTION: Determines if there is an inhieritance link between two Roles.                 *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   name1 -- Child Role's name.                                                        *
 *          name2 -- Parent Role's name.                                                       *
 *          domain -- Describes the domain in which Roles is located.                          *
 *                                                                                             *
 * OUTPUT:   If domain::name1 inherits domain::name2, returns true, else return false.         *
 *                                                                                             *
 * WARNINGS:    Size of domain should not exceed over 1, otherwise, an exception will be thrown*
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
bool DefaultRoleManager::HasLink(std::string name1, std::string name2, std::vector<std::string> domain) {
    if(domain.size() == 1) {
        name1 = domain[0] + "::" + name1;
        name2 = domain[0] + "::" + name2;
    } else if(domain.size() > 1)
        throw CaepRbacException("error: domain should be 1 parameter");

    if(!name1.compare(name2))
        return true;
    if(!HasRole(name1) || !HasRole(name2))
        return false;

    auto role1 = this->CreateRole(name1);
    return role1->HasRole(name2, max_hierarchy_level);
}

/***********************************************************************************************
 ***                        DefaultRoleManager::GetRoles                                     ***
 ***********************************************************************************************
 * DESCRIPTION: Gets all Roles that an user owns.                                              *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   name -- User's name.                                                               *
 *          domain -- Describes the domain in which Roles is located.                          *
 *                                                                                             *
 * OUTPUT:   Returns an array that stores all Roles'name that an user owns.                    *
 *                                                                                             *
 * WARNINGS:    Size of domain should not exceed over 1, otherwise, an exception will be thrown*
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
std::vector<std::string> DefaultRoleManager::GetRoles(std::string name, std::vector<std::string> domain) {
    int domain_length = int(domain.size());
    if(domain_length == 1)
        name = domain[0] + "::" + name;
    else if(domain_length > 1)
        throw CaepRbacException("error: domain should be 1 parameter");

    if(!HasRole(name)) {
        std::vector<std::string> roles;
        return roles;
    }

    auto roles = this->CreateRole(name)->GetRoles();
    if(domain_length == 1) {
        for(auto& role : roles) {
            role = role.substr(domain[0].length() + 2, role.length() - domain[0].length() - 2);
        }
    }

    return roles;
}

/***********************************************************************************************
 ***                        DefaultRoleManager::GetUsers                                     ***
 ***********************************************************************************************
 * DESCRIPTION: Gets all Users that a Role owns.                                               *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   name -- Role's name.                                                               *
 *          domain -- Describes the domain in which Roles is located.                          *
 *                                                                                             *
 * OUTPUT:   Returns an array that stores all Users'name that a Role owns.                     *
 *                                                                                             *
 * WARNINGS:    Size of domain should not exceed over 1, otherwise, an exception will be thrown*
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
std::vector<std::string> DefaultRoleManager::GetUsers(std::string name, std::vector<std::string> domain) {
    int domain_length = int(domain.size());
    if(domain_length == 1)
        name = domain[0] + "::" + name;
    else if(domain_length > 1)
        throw CaepRbacException("error: domain should be 1 parameter");

    if(!HasRole(name))
        throw CaepRbacException("error: name does not exist");

    std::vector<std::string> names;
    for(const auto& it : all_roles) {
        auto role = it.second;
        if(role->HasDirectRole(name))
            names.push_back(it.first);
    }

    if(domain_length == 1) {
        for(auto& name : names) {
            name = name.substr(domain[0].length() + 2, name.length() - domain[0].length() - 2);
        }
    }
    return names;
}

/***********************************************************************************************
 ***                        DefaultRoleManager::PrintRoles                                   ***
 ***********************************************************************************************
 * DESCRIPTION: Prints all Roles in RoleManager and their Roll Tree.                           *
 *                                                                                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   NONE                                                                               *
 *                                                                                             *
 * OUTPUT:  NONE                                                                               *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
void DefaultRoleManager::PrintRoles() {
    std::string text;
    for(auto it = this->all_roles.begin(); it != this->all_roles.end(); ++it)
        text += it->second->ToString() + ";\n";
    std::cout << text << std::endl;
}

} // namespace  caep 


#endif
