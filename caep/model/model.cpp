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
#ifndef CAEP_MODEL_CPP
#define CAEP_MODEL_CPP

#include <sstream>

#include "./model.h"
#include "../config/config.h"
#include "../exception/missing_required_sections.h"
#include "../util/caep_util.h"

namespace caep {

std::unordered_map<std::string, std::string> Model::section_name_map = {
    {"a", "applicability"},
    {"r", "role"},
    {"m", "matcher"},
    {"c", "condition"},
    {"e", "effector"}
};

std::vector<std::string> Model::required_sections{"a", "m", "c", "e"};

/***********************************************************************************************
 ***                              Model::LoadModelFromConfig                                 ***
 ***********************************************************************************************
 * DESCRIPTION: Loads only CONF sections from Config into Model. It won't load PRM policy rules*
 *              into Model, but Model offers other way to load PRM policy rules.               *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   cfg -- Shared_ptr<ConfigInterface>-Type object, any Class that implements          *
 *                 ConfigInterface abstract class, can be loaded.                              *
 *                                                                                             *
 * OUTPUT:   NONE                                                                              *
 *                                                                                             *
 * WARNINGS:    Cfg should be the class that implements ConfigInterface abstract class. Caep   *
 *              offers built-in Config class for you.                                          *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
void Model::LoadModelFromConfig(std::shared_ptr<ConfigInterface> cfg) {
    for(auto& it : section_name_map)
        LoadSection(this, cfg, it.first);

    std::vector<std::string> ms;
    for(const auto& req_sec : required_sections) {
        if(!this->HasSection(req_sec))
            ms.push_back(req_sec);
    }
    
    if(ms.size() > 0)
        throw MissingRequiredSections("missing required sections: " + CaepUtil::Join(ms, ","));
}

/***********************************************************************************************
 ***                                 Model::HasSection                                       ***
 ***********************************************************************************************
 * DESCRIPTION: Determines whether there is a specified CONF section in Model.                 *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   sec -- The CONF section name to be searched for.                                   *
 *                                                                                             *
 * OUTPUT:   Returns true if found, else returns false.                                        *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
bool Model::HasSection(const std::string& sec) {
    return this->m.find(sec) != this->m.end();
}

/***********************************************************************************************
 ***                                 Model::LoadSection                                      ***
 ***********************************************************************************************
 * DESCRIPTION: Loads a CONF section from Config into Model.                                   *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   model -- Pointer to a Model object. CONF messages will be loaded into it.          *
 *                                                                                             *
 *          cfg -- Shared_ptr<ConfigInterface>-Type object, any Class that implements          *
 *                 ConfigInterface abstract class, can be loaded.                              *
 *                                                                                             *
 * OUTPUT:   NONE                                                                              *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
void Model::LoadSection(Model* model, std::shared_ptr<ConfigInterface> cfg, const std::string& sec) {
    int i = 1;
    while(true) {
        if(!LoadNamedSection(model, cfg, sec, sec + GetKeySuffix(i)))
            break;
        else
            ++i;
    }
}

/***********************************************************************************************
 ***                                 Model::GetKeySuffix                                     ***
 ***********************************************************************************************
 * DESCRIPTION: Transforms int-Type into string-Type.                                          *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   i -- The int-Type value, which would be transformed into string-Type.              *
 *                                                                                             *
 * OUTPUT:   Returns a string-Type value, which is transformed from a int-Type value.          *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
std::string Model::GetKeySuffix(int i) {
    if(i == 1)
        return "";
    std::stringstream ss;
    ss << i;
    std::string s;
    ss >> s;
    return s;
}

/***********************************************************************************************
 ***                                 Model::LoadNamedSection                                 ***
 ***********************************************************************************************
 * DESCRIPTION: Loads a type of a CONF section from Config into Model. For example,            *
 *              in CONF section 'role', there are two types, 'r' and 'r2'. You can             *
 *              specify one type of them to load CONF messages into Model.                     *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   model -- Pointer to a Model object. CONF messages will be loaded into it.          *
 *                                                                                             *
 *          cfg -- Shared_ptr<ConfigInterface>-Type object, any Class that implements          *
 *                 ConfigInterface abstract class, can be loaded.                              *
 *                                                                                             *
 *          sec -- The CONF section to be searched for.                                        *
 *                                                                                             *
 *          key -- The type of a CONF section to be searched for.                              *
 *                                                                                             *
 * OUTPUT:   Returns true if {sec, key}->value is found, else returns false.                   *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
bool Model::LoadNamedSection(Model* model, std::shared_ptr<ConfigInterface> cfg, const std::string& sec, const std::string& key) {
    std::string value = cfg->GetString(section_name_map[sec] + "::" + key);
    return model->AddDef(sec, key, value);
}

/***********************************************************************************************
 ***                                 Model::LoadNamedSection                                 ***
 ***********************************************************************************************
 * DESCRIPTION: Loads a type of a CONF section from Config into Model. For example,            *
 *              in CONF section 'role', there are two types, 'r' and 'r2'. You can             *
 *              specify one type of them to load CONF messages into Model.                     *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   model -- Pointer to a Model object. CONF messages will be loaded into it.          *
 *                                                                                             *
 *          cfg -- Shared_ptr<ConfigInterface>-Type object, any Class that implements          *
 *                 ConfigInterface abstract class, can be loaded.                              *
 *                                                                                             *
 *          sec -- The CONF section to be searched for.                                        *
 *                                                                                             *
 *          key -- The type of a CONF section to be searched for.                              *
 *                                                                                             *
 * OUTPUT:   Returns true if {sec, key}->value is found, else returns false.                   *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
bool Model::AddDef(const std::string& sec, const std::string& key, const std::string& value) {
    if(!value.compare(""))
        return false;

    std::shared_ptr<Section> ast = std::make_shared<Section>();
    ast->key = key;
    ast->value = value;
    if(!sec.compare("a")) {
        ast->tokens = CaepUtil::Split(ast->value, ",");
        for(auto& t : ast->tokens)
            t = key + "." + CaepUtil::Trim(t);
    }
    
    if(m.find(sec) == m.end())
        m[sec] = SectionMap();
    ast->policy = {};

    m[sec].section_map[key] = ast;

    return true;
}

/***********************************************************************************************
 ***                                 Model::LoadModelFromFile                                ***
 ***********************************************************************************************
 * DESCRIPTION: Loads PEM configurations from a CONF file into current Model. It does not load *
 *              PRM policy rules but only PEM configurations. Therefore, if you wanna complete *
 *              the whole Model, this function is usually the only way.                        *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   path -- CONF file path, it can be either a relative path or an absolute path.      *
 *                                                                                             *
 * OUTPUT:   NONE                                                                              *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
void Model::LoadModelFromFile(const std::string& path) {
    auto cfg = Config::NewConfigFromFile(path);
    LoadModelFromConfig(cfg);
}

/***********************************************************************************************
 ***                                 Model::LoadModelFromText                                ***
 ***********************************************************************************************
 * DESCRIPTION: Loads PEM configurations from the CONF text into current Model. It doesn't load*
 *              PRM policy rules but only PEM configurations. Therefore, if you wanna complete *
 *              the whole Model, this function is usually the only way.                        *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   text -- Describes PEM configurations.                                              *
 *                                                                                             *
 * OUTPUT:   NONE                                                                              *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
void Model::LoadModelFromText(const std::string& text) {
    auto cfg = Config::NewConfigFromText(text);
    LoadModelFromConfig(cfg);
}

/***********************************************************************************************
 ***                                    Model::Model                                         ***
 ***********************************************************************************************
 * DESCRIPTION: Default constructor for Model object.                                          *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   NONE                                                                               *
 *                                                                                             *
 * OUTPUT:   NONE                                                                              *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
Model::Model() {}

/***********************************************************************************************
 ***                                    Model::Model                                         ***
 ***********************************************************************************************
 * DESCRIPTION: Constructor for Model object. It Allocates a Model object by a CONF file.      *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   path -- CONF file path, it can be either a relative path or an absolute path.      *                                                                               *
 *                                                                                             *
 * OUTPUT:   NONE                                                                              *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
Model::Model(const std::string& path) {
    LoadModelFromFile(path);
}

/***********************************************************************************************
 ***                                    Model::NewModel                                      ***
 ***********************************************************************************************
 * DESCRIPTION: Gets a pointer to Model by calling default constructor.                        *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   NONE                                                                               *
 *                                                                                             *
 * OUTPUT:   Pointer to Model.                                                                 *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
Model* Model::NewModel() {
    return new Model();
}

/***********************************************************************************************
 ***                                    Model::NewModelFromFile                              ***
 ***********************************************************************************************
 * DESCRIPTION: Gets a pointer to Model and loads PEM configurations from a CONF file into it. *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   path -- CONF file path, it can be either a relative path or an absolute path.      *
 *                                                                                             *
 * OUTPUT:   Pointer to Model.                                                                 *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
Model* Model::NewModelFromFile(const std::string& path) {
    auto m = NewModel();
    m->LoadModelFromFile(path);
    return m;
}

/***********************************************************************************************
 ***                                    Model::NewModelFromText                              ***
 ***********************************************************************************************
 * DESCRIPTION: Gets a pointer to Model and loads PEM configurations from a CONF text into it. *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   text -- The CONF text.                                                             *
 *                                                                                             *
 * OUTPUT:   Pointer to Model.                                                                 *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
Model* Model::NewModelFromText(const std::string& text) {
    auto m = NewModel();
    m->LoadModelFromText(text);
    return m;
}

/***********************************************************************************************
 ***                            Model::BuildIncrementalRoleLinks                             ***
 ***********************************************************************************************
 * DESCRIPTION: Adds or deletes inheritance links for all Roles described in specified PRM     *
 *              policy rules.                                                                  *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   rm -- The outter RoleManager, all operations on Roles are performed on it. Messages*
 *          about Role Tree will be stored in rm. Once all messages have been processed, it    *
 *          would be delivered to current Model.                                               *
 *                                                                                             *
 * OUTPUT:   NONE                                                                              *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
void Model::BuildIncrementalRoleLinks(std::shared_ptr<RoleManager> rm, policy_op op, const std::string& sec, const std::string& p_type, const std::vector<std::vector<std::string>>& rules) {
    if(!sec.compare("r"))
        this->m[sec].section_map[p_type]->BuildIncrementalRoleLinks(rm, op, rules);
}

/***********************************************************************************************
 ***                                Model::BuildRoleLinks                                    ***
 ***********************************************************************************************
 * DESCRIPTION: Adds or deletes inheritance links for all Roles described in specified PRM     *
 *              policy rules.                                                                  *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   rm -- The outter RoleManager, all operations on Roles are performed on it. Messages*
 *          about Role Tree will be stored in rm. Once all messages have been processed, it    *
 *          would be delivered to current Model.                                               *
 *                                                                                             *
 * OUTPUT:   NONE                                                                              *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
void Model::BuildRoleLinks(std::shared_ptr<RoleManager> rm) {
    for(auto& it : this->m["r"].section_map)
        (it.second)->BuildRoleLinks(rm);
}

/***********************************************************************************************
 ***                                Model::ClearPolicy                                       ***
 ***********************************************************************************************
 * DESCRIPTION: Clears all PRM policy rules in current Model.                                  *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   NONE                                                                               *
 *                                                                                             *
 * OUTPUT:   NONE                                                                              *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
void Model::ClearPolicy() {
    auto& a_section_map = this->m["a"].section_map;
    for(auto& it : a_section_map) {
        if((it.second)->policy.size() > 0) {
            for(auto& p : (it.second)->policy)
                std::vector<std::string>().swap(p);
        }
    }

    auto& r_section_map = this->m["r"].section_map;
    for(auto& it : r_section_map) {
        if((it.second)->policy.size() > 0) {
            for(auto& p : (it.second)->policy)
                std::vector<std::string>().swap(p);
        }
    }

    auto& m_section_map = this->m["m"].section_map;
    for(auto& it : m_section_map) {
        if((it.second)->policy.size() > 0) {
            for(auto& p : (it.second)->policy)
                std::vector<std::string>().swap(p);
        }
    }
}

/***********************************************************************************************
 ***                                Model::GetPolicy                                         ***
 ***********************************************************************************************
 * DESCRIPTION: Returns all PRM policies that one type of a PRM policy rules owns.             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   sec -- The CONF section to be searched for.                                        *
 *                                                                                             *
 *          p_type -- Type of a PRM policy rule. It can be 'r' or 'r2'.                        *
 *                                                                                             *
 * OUTPUT:   PRM policies that one type of a PRM policy rules owns.                            *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
std::vector<std::vector<std::string>> Model::GetPolicy(const std::string& sec, const std::string& p_type) {
    return this->m[sec].section_map[p_type]->policy;
}

std::vector<std::vector<std::string>> Model::GetFilteredPolicy(const std::string& sec, const std::string& p_type, int field_index, const std::vector<std::string>& field_values) {
    std::vector<std::vector<std::string>> res;
    std::vector<std::vector<std::string>> policy(m[sec].section_map[p_type]->policy);
    for(size_t i = 0; i < policy.size(); ++i) {
        bool matched = true;
        for(size_t j = 0; j < field_values.size(); ++j) {
            if(field_values[j] != "" && (policy[i])[field_index + j] != field_values[j]) {
                matched = false;
                break;
            }
        }
        if(matched)
            res.push_back(policy[i]);
    }

    return res;
}

/***********************************************************************************************
 ***                                Model::HasPolicy                                         ***
 ***********************************************************************************************
 * DESCRIPTION: Determines whether there is a policy rule in current Model. More precisely,    *
 *              it searches for specified rule in {sec, p_type}. That is to say, even if       *
 *              the rule is found in Model, but it does not belong to {sec, p_type}, HasPolicy *
 *              returns false.                                                                 *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   sec -- The CONF section to be searched for.                                        *
 *                                                                                             *
 *          p_type -- Type of a PRM policy rule. It can be 'r' or 'r2'.                        *
 *                                                                                             *
 *          rule -- The PRM policy rule to be searched for.                                    *
 *                                                                                             *
 * OUTPUT:   Returns true if the rule is found, else returns false.                            *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
bool Model::HasPolicy(const std::string& sec, const std::string& p_type, const std::vector<std::string>& rule) {
    auto& policy = this->m[sec].section_map[p_type]->policy;
    for(const auto& policy_it : policy)
        if(CaepUtil::ArrayEqual(rule, policy_it))
            return true;

    return false;
}

/***********************************************************************************************
 ***                                Model::AddPolicy                                         ***
 ***********************************************************************************************
 * DESCRIPTION: Adds a PRM policy rule to current Model.                                       *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   sec -- The CONF section to be searched for.                                        *
 *                                                                                             *
 *          p_type -- Type of a PRM policy rule. It can be 'r' or 'r2'.                        *
 *                                                                                             *
 *          rule -- The PRM policy rule to be added to the Model.                              *
 *                                                                                             *
 * OUTPUT:   Returns false if the rule exists in the Model, else returns false.                *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
bool Model::AddPolicy(const std::string& sec, const std::string& p_type, const std::vector<std::string>& rule) {
    if(!this->HasPolicy(sec, p_type, rule)) {
        m[sec].section_map[p_type]->policy.push_back(rule);
        return true;
    }

    return false;
}

/***********************************************************************************************
 ***                                Model::AddPolicies                                       ***
 ***********************************************************************************************
 * DESCRIPTION: Adds a batch of PRM policy rules to current Model.                             *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   sec -- The CONF section to be searched for.                                        *
 *                                                                                             *
 *          p_type -- Type of a PRM policy rule. It can be 'r' or 'r2'.                        *
 *                                                                                             *
 *          rules -- The PRM policy rules to be added to the Model.                            *
 *                                                                                             *
 * OUTPUT:   Returns false if rules exist in the Model, else returns false.                    *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
bool Model::AddPolicies(const std::string& sec, const std::string& p_type, const std::vector<std::vector<std::string>>& rules) {
    for(const auto& rule : rules)
        if(this->HasPolicy(sec, p_type, rule))
            return false;

    for(const auto& rule : rules)
        this->m[sec].section_map[p_type]->policy.push_back(rule);

    return true;
}

/***********************************************************************************************
 ***                                Model::UpdatePolicy                                      ***
 ***********************************************************************************************
 * DESCRIPTION: Updates a policy rule in current Model.                                        *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   sec -- The CONF section to be searched for.                                        *
 *                                                                                             *
 *          p_type -- Type of a PRM policy rule. It can be 'r' or 'r2'.                        *
 *                                                                                             *
 *          oldRule -- The PRM policy rule to be removed in current Model.                     *
 *                                                                                             *
 *          newRule -- The PRM policy rule to be added into current Model.                     *
 *                                                                                             *
 * OUTPUT:   If oldRule does not exist or newRule exists in current Model, it returns false.   *
 *           If newRule does not exist and oldRule exists in current Model, it returns true.   *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
bool Model::UpdatePolicy(const std::string& sec, const std::string& p_type, const std::vector<std::string>& oldRule, const std::vector<std::string>& newRule) {
    auto& policy = m[sec].section_map[p_type]->policy;

    bool is_oldRule_deleted = false, is_newRule_added = false;

    for(auto it = policy.begin(); it != policy.end(); ++it) {
        if(CaepUtil::ArrayEqual(oldRule, *it)) {
            policy.erase(it);
            is_oldRule_deleted = true;
            break;
        }
    } 

    if(!is_oldRule_deleted)
        return false;

    if(!this->HasPolicy(sec, p_type, newRule)) {
        policy.push_back(newRule);
        is_newRule_added = true;
    }

    if(!is_newRule_added)
        return false;

    return is_oldRule_deleted && is_newRule_added;
}

/***********************************************************************************************
 ***                                Model::UpdatePolicies                                    ***
 ***********************************************************************************************
 * DESCRIPTION: Updates a batch of policy rules in current Model.                              *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   sec -- The CONF section to be searched for.                                        *
 *                                                                                             *
 *          p_type -- Type of a PRM policy rule. It can be 'r' or 'r2'.                        *
 *                                                                                             *
 *          oldRules -- The PRM policy rules to be removed in current Model.                   *
 *                                                                                             *
 *          newRules -- The PRM policy rules to be added into current Model.                   *
 *                                                                                             *
 * OUTPUT:   If oldRules do not exist or newRules exist in current Model, it returns false.    *
 *           If newRules do not exist and oldRules exist in current Model, it returns true.    *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
bool Model::UpdatePolicies(const std::string& sec, const std::string& p_type, const std::vector<std::vector<std::string>>& oldRules, const std::vector<std::vector<std::string>>& newRules) {
    auto& policy = this->m[sec].section_map[p_type]->policy;

    bool is_oldRule_deleted;
    for(const auto& oldRule : oldRules) {
        is_oldRule_deleted = false;
        for(auto it = policy.begin(); it != policy.end(); ++it) {
            if(CaepUtil::ArrayEqual(oldRule, *it)) {
                policy.erase(it);
                is_oldRule_deleted = true;
                break;
            }
        }
        if(!is_oldRule_deleted)
            return false;
    }

    for(const auto& newRule : newRules) {
        if(!this->HasPolicy(sec, p_type, newRule)) 
            continue;
        else
            return false;
    }

    for(const auto& newRule : newRules)
        policy.push_back(newRule);

    return true;
}

/***********************************************************************************************
 ***                                Model::RemovePolicy                                      ***
 ***********************************************************************************************
 * DESCRIPTION: Removes a PRM policy rule from current Model.                                  *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   sec -- The CONF section to be searched for.                                        *
 *                                                                                             *
 *          p_type -- Type of a PRM policy rule. It can be 'r' or 'r2'.                        *
 *                                                                                             *
 *          rule -- The PRM policy rule to be removed.                                         *
 *                                                                                             *
 * OUTPUT:   Removes the PRM policy rule and returns true if the PRM policy rule is found,     *
 *           else returns false.                                                               *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
bool Model::RemovePolicy(const std::string& sec, const std::string& p_type, const std::vector<std::string>& rule) {
    auto& policy = this->m[sec].section_map[p_type]->policy;

    for(auto it = policy.begin(); it != policy.end(); ++it) {
        if(CaepUtil::ArrayEqual(rule, *it)) {
            policy.erase(it);
            return true;
        }
    }
    return false;
}

/***********************************************************************************************
 ***                                Model::RemovePolicies                                    ***
 ***********************************************************************************************
 * DESCRIPTION: Removes a batch of PRM policy rules from current Model.                        *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   sec -- The CONF section to be searched for.                                        *
 *                                                                                             *
 *          p_type -- Type of a PRM policy rule. It can be 'r' or 'r2'.                        *
 *                                                                                             *
 *          rules -- The PRM policy rules to be removed.                                       *
 *                                                                                             *
 * OUTPUT:   Removes the PRM policy rules and returns true if the PRM policy rules are found,  *
 *           else returns false.                                                               *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/

bool Model::RemovePolicies(const std::string& sec, const std::string& p_type, const std::vector<std::vector<std::string>>& rules) {
    auto& policy = this->m[sec].section_map[p_type]->policy;

    bool is_equal;
    for(const auto& rule : rules) {
        is_equal = false;
        for(const auto& policy_it : policy) {
            if(CaepUtil::ArrayEqual(rule, policy_it))
                is_equal = true;
        }
        if(!is_equal)
            return false;
    }
    
    for(const auto& rule : rules)
        policy.erase(remove_if(policy.begin(), policy.end(),
                               [&](const auto& p) {
                                    return CaepUtil::ArrayEqual(rule, p);     
                               }),
                     policy.end());

    return true;
}

/***********************************************************************************************
 ***                                Model::RemoveFilteredPolicy                              ***
 ***********************************************************************************************
 * DESCRIPTION: Removes the filtered PRM policy rule from current Model.                       *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   sec -- The CONF section to be searched for.                                        *
 *                                                                                             *
 *          p_type -- Type of a PRM policy rule. It can be 'r' or 'r2'.                        *
 *                                                                                             *
 *          field_index -- Starting from this index, the values that matches field_values      *
 *                         are retrieved.                                                      *
 *                                                                                             *
 *          field_values -- The PRM policy rule to be filtered out.                            *
 *                                                                                             *
 * OUTPUT:   Removes the PRM policy rules and returns true if the PRM policy rules are found,  *
 *           else returns false.                                                               *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
std::pair<bool, std::vector<std::vector<std::string>>> Model::RemoveFilteredPolicy(const std::string& sec, const std::string& p_type, int field_index, const std::vector<std::string>& field_values) {
    std::vector<std::vector<std::string>> tmp, effects;
    std::vector<std::vector<std::string>> policy(m[sec].section_map[p_type]->policy);
    bool res = false;
    for(size_t i = 0; i < policy.size(); ++i) {
        bool matched = true;
        for(int j = 0; j < field_index; ++j) {
            if(field_values[j] != "" && (policy[i])[field_index + j] != field_values[j]) {
                matched = false;
                break;
            }
        }
        if(matched) {
            effects.push_back(policy[i]);
            res = true;
        }
        else
            tmp.push_back(policy[i]);
    }

    m[sec].section_map[p_type]->policy = tmp;
    std::pair<bool, std::vector<std::vector<std::string>>> result(res, effects);
    return result;
}

/***********************************************************************************************
 ***                                Model::GetValuesForFieldInPolicy                         ***
 ***********************************************************************************************
 * DESCRIPTION: Gets specified values in PRM policy rule, for example:                         *
 *                          #PRM file                                                          *
 *                          a, Alice, data1, read                                              *
 *                          a, Bob, data1, write                                               *
 *                          r, Alice, group                                                    *
 *                          r2, data1, data_group                                              *
 *              a->{Alice, Bob}, a->{data1} and r2->{data1} could be returned. That is to      *
 *              say, it gets all 'subject' or 'resource' or other's values distinctly.         *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   sec -- The CONF section to be searched for.                                        *
 *                                                                                             *
 *          p_type -- Type of a PRM policy rule. It can be 'r' or 'r2'.                        *
 *                                                                                             *
 *          field_index -- Starting from this index, the values that matches field_values      *
 *                         are retrieved.                                                      *
 *                                                                                             *
 * OUTPUT:   Returns specified field values.                                                   *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
std::vector<std::string> Model::GetValuesForFieldInPolicy(const std::string& sec, const std::string& p_type, int field_index) {
    auto values = GetAllValuesForFieldInPolicy(sec, p_type, field_index);
    CaepUtil::ArrayRemoveDuplicates(values);
    return values;
}

/***********************************************************************************************
 ***                                Model::GetAllValuesForFieldInPolicy                      ***
 ***********************************************************************************************
 * DESCRIPTION: Gets specified values in PRM policy rule, for example:                         *
 *                          #PRM file                                                          *
 *                          a, Alice, data1, read                                              *
 *                          a, Bob, data1, write                                               *
 *                          r, Alice, group                                                    *
 *                          r2, data1, data_group                                              *
 *              a->{Alice, Bob}, a->{data1, data1} and r2->{data1} could be returned. That is  *
 *              to say, it gets all 'subject' or 'resource' or other's values.                 *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   sec -- The CONF section to be searched for.                                        *
 *                                                                                             *
 *          p_type -- Type of a PRM policy rule. It can be 'r' or 'r2'.                        *
 *                                                                                             *
 *          field_index -- Starting from this index, the values that matches field_values      *
 *                         are retrieved.                                                      *
 *                                                                                             *
 * OUTPUT:   Returns specified field values.                                                   *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
std::vector<std::string> Model::GetAllValuesForFieldInPolicy(const std::string& sec, const std::string& p_type, int field_index) {
    std::vector<std::string> values;
    std::vector<std::vector<std::string>> policy(m[sec].section_map[p_type]->policy);
    for(const auto& p : policy)
        values.push_back(p[field_index]);

    return values;
}

/***********************************************************************************************
 ***                            Model::GetValuesForFieldInPolicyAllTypes                     ***
 ***********************************************************************************************
 * DESCRIPTION: Gets specified values in PRM policy rule, for example:                         *
 *                          #PRM file                                                          *
 *                          a, Alice, data1, read                                              *
 *                          a, Bob, data1, write                                               *
 *                          r, Alice, group                                                    *
 *                          r2, data1, data_group                                              *
 *              a->{Alice, Bob}, a->{data1} and r->{r.Alice, r2.data1} could be returned. That *
 *              is to say, it gets all 'subject' or 'resource' or other's values distinctly.   *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   sec -- The CONF section to be searched for.                                        *
 *                                                                                             *
 *          field_index -- Starting from this index, the values that matches field_values      *
 *                         are retrieved.                                                      *
 *                                                                                             *
 * OUTPUT:   Returns specified field values.                                                   *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
std::vector<std::string> Model::GetValuesForFieldInPolicyAllTypes(const std::string& sec, int field_index) {
    auto values = GetAllValuesForFieldInPolicyAllTypes(sec, field_index);
    CaepUtil::ArrayRemoveDuplicates(values);
    return values;
}

/***********************************************************************************************
 ***                            Model::GetAllValuesForFieldInPolicyAllTypes                  ***
 ***********************************************************************************************
 * DESCRIPTION: Gets specified values in PRM policy rule, for example:                         *
 *                          #PRM file                                                          *
 *                          a, Alice, data1, read                                              *
 *                          a, Bob, data1, write                                               *
 *                          r, Alice, group                                                    *
 *                          r2, data1, data_group                                              *
 *              a->{Alice, Bob}, a->{data1, data1} and r->{r.Alice, r2.data1} could be returned*
 *              That is to say, it gets all 'subject' or 'resource' or other's values.         *
 *                                                                                             *
 *                                                                                             *
 * INPUT:   sec -- The CONF section to be searched for.                                        *
 *                                                                                             *
 *          field_index -- Starting from this index, the values that matches field_values      *
 *                         are retrieved.                                                      *
 *                                                                                             *
 * OUTPUT:   Returns specified field values.                                                   *
 *                                                                                             *
 * WARNINGS:    NONE                                                                           *
 *                                                                                             *
 * HISTORY:                                                                                    *
 *     08/22/2019 ARZR : Created.                                                              *
 *=============================================================================================*/
std::vector<std::string> Model::GetAllValuesForFieldInPolicyAllTypes(const std::string& sec, int field_index) {
    std::vector<std::string> values;
    for(const auto& sec_m : m[sec].section_map) {
        std::vector<std::string> values_for_dield(this->GetAllValuesForFieldInPolicy(sec, sec_m.first, field_index));
        for(const auto& value : values_for_dield)
            values.push_back(value);
    }

    return values;
}

} // namespace caep 

#endif
