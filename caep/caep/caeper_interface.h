#ifndef CAEP_CAEPER_INTERFACE_H
#define CAEP_CAEPER_INTERFACE_H

#include "../model/model.h"
#include "../model/matcher.h"
#include "../adapter/adapter.h"
#include "../effect/effector.h"

namespace caep {

// ICaeper is the API interface of Caep.
class ICaeper {
public:
    /**
     * @breif Caep API.
     */
    virtual void InitWithFile(const std::string& model_path, const std::string& policy_path) = 0;
    virtual void InitWithAdapter(const std::string& model_path, std::shared_ptr<Adapter> adapter) = 0;
    virtual void InitWithModelAndAdapter(std::shared_ptr<Model> m, std::shared_ptr<Adapter> adapter) = 0;
    virtual void Initialize() = 0;
    virtual void LoadModel() = 0;
    virtual std::shared_ptr<Model> GetModel() = 0;
    virtual void SetModel(std::shared_ptr<Model> m) = 0;
    virtual std::shared_ptr<Adapter> GetAdapter() = 0;
    virtual void SetAdapter(std::shared_ptr<Adapter> adapter) = 0;
    virtual std::shared_ptr<RoleManager> GetRoleManager() = 0;
    virtual void SetRoleManager(std::shared_ptr<RoleManager> rm) = 0;
    virtual void SetEffector(std::shared_ptr<Effector> eft) = 0;
    virtual void LoadPolicy() = 0;
    virtual void ClearPolicy() = 0;

    template <typename Filter>
    void LoadFilteredPolicy(Filter filter);
    
    virtual bool IsFiltered() = 0;
    virtual void SavePolicy() = 0;
    virtual void EnableCaeper(bool enable) = 0;
    virtual void EnableAutoSave(bool auto_save) = 0;
    virtual void EnableAutoBuildRoleLinks(bool auto_build_role_links) = 0;
    virtual void BuildRoleLinks() = 0;
    virtual bool m_caeper(const std::string& matcher, const std::vector<std::string>& req) = 0;
    virtual bool Caep(const std::vector<std::string>& req) = 0;
    virtual std::vector<bool> BatchCaeper(const std::vector<std::vector<std::string>>& reqs) = 0;

    /**
     * @breif Caep RBAC API.
     */
    virtual std::vector<std::string> GetRolesForUser(const std::string& name, const std::vector<std::string>& domain = {}) = 0;
    virtual std::vector<std::string> GetUsersForRole(const std::string& name, const std::vector<std::string>& domain = {}) = 0;
    virtual bool HasRoleForUser(const std::string& name, const std::string& role) = 0;
    virtual bool AddRoleForUser(const std::string& user, const std::string& role) = 0;
    virtual bool AddRolesForUser(const std::string& user, const std::vector<std::string>& roles) = 0;
    virtual bool AddPermissionForUser(const std::string& user, const std::vector<std::string>& permission) = 0;
    virtual bool DeletePermissionForUser(const std::string& user, const std::vector<std::string>& permission) = 0;
    virtual bool DeletePermissionsForUser(const std::string& user) = 0;
    virtual std::vector<std::vector<std::string>> GetPermissionsForUser(const std::string& user) = 0;
    virtual bool HasPermissionForUser(const std::string& user, const std::vector<std::string>& permission) = 0;
    virtual std::vector<std::string> GetImplicitRolesForUser(const std::string& name, const std::vector<std::string>& domain = {}) = 0;
    virtual std::vector<std::vector<std::string>> GetImplicitPermissionsForUser(const std::string& user, const std::vector<std::string>& domain = {}) = 0;
    virtual std::vector<std::string> GetImplicitUsersForPermission(const std::vector<std::string>& permission) = 0;
    virtual bool DeleteRoleForUser(const std::string& user, const std::string& role) = 0;
    virtual bool DeleteRolesForUser(const std::string& user) = 0;
    virtual bool DeleteUser(const std::string& user) = 0;
    virtual bool DeleteRole(const std::string& role) = 0;
    virtual bool DeletePermission(const std::vector<std::string>& permission) = 0;

    /**
     * @breif Caep Management API.
     */
    virtual std::vector<std::string> GetAllSubjects() = 0;
    virtual std::vector<std::string> GetAllNamedSubjects(const std::string& p_type) = 0;
    virtual std::vector<std::string> GetAllObjects() = 0;
    virtual std::vector<std::string> GetAllNamedObjects(const std::string& p_type) = 0;
    virtual std::vector<std::string> GetAllActions() = 0;
    virtual std::vector<std::string> GetAllNamedActions(const std::string& p_type) = 0;
    virtual std::vector<std::string> GetAllRoles() = 0;
    virtual std::vector<std::string> GetAllNamedRoles(const std::string& p_type) = 0;
    virtual std::vector<std::vector<std::string>> GetPolicy() = 0;
    virtual std::vector<std::vector<std::string>> GetFilteredPolicy(int field_index, const std::vector<std::string>& field_values) = 0;
    virtual std::vector<std::vector<std::string>> GetNamedPolicy(const std::string& p_type) = 0;
    virtual std::vector<std::vector<std::string>> GetFilteredNamedPolicy(const std::string& p_type, int field_index, const std::vector<std::string>& field_values) = 0;
    virtual std::vector<std::vector<std::string>> GetRolePolicy() = 0;
    virtual std::vector<std::vector<std::string>> GetNamedRolePolicy(const std::string& p_type) = 0;
    virtual std::vector<std::vector<std::string>> GetFilteredNamedRolePolicy(const std::string& p_type, int field_index, const std::vector<std::string>& field_values) = 0;
    virtual bool HasPolicy(const std::vector<std::string>& params) = 0;
    virtual bool HasNamedPolicy(const std::string& p_type, const std::vector<std::string>& params) = 0;
    virtual bool AddPolicy(const std::vector<std::string>& params) = 0;
    virtual bool AddPolicies(const std::vector<std::vector<std::string>>& rules) = 0;
    virtual bool AddNamedPolicy(const std::string& p_type, const std::vector<std::string>& params) = 0;
    virtual bool AddNamedPolicies(const std::string& p_type, const std::vector<std::vector<std::string>>& rules) = 0;
    virtual bool RemovePolicy(const std::vector<std::string>& params) = 0;
    virtual bool RemovePolicies(const std::vector<std::vector<std::string>>& rules) = 0;
    virtual bool RemoveFilteredPolicy(int field_index, const std::vector<std::string>& field_values) = 0;
    virtual bool RemoveNamedPolicy(const std::string& p_type, const std::vector<std::string>& params) = 0;
    virtual bool RemoveNamedPolicies(const std::string& p_type, const std::vector<std::vector<std::string>>& rules) = 0;
    virtual bool RemoveFilteredNamedPolicy(const std::string& p_type, int field_index, const std::vector<std::string>& field_values) = 0;
    virtual bool HasRolePolicy(const std::vector<std::string>& params) = 0;
    virtual bool HasNamedRolePolicy(const std::string& p_type, const std::vector<std::string>& params) = 0;
    virtual bool AddRolePolicy(const std::vector<std::string>& params) = 0;
    virtual bool AddRolePolicies(const std::vector<std::vector<std::string>>& rules) = 0;
    virtual bool AddNamedRolePolicy(const std::string& p_type, const std::vector<std::string>& params) = 0;
    virtual bool AddNamedRolePolicies(const std::string& p_type, const std::vector<std::vector<std::string>>& rules) = 0;
    virtual bool RemoveRolePolicy(const std::vector<std::string>& params) = 0;
    virtual bool RemoveRolePolicies(const std::vector<std::vector<std::string>>& rules) = 0;
    virtual bool RemoveFilteredRolePolicy(int field_index, const std::vector<std::string>& field_values) = 0;
    virtual bool RemoveNamedRolePolicy(const std::string& p_type, const std::vector<std::string>& params) = 0;
    virtual bool RemoveNamedRolePolicies(const std::string& p_type, const std::vector<std::vector<std::string>>& rules) = 0;
    virtual bool RemoveFilteredNamedRolePolicy(const std::string& p_type, int field_index, const std::vector<std::string>& field_values) = 0;
    virtual void AddMatcher(const std::string& name, MatcherFunc matcher_func) = 0;
    virtual bool UpdateRolePolicy(const std::vector<std::string>& oldRule, const std::vector<std::string>& newRule) = 0;
    virtual bool UpdateNamedRolePolicy(const std::string& ptype, const std::vector<std::string>& oldRule, const std::vector<std::string>& newRule) = 0;
    virtual bool UpdatePolicy(const std::vector<std::string>& oldPolicy, const std::vector<std::string>& newPolicy) = 0;
    virtual bool UpdateNamedPolicy(const std::string& ptype, const std::vector<std::string>& p1, const std::vector<std::string>& p2) = 0;
    virtual bool UpdatePolicies(const std::vector<std::vector<std::string>>& oldPolices, const std::vector<std::vector<std::string>>& newPolicies) = 0;
    virtual bool UpdateNamedPolicies(const std::string& ptype, const std::vector<std::vector<std::string>>& p1, const std::vector<std::vector<std::string>>& p2) = 0;

    /**
     * @breif Caep internal API member functions
     */ 
    virtual bool addPolicy(const std::string& sec, const std::string& p_type, const std::vector<std::string>& rule) = 0;
    virtual bool addPolicies(const std::string& sec, const std::string& p_type, const std::vector<std::vector<std::string>>& rules) = 0;
    virtual bool removePolicy(const std::string& sec , const std::string& p_type , const std::vector<std::string>& rule) = 0;
    virtual bool removePolicies(const std::string& sec, const std::string& p_type, const std::vector<std::vector<std::string>>& rules) = 0;
    virtual bool removeFilteredPolicy(const std::string& sec, const std::string& p_type, int field_index, const std::vector<std::string>& field_values) = 0;
    virtual bool updatePolicy(const std::string& sec, const std::string& p_type, const std::vector<std::string>& oldRule, const std::vector<std::string>& newRule) = 0;
    virtual bool updatePolicies(const std::string& sec, const std::string& p_type, const std::vector<std::vector<std::string>>& p1, const std::vector<std::vector<std::string>>& p2) = 0;

    /**
     * @ breif Caep RBAC API with domains.
     */
    virtual std::vector<std::string> GetUsersForRoleInDomain(const std::string& name, const std::string& domain) = 0;
    virtual std::vector<std::string> GetRolesForUserInDomain(const std::string& name, const std::string& domain) = 0;
    virtual std::vector<std::vector<std::string>> GetPermissionsForUserInDomain(const std::string& user, const std::string& domain) = 0;
    virtual bool AddRoleForUserInDomain(const std::string& user, const std::string& role, const std::string& domain) = 0;
    virtual bool DeleteRoleForUserInDomain(const std::string& user, const std::string& role, const std::string& domain) = 0;    
};

} // namespace caep 

#endif 
