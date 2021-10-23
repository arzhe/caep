#ifndef CAEP_MANAGEMENT_API_CPP
#define CAEP_MANAGEMENT_API_CPP

#include "./caeper.h"

namespace caep {

// GetAllSubjects gets the list of subjects that show up in the current policy.
std::vector<std::string> Caeper::GetAllSubjects() {
    return m_model->GetValuesForFieldInPolicyAllTypes("a", 0);
}

// GetAllNamedObjects gets the list of subjects that show up in the current named policy.
std::vector<std::string> Caeper::GetAllNamedSubjects(const std::string& p_type) {
    return m_model->GetValuesForFieldInPolicy("a", p_type, 0);
}

// GetAllObjects gets the list of objects that show up in the current policy.
std::vector<std::string> Caeper::GetAllResources() {
    return m_model->GetValuesForFieldInPolicyAllTypes("a", 1);
}

// GetAllNamedObjects gets the list of objects that show up in the current named policy.
std::vector<std::string> Caeper::GetAllNamedResources(const std::string& p_type) {
    return m_model->GetValuesForFieldInPolicy("a", p_type, 1);
}

// GetAllActions gets the list of actions that show up in the current policy.
std::vector<std::string> Caeper::GetAllActions() {
    return m_model->GetValuesForFieldInPolicyAllTypes("a", 2);
}

// GetAllNamedActions gets the list of actions that show up in the current named policy.
std::vector<std::string> Caeper::GetAllNamedActions(const std::string& p_type) {
    return m_model->GetValuesForFieldInPolicy("a", p_type, 2);
}

// GetAllRoles gets the list of roles that show up in the current policy.
std::vector<std::string> Caeper::GetAllRoles() {
    return m_model->GetValuesForFieldInPolicyAllTypes("r", 1);
}

// GetAllNamedRoles gets the list of roles that show up in the current named policy.
std::vector<std::string> Caeper::GetAllNamedRoles(const std::string& p_type) {
    return m_model->GetValuesForFieldInPolicy("r", p_type, 1);
}

// GetPolicy gets all the authorization rules in the policy.
std::vector<std::vector<std::string>> Caeper::GetPolicy() {
    return this->GetNamedPolicy("a");
}

// GetFilteredPolicy gets all the authorization rules in the policy, field filters can be specified.
std::vector<std::vector<std::string>> Caeper::GetFilteredPolicy(int field_index, const std::vector<std::string>& field_values) {
    return this->GetFilteredNamedPolicy("a", field_index, field_values);
}

// GetNamedPolicy gets all the authorization rules in the named policy.
std::vector<std::vector<std::string>> Caeper::GetNamedPolicy(const std::string& p_type) {
    return m_model->GetPolicy("a", p_type);
}

// GetFilteredNamedPolicy gets all the authorization rules in the named policy, field filters can be specified.
std::vector<std::vector<std::string>> Caeper::GetFilteredNamedPolicy(const std::string& p_type, int field_index, const std::vector<std::string>& field_values) {
    return m_model->GetFilteredPolicy("a", p_type, field_index, field_values);
}

// GetRolePolicy gets all the role inheritance rules in the policy.
std::vector<std::vector<std::string>> Caeper::GetRolePolicy() {
    return this->GetNamedRolePolicy("r");
}

// GetFilteredRolePolicy gets all the role inheritance rules in the policy, field filters can be specified.
std::vector<std::vector<std::string>> Caeper::GetFilteredRolePolicy(int field_index, const std::vector<std::string>& field_values) {
    return this->GetFilteredNamedRolePolicy("r", field_index, field_values);
}

// GetNamedRolePolicy gets all the role inheritance rules in the policy.
std::vector<std::vector<std::string>> Caeper::GetNamedRolePolicy(const std::string& p_type) {
    return m_model->GetPolicy("r", p_type);
}

// GetFilteredNamedRolePolicy gets all the role inheritance rules in the policy, field filters can be specified.
std::vector<std::vector<std::string>> Caeper::GetFilteredNamedRolePolicy(const std::string& p_type, int field_index, const std::vector<std::string>& field_values) {
    return m_model->GetFilteredPolicy("r", p_type, field_index, field_values);
}

// HasPolicy determines whether an authorization rule exists.
bool Caeper::HasPolicy(const std::vector<std::string>& params) {
    return this->HasNamedPolicy("a", params);
}

// HasNamedPolicy determines whether a named authorization rule exists.
bool Caeper::HasNamedPolicy(const std::string& p_type, const std::vector<std::string>& params) {
    if (params.size() == 1) {
        std::vector<std::string> str_slice{params[0]};
        return m_model->HasPolicy("a", p_type, str_slice);
    }

    std::vector<std::string> policy;
    for (int i = 0; i < params.size(); i++)
        policy.push_back(params[i]);
    return m_model->HasPolicy("a", p_type, policy);
}

// AddPolicy adds an authorization rule to the current policy.
// If the rule already exists, the function returns false and the rule will not be added.
// Otherwise the function returns true by adding the new rule.
bool Caeper::AddPolicy(const std::vector<std::string>& params) {
    return this->AddNamedPolicy("a", params);
}

// AddPolicies adds authorization rules to the current policy.
// If the rule already exists, the function returns false for the corresponding rule and the rule will not be added.
// Otherwise the function returns true for the corresponding rule by adding the new rule.
bool Caeper::AddPolicies( const std::vector<std::vector<std::string>>& rules) {
    return this->AddNamedPolicies("a", rules);
}

// AddNamedPolicy adds an authorization rule to the current named policy.
// If the rule already exists, the function returns false and the rule will not be added.
// Otherwise the function returns true by adding the new rule.
bool Caeper::AddNamedPolicy(const std::string& p_type, const std::vector<std::string>& params) {
    if (params.size() == 1) {
        std::vector<std::string> str_slice{params[0]};
        return this->addPolicy("a", p_type, str_slice);
    }

    std::vector<std::string> policy;
    for (int i = 0 ; i < params.size() ; i++)
        policy.push_back(params[i]);
    return this->addPolicy("a", p_type, policy);
}

// AddNamedPolicies adds authorization rules to the current named policy.
// If the rule already exists, the function returns false for the corresponding rule and the rule will not be added.
// Otherwise the function returns true for the corresponding by adding the new rule.
bool Caeper::AddNamedPolicies(const std::string& p_type,  const std::vector<std::vector<std::string>>& rules) {
    return this->addPolicies("a", p_type, rules);
}

// RemovePolicy removes an authorization rule from the current policy.
bool Caeper::RemovePolicy(const std::vector<std::string>& params) {
    return this->RemoveNamedPolicy("a", params);
}

// RemovePolicies removes authorization rules from the current policy.
bool Caeper::RemovePolicies( const std::vector<std::vector<std::string>>& rules) {
    return this->RemoveNamedPolicies("a", rules);
}

// RemoveFilteredPolicy removes an authorization rule from the current policy, field filters can be specified.
bool Caeper::RemoveFilteredPolicy(int field_index, const std::vector<std::string>& field_values) {
    return this->RemoveFilteredNamedPolicy("a", field_index, field_values);
}

// RemoveNamedPolicy removes an authorization rule from the current named policy.
bool Caeper::RemoveNamedPolicy(const std::string& p_type, const std::vector<std::string>& params) {
    if (params.size() == 1) {
        std::vector<std::string> str_slice{params[0]};
        return this->removePolicy("a", p_type, str_slice);
    }

    std::vector<std::string> policy;
    for (int i = 0 ; i < params.size() ; i++)
        policy.push_back(params[i]);
    return this->removePolicy("a", p_type, policy);
}

// RemoveNamedPolicies removes authorization rules from the current named policy.
bool Caeper::RemoveNamedPolicies(const std::string& p_type,  const std::vector<std::vector<std::string>>& rules) {
	return this->removePolicies("a", p_type, rules);
}

// RemoveFilteredNamedPolicy removes an authorization rule from the current named policy, field filters can be specified.
bool Caeper::RemoveFilteredNamedPolicy(const std::string& p_type, int field_index, const std::vector<std::string>& field_values) {
    return this->removeFilteredPolicy("a", p_type, field_index, field_values);
}

// HasRolePolicy determines whether a role inheritance rule exists.
bool Caeper::HasRolePolicy(const std::vector<std::string>& params) {
    return this->HasNamedRolePolicy("r", params);
}

// HasNamedRolePolicy determines whether a named role inheritance rule exists.
bool Caeper::HasNamedRolePolicy(const std::string& p_type, const std::vector<std::string>& params) {
    if (params.size() == 1) {
        std::vector<std::string> str_slice{params[0]};
        return m_model->HasPolicy("r", p_type, str_slice);
    }

    std::vector<std::string> policy;
    for (int i = 0; i < params.size(); i++)
        policy.push_back(params[i]);
    return m_model->HasPolicy("r", p_type, policy);
}

// AddRolePolicy adds a role inheritance rule to the current policy.
// If the rule already exists, the function returns false and the rule will not be added.
// Otherwise the function returns true by adding the new rule.
bool Caeper::AddRolePolicy(const std::vector<std::string>& params) {
    return this->AddNamedRolePolicy("r", params);
}

// AddRolePolicies adds role inheritance rulea to the current policy.
// If the rule already exists, the function returns false for the corresponding policy rule and the rule will not be added.
// Otherwise the function returns true for the corresponding policy rule by adding the new rule.
bool Caeper::AddRolePolicies( const std::vector<std::vector<std::string>>& rules) {
    return this->AddNamedRolePolicies("r", rules);
}

// AddNamedRolePolicy adds a named role inheritance rule to the current policy.
// If the rule already exists, the function returns false and the rule will not be added.
// Otherwise the function returns true by adding the new rule.
bool Caeper::AddNamedRolePolicy(const std::string& p_type, const std::vector<std::string>& params) {
    bool rule_added;
    if (params.size() == 1) {
        std::vector<std::string> str_slice{params[0]};
        rule_added = this->addPolicy("r", p_type, str_slice);
    } else {
        std::vector<std::string> policy;
        for(int i = 0; i < params.size(); i++)
            policy.push_back(params[i]);

        rule_added = this->addPolicy("r", p_type, policy);
    }

    if(m_auto_build_role_links)
        this->BuildRoleLinks();

    return rule_added;
}

// AddNamedRolePolicies adds named role inheritance rules to the current policy.
// If the rule already exists, the function returns false for the corresponding policy rule and the rule will not be added.
// Otherwise the function returns true for the corresponding policy rule by adding the new rule.
bool Caeper::AddNamedRolePolicies(const std::string& p_type,  const std::vector<std::vector<std::string>>& rules) {
    return this->addPolicies("r", p_type, rules);
}

// RemoveRolePolicy removes a role inheritance rule from the current policy.
bool Caeper::RemoveRolePolicy(const std::vector<std::string>& params) {
    return this->RemoveNamedRolePolicy("r", params);
}

// RemoveRolePolicies removes role inheritance rulea from the current policy.
bool Caeper::RemoveRolePolicies( const std::vector<std::vector<std::string>>& rules) {
    return this->RemoveNamedRolePolicies("r", rules);
}

// RemoveFilteredRolePolicy removes a role inheritance rule from the current policy, field filters can be specified.
bool Caeper::RemoveFilteredRolePolicy(int field_index, const std::vector<std::string>& field_values) {
    return this->RemoveFilteredNamedRolePolicy("r", field_index, field_values);
}

// RemoveNamedRolePolicy removes a role inheritance rule from the current named policy.
bool Caeper::RemoveNamedRolePolicy(const std::string& p_type, const std::vector<std::string>& params) {
    bool rule_removed;
    if(params.size() == 1){
        std::vector<std::string> str_slice{params[0]};
        rule_removed = this->removePolicy("r", p_type, str_slice);
    } else {
        std::vector<std::string> policy;
        for(int i = 0 ; i < params.size() ; i++)
            policy.push_back(params[i]);

        rule_removed = this->removePolicy("r", p_type, policy);
    }

    if(m_auto_build_role_links)
        this->BuildRoleLinks();

    return rule_removed;
}

// RemoveNamedRolePolicies removes role inheritance rules from the current named policy.
bool Caeper::RemoveNamedRolePolicies(const std::string& p_type,  const std::vector<std::vector<std::string>>& rules) {
    return this->removePolicies("r", p_type, rules);
}

// RemoveFilteredNamedRolePolicy removes a role inheritance rule from the current named policy, field filters can be specified.
bool Caeper::RemoveFilteredNamedRolePolicy(const std::string& p_type, int field_index, const std::vector<std::string>& field_values) {
    bool rule_removed = this->removeFilteredPolicy("r", p_type, field_index, field_values);

    if(m_auto_build_role_links)
        this->BuildRoleLinks();

    return rule_removed;
}

// AddMatcher adds a customized matcher.
void Caeper::AddMatcher(const std::string& name, MatcherFunc matcher_func) {
    m_matcher->matcher_map[name] = matcher_func;
}


bool Caeper::UpdateRolePolicy(const std::vector<std::string>& oldRule, const std::vector<std::string>& newRule) {
    return UpdateNamedRolePolicy("r", oldRule, newRule);
}

bool Caeper::UpdateNamedRolePolicy(const std::string& p_type, const std::vector<std::string>& oldRule, const std::vector<std::string>& newRule) {
    return this->updatePolicy("r", p_type, oldRule, newRule);
}

// UpdatePolicy updates an authorization rule from the current policy.
bool Caeper::UpdatePolicy(const std::vector<std::string>& oldPolicy, const std::vector<std::string>& newPolicy) {
    return UpdateNamedPolicy("a", oldPolicy, newPolicy);
}

bool Caeper::UpdateNamedPolicy(const std::string& ptype, const std::vector<std::string>& p1, const std::vector<std::string>& p2) {
    return this->updatePolicy("a", ptype, p1, p2);
}

// UpdatePolicies updates authorization rules from the current policies.
bool Caeper::UpdatePolicies(const std::vector<std::vector<std::string>>& oldPolices, const std::vector<std::vector<std::string>>& newPolicies) {
    return UpdateNamedPolicies("a", oldPolices, newPolicies);
}

bool Caeper::UpdateNamedPolicies(const std::string& ptype, const std::vector<std::vector<std::string>>& p1, const std::vector<std::vector<std::string>>& p2) {
    return this->updatePolicies("a", ptype, p1, p2);
}

} // namespace caep 

#endif
