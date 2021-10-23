#ifndef CAEP_FILE_ADAPTER_H
#define CAEP_FILE_ADAPTER_H

#include "../adapter.h"

namespace caep {

// Adapter is the file adapter for Caep.
// It can load policy from file or save policy to file.
class FileAdapter : virtual public Adapter {
public:
    FileAdapter();

    // NewAdapter is the constructor for Adapter.
    FileAdapter(std::string file_path);

    // LoadPolicy loads all policy rules from the storage.
    void LoadPolicy(Model* model);

    // SavePolicy saves all policy rules to the storage.
    void SavePolicy(Model* model);

    void LoadPolicyFile(Model* model, void (*handler)(std::string, Model*));

    void SavePolicyFile(std::string text);

    // AddPolicy adds a policy rule to the storage.
    void AddPolicy(std::string sec, std::string p_type, std::vector<std::string> rule);

    // RemovePolicy removes a policy rule from the storage.
    void RemovePolicy(std::string sec, std::string p_type, std::vector<std::string> rule);

    // RemoveFilteredPolicy removes policy rules that match the filter from the storage.
    void RemoveFilteredPolicy(std::string sec, std::string p_type, int field_index, std::vector<std::string> field_values);

    // IsFiltered returns true if the loaded policy has been filtered.
    bool IsFiltered();
};

} // namespace caep 

#endif
