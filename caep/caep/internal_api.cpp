#ifndef CAEP_INTERNAL_API_CPP
#define CAEP_INTERNAL_API_CPP

#include "./caeper.h"
#include "../util/caep_util.h"
#include "../exception/unsupported_operation_exception.h"
#include "../adapter/batch_adapter.h"

namespace caep {

// addPolicy adds a rule to the current policy.
bool Caeper::addPolicy(const std::string& sec, const std::string& p_type, const std::vector<std::string>& rule) {
    bool rule_added = m_model->AddPolicy(sec, p_type, rule);
    if(!rule_added)
        return rule_added;

    if (sec == "r") {
        std::vector<std::vector<std::string>> rules{rule};
        this->BuildIncrementalRoleLinks(policy_add, p_type, rules);
    }

    if (m_adapter && m_auto_save) {
        try {
            m_adapter->AddPolicy(sec, p_type, rule);
        }
        catch (UnsupportedOperationException e) {
        }
    }

    return rule_added;
}

// addPolicies adds rules to the current policy.
bool Caeper::addPolicies(const std::string& sec, const std::string& p_type, const std::vector<std::vector<std::string>>& rules) {
    bool rules_added = m_model->AddPolicies(sec, p_type, rules);
    if (!rules_added)
        return rules_added;

    if (sec == "r")
        this->BuildIncrementalRoleLinks(policy_add, p_type, rules);


    if (m_adapter && m_auto_save) {
        try {
            std::dynamic_pointer_cast<BatchAdapter>(m_adapter)->AddPolicies(sec, p_type, rules);
        }
        catch (UnsupportedOperationException e) {
        }
    }

    return rules_added;
}

// removePolicy removes a rule from the current policy.
bool Caeper::removePolicy(const std::string& sec, const std::string& p_type, const std::vector<std::string>& rule) {
    bool rule_removed = m_model->RemovePolicy(sec, p_type, rule);
    if(!rule_removed)
        return rule_removed;

    if (sec == "r") {
        std::vector<std::vector<std::string>> rules{rule};
        this->BuildIncrementalRoleLinks(policy_add, p_type, rules);
    }
    
    if (m_adapter && m_auto_save) {
        try {
            m_adapter->RemovePolicy(sec, p_type, rule);
        }
        catch (UnsupportedOperationException e) {
        }
    }

    return rule_removed;
}

// removePolicies removes rules from the current policy.
bool Caeper::removePolicies(const std::string& sec, const std::string& p_type, const std::vector<std::vector<std::string>>& rules) {
    bool rules_removed = m_model->AddPolicies(sec, p_type, rules);
    if (!rules_removed)
        return rules_removed;

    if (sec == "r")
        this->BuildIncrementalRoleLinks(policy_add, p_type, rules);

    if (m_adapter && m_auto_save) {
        try{
            std::dynamic_pointer_cast<BatchAdapter>(m_adapter)->RemovePolicies(sec, p_type, rules);
        }
        catch (UnsupportedOperationException e){
        }
    }

    return rules_removed;
}

// removeFilteredPolicy removes rules based on field filters from the current policy.
bool Caeper::removeFilteredPolicy(const std::string& sec, const std::string& p_type, int field_index, const std::vector<std::string>& field_values){
    std::pair<int, std::vector<std::vector<std::string>>> p = m_model->RemoveFilteredPolicy(sec, p_type, field_index, field_values);
    bool rule_removed = p.first;
    std::vector<std::vector<std::string>> effects = p.second;

    if(!rule_removed)
        return rule_removed;

    if (sec == "r")
        this->BuildIncrementalRoleLinks(policy_remove, p_type, effects);

    if (m_adapter && m_auto_save) {
        try {
            m_adapter->RemoveFilteredPolicy(sec, p_type, field_index, field_values); \
        }
        catch (UnsupportedOperationException e) {
        }
    }

    return rule_removed;
}

bool Caeper::updatePolicy(const std::string& sec, const std::string& p_type, const std::vector<std::string>& oldRule, const std::vector<std::string>& newRule) {
    bool is_rule_updated = m_model->UpdatePolicy(sec, p_type, oldRule, newRule);
    if(!is_rule_updated)
        return false;
    
    if(sec == "r") {
        this->BuildIncrementalRoleLinks(policy_remove, p_type, { oldRule });
        this->BuildIncrementalRoleLinks(policy_add, p_type, { newRule });
    }
    return is_rule_updated;
}

bool Caeper::updatePolicies(const std::string& sec, const std::string& p_type, const std::vector<std::vector<std::string>>& oldRules, const std::vector<std::vector<std::string>>& newRules) {
    bool is_rules_updated = m_model->UpdatePolicies(sec, p_type, oldRules, newRules);
    if(!is_rules_updated)
        return false;
    
    if(sec == "r") {
        this->BuildIncrementalRoleLinks(policy_remove, p_type, oldRules);
        this->BuildIncrementalRoleLinks(policy_add, p_type, newRules);
    }

    return is_rules_updated;
}

} // namespace caep

#endif
