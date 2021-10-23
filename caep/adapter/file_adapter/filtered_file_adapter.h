#ifndef CAEP_FILTERED_FILE_ADAPTER_H
#define CAEP_FILTERED_FILE_ADAPTER_H

#include "./file_adapter.h"
#include "../filtered_adapter.h"

namespace caep {

class FilteredFileAdapter : public FileAdapter, public FilteredAdapter {
private:
    static bool filterLine(std::string line, Filter* filter);

    static bool filterWords(std::vector<std::string> line, std::vector<std::string> filter);

    void loadFilteredPolicyFile(Model* model, Filter* filter, void (*handle)(std::string, Model*));


public:
    // FilteredFileAdapter is the constructor for FilteredAdapter.
    FilteredFileAdapter(std::string file_path);

    // LoadPolicy loads all policy rules from the storage.
    void LoadPolicy(Model* model);

    // LoadFilteredPolicy loads only policy rules that match the filter.
    void LoadFilteredPolicy(Model* model, Filter* filter);

    // IsFiltered returns true if the loaded policy has been filtered.
    bool IsFiltered();

    // SavePolicy saves all policy rules to the storage.
    void SavePolicy(Model* model);

};

} // namespace caep 

#endif
