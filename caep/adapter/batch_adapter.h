#ifndef CAEP_BATCH_ADAPTER_H
#define CAEP_BATCH_ADAPTER_H

#include "./adapter.h"

namespace caep {

class BatchAdapter : virtual public Adapter {
public:
    // AddPolicies adds policy rules to the storage.
    // This is part of the Auto-Save feature.
    virtual void AddPolicies(std::string sec, std::string p_type, std::vector<std::vector<std::string>> rules) = 0;

    // RemovePolicies removes policy rules from the storage.
    // This is part of the Auto-Save feature.
    virtual void RemovePolicies(std::string sec, std::string p_type, std::vector<std::vector<std::string>> rules) = 0;
};

} // namespace caep 

#endif
