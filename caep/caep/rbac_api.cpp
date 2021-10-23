#ifndef CAEP_RBAC_API_CPP
#define CAEP_RBAC_API_CPP


#include "./caeper.h"
#include "../exception/caep_enforcer_exception.h"
#include "../util/caep_util.h"

namespace caep {

// GetRolesForUser gets the roles that a user has.
std::vector<std::string> Caeper::GetRolesForUser(const std::string& name, const std::vector<std::string>& domain) {
    std::vector<std::string> res = m_model->m["r"].section_map["r"]->rm->GetRoles(name, domain);
    return res;
}

// GetUsersForRole gets the users that has a role.
std::vector<std::string> Caeper::GetUsersForRole(const std::string& name, const std::vector<std::string>& domain) {
    std::vector<std::string> res = m_model->m["r"].section_map["r"]->rm->GetUsers(name, domain);
    return res;
}

// HasRoleForUser determines whether a user has a role.
bool Caeper::HasRoleForUser(const std::string& name, const std::string& role) {
    std::vector<std::string> domain;
    std::vector<std::string> roles = this->GetRolesForUser(name, domain);

    bool has_role = false;
    for (size_t i = 0 ; i < roles.size() ; i++) {
        if (roles[i] == role) {
            has_role = true;
            break;
        }
    }

    return has_role;
}

// AddRoleForUser adds a role for a user.
// Returns false if the user already has the role (aka not affected).
bool Caeper::AddRoleForUser(const std::string& user, const std::string& role) {
    std::vector<std::string> params{user, role};
    return this->AddRolePolicy(params);
}

// AddRolesForUser adds roles for a user.
// Returns false if the user already has the roles (aka not affected).
bool Caeper::AddRolesForUser(const std::string& user, const std::vector<std::string>& roles) {
    bool f = false;
    for(int i=0;i<roles.size();i++) {
        bool b = this->AddRolePolicy({user, roles[i]});
        if(b)
            f = true;
    }
    return f;
}

// AddPermissionForUser adds a permission for a user or role.
// Returns false if the user or role already has the permission (aka not affected).
bool Caeper::AddPermissionForUser(const std::string& user, const std::vector<std::string>& permission) {
    return this->AddPolicy(CaepUtil::JoinSlice(user, permission));
}

// DeletePermissionForUser deletes a permission for a user or role.
// Returns false if the user or role does not have the permission (aka not affected).
bool Caeper::DeletePermissionForUser(const std::string& user, const std::vector<std::string>& permission) {
    return this->RemovePolicy(CaepUtil::JoinSlice(user, permission));
}

// DeletePermissionsForUser deletes permissions for a user or role.
// Returns false if the user or role does not have any permissions (aka not affected).
bool Caeper::DeletePermissionsForUser(const std::string& user) {
    std::vector<std::string> field_values{user};
    return this->RemoveFilteredPolicy(0, field_values);
}

// GetPermissionsForUser gets permissions for a user or role.
std::vector<std::vector<std::string>> Caeper::GetPermissionsForUser(const std::string& user) {
    std::vector<std::string> field_values{user};
    return this->GetFilteredPolicy(0, field_values);
}

// HasPermissionForUser determines whether a user has a permission.
bool Caeper::HasPermissionForUser(const std::string& user, const std::vector<std::string>& permission) {
    return this->HasPolicy(CaepUtil::JoinSlice(user, permission));
}

// GetImplicitRolesForUser gets implicit roles that a user has.
// Compared to GetRolesForUser(), this function retrieves indirect roles besides direct roles.
// For example:
// r, alice, role:admin
// r, role:admin, role:user
//
// GetRolesForUser("alice") can only get: ["role:admin"].
// But GetImplicitRolesForUser("alice") will get: ["role:admin", "role:user"].
std::vector<std::string> Caeper::GetImplicitRolesForUser(const std::string& name, const std::vector<std::string>& domain) {
    std::vector<std::string> res;
    std::unordered_map<std::string, bool> role_set;
    role_set[name] = true;

    std::vector<std::string> q;
    q.push_back(name);

    while (q.size() > 0) {
        std::string name = q[0];
        q.erase(q.begin());

        std::vector<std::string> roles = rm->GetRoles(name, domain);

        for (size_t i = 0; i < roles.size(); i++) {
            if (!(role_set.find(roles[i]) != role_set.end())) {
                res.push_back(roles[i]);
                q.push_back(roles[i]);
                role_set[roles[i]] = true;
            }
        }
    }

    return res;
}

// GetImplicitPermissionsForUser gets implicit permissions for a user or role.
// Compared to GetPermissionsForUser(), this function retrieves permissions for inherited roles.
// For example:
// a, admin, data1, read
// a, alice, data2, read
// r, alice, admin
//
// GetPermissionsForUser("alice") can only get: [["alice", "data2", "read"]].
// But GetImplicitPermissionsForUser("alice") will get: [["admin", "data1", "read"], ["alice", "data2", "read"]].
std::vector<std::vector<std::string>> Caeper::GetImplicitPermissionsForUser(const std::string& user, const std::vector<std::string>& domain) {
    std::vector<std::string> roles = this->GetImplicitRolesForUser(user, domain);
    roles.insert(roles.begin(), user);

    bool with_domain = false;
    if (domain.size() == 1)
        with_domain = true;
    else if (domain.size() > 1)
        throw CaepEnforcerException("Domain should be 1 parameter");

    std::vector<std::vector<std::string>> res;
    std::vector<std::vector<std::string>> permissions;

    for (size_t i = 0; i < roles.size(); i++) {
        if (with_domain)
            permissions = this->GetPermissionsForUserInDomain(roles[i], domain[0]);
        else
            permissions = this->GetPermissionsForUser(roles[i]);

        for (size_t i = 0 ; i < permissions.size() ; i++)
            res.push_back(permissions[i]);
    }

    return res;
}

// GetImplicitUsersForPermission gets implicit users for a permission.
// For example:
// a, admin, data1, read
// a, bob, data1, read
// r, alice, admin
//
// GetImplicitUsersForPermission("data1", "read") will get: ["alice", "bob"].
// Note: only users will be returned, roles (2nd arg in "r") will be excluded.
std::vector<std::string> Caeper::GetImplicitUsersForPermission(const std::vector<std::string>& permission) {
    std::vector<std::string> a_subjects = this->GetAllSubjects();
    std::vector<std::string> r_inherit = m_model->GetValuesForFieldInPolicyAllTypes("r", 1);
    std::vector<std::string> r_subjects = m_model->GetValuesForFieldInPolicyAllTypes("r", 0);

    std::vector<std::string> subjects(a_subjects);
    subjects.insert(subjects.end(), r_subjects.begin(), r_subjects.end());
    CaepUtil::ArrayRemoveDuplicates(subjects);

    std::vector<std::string> res;
    for(size_t i = 0; i < subjects.size(); i++) {
        bool allowed = this->Caep({subjects[i], permission[0], permission[1]});

        if(allowed) {
            res.push_back(subjects[i]);
        }
    }

    res = CaepUtil::SetSubtract(res, r_inherit);
    return res;
}

// DeleteRoleForUser deletes a role for a user.
// Returns false if the user does not have the role (aka not affected).
bool Caeper::DeleteRoleForUser(const std::string& user, const std::string& role) {
    std::vector<std::string> params{user, role};
    return this->RemoveRolePolicy(params);
}

// DeleteRolesForUser deletes all roles for a user.
// Returns false if the user does not have any roles (aka not affected).
bool Caeper::DeleteRolesForUser(const std::string& user) {
    std::vector<std::string> field_values{user};
    return this->RemoveFilteredRolePolicy(0, field_values);
}

// DeleteUser deletes a user.
// Returns false if the user does not exist (aka not affected).
bool Caeper::DeleteUser(const std::string& user) {
    std::vector<std::string> field_values{user};

    bool res1 = this->RemoveFilteredRolePolicy(0, field_values);

    bool res2 = this->RemoveFilteredPolicy(0, field_values);

    return res1 || res2;
}

// DeleteRole deletes a role.
// Returns false if the role does not exist (aka not affected).
bool Caeper::DeleteRole(const std::string& role) {
    std::vector<std::string> field_values{role};

    bool res1 = this->RemoveFilteredRolePolicy(1, field_values);

    bool res2 = this->RemoveFilteredPolicy(0, field_values);

    return res1 || res2;
}

// DeletePermission deletes a permission.
// Returns false if the permission does not exist (aka not affected).
bool Caeper::DeletePermission(const std::vector<std::string>& permission) {
    std::vector<std::string> field_values{permission};
    return this->RemoveFilteredPolicy(1, field_values);
}

} // namespace caep

#endif
