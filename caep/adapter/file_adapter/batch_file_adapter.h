#ifndef CAEP_BATCH_FILE_ADAPTER_H
#define CAEP_BATCH_FILE_ADAPTER_H

#include "./file_adapter.h"
#include "../batch_adapter.h"

namespace caep {

class BatchFileAdapter : public BatchAdapter, public FileAdapter {
public:
    BatchFileAdapter(std::string file_path);

    void AddPolicies(std::string sec, std::string p_type, std::vector<std::vector<std::string>> rules);

    void RemovePolicies(std::string sec, std::string p_type, std::vector<std::vector<std::string>> rules);
};

} // namespace caep 

#endif
