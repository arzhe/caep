#ifndef CAEP_CAEPER_H
#define CAEP_CAEPER_H

#include <tuple>
#include <vector>

#include "../rbac/role_manager.h"
#include "../model/matcher.h"
#include "./caeper_interface.h"

namespace caep {

// Caeper is the main interface for authorization enforcement and policy management.
class Caeper {
private:
    std::string m_model_path;
    std::shared_ptr<Model> m_model;
    std::shared_ptr<Matcher> m_matcher;
    std::shared_ptr<Effector> m_eft;
    std::shared_ptr<Adapter> m_adapter;

    bool m_enabled;
    bool m_auto_save;
    bool m_auto_build_role_links;

    // Caep use a custom matcher to decides whether a "subject" can access a "resource"
    // with the operation "action", input parameters are usually (matcher, sub, res, act),
    // use model matcher by default when matcher is "".
    bool m_caeper(const std::string& matcher, const std::vector<std::string>& req);

public:
    std::shared_ptr<RoleManager> rm;

    /**
     * @breif Caeper is the default constructor.
     */
    Caeper();

    /**
     * @breif Caeper initializes a caep with a model file and a policy file.
     *
     * @param model_path the path of the model file.
     * @param policy_path the path of the policy file.
     */
    Caeper(const std::string& model_path, const std::string& policy_path);
    
    /**
     * @breif Caeper initializes a caep with a model file and a database adapter.
     *
     * @param model_path the path of the model file.
     * @param adapater the adapter.
     */
    Caeper(const std::string& model_path, std::shared_ptr<Adapter> adapter);

    /**
     * @breif Caeper initializes a caep with a model and a database adapter.
     *
     * @param m the model.
     * @param adapter the adapter.
     */
    Caeper(std::shared_ptr<Model> m, std::shared_ptr<Adapter> adapter);

    /**
     * @breif Caeper initializes a caep with a model.
     *
     * @param m the model.
     */
    Caeper(std::shared_ptr<Model> m);

    /**
     * @breif Caeper initializes a caep with a model file.
     *
     * @param model_path the path of the model file.
     */
    Caeper(const std::string& model_path);

    /**
     * @breif Ceaper initializes a caep with a model file, a policy file and an enable log flag.
     *
     * @param model_path the path of the model file.
     * @param policy_path the path of the policy file.
     * @param enable_log whether to enable caep's log.
     */
    // Caeper(const std::string& model_path, const std::string& policy_path, bool enable_log);


    // Initialize initializes a caeper with null.
    void Initialize();
    // InitWithFile initializes an caeper with a model file and a policy file.
    void InitWithFile(const std::string& model_path, const std::string& policy_path);
    // InitWithAdapter initializes a caeper with a database adapter.
    void InitWithAdapter(const std::string& model_path, std::shared_ptr<Adapter> adapter);
    // InitWithModelAndAdapter initializes a caeper with a model and a database adapter.
    void InitWithModelAndAdapter(std::shared_ptr<Model> m, std::shared_ptr<Adapter> adapter);
    
    
    // LoadModel reloads the model from the model INI file.
    // Since the policy is attached to a model, so the policy is invalidated and
    // needs to be reloaded by calling LoadPolicy().
    void LoadModel();
    // GetModel gets the current model.
    std::shared_ptr<Model> GetModel();
    // SetModel sets the current model.
    void SetModel(std::shared_ptr<Model> m);
    // GetAdapter gets the current adapter.
    std::shared_ptr<Adapter> GetAdapter();
    // SetAdapter sets the current adapter.
    void SetAdapter(std::shared_ptr<Adapter> adapter);
    // GetRoleManager gets the current role manager.
    std::shared_ptr<RoleManager> GetRoleManager();
    // SetRoleManager sets the current role manager.
    void SetRoleManager(std::shared_ptr<RoleManager> rm);
    // SetEffector sets the current effector.
    void SetEffector(std::shared_ptr<Effector> eft);
    // LoadPolicy reloads the policy from file or database.
    void LoadPolicy();
    // ClearPolicy clears all policies.
    void ClearPolicy();

    // LoadFilteredPolicy reloads a filtered policy from file or database.
    template <typename Filter>
    void LoadFilteredPolicy(Filter filter);
    // IsFiltered returns true if the loaded policy has been filtered.
    bool IsFiltered();
    // SavePolicy saves the current policy (usually after changed with caep API) back to file or database.
    void SavePolicy();
    // EnableCeaper changes the enforcing state of Caep, when Caep is disabled, all access will be allowed by the Ceap() function.
    void EnableCeaper(bool enable);
    // EnableAutoSave controls whether to save a policy rule automatically to the adapter when it is added or deleted.
    void EnableAutoSave(bool auto_save);
    // EnableAutoBuildRoleLinks controls whether to rebuild the role inheritance relations when a role is added or deleted.
    void EnableAutoBuildRoleLinks(bool auto_build_role_links);
    // BuildRoleLinks manually rebuild the role inheritance relations.
    void BuildRoleLinks();
    // BuildIncrementalRoleLinks provdides incremental build the role inheritance relations.
    void BuildIncrementalRoleLinks(policy_op op, const std::string& p_type, const std::vector<std::vector<std::string>>& rules);
    // Caep with a vector param, whether a "subject" can access a "resource" with the operation "action", input parameters are usually: (sub, res, act).
    bool Caep(const std::vector<std::string>& params);
    // BatchCaeper enforce in batchs.
    std::vector<bool> BatchCaeper(const std::vector<std::vector<std::string>>& reqs);

    /**
     * @breif Caep RBAC API.
     */
    std::vector<std::string> GetRolesForUser(const std::string& name, const std::vector<std::string>& domain = {});
    std::vector<std::string> GetUsersForRole(const std::string& name, const std::vector<std::string>& domain = {});
    bool HasRoleForUser(const std::string& name, const std::string& role);
    bool AddRoleForUser(const std::string& user, const std::string& role);
    bool AddRolesForUser(const std::string& user, const std::vector<std::string>& roles);
    bool AddPermissionForUser(const std::string& user, const std::vector<std::string>& permission);
    bool DeletePermissionForUser(const std::string& user, const std::vector<std::string>& permission);
    bool DeletePermissionsForUser(const std::string& user);
    std::vector<std::vector<std::string>> GetPermissionsForUser(const std::string& user);
    bool HasPermissionForUser(const std::string& user, const std::vector<std::string>& permission);
    std::vector<std::string> GetImplicitRolesForUser(const std::string& name, const std::vector<std::string>& domain = {});
    std::vector<std::vector<std::string>> GetImplicitPermissionsForUser(const std::string& user, const std::vector<std::string>& domain = {});
    std::vector<std::string> GetImplicitUsersForPermission(const std::vector<std::string>& permission);
    bool DeleteRoleForUser(const std::string& user, const std::string& role);
    bool DeleteRolesForUser(const std::string& user);
    bool DeleteUser(const std::string& user);
    bool DeleteRole(const std::string& role);
    bool DeletePermission(const std::vector<std::string>& permission);

    /**
     * @breif Caep Management API.
     */
    std::vector<std::string> GetAllSubjects();
    std::vector<std::string> GetAllNamedSubjects(const std::string& p_type);
    std::vector<std::string> GetAllResources();
    std::vector<std::string> GetAllNamedResources(const std::string& p_type);
    std::vector<std::string> GetAllActions();
    std::vector<std::string> GetAllNamedActions(const std::string& p_type);
    std::vector<std::string> GetAllRoles();
    std::vector<std::string> GetAllNamedRoles(const std::string& p_type);
    std::vector<std::vector<std::string>> GetPolicy();
    std::vector<std::vector<std::string>> GetFilteredPolicy(int field_index, const std::vector<std::string>& field_values);
    std::vector<std::vector<std::string>> GetNamedPolicy(const std::string& p_type);
    std::vector<std::vector<std::string>> GetFilteredNamedPolicy(const std::string& p_type, int field_index, const std::vector<std::string>& field_values);    
    std::vector<std::vector<std::string>> GetRolePolicy();
    std::vector<std::vector<std::string>> GetFilteredRolePolicy(int field_index, const std::vector<std::string>& field_values);
    std::vector<std::vector<std::string>> GetNamedRolePolicy(const std::string& p_type);
    std::vector<std::vector<std::string>> GetFilteredNamedRolePolicy(const std::string& p_type, int field_index, const std::vector<std::string>& field_values);
    bool HasPolicy(const std::vector<std::string>& params);
    bool HasNamedPolicy(const std::string& p_type, const std::vector<std::string>& params);
    bool AddPolicy(const std::vector<std::string>& params);
    bool AddPolicies(const std::vector<std::vector<std::string>>& rules);
    bool AddNamedPolicy(const std::string& p_type, const std::vector<std::string>& params);
    bool AddNamedPolicies(const std::string& p_type, const std::vector<std::vector<std::string>>& rules);
    bool RemovePolicy(const std::vector<std::string>& params);
    bool RemovePolicies(const std::vector<std::vector<std::string>>& rules);
    bool RemoveFilteredPolicy(int field_index, const std::vector<std::string>& field_values);
    bool RemoveNamedPolicy(const std::string& p_type, const std::vector<std::string>& params);
    bool RemoveNamedPolicies(const std::string& p_type, const std::vector<std::vector<std::string>>& rules);
    bool RemoveFilteredNamedPolicy(const std::string& p_type, int field_index, const std::vector<std::string>& field_values);
    bool HasRolePolicy(const std::vector<std::string>& params);
    bool HasNamedRolePolicy(const std::string& p_type, const std::vector<std::string>& params);
    bool AddRolePolicy(const std::vector<std::string>& params);
    bool AddRolePolicies(const std::vector<std::vector<std::string>>& rules);
    bool AddNamedRolePolicy(const std::string& p_type, const std::vector<std::string>& params);
    bool AddNamedRolePolicies(const std::string& p_type, const std::vector<std::vector<std::string>>& rules);
    bool RemoveRolePolicy(const std::vector<std::string>& params);
    bool RemoveRolePolicies(const std::vector<std::vector<std::string>>& rules);
    bool RemoveFilteredRolePolicy(int field_index, const std::vector<std::string>& field_values);
    bool RemoveNamedRolePolicy(const std::string& p_type, const std::vector<std::string>& params);
    bool RemoveNamedRolePolicies(const std::string& p_type, const std::vector<std::vector<std::string>>& rules);
    bool RemoveFilteredNamedRolePolicy(const std::string& p_type, int field_index, const std::vector<std::string>& field_values);
    void AddMatcher(const std::string& name, MatcherFunc matcher_func);
    bool UpdateRolePolicy(const std::vector<std::string>& oldRule, const std::vector<std::string>& newRule);
    bool UpdateNamedRolePolicy(const std::string& ptype, const std::vector<std::string>& oldRule, const std::vector<std::string>& newRule);
    bool UpdatePolicy(const std::vector<std::string>& oldPolicy, const std::vector<std::string>& newPolicy);
    bool UpdateNamedPolicy(const std::string& ptype, const std::vector<std::string>& p1, const std::vector<std::string>& p2);
    bool UpdatePolicies(const std::vector<std::vector<std::string>>& oldPolices, const std::vector<std::vector<std::string>>& newPolicies);
    bool UpdateNamedPolicies(const std::string& ptype, const std::vector<std::vector<std::string>>& p1, const std::vector<std::vector<std::string>>& p2);

    /**
     * @breif Caep internal API member functions
     */ 
    bool addPolicy(const std::string& sec, const std::string& p_type, const std::vector<std::string>& rule);
    bool addPolicies(const std::string& sec, const std::string& p_type, const std::vector<std::vector<std::string>>& rules);
    bool removePolicy(const std::string& sec , const std::string& p_type , const std::vector<std::string>& rule);
    bool removePolicies(const std::string& sec, const std::string& p_type, const std::vector<std::vector<std::string>>& rules);
    bool removeFilteredPolicy(const std::string& sec, const std::string& p_type, int field_index, const std::vector<std::string>& field_values);
    bool updatePolicy(const std::string& sec, const std::string& p_type, const std::vector<std::string>& oldRule, const std::vector<std::string>& newRule);
    bool updatePolicies(const std::string& sec, const std::string& p_type, const std::vector<std::vector<std::string>>& p1, const std::vector<std::vector<std::string>>& p2);

    /**
     * @ breif Caep RBAC API with domains.
     */
    std::vector<std::string> GetUsersForRoleInDomain(const std::string& name, const std::string& domain);
    std::vector<std::string> GetRolesForUserInDomain(const std::string& name, const std::string& domain);
    std::vector<std::vector<std::string>> GetPermissionsForUserInDomain(const std::string& user, const std::string& domain);
    bool AddRoleForUserInDomain(const std::string& user, const std::string& role, const std::string& domain);
    bool DeleteRoleForUserInDomain(const std::string& user, const std::string& role, const std::string& domain);
};

} // namespace caep 

#endif
