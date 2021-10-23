#ifndef CAEP_CAEPER_CPP
#define CAEP_CAEPER_CPP

#include <algorithm>

#include "./caeper.h"
#include "../adapter/file_adapter/file_adapter.h"
#include "../rbac/default_role_manager.h"
#include "../effect/default_effector.h"
#include "../exception/caep_exception.h"
#include "../util/caep_util.h"

namespace caep {

bool Caeper::m_caeper(const std::string& matcher, const std::vector<std::string>& req) {
    if(!m_enabled)
        return true;

    std::string exp_string;
    if(!matcher.compare(""))
        exp_string = m_model->m["c"].section_map["c"]->value;
    else
        exp_string = matcher;
    
    std::vector<std::vector<std::string>> condition_exp;
    auto condition_or_strings = CaepUtil::Split(exp_string, "||");
    for(size_t i = 0; i < condition_or_strings.size(); ++i) {
        auto tmp = CaepUtil::Split(condition_or_strings[i], "&&");
        condition_exp.push_back(tmp);
    }

    for(auto& vc1 : condition_exp) {
        for(auto& vc2 : vc1)
            vc2 = CaepUtil::Trim(vc2);
    }
    
    std::unordered_map<std::string, int> param_index({
                                                     {"a.sub", 0},
                                                     {"a.res", 1},
                                                     {"a.act", 2},
                                                     {"a.dom", 3}
                                                     });
    std::unordered_map<std::string, std::vector<std::string>> field_collection;

    for(size_t i = 0; i < m_model->m["a"].section_map["a"]->policy[0].size(); ++i) {
        std::vector<std::string> field_values = m_model->GetAllValuesForFieldInPolicy("a", "a", i);
        switch(i) {
            case 0 :
                field_collection["a.sub"] = field_values;
                break;
            case 1 :
                field_collection["a.res"] = field_values;
                break;
            case 2 :
                field_collection["a.act"] = field_values;
                break;
            case 3 :
                field_collection["a.dom"] = field_values;
                break;
        }
    }

    size_t policy_count = m_model->m["a"].section_map["a"]->policy.size();
    std::vector<Effect> policy_effects(policy_count, Effect::Indeterminate);
    std::vector<float> matcher_results(policy_count, 0.0f);
    bool logic_negation = false;
    for(size_t i = 0; i < policy_count; ++i) {
        bool single_effect = true;
        for(size_t j = 0; j < condition_exp.size(); ++j) {
            bool matcher_effect = true;
            for(size_t k = 0; k < condition_exp[j].size(); ++k) {
                auto left_parentheses_index = condition_exp[j][k].find("(");
                auto right_parentheses_index = condition_exp[j][k].find(")");

                std::string matcher_name = condition_exp[j][k].substr(0, left_parentheses_index);
                std::string matcher_params = condition_exp[j][k].substr(left_parentheses_index + 1, right_parentheses_index - left_parentheses_index - 1);
                if(matcher_name.find("!") != std::string::npos) {
                    matcher_name = matcher_name.substr(1);
                    logic_negation = true;
                }

                std::vector<std::string> matcher_param = CaepUtil::Split(matcher_params, ",");
                for(auto& it : matcher_param)
                    it = CaepUtil::Trim(it);

                for(size_t h = 0; h < matcher_param.size(); ++h) {
                    if(!matcher_name.compare("RoleMatcher")) {
                        if(m_model->m["a"].section_map["a"]->policy[i].size() == 4) {
                            std::vector<std::string> domain{field_collection["dom"][i]};
                            matcher_effect = this->rm->HasLink(req[param_index[matcher_param[h]]], field_collection[matcher_param[h]][i], domain);
                        }
                        else  
                            matcher_effect = this->rm->HasLink(req[param_index[matcher_param[h]]], field_collection[matcher_param[h]][i]);
                    }
                    else {
                        matcher_effect = (this->m_matcher->matcher_map[matcher_name])(req[param_index[matcher_param[h]]], field_collection[matcher_param[h]][i]);
                    }
                    
                    if(logic_negation)
                        matcher_effect = !matcher_effect;
                    if(!matcher_effect)
                        break;
                }
                if(!matcher_effect)
                    break;
            }
            single_effect = matcher_effect;
            if(single_effect)
                break;
        }
        if(single_effect)
            policy_effects[i] = Effect::Allow;
        else
            policy_effects[i] = Effect::Deny;
    }
    
    bool result = m_eft->MergeEffects(m_model->m["e"].section_map["e"]->value, policy_effects,  matcher_results);
    return result;
}

Caeper::Caeper() {
}

Caeper::Caeper(const std::string& model_path, const std::string& policy_path) {
    m_model_path = model_path;
    m_model = std::make_shared<Model>(model_path);
    m_matcher = std::make_shared<Matcher>();
    m_eft = std::make_shared<DefaultEffector>();
    m_adapter = std::make_shared<FileAdapter>(policy_path);
    
    m_enabled = true;
    m_auto_save = true;
    m_auto_build_role_links = true;
    
    rm = std::make_shared<DefaultRoleManager>(10);

    if(m_adapter && m_adapter->file_path != "")
        this->LoadPolicy();

    m_matcher->LoadMatcherFromModel(m_model.get());
}

//Caeper::Caeper(const std::string& model_path, const std::string& policy_path)
//    : Caeper(model_path, std::make_shared<FileAdapter>(policy_path)) {
//}

Caeper::Caeper(const std::string& model_path, std::shared_ptr<Adapter> adapter)
    : Caeper(std::make_shared<Model>(model_path), adapter) {
        m_model_path = model_path;
}

Caeper::Caeper(std::shared_ptr<Model> m, std::shared_ptr<Adapter> adapter)
    : m_model(m), m_adapter(adapter) {
    this->Initialize();

    if(m_adapter && m_adapter->file_path != "")
        this->LoadPolicy();
}

Caeper::Caeper(std::shared_ptr<Model> m) : Caeper(m, nullptr) {}

Caeper::Caeper(const std::string& model_path) : Caeper(model_path, "") {}

void Caeper::InitWithFile(const std::string& model_path, const std::string& policy_path) {
    std::shared_ptr<Adapter> adapter = std::make_shared<FileAdapter>(policy_path);
    this->InitWithAdapter(model_path, adapter);
}

void Caeper::InitWithAdapter(const std::string& model_path, std::shared_ptr<Adapter> adapter) {
    std::shared_ptr<Model> m = std::shared_ptr<Model>(Model::NewModelFromFile(model_path));
    this->InitWithModelAndAdapter(m, adapter);
    m_model_path = model_path;
}

void Caeper::InitWithModelAndAdapter(std::shared_ptr<Model> m, std::shared_ptr<Adapter> adapter) {
    m_adapter = adapter;
    m_model = m;
    
    this->Initialize();

    if(m_adapter != nullptr && !m_adapter->IsFiltered())
        this->LoadPolicy();
}

void Caeper::Initialize() {
    this->rm = std::make_shared<DefaultRoleManager>(10);
    m_eft = std::make_shared<DefaultEffector>();
    m_matcher->LoadMatcherFromModel(m_model.get());

    m_enabled = true;
    m_auto_save = true;
    m_auto_build_role_links = true;
}

void Caeper::LoadModel() {
    m_model = std::shared_ptr<Model>(Model::NewModelFromFile(m_model_path));

    m_matcher->LoadMatcherFromModel(m_model.get());

    this->Initialize();
}

std::shared_ptr<Model> Caeper::GetModel() {
    return m_model;
}

void Caeper::SetModel(std::shared_ptr<Model> m) {
    m_model = m;
    m_matcher->LoadMatcherFromModel(m.get());
    this->Initialize();
}

std::shared_ptr<Adapter> Caeper::GetAdapter() {
    return m_adapter;
}

void Caeper::SetAdapter(std::shared_ptr<Adapter> adapter) {
    m_adapter = adapter;
}

std::shared_ptr<RoleManager> Caeper::GetRoleManager() {
    return this->rm;
}

void Caeper::SetRoleManager(std::shared_ptr<RoleManager> rm) {
    this->rm = rm;
}

void Caeper::SetEffector(std::shared_ptr<Effector> eft) {
    m_eft = eft;
}

void Caeper::ClearPolicy() {
    m_model->ClearPolicy();
}

void Caeper::LoadPolicy() {
    this->ClearPolicy();
    m_adapter->LoadPolicy(m_model.get());
    
    if(m_auto_build_role_links) {
        this->BuildRoleLinks();
    }
}

template<typename Filter>
void Caeper::LoadFilteredPolicy(Filter filter) {
    this->ClearPolicy();

    std::shared_ptr<FileAdapter> filtered_adapter;

    if(m_adapter->IsFiltered()) {
        filtered_adapter = std::dynamic_pointer_cast<FileAdapter>(m_adapter);
    }
    else
        throw AdapterException("filtered policies are not supported by this adapter");

    filtered_adapter->LoadPolicyFile(m_model, filter);

    if(m_auto_build_role_links) {
        this->BuildRoleLinks();
    }
}

bool Caeper::IsFiltered() {
    return m_adapter->IsFiltered();
}

void Caeper::SavePolicy() {
    if(this->IsFiltered())
        throw CaepEnforcerException("Cannot save a filtered policy");
    
    m_adapter->SavePolicy(m_model.get());
}

void Caeper::EnableCeaper(bool enable) {
    m_enabled = enable;
}

void Caeper::EnableAutoSave(bool auto_save) {
    m_auto_save = auto_save;
}

void Caeper::EnableAutoBuildRoleLinks(bool auto_build_role_links) {
    m_auto_build_role_links = auto_build_role_links;
}

void Caeper::BuildRoleLinks() {
    this->rm->Clear();
    m_model->BuildRoleLinks(this->rm);
}

void Caeper::BuildIncrementalRoleLinks(policy_op op, const std::string& p_type, const std::vector<std::vector<std::string>>& rules) {
    return m_model->BuildIncrementalRoleLinks(this->rm, op, "r", p_type, rules);
}

bool Caeper::Caep(const std::vector<std::string>& params) {
    return m_caeper("", params);
}

std::vector<bool> Caeper::BatchCaeper(const std::vector<std::vector<std::string>>& reqs) {
    std::vector<bool> results;
    results.reserve(reqs.size());
    for(auto req : reqs) {
        results.push_back(this->Caep(req));
    }
    return results;
}

} // namespace caep 

#endif
