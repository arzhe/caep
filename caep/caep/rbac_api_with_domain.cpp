#ifndef CAEP_RBAC_API_WITH_DOMAINS_CPP
#define CAEP_RBAC_API_WITH_DOMAINS_CPP


#include "./caeper.h"

namespace caep {

// GetUsersForRoleInDomain gets the users that has a role inside a domain. Add by Gordon
std::vector<std::string> Caeper::GetUsersForRoleInDomain(const std::string& name, const std::string& domain) {
    std::vector<std::string> domains{domain};
	std::vector<std::string> res = m_model->m["r"].section_map["r"]->rm->GetUsers(name, domains);
	return res;
}

// GetRolesForUserInDomain gets the roles that a user has inside a domain.
std::vector<std::string> Caeper::GetRolesForUserInDomain(const std::string& name, const std::string& domain) {
    std::vector<std::string> domains{domain};
	std::vector<std::string> res = m_model->m["r"].section_map["r"]->rm->GetRoles(name, domains);
	return res;
}

// GetPermissionsForUserInDomain gets permissions for a user or role inside a domain.
std::vector<std::vector<std::string>> Caeper::GetPermissionsForUserInDomain(const std::string& user, const std::string& domain) {
    std::vector<std::string> field_values{user, domain};
	return this->GetFilteredPolicy(0, field_values);
}

// AddRoleForUserInDomain adds a role for a user inside a domain.
// Returns false if the user already has the role (aka not affected).
bool Caeper::AddRoleForUserInDomain(const std::string& user, const std::string& role, const std::string& domain) {
    std::vector<std::string> params{user, role, domain};
	return this->AddRolePolicy(params);
}

// DeleteRoleForUserInDomain deletes a role for a user inside a domain.
// Returns false if the user does not have the role (aka not affected).
bool Caeper::DeleteRoleForUserInDomain(const std::string& user, const std::string& role, const std::string& domain) {
    std::vector<std::string> params{user, role, domain};
	return this->RemoveRolePolicy(params);
}


} // namespace caep

#endif 
