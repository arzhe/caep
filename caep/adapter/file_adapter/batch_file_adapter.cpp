#ifndef CAEP_BATCH_FILE_ADAPTER_CPP
#define CAEP_BATCH_FILE_ADAPTER_CPP

#include "./batch_file_adapter.h"
#include "../../exception/unsupported_operation_exception.h"

namespace caep {

BatchFileAdapter::BatchFileAdapter(std::string file_path) : FileAdapter(file_path) {
}

void BatchFileAdapter::AddPolicies(std::string sec, std::string p_type, std::vector<std::vector<std::string>> rules) {
    throw UnsupportedOperationException("not implemented");
}

void BatchFileAdapter::RemovePolicies(std::string sec, std::string p_type, std::vector<std::vector<std::string>> rules) {
    throw UnsupportedOperationException("not implemented");
}

} // namespace caep 

#endif
