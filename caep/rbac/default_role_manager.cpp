#ifndef CAEP_DEFAULT_ROLE_MANAGER_CPP
#define CAEP_DEFAULT_ROLE_MANAGER_CPP

#include "./default_role_manager.h"
#include "../exception/rbac_exception.h"
#include "../exception/weak_ptr_exception.h"

namespace caep {

SRptr Role::NewRole(std::string name) {
    SRptr role(new Role(name));
    return role;
}

void Role::AddRole(WRptr role) {
    for(auto& wptr : this->roles) {
        try{
            auto sptr = wptr.lock();
            auto sptr1 = role.lock();
            if(!sptr->name.compare(sptr1->name))
                return;
        }
        catch(std::bad_weak_ptr bwp) {
            throw WeakPtrException("Null Role in roles");
        }
    }
    this->roles.push_back(role);
}

void Role::DeleteRole(WRptr role) {
    for(auto it = this->roles.begin(); it != this->roles.end();) {
        try{
            auto sptr = it->lock();
            auto sptr1 = role.lock();
            if(!sptr->name.compare(sptr1->name)) {
                this->roles.erase(it);
                --it; 
            }
            ++it;
        }
        catch(std::bad_weak_ptr bwp) {
            throw WeakPtrException("Null Role in roles");
        }
    }
    return;
}

bool Role::HasRole(std::string name, int hierarchy_level) {
    if(!this->name.compare(name))
        return true;

    if(hierarchy_level <= 0)
        return false;

    for(auto &r : roles) {
        try{
            auto sptr = r.lock();
            if(sptr->HasRole(name, --hierarchy_level))
                return true;
        }
        catch(std::bad_weak_ptr bwp) {
            throw WeakPtrException("Null Role in roles");
        }
    }
    return false;
}

bool Role::HasDirectRole(std::string name) {
    for(auto &r : roles) {
        try{
            auto sptr = r.lock();
            if(!sptr->name.compare(name))
                return true;
            return false;
        }
        catch(std::bad_weak_ptr bwp) {
            throw WeakPtrException("Null Role in roles");
        }
    }
}

std::string Role::ToString() {
    if(this->roles.size() == 0)
        return name;

    std::string names = "";
    if(this->roles.size() != 1) 
        names +=  "(";

    for(int i = 0; i < roles.size(); ++i) {
        auto r = roles[i];
        try{
            auto sptr = r.lock();
            if(i == 0)
                names += sptr->name;
            else
                names += ", " + sptr->name;
        }
        catch(std::bad_weak_ptr bwp) {
            throw WeakPtrException("Null Role in roles");
        }
    }

    if(this->roles.size() != 1)
        names += ")";

    return name + " < " + names;
}

std::vector<std::string> Role::GetRoles() {
    std::vector<std::string> names;
    for(auto &r : roles) {
        try{
            auto sptr = r.lock();
            names.push_back(sptr->name);    
        }
        catch(std::bad_weak_ptr bwp) {
            throw WeakPtrException("Null Role in roles");
        }
    }
    return names;
}

bool DefaultRoleManager::HasRole(std::string name) {
    bool ok = false;
    if(this->has_pattern) {
        for(auto &r : all_roles) {
            if(this->mf(name, r.first))
                ok = true;
        }
    }
    ok = ok || this->all_roles.find(name) != this->all_roles.end();

    return ok;
}

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
        for(auto &r : all_roles) {
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

DefaultRoleManager::DefaultRoleManager(int max_hierarchy_level) {
    this->max_hierarchy_level = max_hierarchy_level;
    this->has_pattern = false;
}

void DefaultRoleManager::AddMatchingFunc(MatchingFunc mf) {
    this->has_pattern = true;
    this->mf = mf;
}

void DefaultRoleManager::Clear() {
    std::unordered_map<std::string, SRptr>().swap(this->all_roles);
}

void DefaultRoleManager::AddLink(std::string name1, std::string name2) {
    auto role1 = this->CreateRole(name1);
    auto role2 = this->CreateRole(name2);
    role1->AddRole(role2);
}

void DefaultRoleManager::DeleteLink(std::string name1, std::string name2) {
    if(!HasRole(name1) || !HasRole(name2))
        throw CaepRbacException("error: name1 or name2 does not exist");

    auto role1 = this->CreateRole(name1);
    auto role2 = this->CreateRole(name2);
    role1->DeleteRole(role2);
}

bool DefaultRoleManager::HasLink(std::string name1, std::string name2) {
    if(!name1.compare(name2))
        return true;
    if(!HasRole(name1) || !HasRole(name2))
        return false;

    auto role1 = this->CreateRole(name1);
    return role1->HasRole(name2, max_hierarchy_level);
}

std::vector<std::string> DefaultRoleManager::GetRoles(std::string name) {
    if(!HasRole(name)) {
        std::vector<std::string> roles;
        return roles;
    }

    auto roles = this->CreateRole(name)->GetRoles();
    return roles;
}

std::vector<std::string> DefaultRoleManager::GetUsers(std::string name) {
    std::vector<std::string> names;
    for(auto& it : all_roles) {
        auto role = it.second;
        if(role->HasDirectRole(name))
            names.push_back(name);
    }
    return names;
}


// Testing function.
void DefaultRoleManager::PrintRoles() {
    std::string text;
    for(auto it = this->all_roles.begin(); it != this->all_roles.end(); ++it)
        text += it->second->ToString() + ";\n";
    std::cout << text << std::endl;
}

} // namespace  caep 


#endif
