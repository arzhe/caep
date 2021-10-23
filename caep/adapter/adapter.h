#ifndef CAEP_ADAPTER_H
#define CAEP_ADAPTER_H

#include <string>
#include <vector>

#include "../model/model.h"

namespace caep {

void LoadPolicyLine(std::string line, Model* model);

class Adapter {
public:
    std::string file_path;
    bool filtered;
    
    virtual ~Adapter() {}

    /**
     * @brief LoadPolicy loads all policy rules from the storage.
     *
     * @param model the model
     */
    virtual void LoadPolicy(Model* model) = 0;

    /*
     * @brief SavePolicy saves all policy rules to the storage.
     *
     * @param model the model
     */
    virtual void SavePolicy(Model* model) = 0;

    /*
     * @brief AddPolicy adds a policy rule to the storage.
     * @brief This is part of the Auto-Save feature.
     *
     * @param sec the section, "a" or "r"
     * @param p_type the policy type, "a", "a2", ... or "r", "r2", ...
     * @param rule the rule, like (Alice, file, read)
     */
    virtual void AddPolicy(std::string sec, std::string p_type, std::vector<std::string> rule) = 0;

    /*
     * @brief RemovePolicy removes a policy rule from the storage.
     * @brief This is part of the Auto-Save feature.
     *
     * @param sec the section, "a" or "r"
     * @param p_type the policy type, "a", "a2", ... or "r", "r2", ...
     * @param rule the rule, like (Alice, file, read)
     */
    virtual void RemovePolicy(std::string sec, std::string p_type, std::vector<std::string> rule) = 0;

    /*
     * @brief RemoveFilteredPolicy removes policy rules that match the filter from the storage.
     * @brief This is part of the Auto-Save feature.
     *
     * @param sec the section, "a" or "r".
     * @param p_type the policy type, "a", "a2", .. or "r", "r2", ..
     * @param field_index the policy rule's start index to be matched.
     * @param field_values the field values to be matched, value "" means not to match this field.
     */
    virtual void RemoveFilteredPolicy(std::string sec, std::string ptype, int field_index, std::vector<std::string> field_values) = 0;

    virtual bool IsFiltered() = 0;

};

} // namespace caep 

#endif
