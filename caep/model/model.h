/* $Header:  ~/code/GitRepositories/MyGit/caep   2.1.0   22 Aug 2019 19:00:00   ArZr        $ */
/***********************************************************************************************
 ***                  C O N F I D E N T I A L  ---  A R Z R  S T U D I O S                   ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : Caep                                                         *
 *                                                                                             *
 *                    File Name : model.cpp                                                    *
 *                                                                                             *
 *                   Programmer : Guan Zhe                                                     *
 *                                                                                             *
 *                   Start Date : Aug 22, 2019                                                 *
 *                                                                                             *
 *                  Last Update : Oct 20, 2021   [ArZr]                                        *
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 *   Model::LoadSection -- Loads all types of a CONF section from Config into Model.           *
 *   Model::LoadNamedSection -- Loads a type of a CONF section from Config into Model.         *
 *   Model::GetKeySuffix -- Transforms int-Type into string-Type.                              *
 *                                                                                             *
 *   Model::Model -- Constructor for Model object.                                             *
 *   Model::Model -- Constructor for Model object, and initialize Model::path.                 *
 *   Model::HasSection -- Determines whether there is specified section in current Model.      *
 *   Model::AddDef -- Adds a CONF section to current Model.                                    *
 *   Model::LoadModelFromFile -- Loads PEM configurations from a CONF file into current Model. *
 *   Model::LoadModelFromText -- Loads PEM configurations from a CONF text into current Model. *
 *   Model::LoadModelFromConfig -- Loads PEM configurations from Config into current Model.    *
 *                                                                                             *
 *   Model::BuildIncrementalRoleLinks -- Adds or deletes inheritance links for all roles.      *
 *   Model::BuildRoleLinks -- Adds inheritance links for all roles.                            *
 *                                                                                             *
 *   Model::ClearPolicy -- Clears all policies in current Model.                               *
 *   Model::GetPolicy -- Returns all policies that one type of a PRM policy rule owns.         *
 *   Model::GetFilteredPolicy -- Returns filtered policy rules.                                *
 *   Model::HasPolicy -- Determines whether there is a policy rule in current Model.           *
 *   Model::AddPolicy -- Adds a policy rule to current Model.                                  *
 *   Model::AddPolicies -- Adds a batch of policy rules to current Model.                      *
 *   Model::UpdatePolicy -- Updates a policy rule in current Model.                            *
 *   Model::UpdatePolicies -- Updates a batch of policy rules in current Model.                *
 *   Model::RemovePolicy -- Removes a policy rule from current Model.                          *
 *   Model::RemovePolicies -- Removes a batch of policy rules from current Model.              *
 *   Model::RemoveFilteredPolicy -- Removes a filtered policy rule from current Model.         *
 *   Model::GetValuesForFieldInPolicy -- Returns one type of distinct field PRM policy values. *
 *   Model::GetAllValuesForFieldInPolicy -- Returns one type of field PRM policy values.       *
 *   Model::GetValuesForFieldInPolicyAllTypes -- Returns distinct field PRM policy values.     *
 *   Model::GetAllValuesForFieldInPolicyAllTypes -- Returns field PRM policy values.           *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
#ifndef CAEP_MODEL_H
#define CAEP_MODEL_H

#include <unordered_map>

#include "../config/config_interface.h"
#include "./section.h"

namespace caep {

/*---------------------------------------------------------------------------
 * @brief SectionMap class describes all types of CONF sections messages.
 * For example: r and r1, they are two different types of a CONF section 'r'.
 */

class SectionMap {
public:
    std::unordered_map<std::string, std::shared_ptr<Section>> section_map;
};


/*---------------------------------------------------------------------------
 * @brief Model represents all of access control model messages, including
 * CONF sections, PRM policy rules and Role Tree if RBAC is enabled.
 */
class Model {
private:
    static std::unordered_map<std::string, std::string> section_name_map;

    static void LoadSection(Model* model, std::shared_ptr<ConfigInterface> cfg, const std::string& sec);

    static std::string GetKeySuffix(int i);

    static bool LoadNamedSection(Model* model, std::shared_ptr<ConfigInterface> cfg, const std::string& sec, const std::string& key);

public:


    /*---------------------------------------------------------------------------
     * @brief Constructor.
     */
    Model();
    Model(const std::string& path);


    /*---------------------------------------------------------------------------
     * @brief Stores all access control model messages.
     */
    std::unordered_map<std::string, SectionMap> m;

    /*
     * @brief Minimum required sections for a model to be valid.
     */
    static std::vector<std::string> required_sections;

    bool HasSection(const std::string& sec);

    bool AddDef(const std::string& sec, const std::string& key, const std::string& value);

    void LoadModelFromFile(const std::string& path);

    void LoadModelFromText(const std::string& text);

    void LoadModelFromConfig(std::shared_ptr<ConfigInterface> cfg);


    static Model* NewModel();

    static Model* NewModelFromFile(const std::string& path);

    static Model* NewModelFromText(const std::string& text);


    void BuildIncrementalRoleLinks(std::shared_ptr<RoleManager> rm, policy_op op, const std::string& sec, const std::string& p_type, const std::vector<std::vector<std::string>>& rules);

    void BuildRoleLinks(std::shared_ptr<RoleManager> rm);

    void ClearPolicy();

    std::vector<std::vector<std::string>> GetPolicy(const std::string& sec, const std::string& p_type);

    std::vector<std::vector<std::string>> GetFilteredPolicy(const std::string& sec, const std::string& p_type, int field_index, const std::vector<std::string>& field_values);

    bool HasPolicy(const std::string& sec, const std::string& p_type, const std::vector<std::string>& rule);

    bool AddPolicy(const std::string& sec, const std::string& p_type, const std::vector<std::string>& rule);

    bool AddPolicies(const std::string& sec, const std::string& p_type, const std::vector<std::vector<std::string>>& rules);

    bool UpdatePolicy(const std::string& sec, const std::string& p_type, const std::vector<std::string>& oldRule, const std::vector<std::string>& newRule);

    bool UpdatePolicies(const std::string& sec, const std::string& p_type, const std::vector<std::vector<std::string>>& oldRules, const std::vector<std::vector<std::string>>& newRules);

    bool RemovePolicy(const std::string& sec, const std::string& p_type, const std::vector<std::string>& rule);

    bool RemovePolicies(const std::string& sec, const std::string& p_type, const std::vector<std::vector<std::string>>& rules);

    std::pair<bool, std::vector<std::vector<std::string>>> RemoveFilteredPolicy(const std::string& sec, const std::string& p_type, int field_index, const std::vector<std::string>& field_values);
    
    std::vector<std::string> GetValuesForFieldInPolicy(const std::string& sec, const std::string& p_type, int field_index);
 
    std::vector<std::string> GetAllValuesForFieldInPolicy(const std::string& sec, const std::string& p_type, int field_index);

    std::vector<std::string> GetValuesForFieldInPolicyAllTypes(const std::string& sec, int field_index);

    std::vector<std::string> GetAllValuesForFieldInPolicyAllTypes(const std::string& sec, int field_index);
};

} // namespace caep 

#endif
