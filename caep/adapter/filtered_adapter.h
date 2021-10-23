#ifndef CAEP_FILTERED_ADAPTER_H
#define CAEP_FILTERED_ADAPTER_H

#include "./adapter.h"

namespace caep {

// Filter defines the filtering rules for a FilteredAdapter's policy.
// Empty values are ignored, but all others must match the filter.
class Filter {
public:
    std::vector<std::string> A;
    std::vector<std::string> R;
    std::vector<std::string> M;
};

// FilteredAdapter is the interface for Caep adapters supporting filtered policies.
class FilteredAdapter : virtual public Adapter {
public:
    // LoadFilteredPolicy loads only policy rules that match the filter.
    void LoadFilteredPolicy(Model* model, Filter* filter);

    // IsFiltered returns true if the loaded policy has been filtered.
    virtual bool IsFiltered();
};

} // namespace caep

#endif
