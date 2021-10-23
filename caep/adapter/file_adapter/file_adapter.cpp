#ifndef CAEP_FILE_ADAPTER_CPP
#define CAEP_FILE_ADAPTER_CPP

#include <fstream>

#include "./file_adapter.h"
#include "../../util/caep_util.h"
#include "../../exception/caep_exception.h"

namespace caep {

FileAdapter::FileAdapter() {
    this->file_path = "";
    this->filtered = false;
}

// NewAdapter is the constructor for Adapter.
FileAdapter::FileAdapter(std::string file_path) {
    this->file_path = file_path;
    this->filtered = false;
}

// LoadPolicy loads all policy rules from the storage.
void FileAdapter::LoadPolicy(Model* model) {
    if (this->file_path == "")
        throw AdapterException("Invalid file path, file path cannot be empty");

    this->LoadPolicyFile(model, LoadPolicyLine);
}

// SavePolicy saves all policy rules to the storage.
void FileAdapter::SavePolicy(Model* model) {
    if (this->file_path == "") {
        throw AdapterException("Invalid file path, file path cannot be empty");
    }

    std::string tmp;


    for (auto it = model->m["a"].section_map.begin() ; it != model->m["a"].section_map.end() ; it++){
        for (size_t i = 0; i < it->second->policy.size(); i++){
            tmp += it->first + ", ";
            tmp += CaepUtil::ArrayToString(it->second->policy[i]);
            tmp += "\n";
        }
    }

    for (auto it = model->m["r"].section_map.begin() ; it != model->m["r"].section_map.end() ; it++){
        for (size_t i = 0; i < it->second->policy.size(); i++){
            tmp += it->first + ", ";
            tmp += CaepUtil::ArrayToString(it->second->policy[i]);
            tmp += "\n";
        }
    }

    for (auto it = model->m["m"].section_map.begin() ; it != model->m["m"].section_map.end() ; it++){
        for (size_t i = 0; i < it->second->policy.size(); i++){
            tmp += it->first + ", ";
            tmp += CaepUtil::ArrayToString(it->second->policy[i]);
            tmp += "\n";
        }
    }

    return this->SavePolicyFile(CaepUtil::RTrim(tmp, "\n"));
}

void FileAdapter::LoadPolicyFile(Model* model, void (*handler)(std::string, Model*)) {
    std::ifstream in_file;
    try {
        in_file.open(this->file_path);
    } catch (const std::ifstream::failure e) {
        throw IOException("Cannot open file.");
    }

    std::string line;
    while(getline(in_file, line, '\n')){
        line = CaepUtil::Trim(line);
        handler(line, model);
    }

    in_file.close();
}

void FileAdapter::SavePolicyFile(std::string text) {
    std::ofstream out_file;
    out_file.open(this->file_path, std::ios::out);
    try {
        out_file.open(this->file_path, std::ios::out);
    } catch (const std::ifstream::failure e) {
        throw IOException("Cannot open file.");
    }

    out_file<<text;

    out_file.close();
}

// AddPolicy adds a policy rule to the storage.
void FileAdapter::AddPolicy(std::string sec, std::string p_type, std::vector<std::string> rule) {
    throw UnsupportedOperationException("not implemented");
}

// RemovePolicy removes a policy rule from the storage.
void FileAdapter::RemovePolicy(std::string sec, std::string p_type, std::vector<std::string> rule) {
    throw UnsupportedOperationException("not implemented");
}

void FileAdapter::RemoveFilteredPolicy(std::string sec, std::string p_type, int field_index, std::vector<std::string> field_values) {
    throw UnsupportedOperationException("not implemented");
}

// IsFiltered returns true if the loaded policy has been filtered.
bool FileAdapter::IsFiltered() {
    return this->filtered;
}

} // namespace caep 

#endif
